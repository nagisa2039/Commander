#pragma once
#include "Actor.h"
#include <list>
#include <array>
#include "Dir.h"
#include "Team.h"
#include <vector>
#include "TimeLine.h"
#include "Status.h"
#include "CharactorType.h"
#include "Astar.h"

class Animator;
class MapCtrl;
class SceneController;
class Effect; 
class BattleCharactor;
class RouteManager;

/// <summary>
/// キャラクター基底クラス
/// </summary>
class Charactor :
	public Actor
{
private:
	// 死亡時のアニメーションに使用するalpha値のタイムライン
	std::unique_ptr<Track<float>> _dyingAnimAlphaTL;

	// 向き情報
	struct DirInf
	{
		// 移動方向
		Vector2Int moveVec;
		// アニメーション名
		std::string animName;
		// 回転量
		float angle = 0.0f;
	};

	// キャラクタータイプ
	CharactorType _charactorType;
	// チーム
	Team _team;
	// マップ管理クラス
	MapCtrl& _mapCtrl;
	// シーン管理クラス
	SceneController& _controller;
	// アニメーション再生クラス
	std::shared_ptr<Animator> _animator;
	// エフェクト管理配列
	std::vector<std::shared_ptr<Effect>>& _effects;
	// 戦闘用キャラクタークラス
	std::shared_ptr<BattleCharactor> _battleC;
	// 移動経路管理クラス
	std::shared_ptr<RouteManager> _routeManager;

	// 名前
	std::string _name;

	// 移動アニメーション中
	bool _isMoveAnim;	
	// 移動情報テーブル
	std::array<DirInf, static_cast<size_t>(Dir::max)> _dirTable;

	// 移動可能か
	bool _canMove;
	// ユニットがアクティブ状態を表す(移動範囲外でも動くか)
	bool _moveActive;
	// ユニットグループ番号
	unsigned int _groupNum;

	// 移動時のSE 
	int _mouveSEH;
	// 移動速度
	int _moveSpeed;
	// 選択中か
	bool _isSelect;
	// 向き
	Dir _dir;

	// ターン開始時のマップ座標
	Vector2Int _startMapPos;

	//	死亡アニメーション中
	bool _isDying;
	// DyingUpdateの初回のみに行いたい処理用フラグ
	bool _beginDying;

	// 現在のステータス
	Status _status;
	// 開始時のステータス
	Status _startStatus;

	// アイコンの画像パス
	std::string _iconPath;

	// 攻撃開始エフェクト
	std::shared_ptr<Effect> _battleStartEffect;

	// 地形効果エフェクト
	std::shared_ptr<Effect> _terrainEffect;

	// 移動待ちカウンド
	int _moveStandbyCnt;

	// 場面ごとの更新を行う
	void(Charactor::* _updater)(const Input& input);
	// 場面ごとの描画を行う
	void(Charactor::* _drawer)();

	/// <summary>
	/// 通常時の更新
	/// </summary>
	/// <param name="input">入力</param>
	void NormalUpdate(const Input& input);

	/// <summary>
	/// 戦闘開始時
	/// </summary>
	/// <param name="input">入力</param>
	void BattaleStartUpdate(const Input& input);

	/// <summary>
	/// 死亡時の更新
	/// </summary>
	/// <param name="input">入力</param>
	void DyingUpdate(const Input& input);

	/// <summary>
	/// 通常時の描画
	/// </summary>
	void NormalDraw();

	/// <summary>
	/// 死亡時の描画
	/// </summary>
	void DyingDraw();

	/// <summary>
	/// 戦闘を開始する
	/// </summary>
	/// <param name="targetCharactor">相手のキャラクター</param>
	void BattaleStart(Charactor* targetCharactor);

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// チームの色を取得
	/// </summary>
	/// <returns>色</returns>
	unsigned int GetTeamColor()const;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="type">キャラクタータイプ</param>
	/// <param name="level">レベル</param>
	void CharactorDataInit(const CharactorType& type, const uint8_t& level);

public:
	/// <param name="type">キャラクタータイプ</param>
	/// <param name="level">レベル</param>
	/// <param name="mapPos">マップ上の座標(マス目数)</param>
	/// <param name="team">チーム情報</param>
	/// <param name="groupNum">グループ番号</param>
	/// <param name="mapCtrl">マップ管理クラス</param>
	/// <param name="ctrl">シーン管理クラス</param>
	/// <param name="effects">エフェクト管理配列</param>
	/// <param name="camera">カメラ</param>
	Charactor(const CharactorType type, const uint8_t level, const Vector2Int& mapPos, const Team team, const unsigned int groupNum, MapCtrl& mapCtrl, SceneController& ctrl,
		std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);
	virtual ~Charactor();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力情報</param>
	virtual void Update(const Input& input)override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw()override;

	/// <summary>
	/// アニメーション初期化
	/// </summary>
	virtual void InitAnim();

	/// <summary>
	/// アニメーションのリスタート
	/// </summary>
	void AnimRestart();

	/// <summary>
	/// マップ上の座標取得
	/// </summary>
	Vector2Int GetMapPos()const;

	/// <summary>
	/// チーム情報の取得
	/// </summary>
	Team GetTeam()const;

	/// <summary>
	/// 指定したマップ座標に移動
	/// </summary>
	/// <param name="mapPos">マップ座標</param>
	/// <returns>移動可能か</returns>
	bool MoveMapPos(const Vector2Int& mapPos);

	/// <summary>
	/// 移動可能かを返す
	/// </summary>
	bool GetCanMove()const;

	/// <summary>
	/// ユニットがアクティブ状態を表す
	/// </summary>
	bool GetMoveActive()const;

	/// <summary>
	/// 硬直状態か
	/// </summary>
	bool GetMoveStandby()const;

	/// <summary>
	/// 開始時のステータスを取得
	/// </summary>
	const Status& GetStartStatus()const;

	/// <summary>
	/// 現在のステータスを取得
	/// </summary>
	const Status& GetStatus()const;

	/// <summary>
	/// 戦闘時のステータス取得
	/// </summary>
	BattleStatus GetBattleStatus()const;

	/// <summary>
	/// 死にゆく状態か
	/// </summary>
	bool GetIsDying()const;

	/// <summary>
	/// 向きの取得
	/// </summary>
	Dir GetDir()const;

	/// <summary>
	/// 中心座標の取得
	/// </summary>
	Vector2 GetCenterPos()const override;

	/// <summary>
	/// 戦闘用キャラクターの取得
	/// </summary>
	BattleCharactor& GetBattleC()const;

	/// <summary>
	/// 移動アニメーション中か
	/// </summary>
	bool GetIsMoveAnim()const;

	/// <summary>
	/// 攻撃範囲の取得
	/// </summary>
	const Range& GetAttackRange()const;

	/// <summary>
	/// 名前の取得
	/// </summary>
	const std::string& GetName()const;

	/// <summary>
	/// グループ番号の取得
	/// </summary>
	unsigned int GetGroupNum()const;

	/// <summary>
	/// 最大体力から現在の体力を引いた値を返す
	/// </summary>
	const int GetHurtPoint()const;

	/// <summary>
	/// 移動済みか調べる
	/// </summary>
	bool GetMoved()const;

	/// <summary>
	/// キャラクタータイプの取得
	/// </summary>
	CharactorType GetCharactorType()const;

	/// <summary>
	/// 移動経路管理クラスの取得
	/// </summary>
	std::shared_ptr<RouteManager> GetRouteManager();

	/// <summary>
	/// マップ座標を基に初期化
	/// </summary>
	/// <param name="mapPos">マップ座標</param>
	void InitmapPos(const Vector2Int& mapPos);

	/// <summary>
	/// 選択中かを設定
	/// </summary>
	/// <param name="select">選択中か</param>
	void SetIsSelect(const bool select);

	/// <summary>
	/// 死にゆく状態にする
	/// </summary>
	void SetIsDying();

	/// <summary>
	/// 向きの設定
	/// </summary>
	/// <param name="dir">向き</param>
	void SetDir(const Dir dir);

	/// <summary>
	/// ステータスを設定
	/// </summary>
	/// <param name="status">ステータス</param>
	void SetStatus(const Status& status);

	/// <summary>
	/// ステータスの初期化
	/// </summary>
	/// <param name="status">ステータス</param>
	void InitStatus(const Status& status);

	/// <summary>
	/// ユニットがアクティブ状態かを設定
	/// </summary>
	/// <param name="active">アクティブ状態か</param>
	void SetMoveActive(const bool active);

	/// <summary>
	/// 待機フレーム数の設定
	/// </summary>
	/// <param name="time">待機フレーム数</param>
	void SetMoveStandby(const int time);

	/// <summary>
	/// 移動を決定する
	/// </summary>
	void MoveDecision();

	/// <summary>
	/// 移動を終了する
	/// </summary>
	/// <param name="canMove">再移動するか</param>
	/// <param name="removeCamera">カメラのターゲットから外すか</param>
	void MoveEnd(const bool canMove = false, bool removeCamera = true);

	/// <summary>
	/// 移動アニメーションを止める
	/// </summary>
	void StopMoveAnim();

	/// <summary>
	/// 移動経路検索
	/// </summary>
	void RouteSearch();

	/// <summary>
	/// ターンごとのリセット
	/// </summary>
	void TurnReset();

	/// <summary>
	/// 移動をキャンセルする
	/// </summary>
	void MoveCancel();

	/// <summary>
	/// ダメージを与える
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	void AddDamage(const int damage);

	/// <summary>
	/// キャラクターを指定したRectの範囲に描画する
	/// </summary>
	/// <param name="drawRect">描画矩形</param>
	void DrawCharactorIcon(const Rect& drawRect)const;

	/// <summary>
	/// ターン開始時に地形効果を受けるならtrueを返す
	/// </summary>
	/// <returns>成否</returns>
	bool StartTerrainEffect();

	/// <summary>
	/// 地形エフェクトの再生が終わっているか
	/// </summary>
	bool GetTerrainEffectEnd();

	/// <summary>
	/// 方向情報テーブルの取得
	/// </summary>
	const std::array<DirInf, static_cast<size_t>(Dir::max)>& GetDirTable()const;
};