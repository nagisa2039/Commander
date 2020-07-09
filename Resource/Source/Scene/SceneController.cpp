#include <cassert>
#include <Dxlib.h>
#include "SceneController.h"
#include "Scene.h"
#include "Input.h"

using namespace std;

SceneController::SceneController()
{
}


SceneController::~SceneController()
{
}

void SceneController::SceneUpdate(const Input & input)
{
	bool quick = input.GetButton(KEY_INPUT_Q);

	for (int i = 0; i < (quick ? 3 : 1); i++)
	{
		// �X�^�b�N�̈�ԏ�̃V�[�����X�V
		_scene.front()->Update(input);
	}

	// �X�^�b�N�̈�ԉ����珇�Ԃɕ`��
	for (auto rit = _scene.rbegin(); rit != _scene.rend(); rit++)
	{
		(*rit)->Draw();
	}
}

void SceneController::ChangeScene(shared_ptr<Scene> scene)
{
	_scene.clear();
	_scene.push_front(move(scene));
}

void SceneController::PushScene(std::shared_ptr<Scene> scene)
{
	_scene.push_front(move(scene));
}

void SceneController::PopScene(void)
{
	assert(_scene.size() > 0);
	_scene.pop_front();
}
