#include "BattleScene.h"
#include "../Game/Charactor.h"
#include "DxLibUtility.h"
#include "../System/Application.h"
#include "../Utility/Input.h"
#include "../System/FileSystem.h"
#include "Effect.h"
#include "Camera.h"
#include <DxLib.h>
#include "FlyText.h"

constexpr auto game_screen_size_x = 1280;
constexpr auto game_screen_size_y = 720;

using namespace std;

BattleScene::BattleScene(BattleCharactor& leftBC, BattleCharactor& rightBC, SceneController& ctrl)
	: Scene(ctrl), _screenSize(Application::Instance().GetWindowSize()), _leftBC(leftBC), _rightBC(rightBC)
{
	auto wsize = Application::Instance().GetWindowSize();
	auto cameraPos = wsize.ToVector2Int() * 0.5;
	_camera = make_shared<Camera>(Rect(cameraPos, wsize));
	_camera->SetPos(Vector3((cameraPos).ToVector2()));

	_screenH = MakeScreen(_screenSize.w, _screenSize.h, true);

	_effects.clear();


	auto floorY = _screenSize.h / 3 * 2;
	auto screenCenter = _screenSize.ToVector2Int() * 0.5f;

	_leftBC.SetStartPos(Vector2(screenCenter.x - 200, floorY));
	_leftBC.SetDir(Dir::left);

	_rightBC.SetStartPos(Vector2(screenCenter.x + 200, floorY));
	_rightBC.SetDir(Dir::right);

	_effects.emplace_back(_leftBC.CreateAttackEffect(_rightBC.GetCenterPos()));
	_effects.emplace_back(make_shared<FlyText>("50", _rightBC.GetCenterPos(), 60 * 1));
}

BattleScene::~BattleScene()
{
}

void BattleScene::Update(const Input& input)
{
	_camera->Update();
	_leftBC.Update(input);
	_rightBC.Update(input);
	if (input.GetButtonDown(0, "space"))
	{
		_controller.PopScene();
		return;
	}

	for (auto& effect : _effects)
	{
		effect->Update(input);
	}
	auto newEnd = remove_if(_effects.begin(), _effects.end(),
		[](const std::shared_ptr<Effect>& effect) { return effect->GetDelete(); });
	_effects.erase(newEnd, _effects.end());
}

void BattleScene::Draw(void)
{
	SetDrawScreen(_screenH);
	ClsDrawScreen();

	auto floorY = _screenSize.h / 3 * 2;
	auto screenCenter = _screenSize.ToVector2Int() * 0.5f;

	// °‚Ì•`‰æ
	DrawRotaGraph(screenCenter.x, floorY, 1.0, 0.0,
		Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/Battle/floor.png"), true);

	auto charSize = Size(128, 128);

	_leftBC.Draw(*_camera);
	_rightBC.Draw(*_camera);

	for (auto& effect : _effects)
	{
		effect->Draw(*_camera);
	}

	SetDrawScreen(DX_SCREEN_BACK);

	DrawGraph(0,0,_screenH, true);
	ScreenFlip();
}