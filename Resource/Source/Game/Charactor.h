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
	// ���S���̃A�j���[�V�����Ɏg�p����alpha�l�̃^�C�����C��
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

	bool _isMoveAnim;	// �ړ��A�j���[�V������
	std::list<MoveInf> _moveDirList;
	std::array<DirInf, static_cast<size_t>(Dir::max)> _dirTable;

	bool _canMove;
	bool _moveActive;	// true�@������x�߂Â��Ă����瓮��		false �U���͈͓��ɓ������瓮��
	unsigned int _groupNum;	// �W�c�ԍ�

	int _moveSpeed;
	bool _isSelect;
	Dir _dir;

	// �^�[���J�n���̃}�b�v���W
	Vector2Int _startMapPos;

	//	���S�A�j���[�V������
	bool _isDying;

	Status _status; 
	Status _startStatus;

	std::string _iconPath;	// �A�C�R���̉摜�p�X

	// �U���J�n�G�t�F�N�g
	std::shared_ptr<Effect> _battleStartEffect;

	// �n�`���ʃG�t�F�N�g
	std::shared_ptr<Effect> _terrainEffect;

	// �d������
	int _rigid;
	// �d���������Ȃ�����ړ����J�n����
	bool _moveStandby;

	// �f�o�b�O�p
	// �͈͓��̓G�Ɣ͈͊O�̓G�̃J�E���g
	Vector2Int _targetCnt;
	int _onelineListCnt;

	// ��ʌ�̂̍X�V���s��
	void(Charactor::* _updater)(const Input& input);
	// ��ʌ�̂̕`����s��
	void(Charactor::* _drawer)();

	// �ʏ펞�̍X�V
	void NormalUpdate(const Input& input);
	void BattaleStartUpdate(const Input& input);
	void BattaleStart(Charactor* charactor);
	// ���S���̍X�V
	void DyingUpdate(const Input& input);

	// �ʏ펞�̕`��
	void NormalDraw();
	// ���S���̕`��
	void DyingDraw();

	void Move();

	unsigned int GetTeamColor()const;

	void CharactorDataInit(const CharactorType& type, const uint8_t& level);

	// �w�肵�����W�ɑΏۂ̃}�X�����邩���ׂ�
	bool CheckMapPos(const Vector2Int mapPos, const bool attack)const;
	// �w�肵���}�X�܂ł�ResultPosList���擾����
	std::list<Astar::ResultPos> CreateResultPosList(const Vector2Int mapPos)const;
	// resultPosList����MoveDirList���쐬����
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
	const int GetHurtPoint()const;	// �ő�̗͂��猻�݂̗̑͂��������l��Ԃ�
	bool GetMoved()const;	// �ړ��ς݂����ׂ�
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

	// �L�����N�^�[���w�肵��Rect�͈̔͂ɕ`�悷��
	void DrawCharactorIcon(const Rect& drawRect)const;

	// �w�肵�����W�܂ł̌o�H��`�悷��
	void DrawRoute(const Vector2Int& targetPos);

	// �^�[���J�n���ɒn�`���ʂ��󂯂�Ȃ�true��Ԃ�
	bool StartTerrainEffect();
	bool GetTerrainEffectEnd();

	// ���݂̏ꏊ����U���ł��鑊��̍��W��Ԃ�
	std::list<Vector2Int> GetAttackPosList()const;

	// �o���l�擾	���x���A�b�v����Ȃ�true��Ԃ�
	bool AddExp(uint8_t exp, const uint8_t expMax);

	Status GetLevelUpStatus();
};