#pragma once
#include<memory>
#include<vector>
#include <windows.h>
#include "../Utility/Geometry.h"

class Input;
struct Vector2Int;
class SceneController;
class SaveData;
class AnkerCalculation;

// アプリケーション全体を
// 制御するシングルトンクラス
class Application
{
public:
	class Configure 
	{
	private:
		Size _winSize;
	public:
		Configure();
		const Size& GetWindowSize()const 
		{
			return _winSize;
		}
	};

	~Application();

	// シングルトン実体を返す
	static Application& Instance()
	{
		static Application instance;
		return instance;
	}
	// アプリケーションを初期化する
	bool Initialize();

	void KeySetUp();

	// アプリケーションを実行する
	// 実質、中身にゲームループが入る
	void Run();

	// 終了処理
	void Terminate();


	// Configureを参照で返す
	Configure& GetConfigure (void);

	// 画面サイズを参照で返す
	const Size& GetWindowSize(void);

	SaveData& GetSaveData();

	const AnkerCalculation& GetAnkerCalculation()const;
	SceneController& GetSceneController()const;

private:
	Application();
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	std::unique_ptr<Input> _input;
	std::unique_ptr<SceneController> _sceneController;

	std::vector<Vector2Int> _poss;

	std::unique_ptr<Configure> _configure;
	std::unique_ptr<SaveData> _saveData;

	std::unique_ptr<AnkerCalculation> _ankerCalculation;
};

