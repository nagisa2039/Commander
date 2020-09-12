#include <cassert>
#include <Dxlib.h>
#include "SceneController.h"
#include "Scene.h"
#include "Input.h"
#include "Fade.h"

using namespace std;

SceneController::SceneController()
{
	_fade = make_shared<Fade>();
	_quick = true;
}


SceneController::~SceneController()
{
}

void SceneController::SceneUpdate(const Input & input)
{
	if (input.GetButtonDown("debug"))
	{
		_quick = !_quick;
	}
	for (int i = 0; i < (_quick ? 10 : 1); i++)
	{
		// スタックの一番上のシーンを更新
		_scene.front()->Update(input);
	}
	_fade->Update();

	// スタックの一番下から順番に描画
	for (auto rit = _scene.rbegin(); rit != _scene.rend(); rit++)
	{
		(*rit)->Draw();
	}
	_fade->Draw();
}

void SceneController::ChangeScene(shared_ptr<Scene> scene)
{
	if (_scene.size() > 0)
	{
		_scene.front()->Off();
	}
	_scene.clear();
	_scene.push_front(scene);
	scene->On();
}

void SceneController::PushScene(std::shared_ptr<Scene> scene)
{
	if (_scene.size() > 0)
	{
		_scene.front()->Off();
	}
	_scene.push_front(scene);
	scene->On();
}

void SceneController::PopScene(void)
{
	assert(_scene.size() > 0);
	_scene.front()->Off();
	_scene.pop_front();
	if (_scene.size() > 0)
	{
		_scene.front()->On();
	}
}

Scene& SceneController::GetCurrentScene()
{
	assert(_scene.size() > 0);
	return *_scene.front();
}

Fade& SceneController::GetFade()
{
	assert(_fade);
	return *_fade;
}
