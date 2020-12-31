#pragma once
#include "../Scene/SceneController.h"

class Input;

/// <summary>
/// シーンの基底クラス
/// </summary>
class Scene
{
protected:
	/// <summary>
	/// シーン管理クラス
	/// </summary>
	SceneController& _controller;

public:
	/// <param name="controller">シーン管理クラス</param>
	Scene(SceneController & controller);
	virtual ~Scene();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力情報</param>
	virtual void Update(const Input& input) = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(void) = 0;

	/// <summary>
	/// シーンがアクティブになったら呼ばれる
	/// </summary>
	virtual void On() {};

	/// <summary>
	/// シーンが非アクティブになったら呼ばれる
	/// </summary>
	virtual void Off() {};
};

