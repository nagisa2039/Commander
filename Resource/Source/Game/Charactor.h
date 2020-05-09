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

		Attribute attribute;		// ����

		uint8_t fefenseCorrection;		// ����͕␳
		uint8_t avoidanceCorrection;	// ���͕␳

		Status(): level(1), health(1), power(1), defense(1), speed(1), skill(1), move(1), 
			attribute(Attribute::normal), fefenseCorrection(0), avoidanceCorrection(0.0f){};
		Status(const uint8_t lv, const uint8_t he, const uint8_t pw, const uint8_t df, 
			const uint8_t sp, const uint8_t sk, const uint8_t lu, const uint8_t mv, const Attribute at)
			: level(lv), health(he), power(pw), defense(df), speed(sp), skill(sk), move(mv), 
			attribute(at), fefenseCorrection(0), avoidanceCorrection(0) {};

		int GetDamage(const Status& target)const;	// �_���[�W
		int GetHit(const Status& target)const;	// ������
		int GetCritical(const Status& target)const;	// �K�E��
	};

private:
	// ���S���̃A�j���[�V�����Ɏg�p����alpha�l�̃^�C�����C��
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

	bool _isMoveAnim;	// �ړ��A�j���[�V������
	std::list<MoveInf> _moveDirList;
	std::array<DirInf, Dir::max> _dirTable;

	bool _canMove;
	bool _moveActive;	// true�@������x�߂Â��Ă����瓮��		false �U���͈͓��ɓ������瓮��

	int _moveSpeed;
	bool _isSelect;
	Dir _dir;

	//	���S�A�j���[�V������
	bool _isDying;

	Status _status; 
	Status _startStatus;

	// �U���͈�
	Range _attackRange;

	std::string _iconPath;	// �A�C�R���̉摜�p�X

	// ��ʌ�̂̍X�V���s��
	void(Charactor::* _updater)(const Input& input);
	// ��ʌ�̂̕`����s��
	void(Charactor::* _drawer)();

	// �ʏ펞�̍X�V
	void NormalUpdate(const Input& input);
	// ���S���̍X�V
	void DyingUpdate(const Input& input);

	// �ʏ펞�̕`��
	void NormalDraw();
	// ���S���̕`��
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