#pragma once
#include<memory>
#include<vector>
#include <windows.h>
#include "../Utility/Geometry.h"

class Input;
struct Vector2Int;
class SceneController;
class FileSystem;
class DataBase;
class SaveData;
class AnkerCalculation;

// �A�v���P�[�V�����S�̂�
// ���䂷��V���O���g���N���X
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

	// �V���O���g�����̂�Ԃ�
	static Application& Instance()
	{
		static Application instance;
		return instance;
	}
	// �A�v���P�[�V����������������
	bool Initialize();

	void FontSetUp();

	void KeySetUp();

	// �A�v���P�[�V���������s����
	// �����A���g�ɃQ�[�����[�v������
	void Run();

	// �I������
	void Terminate();


	// Configure���Q�ƂŕԂ�
	Configure& GetConfigure (void);

	// ��ʃT�C�Y���Q�ƂŕԂ�
	const Size& GetWindowSize(void);

	const DataBase& GetDataBase()const;

	SaveData& GetSaveData();

	FileSystem& GetFileSystem();
	const AnkerCalculation& GetAnkerCalculation()const;
	
	double GetDeltaTime()const;
	unsigned int GetFPS()const;

private:
	Application();
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	std::unique_ptr<Input> _input;
	std::unique_ptr<SceneController> _sceneController;
	std::unique_ptr<FileSystem> _fileSystem;

	std::vector<Vector2Int> _poss;

	std::unique_ptr<Configure> _configure;
	std::unique_ptr<DataBase> _dataBase;
	std::unique_ptr<SaveData> _saveData;

	std::unique_ptr<AnkerCalculation> _ankerCalculation;

	LARGE_INTEGER _freq;
	LARGE_INTEGER _start;
	LARGE_INTEGER _end;

};

