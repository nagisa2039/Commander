#include "BattleScene.h"
#include "../Game/Charactor.h"
#include "DxLibUtility.h"
#include "../System/Application.h"
#include "../Utility/Input.h"
#include "../System/FileSystem.h"
#include "Effect.h"
#include "Camera.h"

constexpr auto game_screen_size_x = 1280;
constexpr auto game_screen_size_y = 720;

using namespace std;

BattleScene::BattleScene(Charactor& lChar, Charactor& rChar, SceneController& ctrl)
	: _lCharInf(lChar), _rCharInf(rChar), Scene(ctrl), _screenSize(Application::Instance().GetWindowSize())
{
	auto wsize = Application::Instance().GetWindowSize();
	auto cameraPos = wsize.ToVector2Int() * 0.5;
	_camera = make_shared<Camera>(Rect(cameraPos, wsize));
	_camera->SetPos(Vector3((cameraPos).ToVector2()));

	_screenH = MakeScreen(_screenSize.w, _screenSize.h, true);

	_effects.clear();


	auto floorY = _screenSize.h / 3 * 2;
	auto screenCenter = _screenSize.ToVector2Int() * 0.5f;

	_lCharInf.drawPos = Vector2Int(screenCenter.x - 200, floorY);
	_lCharInf.charactor.SetDir(Dir::right);
	_lCharInf.charactor.AnimRestart();

	_rCharInf.drawPos = Vector2Int(screenCenter.x + 200, floorY);
	_rCharInf.charactor.SetDir(Dir::left);
	_rCharInf.charactor.AnimRestart();

	auto charSize = Size(128, 128);
	_effects.emplace_back(_lCharInf.charactor.AddAttackEffect(_rCharInf.drawPos - Vector2Int(0, charSize.h/2)));

}

BattleScene::~BattleScene()
{
}

void BattleScene::Update(const Input& input)
{
	_camera->Update();
	_lCharInf.charactor.Update(input);
	_rCharInf.charactor.Update(input);
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

	_lCharInf.charactor.BattleDraw(_lCharInf.drawPos, charSize);
	_rCharInf.charactor.BattleDraw(_rCharInf.drawPos, charSize);

	for (auto& effect : _effects)
	{
		effect->Draw(*_camera);
	}

	SetDrawScreen(DX_SCREEN_BACK);

	DrawGraph(0,0,_screenH, true);
	ScreenFlip();
}

BattleScene::CharInf::CharInf(Charactor& charactor)
	:charactor(charactor), dir(charactor.GetDir()), drawPos(Vector2Int())
{
}
