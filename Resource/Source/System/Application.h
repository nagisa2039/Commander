#pragma once
#include<memory>
#include<vector>
#include "../Utility/Geometry.h"

class Input;
struct Vector2Int;
class SceneController;
class FileSystem;
class DataBase;

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

	// Configureを参照で返す
	Configure& GetConfigure (void);

	// 画面サイズを参照で返す
	const Size& GetWindowSize(void);

	const DataBase& GetDataBase()const;

	std::shared_ptr<FileSystem> const GetFileSystem()const;

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

private:
	Application();
	Application(const Application&);
	Application& operator=(const Application&);

	std::shared_ptr<Input> _input;
	std::shared_ptr<SceneController> _sceneController;
	std::shared_ptr<FileSystem> _fileSystem;

	std::vector<Vector2Int> _poss;

	std::shared_ptr<Configure> _configure;
	std::shared_ptr<DataBase> _dataBase;
};

