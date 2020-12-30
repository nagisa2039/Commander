#pragma once
#include "UI.h"
#include "TimeLine.h"
#include <memory>
#include <list>

class MapCtrl;
class MapCursor;
class PlayerUI;

/// <summary>
/// �X�e�[�^�X���
/// </summary>
class StatusInf
	: public UI
{
private:
	// �}�b�v�Ǘ��N���X
	const MapCtrl& _mapCtrl;
	// �J�[�\���̃}�b�v���W
	const Vector2Int& _mapPos;

	// �ڕW�}�b�v���W
	Vector2Int _targetMapPos;

	// �ړ��A�j���[�V�����g���b�N
	std::shared_ptr<Track<float>> _moveAnimTrack;

public:
	/// <param name="uiDeque">UI�̐e�q�֌W�Ǘ�Deque</param>
	/// <param name="mapCtrl">�}�b�v�Ǘ��N���X</param>
	/// <param name="mapPos"></param>
	StatusInf(std::deque<std::shared_ptr<UI>>* uiDeque, const MapCtrl& mapCtrl, const Vector2Int& mapPos);
	~StatusInf();

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

