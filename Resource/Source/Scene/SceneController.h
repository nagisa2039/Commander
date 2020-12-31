#pragma once
#include <stack>
#include <memory>

class Scene;
class Input;
class Fade;

/// <summary>
/// �V�[���Ǘ��N���X
/// </summary>
class SceneController
{
private:
	// �V�[���Ǘ��z��
	std::deque<std::shared_ptr<Scene>> _scene;
	// �t�F�[�h�Đ��N���X
	std::shared_ptr<Fade> _fade;
	// Update���������邩
	bool _quick;

public:
	SceneController();
	~SceneController();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void SceneUpdate(const Input& input);

	/// <summary>
	/// �V�[����ύX
	/// </summary>
	/// <param name="scene">�J�ڂ���V�[��</param>
	void ChangeScene(std::shared_ptr<Scene> scene);

	/// <summary>
	/// �V�[���̃X�^�b�N
	/// </summary>
	/// <param name="scene">�J�ڂ���V�[��</param>
	void PushScene(std::shared_ptr<Scene> scene);

	/// <summary>
	/// �擪�V�[���̍폜
	/// </summary>
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

