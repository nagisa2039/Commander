#pragma once
#include "UI.h"
#include "TimeLine.h"

/// <summary>
/// ���j���[���ڑI��p���
/// </summary>
class SelectPen :
    public UI
{
private:
	// �\�����W
    Vector2Int _pos;
	// �A�j���[�V�����g���b�N
    std::unique_ptr<Track<float>> _penAnimTrack;

public:
	/// <param name="uiDeque">UI�̐e�q�֌W�Ǘ�Deque</param>
    SelectPen(std::deque<std::shared_ptr<UI>>* uiDeque);
    ~SelectPen();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void Update(const Input& input)override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw()override;

	/// <summary>
	/// �\�����W�ݒ�
	/// </summary>
	/// <param name="pos">�\�����W</param>
	void SetPos(const Vector2Int& pos);
};

