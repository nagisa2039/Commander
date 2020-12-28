#pragma once
#include "Actor.h"
#include <string>

class MapCtrl;

/// <summary>
/// マップカーソル基底クラス
/// </summary>
class MapCursor :
	public Actor
{
private:
	/// <summary>
	/// キーボードでの制御
	/// </summary>
	/// <param name="input">入力情報</param>
	void KeybordMove(const Input& input);

	/// <summary>
	/// マウスでの制御
	/// </summary>
	/// <param name="input">入力情報</param>
	void MouseMove(const Input& input);

	// 移動関数用関数ポインタ
	void(MapCursor::* _cursorMover)(const Input&);

protected:
	// マップ座標
	Vector2Int _mapPos;
	// マップ管理クラス
	MapCtrl& _mapCtrl;

	// 入力受付までのフレーム数
	int _moveItv;
	// 入力受付までのフレーム数の最大値
	int _moveItvCurrentMax;
	// 移動時のSEハンドル
	int _moveSEH;
	// 最初の入力受付までのフレーム数の最大値
	const int _moveItvMax;

	// 決定入力受付までのフレーム数
	int _putItv;
	// 決定入力受付までのフレーム最大数
	int _putItvCurrentMax;
	// 最初の決定入力受付までのフレーム最大数
	const int _putItvMax;

	/// <summary>
	/// 移動
	/// </summary>
	/// <param name="input">入力情報</param>
	void CursorMove(const Input& input);

	/// <summary>
	/// キーボード入力に切り替え
	/// </summary>
	void ChangeKeybordInput();

	/// <summary>
	/// マウス入力に切り替え
	/// </summary>
	void ChangeMouseInput();

	/// <summary>
	/// 決定入力が受け付けられるかを確認
	/// </summary>
	/// <param name="input">入力情報</param>
	/// <param name="key">確認するボタン名</param>
	bool PutCheck(const Input& input, const std::string& key);

	/// <summary>
	/// カーソルが移動瞬間のコールバック
	/// </summary>
	virtual void CursorMoveMoment();

public:
	/// <param name="mapCtrl">マップ管理クラス</param>
	/// <param name="camera">カメラ</param>
	MapCursor(MapCtrl& mapCtrl, Camera& camera);
	virtual ~MapCursor();

	/// <summary>
	/// マップ座標
	/// </summary>
	const Vector2Int& GetMapPos()const;

	/// <summary>
	/// マップ座標の設定
	/// </summary>
	/// <param name="mapPos">マップ座標</param>
	void SetMapPos(const Vector2Int& mapPos);

	/// <summary>
	/// 中心座標の取得
	/// </summary>
	Vector2 GetCenterPos()const override;
};