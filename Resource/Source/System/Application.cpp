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
#include "DataBase.h"

using namespace std;

constexpr auto default_window_size_wide		= 1280;
constexpr auto default_window_size_hight	= 720;

constexpr float jumpPower = 2.5f;
constexpr float gravity = 0.8f;

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

const DataBase& Application::GetDataBase() const
{
	return *_dataBase;
}

std::shared_ptr<FileSystem> const Application::GetFileSystem() const
{
	return _fileSystem;
}

bool Application::Initialize()
{
	_configure = make_shared<Configure>();
	_dataBase = make_shared<DataBase>();

	// Dxlibの初期化
	ChangeWindowMode(TRUE);
	auto wSize = GetWindowSize();
	SetGraphMode(wSize.w, wSize.h, 32);
	if (DxLib_Init() != 0) { return false; }
	srand(static_cast<unsigned int>(time(NULL)));
	SetDrawScreen(DX_SCREEN_BACK);
	SetWindowText("Commander");

	_fileSystem = make_shared<FileSystem>();
	_sceneController = make_unique<SceneController>();
	//_sceneController->ChangeScene(make_unique<PlayScene>(*_sceneController));
	_sceneController->ChangeScene(make_unique<MapEditScene>(*_sceneController));

	// inputの初期化
	_input = make_unique<Input>();
	_input->SetPlayerCnt(_input->GetConnectPadCnt());

	// コマンドの登録
	KeySetUp();


	return true;
}

void Application::KeySetUp()
{
	// キーボードの入力
	_input->AddCommand(0, "down", 0, KEY_INPUT_DOWN);
	_input->AddCommand(0, "up", 0, KEY_INPUT_UP);
	_input->AddCommand(0, "right", 0, KEY_INPUT_RIGHT);
	_input->AddCommand(0, "left", 0, KEY_INPUT_LEFT);
	_input->AddCommand(0, "pause", 0, KEY_INPUT_P);
	_input->AddCommand(0, "ctrl", 0, KEY_INPUT_LCONTROL);
	_input->AddCommand(0, "debug", 0, KEY_INPUT_D);
	_input->AddCommand(0, "ok", 0, KEY_INPUT_SPACE);
	_input->AddCommand(0, "change", 0, KEY_INPUT_C);
	_input->AddCommand(0, "team", 0, KEY_INPUT_T);
	_input->AddCommand(0, "back", 0, KEY_INPUT_BACK);
	_input->AddCommand(0, "status", 0, KEY_INPUT_X);
	_input->AddCommand(0, "+", 0, KEY_INPUT_P);
	_input->AddCommand(0, "-", 0, KEY_INPUT_O);
	_input->AddCommand(0, "F1", 0, KEY_INPUT_F1);
	_input->AddCommand(0, "F2", 0, KEY_INPUT_F2);
	_input->AddCommand(0, "F3", 0, KEY_INPUT_F3);
	_input->AddCommand(0, "F4", 0, KEY_INPUT_F4);
	_input->AddCommand(0, "F5", 0, KEY_INPUT_F5);
	_input->AddCommand(0, "F6", 0, KEY_INPUT_F6);

	// マウスの入力
	_input->AddCommand(0, "mouseLeft", 5, MOUSE_INPUT_LEFT);
	_input->AddCommand(0, "mouseRight", 5, MOUSE_INPUT_RIGHT);
	_input->AddCommand(0, "mouseMiddle", 5, MOUSE_INPUT_MIDDLE);

	// プレイヤーごとのパッド入力
	auto wSize = GetWindowSize();
	_poss.resize(_input->GetPlayerCnt());
	for (int j = 0; j < _input->GetPlayerCnt(); j++)
	{
		_poss[j] = Vector2Int(wSize.w / 2, wSize.h / 2);
		_input->AddCommand(j, "down", DX_INPUT_PAD1 + j, PAD_INPUT_DOWN);
		_input->AddCommand(j, "up", DX_INPUT_PAD1 + j, PAD_INPUT_UP);
		_input->AddCommand(j, "right", DX_INPUT_PAD1 + j, PAD_INPUT_RIGHT);
		_input->AddCommand(j, "left", DX_INPUT_PAD1 + j, PAD_INPUT_LEFT);
		_input->AddCommand(j, "pause", DX_INPUT_PAD1 + j, PAD_INPUT_START);
		_input->AddCommand(j, "ok", DX_INPUT_PAD1 + j, PAD_INPUT_1);		// Aボタン
		_input->AddCommand(j, "back", DX_INPUT_PAD1 + j, PAD_INPUT_2);		// Bボタン
		//_input->AddCommand(j, "", DX_INPUT_PAD1 + j, PAD_INPUT_3);		// Xボタン
		_input->AddCommand(j, "status", DX_INPUT_PAD1 + j, PAD_INPUT_4);	// Yボタン
	}
}

void Application::Run()
{
	while (ProcessMessage() == 0 && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		_input->Update();

		ClsDrawScreen();


		_sceneController->SceneUpdate(*_input);

		ScreenFlip();
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