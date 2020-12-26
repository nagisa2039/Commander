#pragma once
#include "../Utility/Geometry.h"

class Input;
class Camera;

/// <summary>
/// �A�N�^�[�N���X
/// </summary>
class Actor
{
protected:
	//�@���W
	Vector2 _pos;
	// �폜�t���O
	bool _delete;
	// �J����
	Camera& _camera;

public:
	/// <param name="camera">�J����</param>
	Actor(Camera& camera);
	virtual ~Actor();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	virtual void Update(const Input& input) = 0;

	/// <summary>
	///�@�`��
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// ���W�̎擾
	/// </summary>
	Vector2 GetActorPos()const;

	/// <summary>
	/// ���S���W�̎擾
	/// </summary>
	virtual Vector2 GetCenterPos()const;

	/// <summary>
	/// �폜���Ă�����
	/// </summary>
	bool GetDelete()const;

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPos(const Vector2& pos);

	/// <summary>
	/// �폜���Ă�������ݒ�
	/// </summary>
	/// <param name="value">�폜���Ă�����</param>
	void SetDelete(const bool value);
};

