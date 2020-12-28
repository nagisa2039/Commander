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
/// �퓬�p�L�����N�^�[
/// </summary>
class BattleCharactor
{
public:
	/// <summary>
	/// �_���[�W��
	/// </summary>
	enum class damageType
	{
		// �������Ă��Ȃ�
		none,		
		// ���Ă�
		damage,		
		// ��S�U���𓖂Ă�
		critical,	
		max
	};

protected:
	// ���L�҂̃L�����N�^�[
	Charactor& _selfChar;
	// �ΐ푊��̃L�����N�^�[
	BattleCharactor* _targetChar;
	// �J����
	Camera& _camera;

	// ����
	Dir _dir;
	// �J�n���W
	Vector2 _startPos;
	// ���݂̍��W
	Vector2 _pos;
	// �\���T�C�Y
	Size _size;
	// �A�j���[�V�����Ǘ��N���X
	std::shared_ptr<Animator> _animator;

	// �U���A�j���[�V�����g���b�N
	std::unique_ptr<Track_i> _attackAnimX;
	// �҂��J�E���g�g���b�N
	std::unique_ptr<Track_i> _waitT;
	// �U���G�t�F�N�g
	std::shared_ptr<Effect> _attackEffect;

	// �\���\�̗�
	uint8_t _animHealth;
	// �̗̓A�j���[�V�����p����
	int _animHealthCnt;

	// �U�������蔻��p
	damageType _gaveDamageType;
	// �U���G�t�F�N�g�����֐�
	using attackEffectFunc_t = std::function<void(BattleScene&, const Vector2Int&)>;
	// �U���G�t�F�N�g�����֐��e�[�u��
	std::array<attackEffectFunc_t, static_cast<size_t>(damageType::max)> _attackEffectFuncs;

	// �^�����_���[�W
	unsigned int _givenDamage;	

	// HP�\���p�}�X�N
	int _hpDotMaskH;

	// �󂯂��_���[�W�^�C�v�\���ϐ�
	damageType _receiveDamageType;

	// �X�V�p�֐��|�C���^�ϐ�
	void(BattleCharactor::* _updater)(BattleScene&);
	// wait�̎���update�֐��|�C���^�ϐ�
	void(BattleCharactor::* _waitNextUpdater)(BattleScene&);	

	/// <summary>
	/// �҂���Ԃ̍X�V
	/// </summary>
	/// <param name="battleScene">�퓬�V�[�����</param>
	void WaitUpdate(BattleScene& battleScene);

	/// <summary>
	/// �ʏ펞�̍X�V
	/// </summary>
	/// <param name="battleScene">�퓬�V�[�����</param>
	void NormalUpdate(BattleScene& battleScene);

	/// <summary>
	/// �J�b�g�C�����̍X�V
	/// </summary>
	/// <param name="battleScene">�퓬�V�[�����</param>
	void CutInUpdate(BattleScene& battleScene);

	/// <summary>
	/// �҂���Ԃւ̈ڍs
	/// </summary>
	/// <param name="nextUpdate">���̏��</param>
	void WaitStart(void(BattleCharactor::* nextUpdate)(BattleScene&));

	/// <summary>
	/// ���O�̕\��
	/// </summary>
	/// <param name="teamString">�`�[����</param>
	/// <param name="nameWindowRect">�\���g��`</param>
	/// <param name="fileSystem">�t�@�C���Ǘ��N���X</param>
	/// <param name="fontHandle">�t�H���g�n���h��</param>
	void DrawName(const char* teamString, Rect& nameWindowRect, FileSystem& fileSystem, int fontHandle);

	/// <summary>
	/// �p�����[�^�[�\��
	/// </summary>
	/// <param name="teamString"></param>
	/// <param name="fileSystem">�t�@�C���Ǘ��N���X</param>
	/// <param name="paramWindowRect">�\���g��`</param>
	void DrawParameter(const char* teamString, FileSystem& fileSystem, Rect& paramWindowRect);

