#pragma once
#include "../Utility/Geometry.h"
#include <memory>
#include <deque>

class Actor;

/// <summary>
/// カメラ
/// </summary>
class Camera
{
private:
	// 描画範囲矩形
	Rect _rect;

	// 追跡ターゲットDeque
	std::deque<Actor*> _targets;

	// 座標
	Vector2 _pos;
	// 移動範囲矩形
	Rect _limitRect;
	// 追跡ターゲットからのオフセット
	Vector2 _offset;

	// 更新関数ポインタ
	void(Camera::* _follower)();

	/// <summary>
	/// ターゲットに向かって補完しながらついていく
	/// </summary>
	void NormalFollow();

	/// <summary>
	/// ターゲットが範囲外に行こうとした時だけついていく
	/// </summary>
	void LooseFollow();

public:
	/// <param name="rect">描画矩形</param>
	Camera(const Rect& rect);
	~Camera();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 追跡ターゲットの追加
	/// </summary>
	/// <param name="target">追跡ターゲット</param>
	void AddTargetActor(Actor* target);

	/// <summary>
	/// 追跡ターゲットの削除
	/// </summary>
	/// <param name="target">追跡ターゲット</param>
	void RemoveTargetActor(Actor* target);

	/// <summary>
	/// 一番最後に追加された追跡ターゲットを削除
	/// </summary>
	void PopTargetActor();

	/// <summary>
	/// 追跡ターゲットを全削除
	/// </summary>
	void ClearTargetActor();

	/// <summary>
	/// カメラオフセットの取得
	/// </summary>
	Vector2Int GetCameraOffset()const;

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPos(const Vector3& pos);

	/// <summary>
	/// 移動制限の設定
	/// </summary>
	/// <param name="rect">移動制限</param>
	void SetLimitRect(const Rect& rect);

	/// <summary>
	/// LooseFollowに切り替える
	/// </summary>
	/// <param name="loose">切り替えるか</param>
	void SetLooseFollow(bool loose);

	/// <summary>
	/// 描画矩形の取得
	/// </summary>
	const Rect& GetRect()const;

	/// <summary>
	/// カメラオフセットの設定
	/// </summary>
	/// <param name="offset">カメラオフセット</param>
	void SetOffset(const Vector2& offset);
};

