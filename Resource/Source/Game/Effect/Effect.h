#pragma once
#include "Actor.h"
#include <memory>
#include "Geometry.h"

class Animator;

/// <summary>
/// �G�t�F�N�g���N���X
/// </summary>
class Effect :
	public Actor
{
protected:
	// �A�j���[�V�����Đ��N���X
	std::shared_ptr<Animator> _animator;

	// �G�t�F�N�g�T�C�Y
	Size _size;

	// �J�������l�����ĕ`�悷�邩
	bool _cameraActive;

public:
	/// <param name="pos">���W</param>
	/// <param name="camera">�J����</param>
	/// <param name="cameraActive">�J�������l�����ĕ`�悷�邩</param>
	Effect(const Vector2Int& pos, Camera& camera, bool cameraActive = false);
	~Effect();

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

