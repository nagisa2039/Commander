#include "BattleScene.h"
#include "../Game/Charactor.h"
#include "../Utility/DxLibUtility.h"
#include "../System/Application.h"
#include "../Utility/Input.h"
#include "../System/FileSystem.h"

constexpr auto game_screen_size_x = 1280;
constexpr auto game_screen_size_y = 720;

BattleScene::BattleScene(Charactor& lChar, Charactor& rChar, SceneController& ctrl)
	: _lCharInf(lChar), _rCharInf(rChar), Scene(ctrl), _screenSize(Application::Instance().GetWindowSize())
{
	_screenH = MakeScreen(_screenSize.w, _screenSize.h, true);

	_lCharInf.charactor.SetDir(Dir::right);
	_lCharInf.charactor.AnimRestart();
	_rCharInf.charactor.SetDir(Dir::left);
	_rCharInf.charactor.AnimRestart();
}

BattleScene::~BattleScene()
{
}

void BattleScene::Update(const Input& input)
{
	_lCharInf.charactor.Update(input);
	_rCharInf.charactor.Update(input);
	if (input.GetButtonDown(0, "space"))
	{
		_controller.PopScene();
	}
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

	_lCharInf.charactor.BattleDraw(Vector2Int(screenCenter.x - 200, floorY), charSize);
	_rCharInf.charactor.BattleDraw(Vector2Int(screenCenter.x + 200, floorY), charSize);

	SetDrawScreen(DX_SCREEN_BACK);

	DrawGraph(0,0,_screenH, true);
	ScreenFlip();
}

BattleScene::CharInf::CharInf(Charactor& charactor):charactor(charactor), dir(charactor.GetDir())
{
}
