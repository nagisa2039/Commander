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
#include "../Scene/MapSelectScene.h"
#include "DataBase.h"
#include "SaveData.h"
#include "AnkerCalculation.h"

using namespace std;

constexpr auto default_window_size_wide		= 1280;
constexpr auto default_window_size_hight	= 720;

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

SaveData& Application::GetSaveData()
{
	return *_saveData;
}

FileSystem& Application::GetFileSystem()
{
	return *_fileSystem;
}

const AnkerCalculation& Application::GetAnkerCalculation() const
{
	return *_ankerCalculation;
}

bool Application::Initialize()
{
	_configure = make_unique<Configure>();
	_dataBase = make_unique<DataBase>();
	_saveData = make_unique<SaveData>();
	_ankerCalculation = make_unique<AnkerCalculation>();

	// Dxlibの初期化
	ChangeWindowMode(TRUE);
	auto wSize = GetWindowSize();
	SetGraphMode(wSize.w, wSize.h, 32);
	if (DxLib_Init() != 0) { return false; }
	srand(static_cast<unsigned int>(time(NULL)));
	SetDrawScreen(DX_SCREEN_BACK);
	SetWindowText("Commander");

	_fileSystem = make_unique<FileSystem>();
	_sceneController = make_unique<SceneController>();
	//_sceneController->ChangeScene(make_unique<PlayScene>(*_sceneController));
	//_sceneController->ChangeScene(make_unique<MapEditScene>(*_sceneController));
	_sceneController->ChangeScene(make_unique<MapSelectScene>(*_sceneController));

	// inputの初期化
	_input = make_unique<Input>();
	_input->SetPlayerCnt(_input->GetConnectPadCnt());

	// コマンドの登録
	KeySetUp();

	// フォント読込
	_fileSystem->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin20", 20, 1, false, false);
	_fileSystem->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin30", 30, 1, false, false);
	_fileSystem->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin40", 40, 1, false, false);
	_fileSystem->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin50", 50, 1, false, false);
	_fileSystem->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin60", 60, 1, false, false);
	_fileSystem->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin70", 70, 1, false, false);
	_fileSystem->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin80", 80, 1, false, false);
	_fileSystem->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin100", 100, 1, false, false);
	_fileSystem->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin200", 200, 1, false, false);

	_fileSystem->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin20edge", 20, 1, true, false);
	_fileSystem->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin30edge", 30, 1, true, false);
	_fileSystem->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin40edge", 40, 1, true, false);
	_fileSystem->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin50edge", 50, 1, true, false);
	_fileSystem->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin60edge", 60, 1, true, false);
	_fileSystem->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin70edge", 70, 1, true, false);
	_fileSystem->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin80edge", 80, 1, true, false);
	_fileSystem->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin100edge", 100, 1, true, false);
	_fileSystem->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin200edge", 200, 1, true, false);

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