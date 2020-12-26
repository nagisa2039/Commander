#pragma once
#include <memory>
#include <vector>
#include <array>
#include <functional>
#include "BattleEffectType.h"

class Effect;
class Camera;
class BattleCharactor;

/// <summary>
/// �U���G�t�F�N�g�����N���X
/// </summary>
class BattleEffectFactory
{
private:
	using CrateBattleEffectFunc_t = std::function<std::shared_ptr<Effect>(
		BattleCharactor&, BattleCharactor&, std::vector<std::shared_ptr<Effect>>&,bool, Camera&, bool)>;

	// �U���G�t�F�N�g�킲�Ƃ̐����֐��z��
	std::array<CrateBattleEffectFunc_t, static_cast<size_t>(BattleEffectType::max)> _createBattleEffectFuncs;

public:
	BattleEffectFactory();
	~BattleEffectFactory()=default;

	/// <summary>
	/// �U���G�t�F�N�g�̐���
	/// </summary>
	/// <param name="type">�U���G�t�F�N�g�̎��</param>
	/// <param name="self">���������퓬�L�����N�^�[</param>
	/// <param name="target">�ڕW�̃^�[�Q�b�g�L�����N�^�[</param>
	/// <param name="effects">�G�t�F�N�g�Ǘ��p�z��</param>
	/// <param name="critical">�N���e�B�J���t���O</param>
	/// <param name="camera">�J����</param>
	/// <param name="cameraActive">�J�������l�������`������邩</param>
	/// <returns>���������U���G�t�F�N�g</returns>
	std::shared_ptr<Effect> CreateBattleEffect(const BattleEffectType type, BattleCharactor & self, BattleCharactor & target, std::vector<std::shared_ptr<Effect>> & effects,
		bool critical, Camera & camera, bool cameraActive = false)const;
};