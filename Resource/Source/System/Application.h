#pragma once
#include<memory>
#include<vector>
#include "../Utility/Geometry.h"

class Input;
struct Vector2Int;
class SceneController;
class FileSystem;
class DataBase;

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

	// Configure���Q�ƂŕԂ�
	Configure& GetConfigure (void);

	// ��ʃT�C�Y���Q�ƂŕԂ�
	const Size& GetWindowSize(void);

	const DataBase& GetDataBase()const;

	std::shared_ptr<FileSystem> const GetFileSystem()const;

	// �V���O���g�����̂�Ԃ�
	static Application& Instance()
	{
		static Application instance;
		return instance;
	}
	// �A�v���P�[�V����������������
	bool Initialize();

	void KeySetUp();

	// �A�v���P�[�V���������s����
	// �����A���g�ɃQ�[�����[�v������
	void Run();

	// �I������
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

