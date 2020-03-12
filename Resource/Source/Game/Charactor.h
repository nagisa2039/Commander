#pragma once
#include "Actor.h"
#include <list>
#include "Astar.h"
#include <array>
#include "Dir.h"
#include "Team.h"
#include <vector>

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

		MoveInf() : dir(Dir::left), attack(false) {};
		MoveInf(const Dir d, const bool at) : dir(d), attack(at) {};
	};

	struct Status
	{
		uint8_t level;
		uint8_t health;
		uint8_t power;
		uint8_t defense;
		uint8_t speed;
		uint8_t skill;
		uint8_t luck;
		uint8_t move;

		Status(): level(1), health(1), power(1), defense(1), speed(1), skill(1), luck(1), move(1) {};
		Status(const uint8_t lv, const uint8_t he, const uint8_t pw, const uint8_t df, 
			const uint8_t sp, const uint8_t sk, const uint8_t lu, const uint8_t mv)
			: level(lv), health(he), power(pw), defense(df), speed(sp), skill(sk), luck(lu), move(mv) {};

		int GetDamage(const Status& target)const;	// ダメージ
		int GetHit(const Status& target)const;	// 命中率
		int GetCritical(const Status& target)const;	// 必殺率
	};

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

	int _moveSpeed;
	bool _isSelect;
	Dir _dir;
	bool _isDying;//	死亡アニメーション中

	Status _status; 
	Status _startStatus;

	void Move();

	unsigned int GetTeamColor()const;
	void DrawMovableMass(const Camera& camera)const;

public:
	Charactor(const Vector2Int& mapPos, const Team team, MapCtrl& mapCtrl, SceneController& ctrl, 
		std::vector<std::shared_ptr<Effect>>& effects);
	~Charactor();

	virtual void Update(const Input& input)override = 0;
	virtual void Draw(const Camera& camera)override = 0;
	void AnimRestart();

	Vector2Int GetMapPos()const;
	Team GetTeam()const;

	bool MoveMapPos(const Vector2Int& mapPos);

	bool GetIsSelect()const;
	bool GetCanMove()const;
	Status GetStartStatus()const;
	Status GetStatus()const;
	bool GetIsDying()const;
	Dir GetDir()const;
	Vector2Int GetCenterPos()const;
	BattleCharactor& GetBattleC()const;
	bool GetIsMoveAnim()const;

	void SetIsSelect(const bool select);
	void SetIsDying(const bool dying);
	void SetDir(const Dir dir);
	void SetStatus(const Status& status);

	void MoveEnd();

	void RouteSearch();
	void TurnReset();

	void SearchAndMove();
	std::list<Astar::ResultPos>& GetResutlPosList();
};

class BattleScene;

class BattleCharactor
{
protected:
	Charactor& _selfChar;
	BattleCharactor* _targetChar;

	Dir _dir;
	Vector2 _startPos;
	Vector2 _pos;
	Size _size;
	std::shared_ptr<Animator> _animator;
	bool _createEffect;

	Size _uiSize;

	int _attackAnimCnt;
	int _attackAnimCntMax;

	uint8_t _animHealth;
	int _animHealthCnt;

public:
	BattleCharactor(Charactor& charactor);
	~BattleCharactor();

	virtual void Init(const Vector2& startPos, const Dir dir, BattleCharactor* target);

	virtual void AnimUpdate();
	virtual void AttackUpdate(BattleScene& battleScene);
	virtual void Draw();

	virtual void UIAnimUpdate();
	virtual void UIDraw();

	void StartAttackAnim();
	bool GetAttackAnimEnd();

	void StartHPAnim();
	bool GetHPAnimEnd();


	Size GetSize()const;
	Vector2Int GetCenterPos()const;
	Charactor& GetSelfCharacotr();

	void SetStartPos(const Vector2& startPos);	// 描画する座標(中央下)
	virtual void SetDir(const Dir dir) = 0;	//  攻撃か守備の設定
	void SetTargetCharactor(BattleCharactor* target);	// 戦う相手のポインター

	void AddDamage(const int damage);	// ダメージを与える

	virtual std::shared_ptr<Effect> CreateAttackEffect(const Vector2Int& effectPos) = 0;
};

