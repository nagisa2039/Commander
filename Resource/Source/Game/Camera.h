#pragma once
#include "../Utility/Geometry.h"
#include <memory>
#include <deque>

class Actor;

/// <summary>
/// �J����
/// </summary>
class Camera
{
private:
	// �`��͈͋�`
	Rect _rect;

	// �ǐՃ^�[�Q�b�gDeque
	std::deque<Actor*> _targets;

	// ���W
	Vector2 _pos;
	// �ړ��͈͋�`
	Rect _limitRect;
	// �ǐՃ^�[�Q�b�g����̃I�t�Z�b�g
	Vector2 _offset;

	// �X�V�֐��|�C���^
	void(Camera::* _follower)();

	/// <summary>
	/// �^�[�Q�b�g�Ɍ������ĕ⊮���Ȃ�����Ă���
	/// </summary>
	void NormalFollow();

	/// <summary>
	/// �^�[�Q�b�g���͈͊O�ɍs�����Ƃ������������Ă���
	/// </summary>
	void LooseFollow();

public:
	/// <param name="rect">�`���`</param>
	Camera(const Rect& rect);
	~Camera();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �ǐՃ^�[�Q�b�g�̒ǉ�
	/// </summary>
	/// <param name="target">�ǐՃ^�[�Q�b�g</param>
	void AddTargetActor(Actor* target);

	/// <summary>
	/// �ǐՃ^�[�Q�b�g�̍폜
	/// </summary>
	/// <param name="target">�ǐՃ^�[�Q�b�g</param>
	void RemoveTargetActor(Actor* target);

	/// <summary>
	/// ��ԍŌ�ɒǉ����ꂽ�ǐՃ^�[�Q�b�g���폜
	/// </summary>
	void PopTargetActor();

	/// <summary>
	/// �ǐՃ^�[�Q�b�g��S�폜
	/// </summary>
	void ClearTargetActor();

	/// <summary>
	/// �J�����I�t�Z�b�g�̎擾
	/// </summary>
	Vector2Int GetCameraOffset()const;

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPos(const Vector3& pos);

	/// <summary>
	/// �ړ������̐ݒ�
	/// </summary>
	/// <param name="rect">�ړ�����</param>
	void SetLimitRect(const Rect& rect);

	/// <summary>
	/// LooseFollow�ɐ؂�ւ���
	/// </summary>
	/// <param name="loose">�؂�ւ��邩</param>
	void SetLooseFollow(bool loose);

	/// <summary>
	/// �`���`�̎擾
	/// </summary>
	const Rect& GetRect()const;

	/// <summary>
	/// �J�����I�t�Z�b�g�̐ݒ�
	/// </summary>
	/// <param name="offset">�J�����I�t�Z�b�g</param>
	void SetOffset(const Vector2& offset);
};

