#pragma once
#include "Commander.h"
#include <vector>

class EnemyCommander :
	public Commander
{
private:
	// ��ԍX�V�֐��|�C���^
	void(EnemyCommander::* _uniqueUpdater)(const Input& input);

	/// <summary>
	/// �ʏ�X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void NormalUpdate(const Input& input);

	/// <summary>
	/// �n�`���ʉ��o���X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void TerrainEffectUpdate(const Input& input);

public:
	/// <param name="charactors">�L�����N�^�[�Ǘ��z��</param>
	/// <param name="mapCtrl">�}�b�v�Ǘ��N���X</param>
	/// <param name="ctrlTeam">���삷��`�[��</param>
	/// <param name="camera">�J����</param>
	EnemyCommander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam, Camera& camera);
	~EnemyCommander();

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
};

