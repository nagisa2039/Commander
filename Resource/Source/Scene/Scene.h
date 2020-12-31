#pragma once
#include "../Scene/SceneController.h"

class Input;

/// <summary>
/// �V�[���̊��N���X
/// </summary>
class Scene
{
protected:
	/// <summary>
	/// �V�[���Ǘ��N���X
	/// </summary>
	SceneController& _controller;

public:
	/// <param name="controller">�V�[���Ǘ��N���X</param>
	Scene(SceneController & controller);
	virtual ~Scene();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	virtual void Update(const Input& input) = 0;

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw(void) = 0;

	/// <summary>
	/// �V�[�����A�N�e�B�u�ɂȂ�����Ă΂��
	/// </summary>
	virtual void On() {};

	/// <summary>
	/// �V�[������A�N�e�B�u�ɂȂ�����Ă΂��
	/// </summary>
	virtual void Off() {};
};

