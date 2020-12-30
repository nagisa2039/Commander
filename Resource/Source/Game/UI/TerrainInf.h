#pragma once
#include "UI.h"
#include "TimeLine.h"

class MapCtrl;

/// <summary>
/// �n�`���
/// </summary>
class TerrainInf :
    public UI
{
private:
	// �A�j���[�V�����g���b�N
    std::shared_ptr<Track<float>> _animTrack;
	// �}�b�v�Ǘ��N���X
    const MapCtrl& _mapCtrl;
	// �J�[�\���̃}�b�v���W
    const Vector2Int& _mapPos;

public:
	/// <param name="uiDeque">UI�̐e�q�֌W�Ǘ�Deque</param>
	/// <param name="mapCtrl">�}�b�v�Ǘ��N���X</param>
    /// <param name="mapPos"></param>
    TerrainInf(std::deque<std::shared_ptr<UI>>* uiDeque, const MapCtrl& mapCtrl, const Vector2Int& mapPos);
    ~TerrainInf();

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

