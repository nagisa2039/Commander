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
		uint8_t luck;
		uint8_t move;

		Attribute attribute;

		Status(): level(1), health(1), power(1), defense(1), speed(1), skill(1), luck(1), move(1), attribute(Attribute::normal){};
		Status(const uint8_t lv, const uint8_t he, const uint8_t pw, const uint8_t df, 
			const uint8_t sp, const uint8_t sk, const uint8_t lu, const uint8_t mv, const Attribute at)
			: level(lv), health(he), power(pw), defense(df), speed(sp), skill(sk), luck(lu), move(mv), attribute(at){};

		int GetDamage(const Status& target)const;	// ダメージ
		int GetHit(const Status& target)const;	// 命中率
		int GetCritical(const Status& target)const;	// 必殺率
	};

private:
	// 死亡時のアニメーションに使用するalpha値のタイムライン
	std::unique_ptr<TimeLine<float>> _dyingAnimAlphaTL;

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

	//	死亡アニメーション中
	bool _isDying;

	Status _status; 
	Status _startStatus;

	std::string _iconPath;	// アイコンの画像パス

	// 場面後のの更新を行う
	void(Charactor::* _updater)();
	// 場面後のの描画を行う
	void(Charactor::* _drawer)(const Camera& camera);

	// 通常時の更新
	void NormalUpdate();
	// 死亡時の更新
	void DyingUpdate();

	// 通常時の描画
	void NormalDraw(const Camera& camera);
	// 死亡時の描画
	void DyingDraw(const Camera& camera);

	void Move();

	unsigned int GetTeamColor()const;
	void DrawMovableMass(const Camera& camera)const;

public:
	Charactor(const uint8_t level, const Vector2Int& mapPos, const Team team, MapCtrl& mapCtrl, SceneController& ctrl, 
		std::vector<std::shared_ptr<Effect>>& effects);
	~Charactor();

	virtual void Update(const Input& input)override;
	virtual void Draw(const Camera& camera)override;
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