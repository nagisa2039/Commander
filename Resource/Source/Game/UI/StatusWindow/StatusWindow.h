#pragma once
#include "../UI.h"
#include "TimeLine.h"

class Charactor;
class WeaponWindow;
class PlayScene;

/// <summary>
/// �X�e�[�^�X�m�F���
/// </summary>
class StatusWindow :
	public UI
{
private:
	// �ΏۃL�����N�^�[
	const Charactor& _charactor;

	// ����ڍו\�����
	std::unique_ptr<WeaponWindow> _weaponWindow;
	// �g�k�A�j���[�V�����p�g���b�N
	std::unique_ptr<Track<float>> _animTrack;

	// �`���X�N���[���n���h��
	int _windowH;

	// ���ʕ\���g�摜�n���h��
	Size _sideWindowSize;
	// �����\���g�摜�n���h��
	Size _centerWindowSize;
	// PlayScene�̃|�C���^
	PlayScene* _playScene;

	// �X�V�֐��|�C���^
	void(StatusWindow::* _updater)(const Input& input);

	/// <summary>
	/// �ʏ�X�V
	/// </summary>
	/// <param name="input"></param>
	void NormalUpdate(const Input& input);

	/// <summary>
	/// �g�k�X�V
	/// </summary>
	/// <param name="input"></param>
	void ScaleUpdate(const Input& input);

	/// <summary>
	/// �`��Ώ�
	/// </summary>
	void DrawToWindowScreen();

	/// <summary>
	/// �A�C�R���\��
	/// </summary>
	/// <param name="iconRect">�\����`</param>
	void DrawIcon(const Rect& iconRect);

	/// <summary>
	/// ���O�⃌�x�����̊�{���\��
	/// </summary>
	/// <param name="levelRect">�\����`</param>
	void DrawBaseInf(const Rect& levelRect);

	/// <summary>
	/// �퓬�X�e�[�^�X�\��
	/// </summary>
	/// <param name="statusRect1">�\����`</param>
	void DrawBattleStatus(const Rect& statusRect1);

	/// <summary>
	/// �X�e�[�^�X�\��
	/// </summary>
	/// <param name="statusRect2">�\����`</param>
	void DrawStatus(const Rect& statusRect2);

public:
	/// <param name="uiDeque">UI�̐e�q�֌W�Ǘ�Deque</param>
	/// <param name="charactor">�Ώۂ̃L�����N�^�[</param>
	StatusWindow(std::deque<std::shared_ptr<UI>>* uiDeque, const Charactor& charactor);
	~StatusWindow();

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

