#pragma once
#include "Menu.h"
#include <list>

/// <summary>
/// �L�����N�^�[
/// </summary>
class MoveMenu :
	public Menu
{
private:
	/// <summary>
	/// ���ڎ�
	/// </summary>
	enum class Content
	{
		// �퓬
		battle,
		// �ҋ@
		wait,
		// ����
		close,
		max
	};

	// �擪���ڂ�I�񂾂�
	bool _isBattle;
	// �U���\�}�X���X�g
	std::list<Vector2Int> _attackPosList;
	// �I�𒆂̐퓬�\�}�X
	std::list<Vector2Int>::iterator _attackPosListItr;

	/// <summary>
	/// ����
	/// </summary>
	void Decision()override;

	/// <summary>
	/// �߂�
	/// </summary>
	void Back()override;

	/// <summary>
	/// �J���Ă��鎞�̍X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void OpenUpdate(const Input& input)override;

public:
	/// <param name="uiDeque">UI�̐e�q�֌W�Ǘ�Deque</param>
	/// <param name="playerCom">�v���C���[�w����</param>
	/// <param name="mapCtrl">�}�b�v�Ǘ��N���X</param>
	MoveMenu(std::deque<std::shared_ptr<UI>>* uiDeque, PlayerCommander& playerCom, const MapCtrl& mapCtrl);
	~MoveMenu();

	/// <summary>
	/// ���ڏ���ݒ�
	/// </summary>
	/// <param name="attackPosList">�U���Ώۍ��W</param>
	void SetContent(const std::list<Vector2Int>& attackPosList);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw()override;
};

