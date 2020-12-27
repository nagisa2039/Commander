#pragma once
#include "MapCursor.h"
#include <memory>
#include <vector>
#include "Team.h"

class Charactor;
class Animator;

/// <summary>
/// �w�����N���X
/// </summary>
class Commander :
	public MapCursor
{
protected:
	// �L�����N�^�[�Ǘ��z��
	std::vector<std::shared_ptr<Charactor>>& _charactors;
	// �I�𒆂̃L�����N�^�[
	Charactor* _selectChar;
	// ���삷��`�[��(���R)
	const Team _ctrlTeam;
	// �A�j���[�V�����p�J�E���^�[
	int _animCnt;
	// �d���t���[����
	int _rigid;
	// �s���I������p�t���O
	bool _end;

	/// <summary>
	/// ���R�̍s�����I����������m�F
	/// </summary>
	bool ChaeckCharactor();

	/// <summary>
	/// �n�`���ʃG�t�F�N�g���J�n����
	/// </summary>
	void StartTerrainEffect();

public:
	/// <param name="charactors">�L�����N�^�[�Ǘ��z��</param>
	/// <param name="mapCtrl">�}�b�v�Ǘ��N���X</param>
	/// <param name="ctrlTeam">���삷��`�[��</param>
	/// <param name="camera">�J����</param>
	Commander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam, Camera& camera);
	virtual ~Commander();

	/// <summary>
	/// �I�𒆂̃L�����N�^�[�ړ��͈͂�`�悷��
	/// </summary>
	virtual void DrawMovableMass();

	/// <summary>
	/// �}�b�v�I���ֈڍs���邩���m�F
	/// </summary>
	virtual bool GetBackMapSelect()const;

	/// <summary>
	/// �s�����I������
	/// </summary>
	virtual void End();

	/// <summary>
	/// �s�����I�����������m�F
	/// </summary>
	bool CheckEnd()const;

	/// <summary>
	/// �^�[�����J�n���邽�߂̏�����
	/// </summary>
	void TurnReset();

	/// <summary>
	/// �I�𒆂̃L�����N�^�[�̃|�C���^�[��Ԃ�
	/// </summary>
	Charactor* GetSelectCharactor()const;

	/// <summary>
	/// �n�`���ʂ��󂯂ă^�[�����J�n����
	/// </summary>
	virtual void StartTerrainEffectUpdate() = 0;

	/// <summary>
	/// �n�`���ʂ��󂯂Ȃ��Ń^�[�����J�n����	1�^�[���ڂɎg�p
	/// </summary>
	virtual void StartNormalUpdate() = 0;
};

