#pragma once
#include "Geometry.h"
#include "Dir.h"
#include <memory>
#include <string>
#include <array>
#include <functional>
#include "TimeLine.h"

class Animator;
class Effect;
class Charactor;
class Camera;
class FileSystem;

class BattleScene;

/// <summary>
/// 戦闘用キャラクター
/// </summary>
class BattleCharactor
{
public:
	/// <summary>
	/// ダメージ種
	/// </summary>
	enum class damageType
	{
		// 当たっていない
		none,		
		// 当てた
		damage,		
		// 会心攻撃を当てた
		critical,	
		max
	};

protected:
	// 所有者のキャラクター
	Charactor& _selfChar;
	// 対戦相手のキャラクター
	BattleCharactor* _targetChar;
	// カメラ
	Camera& _camera;

	// 方向
	Dir _dir;
	// 開始座標
	Vector2 _startPos;
	// 現在の座標
	Vector2 _pos;
	// 表示サイズ
	Size _size;
	// アニメーション管理クラス
	std::shared_ptr<Animator> _animator;

	// 攻撃アニメーショントラック
	std::unique_ptr<Track_i> _attackAnimX;
	// 待ちカウントトラック
	std::unique_ptr<Track_i> _waitT;
	// 攻撃エフェクト
	std::shared_ptr<Effect> _attackEffect;

	// 表示表体力
	uint8_t _animHealth;
	// 体力アニメーション用ｶｳﾝﾄ
	int _animHealthCnt;

	// 攻撃当たり判定用
	damageType _gaveDamageType;
	// 攻撃エフェクト発生関数
	using attackEffectFunc_t = std::function<void(BattleScene&, const Vector2Int&)>;
	// 攻撃エフェクト発生関数テーブル
	std::array<attackEffectFunc_t, static_cast<size_t>(damageType::max)> _attackEffectFuncs;

	// 与えたダメージ
	unsigned int _givenDamage;	

	// HP表示用マスク
	int _hpDotMaskH;

	// 受けたダメージタイプ表示変数
	damageType _receiveDamageType;

	// 更新用関数ポインタ変数
	void(BattleCharactor::* _updater)(BattleScene&);
	// waitの次のupdate関数ポインタ変数
	void(BattleCharactor::* _waitNextUpdater)(BattleScene&);	

	/// <summary>
	/// 待ち状態の更新
	/// </summary>
	/// <param name="battleScene">戦闘シーン情報</param>
	void WaitUpdate(BattleScene& battleScene);

	/// <summary>
	/// 通常時の更新
	/// </summary>
	/// <param name="battleScene">戦闘シーン情報</param>
	void NormalUpdate(BattleScene& battleScene);

	/// <summary>
	/// カットイン時の更新
	/// </summary>
	/// <param name="battleScene">戦闘シーン情報</param>
	void CutInUpdate(BattleScene& battleScene);

	/// <summary>
	/// 待ち状態への移行
	/// </summary>
	/// <param name="nextUpdate">次の状態</param>
	void WaitStart(void(BattleCharactor::* nextUpdate)(BattleScene&));

	/// <summary>
	/// 名前の表示
	/// </summary>
	/// <param name="teamString">チーム名</param>
	/// <param name="nameWindowRect">表示枠矩形</param>
	/// <param name="fileSystem">ファイル管理クラス</param>
	/// <param name="fontHandle">フォントハンドル</param>
	void DrawName(const char* teamString, Rect& nameWindowRect, FileSystem& fileSystem, int fontHandle);

	/// <summary>
	/// パラメーター表示
	/// </summary>
	/// <param name="teamString"></param>
	/// <param name="fileSystem">ファイル管理クラス</param>
	/// <param name="paramWindowRect">表示枠矩形</param>
	void DrawParameter(const char* teamString, FileSystem& fileSystem, Rect& paramWindowRect);

	/// <summary>
	/// HP表示
	/// </summary>
	/// <param name="windowRect">表示枠矩形</param>
	/// <param name="fontHandle">フォントハンドル</param>
	void DrawHP(Rect& windowRect, int fontHandle);

	/// <summary>
	/// 武器名表示
	/// </summary>
	/// <param name="fileSystem">ファイル管理クラス</param>
	/// <param name="weaponNameRect">表示枠矩形</param>
	void DrawWeaponName(FileSystem& fileSystem, Rect& weaponNameRect);

public:
	/// <param name="charactor">所有者のキャラクター</param>
	/// <param name="imageHandle">キャラクター画像のハンドル</param>
	/// <param name="camera">カメラ</param>
	BattleCharactor(Charactor& charactor, const int imageHandle, Camera& camera);
	~BattleCharactor();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="startPos">開始座標</param>
	/// <param name="dir">向き</param>
	/// <param name="target">戦闘相手</param>
	void Init(const Vector2& startPos, const Dir dir, BattleCharactor* target);

	/// <summary>
	/// アニメーション更新
	/// </summary>
	void AnimUpdate();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="battleScene"></param>
	void Update(BattleScene& battleScene);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// UI更新
	/// </summary>
	void UIAnimUpdate();

	/// <summary>
	/// UI描画
	/// </summary>
	void UIDraw();

	/// <summary>
	/// 攻撃アニメーションを開始する
	/// </summary>
	/// <param name="battleScene"></param>
	void StartAttackAnim(BattleScene& battleScene);

	/// <summary>
	/// 攻撃アニメーションが終了したかを確認
	/// </summary>
	bool GetAttackAnimEnd();

	/// <summary>
	/// HPアニメーションを開始する
	/// </summary>
	void StartHPAnim();

	/// <summary>
	/// HPアニメーションが終了したかを確認
	/// </summary>
	bool GetHPAnimEnd();

	/// <summary>
	/// 表示サイズを取得
	/// </summary>
	Size GetSize()const;

	/// <summary>
	/// 中心座標を取得
	/// </summary>
	Vector2Int GetCenterPos()const;

	/// <summary>
	/// 足元の座標を取得
	/// </summary>
	Vector2Int GetPos()const;	

	/// <summary>
	/// 開始位置の足元座標を取得
	/// </summary>
	Vector2Int GetStartPos()const;	

	/// <summary>
	/// 所有者のキャラクターを取得
	/// </summary>
	Charactor& GetCharacotr();

	/// <summary>
	/// 与えたダメージ量を取得
	/// </summary>
	const unsigned int& GetGivenDamage()const;

	/// <summary>
	/// 戦闘相手を取得
	/// </summary>
	BattleCharactor* GetTargetBattleCharactor();

	/// <summary>
	/// 与えたダメージ種を取得
	/// </summary>
	damageType GetDamageType()const;

	/// <summary>
	/// 描画する座標(中央下)
	/// </summary>
	/// <param name="startPos">開始座標</param>
	void SetStartPos(const Vector2& startPos);	

	/// <summary>
	/// left(攻撃)かright(守備)の設定
	/// </summary>
	/// <param name="dir">向き</param>
	void SetDir(const Dir dir);

	/// <summary>
	/// 戦う相手のポインターを設定
	/// </summary>
	/// <param name="target">戦う相手のポインター</param>
	void SetTargetCharactor(BattleCharactor* target);

	/// <summary>
	/// 受けたダメージタイプを設定
	/// </summary>
	/// <param name="dt">ダメージ種</param>
	void SetReceiveDamageType(const damageType dt);

	/// <summary>
	/// 与えるダメージ量の設定
	/// </summary>
	/// <param name="value">ダメージ量</param>
	void SetGivenDamage(const unsigned int value);
};