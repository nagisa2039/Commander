#pragma once
#include "Menu.h"

class MenuTopInf;

/// <summary>
/// �v���C���[�w�������j���[
/// </summary>
class PlayerMenu :
	public Menu
{
private:
	/// <summary>
	/// ����
	/// </summary>
	enum class Content
	{
		// �틵�m�F
		situation,
		// �ދp
		retreat,
		// �^�[���I��
		turnEnd,
		max
	};

	// ���\���N���X
	std::shared_ptr<MenuTopInf> _menuTop;

public:
	/// <param name="uiDeque">UI�̐e�q�֌W�Ǘ�Deque</param>
	/// <param name="playerCom"></param>
	/// <param name="mapCtrl">�}�b�v�Ǘ��N���X</param>
	/// <param name="turnCnt"></param>
	PlayerMenu(std::deque<std::shared_ptr<UI>>* uiDeque, PlayerCommander& playerCom, const MapCtrl& mapCtrl, const unsigned char& turnCnt);
	~PlayerMenu();

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

