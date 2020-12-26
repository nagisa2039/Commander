#pragma once
#include "BattleEffect.h"

/// <summary>
/// ���@�U���G�t�F�N�g
/// </summary>
class MagicEffect :
	public BattleEffect
{
public:
	/// <param name="self">���������퓬�L�����N�^�[</param>
	/// <param name="target">�ڕW�̃^�[�Q�b�g�L�����N�^�[</param>
	/// <param name="effects">�G�t�F�N�g�Ǘ��p�z��</param>
	/// <param name="critical">�N���e�B�J���t���O</param>
	/// <param name="camera">�J����</param>
	MagicEffect(BattleCharactor& self, BattleCharactor& target, std::vector<std::shared_ptr<Effect>>& effects, 
		bool critical, Camera& camera);
	~MagicEffect();
};

