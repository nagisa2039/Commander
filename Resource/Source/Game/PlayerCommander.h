#pragma once
#include "Commander.h"
#include <memory>
#include <deque>
#include <list>

class PlayerUI;
class UI;

class PlayerCommander :
	public Commander
{
private:
	// UI�Ǘ��p�N���X
	std::unique_ptr<PlayerUI> _playerUI;
	// �}�b�v�I���Ɉڍs���邩
	bool _backMapSelect;

	// ���莞SE
	int _okH;
	// �L�����Z����SE
	int _canselH;

	void(PlayerCommander::* _uniqueUpdater)(const Input& input);

	/// <summary>
	///  _mapPos��_selectChar�̍s���͈͓����̊m�F
	/// </summary>
	bool CheckMoveRange();

	/// <summary>
	/// _mapPos��_selectChar�̍U���}�X���ǂ����̊m�F
	/// </summary>
	bool CheckAttackMass();

	/// <summary>
	/// �퓬�\�����
	/// </summary>
	/// <param name="input">���͏��</param>
	void BattlePredictionUpdate(const Input& input);

	/// <summary>
	/// �n�`���ʉ��o��
	/// </summary>
	/// <param name="input">���͏��</param>
	void TerrainEffectUpdate(const Input& input);

	/// <summary>
	/// �ʏ펞
	/// </summary>
	/// <param name="input">���͏��</param>
	void NormalUpdate(const Input& input);

	/// <summary>
	/// �L�����N�^�[�I��
	/// </summary>
	/// <param name="input">���͏��</param>
	void SelectUpdate(const Input& input);

	/// <summary>
	/// �퓬��
	/// </summary>
	/// <param name="input">���͏��</param>
	void BattaleUpdate(const Input& input);

public:
	/// <param name="charactors">�L�����N�^�[�Ǘ��z��</param>
	/// <param name="mapCtrl">�}�b�v�Ǘ��N���X</param>
	/// <param name="ctrlTeam">���삷��`�[��</param>
	/// <param name="camera">�J����</param>
	/// <param name="turnCnt">�^�[����</param>
	PlayerCommander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam, Camera& camera, const unsigned char & turnCnt);
	~PlayerCommander();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void Update(const Input& input)override final;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw()override final;

	/// <summary>
	/// �n�`���ʂ��󂯂ă^�[�����J�n����
	/// </summary>
	void StartTerrainEffectUpdate()override;

	/// <summary>
	/// �n�`���ʂ��󂯂Ȃ��Ń^�[�����J�n����	1�^�[���ڂɎg�p
	/// </summary>
	void StartNormalUpdate()override;

	/// <summary>
	/// �I�𒆂̃L�����N�^�[�ړ��͈͂�`�悷��
	/// </summary>
	void DrawMovableMass()override;

	/// <summary>
	/// �s�����I������
	/// </summary>
	void End()override;

	/// <summary>
	/// �L�����N�^�[��I������
	/// </summary>
	/// <param name="charactor">�I������L�����N�^�[</param>
	/// <param name="nextTarget"></param>
	void SelectCharactor(Charactor* charactor, const bool nextTarget);

	/// <summary>
	/// �w�肵�����W����ɐ퓬�����ֈڍs����
	/// </summary>
	/// <param name="mapPos">�퓬�����ֈڍs������W</param>
	bool AttackPrePos(const Vector2Int& mapPos);

	/// <summary>
	/// �퓬�J�n
	/// </summary>
	void BattleStart();

	/// <summary>
	/// �퓬������ʂɖ߂�
	/// </summary>
	void BackBattalePrediction();

	/// <summary>
	/// �ړ���������
	/// </summary>
	/// <returns>����</returns>
	bool MoveCancel();

	/// <summary>
	/// �}�b�v�I���ֈڍs���邩���m�F
	/// </summary>
	virtual bool GetBackMapSelect()const;

	/// <summary>
	/// �}�b�v�I���ֈڍs���邩��ݒ�
	/// </summary>
	/// <param name="value">�}�b�v�I���ֈڍs���邩</param>
	void SetBackMapSelect(const bool value);
};

