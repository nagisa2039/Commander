#pragma once
#include "../Effect.h"
#include <vector>

class BattleCharactor;

/// <summary>
/// �U�����p�G�t�F�N�g
/// </summary>
class BattleEffect :
	public Effect
{
protected:
	// �G�t�F�N�g�Ǘ��p�z��
	std::vector<std::shared_ptr<Effect>>& _effects;
	// ���������퓬�L�����N�^�[
	BattleCharactor& _self;
	// �ڕW�̃^�[�Q�b�g�L�����N�^�[
	BattleCharactor& _target; 
	// �N���e�B�J���t���O
	const bool _critical;

	/// <summary>
	/// �ڕW�L�����N�^�[�Ƀ_���[�W��^����
	/// </summary>
	/// <returns>�_���[�W��</returns>
	unsigned int AddDamage();

public:
	/// <param name="self">���������퓬�L�����N�^�[</param>
	/// <param name="target">�ڕW�̃^�[�Q�b�g�L�����N�^�[</param>
	/// <param name="effects">�G�t�F�N�g�Ǘ��p�z��</param>
	/// <param name="critical">�N���e�B�J���t���O</param>
	/// <param name="camera">�J����</param>
	/// <param name="cameraActive">�J�������l�������`������邩</param>
	BattleEffect(BattleCharactor& self, BattleCharactor& target, std::vector<std::shared_ptr<Effect>>& effects,
		bool critical, Camera& camera, bool cameraActive = false);
	virtual ~BattleEffect();
};

