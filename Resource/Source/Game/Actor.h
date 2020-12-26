#pragma once
#include "../Utility/Geometry.h"

class Input;
class Camera;

/// <summary>
/// アクタークラス
/// </summary>
class Actor
{
protected:
	//　座標
	Vector2 _pos;
	// 削除フラグ
	bool _delete;
	// カメラ
	Camera& _camera;

public:
	/// <param name="camera">カメラ</param>
	Actor(Camera& camera);
	virtual ~Actor();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力情報</param>
	virtual void Update(const Input& input) = 0;

	/// <summary>
	///　描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 座標の取得
	/// </summary>
	Vector2 GetActorPos()const;

	/// <summary>
	/// 中心座標の取得
	/// </summary>
	virtual Vector2 GetCenterPos()const;

	/// <summary>
	/// 削除していいか
	/// </summary>
	bool GetDelete()const;

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPos(const Vector2& pos);

	/// <summary>
	/// 削除していいかを設定
	/// </summary>
	/// <param name="value">削除していいか</param>
	void SetDelete(const bool value);
};

