#pragma once
#include<memory>
#include<vector>
#include "../Utility/Geometry.h"

class Input;
struct Vector2Int;
class SceneController;
class FileSystem;
class DataBase;
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

	const DataBase& GetDataBase()const;

	SaveData& GetSaveData();

	FileSystem& GetFileSystem()const;
	const AnkerCalculation& GetAnkerCalculation()const;
	

private:
	Application();
	Application(const Application&);
	Application& operator=(const Application&);

	std::unique_ptr<Input> _input;
	std::unique_ptr<SceneController> _sceneController;
	std::unique_ptr<FileSystem> _fileSystem;

	std::vector<Vector2Int> _poss;

	std::unique_ptr<Configure> _configure;
	std::unique_ptr<DataBase> _dataBase;
	std::unique_ptr<SaveData> _saveData;

	std::unique_ptr<AnkerCalculation> _ankerCalculation;
};

