#pragma once
#include "../Scene/SceneController.h"

class Input;
// �V�[���̊��N���X
class Scene
{
protected:
	SceneController& _controller;
public:
	Scene(SceneController & controller);
	virtual ~Scene();
	// �V�[���̍X�V���s��
	// @param input ���̓I�u�W�F�N�g
	virtual void Update(const Input& input) = 0;

	virtual void Draw(void) = 0;
};

