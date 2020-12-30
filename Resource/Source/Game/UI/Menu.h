#pragma once
#include "UI.h"
#include <array>
#include <functional>
#include <vector>
#include "Geometry.h"
#include "TimeLine.h"

class SelectPen;
class PlayerCommander;
class MapCtrl;

/// <summary>
/// ���j���[�\���p���N���X
/// </summary>
class Menu :
	public UI
{
protected:
	// �v���C���[�w����
	PlayerCommander& _playerCommander;
	// �}�b�v�Ǘ��N���X
	const MapCtrl& _mapCtrl;

	// ���ڃT�C�Y
	Size _contentSize;
	// �I�𒆂̍��ڃC���f�b�N�X
	size_t _selectIdx;

	// �J��SE
	int _openSEH;
	// ����SE
	int _closeSEH;
	// �ړ�SE
	int _moveSEH;

	// �J���A�j���[�V�����g���b�N
	std::unique_ptr<Track<float>> _openAnimTrack;
	// ����A�j���[�V�����g���b�N
	std::unique_ptr<Track<float>> _closeAnimTrack;

	// �I�𒆂̖��
	std::unique_ptr<SelectPen> _selectPen;

	/// <summary>
	/// ���ڏ��
	/// </summary>
	struct ContentInf
	{
		// ���ږ�
		std::string name;
		// �\�����W
		Vector2Int centerPos;
		// �I�����ɌĂԊ֐�
		std::function<void()> func;
	};

	// ���ڊǗ��z��
	std::vector<ContentInf> _contentInfs;
	// ���ڃC���f�b�N�X�Ǘ����X�g
	std::vector<unsigned int> _contentList;

	// �X�V�֐��|�C���^
	void (Menu::* _updater)(const Input& input);
	// �`��֐��|�C���^
	void (Menu::* _drawer)();

	/// <summary>
	/// �J���ꂽ��ԍX�V
	/// </summary>
	/// <param name="input">���͏��</param>
	virtual void OpenUpdate(const Input& input);

	/// <summary>
	/// ������ԍX�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void CloseUpdate(const Input& input);

	/// <summary>
	/// �J���A�j���[�V�����X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void OpenAnimUpdate(const Input& input);

	/// <summary>
	/// ����A�j���[�V�����X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void CloseAnimUpdate(const Input& input);

	/// <summary>
	/// �J���ꂽ��ԕ`��
	/// </summary>
	void OpenDraw();

	/// <summary>
	/// ������ԕ`��
	/// </summary>
	void CloseDraw();

	/// <summary>
	/// �J���A�j���[�V�����`��
	/// </summary>
	void OpenAnimDraw();

	/// <summary>
	/// ����A�j���[�V�����`��
	/// </summary>
	void CloseAnimDraw();

	/// <summary>
	/// ���ڕ`��
	/// </summary>
	/// <param name="drawCenterPos">�\�����S���W</param>
	/// <param name="idx">���ڃC���f�b�N�X</param>
	void DrawContent(const Vector2Int& drawCenterPos, const unsigned int idx);

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="contentNum">���ڐ�</param>
	/// <param name="frameH">���ڕ\���g�摜�n���h��</param>
	void Init(const size_t contentNum, const int frameH);

	/// <summary>
	/// �߂�
	/// </summary>
	virtual void Back();

	/// <summary>
	/// ����
	/// </summary>
	virtual void Decision();

public:
	/// <param name="uiDeque">UI�̐e�q�֌W�Ǘ�Deque</param>
	/// <param name="playerCom">�v���C���[�w����</param>
	/// <param name="mapCtrl">�}�b�v�Ǘ��N���X</param>
	Menu(std::deque<std::shared_ptr<UI>>* uiDeque, PlayerCommander& playerCom, const MapCtrl& mapCtrl);
	~Menu();

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
	/// <param name="animation">�A�j���[�V���������邩</param>
	void Open(bool animation = true);

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="animation">�A�j���[�V���������邩</param>
	void Close(bool animation = true);
};