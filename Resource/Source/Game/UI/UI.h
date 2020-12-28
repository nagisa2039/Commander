#pragma once
#include <Deque>
#include <memory>

class Input;

/// <summary>
/// UI基底クラス
/// </summary>
class UI
{
protected:
	// UIの親子関係管理Deque
	std::deque<std::shared_ptr<UI>>* _uiDeque;
	// 削除判定
	bool _delete;
	// 開いている状態か
	bool _isOpen;

public:
	/// <param name="uiDeque">UIの親子関係管理Deque</param>
	UI(std::deque<std::shared_ptr<UI>>* uiDeque);
	virtual ~UI();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力情報</param>
	virtual void Update(const Input& input) = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 削除していいか
	/// </summary>
	bool GetDelete()const;

	/// <summary>
	/// 開いているか(表示中)
	/// </summary>
	bool GetIsOpen()const;

	/// <summary>
	/// 最前面に表示するときなどに更新処理を走らせる
	/// </summary>
	virtual void OnActive();
};