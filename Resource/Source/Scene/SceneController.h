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

	// シーンの更新
	// @param input 入力の情報
	void SceneUpdate(const Input& input);

	// シーンを変更
	// @param scene	遷移するシーンのunique_ptr
	void ChangeScene(std::shared_ptr<Scene> scene);

	// シーンのスタック
	// @param scene	スタックするシーンのunique_ptr
	void PushScene(std::shared_ptr<Scene> scene);

	// シーンのポップ
	void PopScene(void);

	/// <summary>
	/// 現在のシーン参照の取得
	/// </summary>
	Scene& GetCurrentScene();

	/// <summary>
	/// シーン遷移フェード
	/// </summary>
	/// <returns></returns>
	Fade& GetFade();
};

