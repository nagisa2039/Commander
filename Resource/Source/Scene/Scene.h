#pragma once
#include "../Scene/SceneController.h"

class Input;
// シーンの基底クラス
class Scene
{
protected:
	SceneController& _controller;
public:
	Scene(SceneController & controller);
	virtual ~Scene();
	// シーンの更新を行う
	// @param input 入力オブジェクト
	virtual void Update(const Input& input) = 0;

	virtual void Draw(void) = 0;
};

