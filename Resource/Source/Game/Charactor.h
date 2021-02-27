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
/// �L�����N�^�[���N���X
/// </summary>
class Charactor :
	public Actor
{
private:
	// ���S���̃A�j���[�V�����Ɏg�p����alpha�l�̃^�C�����C��
	std::unique_ptr<Track<float>> _dyingAnimAlphaTL;

	// �������
	struct DirInf
	{
		// �ړ�����
		Vector2Int moveVec;
		// �A�j���[�V������
		std::string animName;
		// ��]��
		float angle = 0.0f;
	};

	// �L�����N�^�[�^�C�v
	CharactorType _charactorType;
	// �`�[��
	Team _team;
	// �}�b�v�Ǘ��N���X
	MapCtrl& _mapCtrl;
	// �V�[���Ǘ��N���X
	SceneController& _controller;
	// �A�j���[�V�����Đ��N���X
	std::shared_ptr<Animator> _animator;
	// �G�t�F�N�g�Ǘ��z��
	std::vector<std::shared_ptr<Effect>>& _effects;
	// �퓬�p�L�����N�^�[�N���X
	std::shared_ptr<BattleCharactor> _battleC;
	// �ړ��o�H�Ǘ��N���X
	std::shared_ptr<RouteManager> _routeManager;

	// ���O
	std::string _name;

	// �ړ��A�j���[�V������
	bool _isMoveAnim;	
	// �ړ����e�[�u��
	std::array<DirInf, static_cast<size_t>(Dir::max)> _dirTable;

	// �ړ��\��
	bool _canMove;
	// ���j�b�g���A�N�e�B�u��Ԃ�\��(�ړ��͈͊O�ł�������)
	bool _moveActive;
	// ���j�b�g�O���[�v�ԍ�
	unsigned int _groupNum;

	// �ړ�����SE 
	int _mouveSEH;
	// �ړ����x
	int _moveSpeed;
	// �I�𒆂�
	bool _isSelect;
	// ����
	Dir _dir;

	// �^�[���J�n���̃}�b�v���W
	Vector2Int _startMapPos;

	//	���S�A�j���[�V������
	bool _isDying;
	// DyingUpdate�̏���݂̂ɍs�����������p�t���O
	bool _beginDying;

	// ���݂̃X�e�[�^�X
	Status _status;
	// �J�n���̃X�e�[�^�X
	Status _startStatus;

	// �A�C�R���̉摜�p�X
	std::string _iconPath;

	// �U���J�n�G�t�F�N�g
	std::shared_ptr<Effect> _battleStartEffect;

	// �n�`���ʃG�t�F�N�g
	std::shared_ptr<Effect> _terrainEffect;

	// �ړ��҂��J�E���h
	int _moveStandbyCnt;

	// ��ʂ��Ƃ̍X�V���s��
	void(Charactor::* _updater)(const Input& input);
	// ��ʂ��Ƃ̕`����s��
	void(Charactor::* _drawer)();

	/// <summary>
	/// �ʏ펞�̍X�V
	/// </summary>
	/// <param name="input">����</param>
	void NormalUpdate(const Input& input);

	/// <summary>
	/// �퓬�J�n��
	/// </summary>
	/// <param name="input">����</param>
	void BattaleStartUpdate(const Input& input);

	/// <summary>
	/// ���S���̍X�V
	/// </summary>
	/// <param name="input">����</param>
	void DyingUpdate(const Input& input);

	/// <summary>
	/// �ʏ펞�̕`��
	/// </summary>
	void NormalDraw();

	/// <summary>
	/// ���S���̕`��
	/// </summary>
	void DyingDraw();

	/// <summary>
	/// �퓬���J�n����
	/// </summary>
	/// <param name="targetCharactor">����̃L�����N�^�[</param>
	void BattaleStart(Charactor* targetCharactor);

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// �`�[���̐F���擾
	/// </summary>
	/// <returns>�F</returns>
	unsigned int GetTeamColor()const;

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="type">�L�����N�^�[�^�C�v</param>
	/// <param name="level">���x��</param>
	void CharactorDataInit(const CharactorType& type, const uint8_t& level);

public:
	/// <param name="type">�L�����N�^�[�^�C�v</param>
	/// <param name="level">���x��</param>
	/// <param name="mapPos">�}�b�v��̍��W(�}�X�ڐ�)</param>
	/// <param name="team">�`�[�����</param>
	/// <param name="groupNum">�O���[�v�ԍ�</param>
	/// <param name="mapCtrl">�}�b�v�Ǘ��N���X</param>
	/// <param name="ctrl">�V�[���Ǘ��N���X</param>
	/// <param name="effects">�G�t�F�N�g�Ǘ��z��</param>
	/// <param name="camera">�J����</param>
	Charactor(const CharactorType type, const uint8_t level, const Vector2Int& mapPos, const Team team, const unsigned int groupNum, MapCtrl& mapCtrl, SceneController& ctrl,
		std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);
	virtual ~Charactor();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	virtual void Update(const Input& input)override;

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw()override;

	/// <summary>
	/// �A�j���[�V����������
	/// </summary>
	virtual void InitAnim();

	/// <summary>
	/// �A�j���[�V�����̃��X�^�[�g
	/// </summary>
	void AnimRestart();

	/// <summary>
	/// �}�b�v��̍��W�擾
	/// </summary>
	Vector2Int GetMapPos()const;

	/// <summary>
	/// �`�[�����̎擾
	/// </summary>
	Team GetTeam()const;

	/// <summary>
	/// �w�肵���}�b�v���W�Ɉړ�
	/// </summary>
	/// <param name="mapPos">�}�b�v���W</param>
	/// <returns>�ړ��\��</returns>
	bool MoveMapPos(const Vector2Int& mapPos);

	/// <summary>
	/// �ړ��\����Ԃ�
	/// </summary>
	bool GetCanMove()const;

	/// <summary>
	/// ���j�b�g���A�N�e�B�u��Ԃ�\��
	/// </summary>
	bool GetMoveActive()const;

	/// <summary>
	/// �d����Ԃ�
	/// </summary>
	bool GetMoveStandby()const;

	/// <summary>
	/// �J�n���̃X�e�[�^�X���擾
	/// </summary>
	const Status& GetStartStatus()const;

	/// <summary>
	/// ���݂̃X�e�[�^�X���擾
	/// </summary>
	const Status& GetStatus()const;

	/// <summary>
	/// �퓬���̃X�e�[�^�X�擾
	/// </summary>
	BattleStatus GetBattleStatus()const;

	/// <summary>
	/// ���ɂ䂭��Ԃ�
	/// </summary>
	bool GetIsDying()const;

	/// <summary>
	/// �����̎擾
	/// </summary>
	Dir GetDir()const;

	/// <summary>
	/// ���S���W�̎擾
	/// </summary>
	Vector2 GetCenterPos()const override;

	/// <summary>
	/// �퓬�p�L�����N�^�[�̎擾
	/// </summary>
	BattleCharactor& GetBattleC()const;

	/// <summary>
	/// �ړ��A�j���[�V��������
	/// </summary>
	bool GetIsMoveAnim()const;

	/// <summary>
	/// �U���͈͂̎擾
	/// </summary>
	const Range& GetAttackRange()const;

	/// <summary>
	/// ���O�̎擾
	/// </summary>
	const std::string& GetName()const;

	/// <summary>
	/// �O���[�v�ԍ��̎擾
	/// </summary>
	unsigned int GetGroupNum()const;

	/// <summary>
	/// �ő�̗͂��猻�݂̗̑͂��������l��Ԃ�
	/// </summary>
	const int GetHurtPoint()const;

	/// <summary>
	/// �ړ��ς݂����ׂ�
	/// </summary>
	bool GetMoved()const;

	/// <summary>
	/// �L�����N�^�[�^�C�v�̎擾
	/// </summary>
	CharactorType GetCharactorType()const;

	/// <summary>
	/// �ړ��o�H�Ǘ��N���X�̎擾
	/// </summary>
	std::shared_ptr<RouteManager> GetRouteManager();

	/// <summary>
	/// �}�b�v���W����ɏ�����
	/// </summary>
	/// <param name="mapPos">�}�b�v���W</param>
	void InitmapPos(const Vector2Int& mapPos);

	/// <summary>
	/// �I�𒆂���ݒ�
	/// </summary>
	/// <param name="select">�I�𒆂�</param>
	void SetIsSelect(const bool select);

	/// <summary>
	/// ���ɂ䂭��Ԃɂ���
	/// </summary>
	void SetIsDying();

	/// <summary>
	/// �����̐ݒ�
	/// </summary>
	/// <param name="dir">����</param>
	void SetDir(const Dir dir);

	/// <summary>
	/// �X�e�[�^�X��ݒ�
	/// </summary>
	/// <param name="status">�X�e�[�^�X</param>
	void SetStatus(const Status& status);

	/// <summary>
	/// �X�e�[�^�X�̏�����
	/// </summary>
	/// <param name="status">�X�e�[�^�X</param>
	void InitStatus(const Status& status);

	/// <summary>
	/// ���j�b�g���A�N�e�B�u��Ԃ���ݒ�
	/// </summary>
	/// <param name="active">�A�N�e�B�u��Ԃ�</param>
	void SetMoveActive(const bool active);

	/// <summary>
	/// �ҋ@�t���[�����̐ݒ�
	/// </summary>
	/// <param name="time">�ҋ@�t���[����</param>
	void SetMoveStandby(const int time);

	/// <summary>
	/// �ړ������肷��
	/// </summary>
	void MoveDecision();

	/// <summary>
	/// �ړ����I������
	/// </summary>
	/// <param name="canMove">�Ĉړ����邩</param>
	/// <param name="removeCamera">�J�����̃^�[�Q�b�g����O����</param>
	void MoveEnd(const bool canMove = false, bool removeCamera = true);

	/// <summary>
	/// �ړ��A�j���[�V�������~�߂�
	/// </summary>
	void StopMoveAnim();

	/// <summary>
	/// �ړ��o�H����
	/// </summary>
	void RouteSearch();

	/// <summary>
	/// �^�[�����Ƃ̃��Z�b�g
	/// </summary>
	void TurnReset();

	/// <summary>
	/// �ړ����L�����Z������
	/// </summary>
	void MoveCancel();

	/// <summary>
	/// �_���[�W��^����
	/// </summary>
	/// <param name="damage">�_���[�W��</param>
	void AddDamage(const int damage);

	/// <summary>
	/// �L�����N�^�[���w�肵��Rect�͈̔͂ɕ`�悷��
	/// </summary>
	/// <param name="drawRect">�`���`</param>
	void DrawCharactorIcon(const Rect& drawRect)const;

	/// <summary>
	/// �^�[���J�n���ɒn�`���ʂ��󂯂�Ȃ�true��Ԃ�
	/// </summary>
	/// <returns>����</returns>
	bool StartTerrainEffect();

	/// <summary>
	/// �n�`�G�t�F�N�g�̍Đ����I����Ă��邩
	/// </summary>
	bool GetTerrainEffectEnd();

	/// <summary>
	/// �������e�[�u���̎擾
	/// </summary>
	const std::array<DirInf, static_cast<size_t>(Dir::max)>& GetDirTable()const;
};