	/// <summary>
	/// HP�\��
	/// </summary>
	/// <param name="windowRect">�\���g��`</param>
	/// <param name="fontHandle">�t�H���g�n���h��</param>
	void DrawHP(Rect& windowRect, int fontHandle);

	/// <summary>
	/// ���햼�\��
	/// </summary>
	/// <param name="fileSystem">�t�@�C���Ǘ��N���X</param>
	/// <param name="weaponNameRect">�\���g��`</param>
	void DrawWeaponName(FileSystem& fileSystem, Rect& weaponNameRect);

public:
	/// <param name="charactor">���L�҂̃L�����N�^�[</param>
	/// <param name="imageHandle">�L�����N�^�[�摜�̃n���h��</param>
	/// <param name="camera">�J����</param>
	BattleCharactor(Charactor& charactor, const int imageHandle, Camera& camera);
	~BattleCharactor();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="startPos">�J�n���W</param>
	/// <param name="dir">����</param>
	/// <param name="target">�퓬����</param>
	void Init(const Vector2& startPos, const Dir dir, BattleCharactor* target);

	/// <summary>
	/// �A�j���[�V�����X�V
	/// </summary>
	void AnimUpdate();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="battleScene"></param>
	void Update(BattleScene& battleScene);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// UI�X�V
	/// </summary>
	void UIAnimUpdate();

	/// <summary>
	/// UI�`��
	/// </summary>
	void UIDraw();

	/// <summary>
	/// �U���A�j���[�V�������J�n����
	/// </summary>
	/// <param name="battleScene"></param>
	void StartAttackAnim(BattleScene& battleScene);

	/// <summary>
	/// �U���A�j���[�V�������I�����������m�F
	/// </summary>
	bool GetAttackAnimEnd();

	/// <summary>
	/// HP�A�j���[�V�������J�n����
	/// </summary>
	void StartHPAnim();

	/// <summary>
	/// HP�A�j���[�V�������I�����������m�F
	/// </summary>
	bool GetHPAnimEnd();

	/// <summary>
	/// �\���T�C�Y���擾
	/// </summary>
	Size GetSize()const;

	/// <summary>
	/// ���S���W���擾
	/// </summary>
	Vector2Int GetCenterPos()const;

	/// <summary>
	/// �����̍��W���擾
	/// </summary>
	Vector2Int GetPos()const;	

	/// <summary>
	/// �J�n�ʒu�̑������W���擾
	/// </summary>
	Vector2Int GetStartPos()const;	

	/// <summary>
	/// ���L�҂̃L�����N�^�[���擾
	/// </summary>
	Charactor& GetCharacotr();

	/// <summary>
	/// �^�����_���[�W�ʂ��擾
	/// </summary>
	const unsigned int& GetGivenDamage()const;

	/// <summary>
	/// �퓬������擾
	/// </summary>
	BattleCharactor* GetTargetBattleCharactor();

	/// <summary>
	/// �^�����_���[�W����擾
	/// </summary>
	damageType GetDamageType()const;

	/// <summary>
	/// �`�悷����W(������)
	/// </summary>
	/// <param name="startPos">�J�n���W</param>
	void SetStartPos(const Vector2& startPos);	

	/// <summary>
	/// left(�U��)��right(���)�̐ݒ�
	/// </summary>
	/// <param name="dir">����</param>
	void SetDir(const Dir dir);

	/// <summary>
	/// �키����̃|�C���^�[��ݒ�
	/// </summary>
	/// <param name="target">�키����̃|�C���^�[</param>
	void SetTargetCharactor(BattleCharactor* target);

	/// <summary>
	/// �󂯂��_���[�W�^�C�v��ݒ�
	/// </summary>
	/// <param name="dt">�_���[�W��</param>
	void SetReceiveDamageType(const damageType dt);

	/// <summary>
	/// �^����_���[�W�ʂ̐ݒ�
	/// </summary>
	/// <param name="value">�_���[�W��</param>
	void SetGivenDamage(const unsigned int value);
};