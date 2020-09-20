 #include <DxLib.h>
#include <time.h>
#include "../Utility/Input.h"
#include "../Scene/SceneController.h"
#include "../Scene/PlayScene.h"
#include "Application.h"
#include "../Utility/Geometry.h"
#include "../System/FileSystem.h"
#include "../Scene/PlayScene.h"
#include "../Scene/MapEditScene.h"
#include "../Scene/PlayScene.h"
#include "../Scene/TitleScene.h"
#include "DataBase.h"
#include "SaveData.h"
#include "AnkerCalculation.h"
#include "FPSManager.h"

using namespace std;

namespace
{
	constexpr auto default_window_size_wide = 1280;
	constexpr auto default_window_size_hight = 720;

	constexpr int FPS = 60;
}

Application::Application()
{
}

Application::~Application()
{
}

Application::Configure & Application::GetConfigure(void)
{
	return *_configure;
}

const Size & Application::GetWindowSize(void)
{
	return _configure->GetWindowSize();
}

SaveData& Application::GetSaveData()
{
	return *_saveData;
}

const AnkerCalculation& Application::GetAnkerCalculation() const
{
	return *_ankerCalculation;
}

SceneController& Application::GetSceneController() const
{
	assert(_sceneController);
	return *_sceneController;
}

bool Application::Initialize()
{
	_fpsManager.reset(new FPSManager(FPS));
	_configure = make_unique<Configure>();
	_ankerCalculation = make_unique<AnkerCalculation>();
	// Dxlibの初期化
	ChangeWindowMode(TRUE);
	auto wSize = GetWindowSize();
	SetGraphMode(wSize.w, wSize.h, 32);
	if (DxLib_Init() != 0) { return false; }
	srand(static_cast<unsigned int>(time(NULL)));
	SetDrawScreen(DX_SCREEN_BACK);
	SetWindowText("Commander");
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	SetWindowIconID(100);

	DataBase::Instance().Init();
	_saveData = make_unique<SaveData>();

	// inputの初期化
	_input = make_unique<Input>();

	// コマンドの登録
	KeySetUp();

	_sceneController = make_unique<SceneController>();
	//_sceneController->ChangeScene(make_unique<PlayScene>(*_sceneController));
	//_sceneController->ChangeScene(make_unique<MapEditScene>(*_sceneController));
	_sceneController->ChangeScene(make_unique<TitleScene>(*_sceneController));

	return true;
}

void Application::KeySetUp()
{
	const auto keybord = Input::PeripheralType::keybord;
	_input->AddCommand("down", keybord, KEY_INPUT_DOWN);
	_input->AddCommand("up", keybord, KEY_INPUT_UP);
	_input->AddCommand("right", keybord, KEY_INPUT_RIGHT);
	_input->AddCommand("left", keybord, KEY_INPUT_LEFT);
	_input->AddCommand("pause", keybord, KEY_INPUT_P);
	_input->AddCommand("ctrl", keybord, KEY_INPUT_LCONTROL);
	_input->AddCommand("ok", keybord, KEY_INPUT_SPACE);
	_input->AddCommand("change", keybord, KEY_INPUT_C);
	_input->AddCommand("team", keybord, KEY_INPUT_T);
	_input->AddCommand("back", keybord, KEY_INPUT_BACK);
	_input->AddCommand("status", keybord, KEY_INPUT_X);
//#ifdef _DEBUG
	_input->AddCommand("debug", keybord, KEY_INPUT_D);
//#endif
	_input->AddCommand("F1", keybord, KEY_INPUT_F1);
	_input->AddCommand("F2", keybord, KEY_INPUT_F2);

	// マウスの入力
	const auto mouse = Input::PeripheralType::mouse;
	_input->AddCommand("ok", mouse, MOUSE_INPUT_LEFT);
	_input->AddCommand("back", mouse, MOUSE_INPUT_RIGHT);
	_input->AddCommand("status", mouse, MOUSE_INPUT_MIDDLE);

	// プレイヤーごとのパッド入力
	const auto pad = Input::PeripheralType::gamepad;
	_input->AddCommand("down", pad, PAD_INPUT_DOWN);
	_input->AddCommand("up", pad, PAD_INPUT_UP);
	_input->AddCommand("right", pad, PAD_INPUT_RIGHT);
	_input->AddCommand("left", pad, PAD_INPUT_LEFT);
	_input->AddCommand("pause", pad, PAD_INPUT_START);
	_input->AddCommand("ok", pad, PAD_INPUT_1);		// Aボタン
	_input->AddCommand("back", pad, PAD_INPUT_2);		// Bボタン
	_input->AddCommand("status", pad, PAD_INPUT_4);	// Yボタン
}

void Application::Run()
{
	while (ProcessMessage() == 0 && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		_input->Update();

		ClsDrawScreen();

		_sceneController->SceneUpdate(*_input);

		ScreenFlip();
		_fpsManager->Wait();
	}
	DxLib_End();
}

void Application::Terminate()
{
}

Application::Configure::Configure()
{
	_winSize = { default_window_size_wide, default_window_size_hight };
}