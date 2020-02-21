#pragma once
#include <stack>
#include <memory>

class Scene;
class Input;

class SceneController
{

private:
	std::deque<std::shared_ptr<Scene>> _scene;

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
};

