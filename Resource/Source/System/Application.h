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
/// �A�v���P�[�V�����S�̂�
/// ���䂷��V���O���g���N���X
/// </summary>
class Application
{
private:
	Application();
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	// ���͏��Ǘ��N���X
	std::unique_ptr<Input> _input;
	// �V�[���Ǘ��N���X
	std::unique_ptr<SceneController> _sceneController;

	// �Z�[�u�f�[�^�Ǘ��N���X
	std::unique_ptr<SaveData> _saveData;

	// �A���J�[���g�p�����`����W�v�Z�N���X
	std::unique_ptr<AnkerCalculation> _ankerCalculation;
	// FPS�Ǘ��N���X
	std::unique_ptr<FPSManager> _fpsManager;
	// �E�B���h�E�T�C�Y
	Size _winSize;

	/// <summary>
	/// ���͏��̃Z�b�g�A�b�v
	/// </summary>
	void KeySetUp();

public:
	~Application();

	/// <summary>
	/// �V���O���g�����̂�Ԃ�
	/// </summary>
	static Application& Instance()
	{
		static Application instance;
		return instance;
	}
	
	/// <summary>
	/// �A�v���P�[�V����������������
	/// </summary>
	bool Initialize();

	/// <summary>
	/// �A�v���P�[�V���������s����
	/// �����A���g�ɃQ�[�����[�v������
	/// </summary>
	void Run();

	/// <summary>
	/// �I������
	/// </summary>
	void Terminate();

	/// <summary>
	/// ��ʃT�C�Y���Q�ƂŕԂ�
	/// </summary>
	const Size& GetWindowSize(void);

	/// <summary>
	/// �Z�[�u�f�[�^�̎擾
	/// </summary>
	SaveData& GetSaveData();

	/// <summary>
	/// �A���J�[���g�p�����`����W�v�Z�N���X���擾
	/// </summary>
	const AnkerCalculation& GetAnkerCalculation()const;

	/// <summary>
	/// �V�[���Ǘ��N���X�̎擾
	/// </summary>
	SceneController& GetSceneController()const;
};

