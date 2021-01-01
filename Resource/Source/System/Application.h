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
class FPSManager;

/// <summary>
/// アプリケーション全体を
/// 制御するシングルトンクラス
/// </summary>
class Application
{
private:
	Application();
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	// 入力情報管理クラス
	std::unique_ptr<Input> _input;
	// シーン管理クラス
	std::unique_ptr<SceneController> _sceneController;

	// セーブデータ管理クラス
	std::unique_ptr<SaveData> _saveData;

	// アンカーを使用した描画座標計算クラス
	std::unique_ptr<AnkerCalculation> _ankerCalculation;
	// FPS管理クラス
	std::unique_ptr<FPSManager> _fpsManager;
	// ウィンドウサイズ
	Size _winSize;

	/// <summary>
	/// 入力情報のセットアップ
	/// </summary>
	void KeySetUp();

public:
	~Application();

	/// <summary>
	/// シングルトン実体を返す
	/// </summary>
	static Application& Instance()
	{
		static Application instance;
		return instance;
	}
	
	/// <summary>
	/// アプリケーションを初期化する
	/// </summary>
	bool Initialize();

	/// <summary>
	/// アプリケーションを実行する
	/// 実質、中身にゲームループが入る
	/// </summary>
	void Run();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Terminate();

	/// <summary>
	/// 画面サイズを参照で返す
	/// </summary>
	const Size& GetWindowSize(void);

	/// <summary>
	/// セーブデータの取得
	/// </summary>
	SaveData& GetSaveData();

	/// <summary>
	/// アンカーを使用した描画座標計算クラスを取得
	/// </summary>
	const AnkerCalculation& GetAnkerCalculation()const;

	/// <summary>
	/// シーン管理クラスの取得
	/// </summary>
	SceneController& GetSceneController()const;
};

