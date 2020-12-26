#pragma once
#include "BattleEffect.h"
#include "TimeLine.h"

/// <summary>
/// �|�U���G�t�F�N�g
/// </summary>
class ArrowEffect :
	public BattleEffect
{
private:
	// �ړ��A�j���[�V�����g���b�N
	std::shared_ptr<Track<Vector2Int>> _moveTrack;

public:
	/// <param name="self">���������퓬�L�����N�^�[</param>
	/// <param name="target">�ڕW�̃^�[�Q�b�g�L�����N�^�[</param>
	/// <param name="effects">�G�t�F�N�g�Ǘ��p�z��</param>
	/// <param name="critical">�N���e�B�J���t���O</param>
	/// <param name="camera">�J����</param>
	ArrowEffect(BattleCharactor& self, BattleCharactor& target, std::vector<std::shared_ptr<Effect>>& effects, 
		bool critical, Camera& camera);
	~ArrowEffect();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void Update(const Input& input)override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw()override;
};

