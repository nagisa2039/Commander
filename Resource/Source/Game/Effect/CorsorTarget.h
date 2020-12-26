#pragma once
#include "Effect.h"
#include <memory>
#include "TimeLine.h"

/// <summary>
/// �J�[�\���G�t�F�N�g
/// </summary>
class CorsorTarget :
	public Effect
{
private:
	// �g�k�A�j���[�V�����g���b�N
	std::unique_ptr<Track<float>> _exRateTrack;
	// �}�b�v�`�b�v�T�C�Y
	const Size& _chipSize;

public:
	/// <param name="centerPos">���S���W</param>
	/// <param name="camera">�J����</param>
	/// <param name="cameraActive">�J�������l�����ĕ`�悷�邩</param>
	/// <param name="chipSize">1�}�X�̃T�C�Y</param>
	CorsorTarget(const Vector2Int& centerPos, Camera& camera, bool cameraActive, const Size& chipSize);
	~CorsorTarget();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	virtual void Update(const Input& input) override;

	/// <summary>
	///�@�`��
	/// </summary>
	virtual void Draw() override;

	/// <summary>
	/// ������x�Đ�����ۂ�
	/// ����������
	/// </summary>
	virtual void Reset();
};