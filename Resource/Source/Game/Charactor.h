#pragma once
#include "Actor.h"
#include <list>
#include "Astar.h"
#include <array>
#include "Dir.h"
#include "Team.h"
#include <vector>
#include "TimeLine.h"
#include "Status.h"
#include "CharactorType.h"

class Animator;
class MapCtrl;
class SceneController;
class Effect; 
class BattleCharactor;

class Charactor :
	public Actor
{
public:
	struct MoveInf
	{
		Dir dir;
		bool attack;
		Vector2Int mapPos;

		MoveInf() : dir(Dir::left), attack(false), mapPos(Vector2Int()) {};
		MoveInf(const Dir d, const bool at, const Vector2Int mp) : dir(d), attack(at), mapPos(mp) {};
	};

private:
	// 死亡時のアニメーションに使用するalpha値のタイムライン
	std::unique_ptr<Track<float>> _dyingAnimAlphaTL;

protected:
	struct DirInf
	{
		Vector2Int moveVec;
		std::string animName;
		float angle;

		DirInf() : moveVec(Vector2Int()), animName(""), angle(){};
		DirInf(const Vector2Int& vec2Int, const std::string& animN, const float angle)
			: moveVec(vec2Int), animName(animN), angle(angle) {};
	};

	CharactorType _charactorType;
	Team _team;
	MapCtrl& _mapCtrl;
	SceneController& _controller;
	std::vector<std::vector<std::list<Astar::ResultPos>>> _resultPosListVec2;
	std::shared_ptr<Animator> _animator;
	std::vector<std::shared_ptr<Effect>>& _effects;
	std::shared_ptr<BattleCharactor> _battleC;

	std::string _name;

	bool _isMoveAnim;	// 移動アニメーション中
	std::list<MoveInf> _moveDirList;
	std::array<DirInf, static_cast<size_t>(Dir::max)> _dirTable;

	bool _canMove;
	bool _moveActive;	// true　ある程度近づいていたら動く		false 攻撃範囲内に入ったら動く
	unsigned int _groupNum;	// 集団番号

	int _moveSpeed;
	bool _isSelect;
	Dir _dir;

	// ターン開始時のマップ座標
	Vector2Int _startMapPos;

	//	死亡アニメーション中
	bool _isDying;

	Status _status; 
	Status _startStatus;

	std::string _iconPath;	// アイコンの画像パス

	// 攻撃開始エフェクト
	std::shared_ptr<Effect> _battleStartEffect;

	// 地形効果エフェクト
	std::shared_ptr<Effect> _terrainEffect;

	// 硬直ｶｳﾝﾄ
	int _rigid;
	// 硬直が無くなったら移動を開始する
	bool _moveStandby;

	// デバッグ用
	// 範囲内の敵と範囲外の敵のカウント
	Vector2Int _targetCnt;
	int _onelineListCnt;

	// 場面後のの更新を行う
	void(Charactor::* _updater)(const Input& input);
	// 場面後のの描画を行う
	void(Charactor::* _drawer)();

	// 通常時の更新
	void NormalUpdate(const Input& input);
	void BattaleStartUpdate(const Input& input);
	void BattaleStart(Charactor* charactor);
	// 死亡時の更新
	void DyingUpdate(const Input& input);

	// 通常時の描画
	void NormalDraw();
	// 死亡時の描画
	void DyingDraw();

	void Move();

	unsigned int GetTeamColor()const;

	void CharactorDataInit(const CharactorType& type, const uint8_t& level);

	// 指定した座標に対象のマスがあるか調べる
	bool CheckMapPos(const Vector2Int mapPos, const bool attack)const;
	// 指定したマスまでのResultPosListを取得する
	std::list<Astar::ResultPos> CreateResultPosList(const Vector2Int mapPos)const;
	// resultPosListからMoveDirListを作成する
	void CreateMoveDirList(const std::list<Astar::ResultPos>& resultPosList);

public:
	Charactor(const uint8_t level, const Vector2Int& mapPos, const Team team, const unsigned int groupNum, MapCtrl& mapCtrl, SceneController& ctrl, 
		std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);
	~Charactor();

	virtual void Update(const Input& input)override;
	virtual void Draw()override;
	void DrawMovableMass(const uint8_t alpha)const;

	virtual void InitAnim();

	void AnimRestart();

	Vector2Int GetMapPos()const;
	Team GetTeam()const;

	bool MoveMapPos(const Vector2Int& mapPos);

	bool GetIsSelect()const;
	bool GetCanMove()const;
	bool GetMoveActive()const;
	bool GetMoveStandby()const;

	const Status& GetStartStatus()const;
	const Status& GetStatus()const;
	BattleStatus GetBattleStatus()const;

	bool GetIsDying()const;
	Dir GetDir()const;
	Vector2 GetCenterPos()const override;
	BattleCharactor& GetBattleC()const;
	bool GetIsMoveAnim()const;
	const Range& GetAttackRange()const;
	const std::string& GetName()const;
	const std::list<Astar::ResultPos>& GetResutlPosList(const Vector2Int& mapPos);
	unsigned int GetGroupNum()const;
	const int GetHurtPoint()const;	// 最大体力から現在の体力を引いた値を返す
	bool GetMoved()const;	// 移動済みか調べる
	CharactorType GetCharactorType()const;
	bool GetAttackStartPos(Vector2Int& attackPos, const Vector2Int& targetMapPos)const;

	void InitmapPos(const Vector2Int& mapPos);
	void SetIsSelect(const bool select);
	void SetIsDying();
	void SetDir(const Dir dir);
	void SetStatus(const Status& status);
	void InitStatus(const Status& status);
	void SetMoveActive(const bool active);
	void SetMoveStandby(const int time);
	void MoveDecision();

	void MoveEnd(const bool canMove = false, bool removeCamera = true);

	void RouteSearch();
	void TurnReset();
	void MoveCancel();

	void SearchAndMove();
	std::vector<std::vector<std::list<Astar::ResultPos>>>& GetResutlPosListVec2();

	void AddDamage(const int damage);

	// キャラクターを指定したRectの範囲に描画する
	void DrawCharactorIcon(const Rect& drawRect)const;

	// 指定した座標までの経路を描画する
	void DrawRoute(const Vector2Int& targetPos);

	// ターン開始時に地形効果を受けるならtrueを返す
	bool StartTerrainEffect();
	bool GetTerrainEffectEnd();

	// 現在の場所から攻撃できる相手の座標を返す
	std::list<Vector2Int> GetAttackPosList()const;

	// 経験値取得	レベルアップするならtrueを返す
	bool AddExp(uint8_t exp, const uint8_t expMax);

	Status GetLevelUpStatus();
};