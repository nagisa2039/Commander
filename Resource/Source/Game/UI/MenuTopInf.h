#pragma once
#include "UI.h"
#include "TimeLine.h"

class MapCtrl;

/// <summary>
/// PlayScene�̃��j���[�I�����ɕ\���������\��
/// </summary>
class MenuTopInf :
    public UI
{
private:
    // �}�b�v�Ǘ��N���X
    const MapCtrl& _mapCtrl;
    // �^�[����
    const unsigned char& _turnCnt;

    // �ړ��A�j���[�V�����g���b�N
    std::unique_ptr<Track<Vector2Int>> _moveAnimTrack;

public:
	/// <param name="mapCtrl">�}�b�v�Ǘ��N���X</param>
    /// <param name="turnCnt">�^�[����</param>
	/// <param name="uiDeque">UI�̐e�q�֌W�Ǘ�Deque</param>
    MenuTopInf(const MapCtrl& mapCtrl, const unsigned char& turnCnt, std::deque<std::shared_ptr<UI>>* uiDeque);
    ~MenuTopInf();

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
    /// �J��
    /// </summary>
    void Open();

    /// <summary>
    /// ����
    /// </summary>
    void Close();
};

