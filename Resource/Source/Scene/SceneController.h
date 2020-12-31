#pragma once
#include <stack>
#include <memory>

class Scene;
class Input;
class Fade;

/// <summary>
/// シーン管理クラス
/// </summary>
class SceneController
{
private:
	// シーン管理配列
	std::deque<std::shared_ptr<Scene>> _scene;
	// フェード再生クラス
	std::shared_ptr<Fade> _fade;
	// Updateを加速するか
	bool _quick;

public:
	SceneController();
	~SceneController();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void SceneUpdate(const Input& input);

	/// <summary>
	/// シーンを変更
	/// </summary>
	/// <param name="scene">遷移するシーン</param>
	void ChangeScene(std::shared_ptr<Scene> scene);

	/// <summary>
	/// シーンのスタック
	/// </summary>
	/// <param name="scene">遷移するシーン</param>
	void PushScene(std::shared_ptr<Scene> scene);

	/// <summary>
	/// 先頭シーンの削除
	/// </summary>
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

