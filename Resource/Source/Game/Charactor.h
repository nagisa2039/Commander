#pragma once
#include "Actor.h"
#include <list>
#include "Astar.h"
#include <array>
#include "Dir.h"
#include "Team.h"
#include <vector>
#include "Attribute.h"
#include "TimeLine.h"

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

	struct Status
	{
		uint8_t level;
		uint8_t health;
		uint8_t power;
		uint8_t defense;
		uint8_t speed;
		uint8_t skill;
		uint8_t move;

		Attribute attribute;		// 属性

		uint8_t fefenseCorrection;		// 守備力補正
		uint8_t avoidanceCorrection;	// 回避力補正

		Status(): level(1), health(1), power(1), defense(1), speed(1), skill(1), move(1), 
			attribute(Attribute::normal), fefenseCorrection(0), avoidanceCorrection(0.0f){};
		Status(const uint8_t lv, const uint8_t he, const uint8_t pw, const uint8_t df, 
			const uint8_t sp, const uint8_t sk, const uint8_t lu, const uint8_t mv, const Attribute at)
			: level(lv), health(he), power(pw), defense(df), speed(sp), skill(sk), move(mv), 
			attribute(at), fefenseCorrection(0), avoidanceCorrection(0) {};

		int GetDamage(const Status& target)const;	// ダメージ
		int GetHit(const Status& target)const;	// 命中率
		int GetCritical(const Status& target)const;	// 必殺率
	};

private:
	// 死亡時のアニメーションに使用するalpha値のタイムライン
	std::unique_ptr<Track<float>> _dyingAnimAlphaTL;

protected:
	struct DirInf
	{
		Vector2Int moveVec;
		std::string animName;

		DirInf() : moveVec(Vector2Int()), animName("") {};
		DirInf(const Vector2Int& vec2Int, const std::string& animN)
			: moveVec(vec2Int), animName(animN) {};
	};

	Team _team;
	MapCtrl& _mapCtrl;
	SceneController& _controller;
	std::list<Astar::ResultPos> _resutlPosList;
	std::shared_ptr<Animator> _animator;
	std::vector<std::shared_ptr<Effect>>& _effects;
	std::shared_ptr<BattleCharactor> _battleC;

	bool _isMoveAnim;	// 移動アニメーション中
	std::list<MoveInf> _moveDirList;
	std::array<DirInf, Dir::max> _dirTable;

	bool _canMove;
	bool _moveActive;	// true　ある程度近づいていたら動く		false 攻撃範囲内に入ったら動く

	int _moveSpeed;
	bool _isSelect;
	Dir _dir;

	//	死亡アニメーション中
	bool _isDying;

	Status _status; 
	Status _startStatus;

	// 攻撃範囲
	Range _attackRange;

	std::string _iconPath;	// アイコンの画像パス

	// 場面後のの更新を行う
	void(Charactor::* _updater)(const Input& input);
	// 場面後のの描画を行う
	void(Charactor::* _drawer)();

	// 通常時の更新
	void NormalUpdate(const Input& input);
	// 死亡時の更新
	void DyingUpdate(const Input& input);

	// 通常時の描画
	void NormalDraw();
	// 死亡時の描画
	void DyingDraw();

	void Move();

	unsigned int GetTeamColor()const;
	void DrawMovableMass()const;

public:
	Charactor(const uint8_t level, const Vector2Int& mapPos, const Team team, MapCtrl& mapCtrl, SceneController& ctrl, 
		std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);
	~Charactor();

	virtual void Update(const Input& input)override;
	virtual void Draw()override;

	virtual void InitAnim();

	void AnimRestart();

	Vector2Int GetMapPos()const;
	Team GetTeam()const;

	bool MoveMapPos(const Vector2Int& mapPos);

	bool GetIsSelect()const;
	bool GetCanMove()const;
	bool GetMoveActive()const;
	Status GetStartStatus()const;
	Status GetStatus()const;
	bool GetIsDying()const;
	Dir GetDir()const;
	Vector2 GetCenterPos()const override;
	BattleCharactor& GetBattleC()const;
	bool GetIsMoveAnim()const;
	Range GetAttackRange()const;

	void SetIsSelect(const bool select);
	void SetIsDying();
	void SetDir(const Dir dir);
	void SetStatus(const Status& status);

	void MoveEnd();

	void RouteSearch();
	void TurnReset();

	void SearchAndMove();
	std::list<Astar::ResultPos>& GetResutlPosList();

	void AddDamage(const int damage);
};