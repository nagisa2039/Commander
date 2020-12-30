#pragma once
#include "Geometry.h"
#include <memory>
#include <deque>
#include "TimeLine.h"

class PlayerCommander;
class MapCtrl;
class Input;
class UI;
class PlayerMenu;
class MoveMenu;
class StatusInf;
class TerrainInf;
class StatusWindow;

/// <summary>
/// �v���C���[��UI�Ǘ��N���X
/// </summary>
class PlayerUI
{
private:
	// �}�b�v�Ǘ��N���X
	const MapCtrl& _mapCtrl;
	// �v���C���[�w�����N���X
	PlayerCommander& _playerCommander;

	// �v���C���[���j���[�p�e�q�֌WDeque
	std::deque<std::shared_ptr<UI>> _playerMenuDeque;
	// �v���C���[���j���[
	std::shared_ptr<PlayerMenu> _playerMenu;

	// �ړ����j���[�p�e�q�֌WDeque
	std::deque<std::shared_ptr<UI>> _moveMenuDeque;
	// �ړ����j���[�p�e�q�֌WDeque
	std::shared_ptr<MoveMenu> _moveMenu;

	// �n�`���
	std::shared_ptr<TerrainInf> _terrainInf;
	// �X�e�[�^�X���
	std::shared_ptr<StatusInf> _statusInf;
	// �X�e�[�^�X�E�B���h�E
	std::shared_ptr<UI> _statusWindow;
	// �퓬�\��
	std::shared_ptr<UI> _battlePre;

public:
	/// <param name="playerCommander"></param>
	/// <param name="mapCtrl">�}�b�v�Ǘ��N���X</param>
	/// <param name="turnCnt"></param>
	PlayerUI(PlayerCommander& playerCommander, const MapCtrl& mapCtrl, const unsigned char& turnCnt);
	~PlayerUI();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void Update(const Input& input);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���������UI���J���Ă��邩
	/// </summary>
	bool GetUIIsOpen()const;

	/// <summary>
	/// �w�������j���[���J��
	/// </summary>
	/// <param name="animation">�A�j���[�V�������s����</param>
	void OpenPlayerMenu(bool animation = true);

	/// <summary>
	/// �w�������j���[�����
	/// </summary>
	/// <param name="animation">�A�j���[�V�������s����</param>
	void ClosePlayerMenu(bool animation = true);

	/// <summary>
	/// �ړ����j���[���擾
	/// </summary>
	std::shared_ptr<MoveMenu> GetMoveMenu();

	/// <summary>
	/// �퓬�\����ʂ�ǉ�����
	/// </summary>
	void OpenBattlePrediction();

	/// <summary>
	/// �퓬�\����ʂ����
	/// </summary>
	void ClearBattlePrediction();
};

