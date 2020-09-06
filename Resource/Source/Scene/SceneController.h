#pragma once
#include <stack>
#include <memory>

class Scene;
class Input;
class Fade;

class SceneController
{

private:
	std::deque<std::shared_ptr<Scene>> _scene;
	std::shared_ptr<Fade> _fade;

public:
	SceneController();
	~SceneController();

	// �V�[���̍X�V
	// @param input ���͂̏��
	void SceneUpdate(const Input& input);

	// �V�[����ύX
	// @param scene	�J�ڂ���V�[����unique_ptr
	void ChangeScene(std::shared_ptr<Scene> scene);

	// �V�[���̃X�^�b�N
	// @param scene	�X�^�b�N����V�[����unique_ptr
	void PushScene(std::shared_ptr<Scene> scene);

	// �V�[���̃|�b�v
	void PopScene(void);

	/// <summary>
	/// ���݂̃V�[���Q�Ƃ̎擾
	/// </summary>
	Scene& GetCurrentScene();

	/// <summary>
	/// �V�[���J�ڃt�F�[�h
	/// </summary>
	/// <returns></returns>
	Fade& GetFade();
};

