#pragma once
#include "UI.h"
#include <string>
#include "TimeLine.h"
#include <functional>
#include <memory>

class PlayerCommander;

/// <summary>
/// �|�b�v�A�b�v�E�B���h�E
/// </summary>
class PopupWindow :
	public UI
{
protected:
	// �\���p�e�L�X�g
	std::string _messageStr;
	// �g�k�A�j���[�V�����g���b�N
	std::unique_ptr<Track<float>> _exRateTrack;
	// �I�𒆍��ڃA�j���[�V�����g���b�N
	std::unique_ptr<Track<float>> _selectExRateTrack;
	// �`���E�B���h�E
	int _messageImageH;

	// �X�V�֐��|�C���^
	void (PopupWindow::* _updater)(const Input& input);
	// �`��֐��|�C���^
	void (PopupWindow::* _drawer)();

	/// <summary>
	/// �ʏ�X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	virtual void NormalUpdate(const Input& input);

	/// <summary>
	/// �g�k�X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	virtual void ScalingUpdate(const Input& input);

	/// <summary>
	/// ���؂����u��
	/// </summary>
	virtual void Closed();
	
	/// <summary>
	/// �ʏ�`��
	/// </summary>
	virtual void NormalDraw();

	/// <summary>
	/// �g�k�`��
	/// </summary>
	virtual void ScalingDraw();

	/// <summary>
	/// �E�B���h�E�ւ̕`��
	/// </summary>
	void DrawToWindow();

public:
	/// <param name="messageStr">���b�Z�[�W�e�L�X�g</param>
	/// <param name="uiDeque">UI�̐e�q�֌W�Ǘ�Deque</param>
	PopupWindow(const std::string& messageStr, std::deque<std::shared_ptr<UI>>* uiDeque);
	~PopupWindow();

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

/// <summary>
/// �I��������̃��b�Z�[�W�E�B���h�E
/// </summary>
class CheckWindow :
	public PopupWindow
{
private:
	/// <summary>
	/// �I������
	/// </summary>
	enum class Select
	{
		yes,
		no,
		max
	};

	// �I�𒆂̍���
	Select _select;
	// yes�I�������s�����֐�
	std::function<void()> _func;
	// yes���ډ摜�n���h��
	int _yesImageH;
	// no���ډ摜�n���h��
	int _noImageH;

	// yes���ڋ�`
	Rect _yesSelectRect;
	// no���ڋ�`
	Rect _noSelectRect;

	/// <summary>
	/// �ʏ�X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void NormalUpdate(const Input& input)override;

	/// <summary>
	/// ����
	/// </summary>
	void StartClose();

	/// <summary>
	/// ���ڂւ̕`��
	/// </summary>
	void DrawToSelectImage();

	/// <summary>
	/// ���؂����u��
	/// </summary>
	void Closed()override;

	/// <summary>
	/// �ʏ�`��
	/// </summary>
	void NormalDraw()override;

public:
	/// <param name="messageStr">���b�Z�[�W�e�L�X�g</param>
	/// <param name="uiDeque">UI�̐e�q�֌W�Ǘ�Deque</param>
	/// <param name="func">yes�I�����Ɏ��s�����֐�</param>
	CheckWindow(const std::string& messageStr, std::deque<std::shared_ptr<UI>>* uiDeque, const std::function<void()>& func);
	~CheckWindow();
};

/// <summary>
/// ���b�Z�[�W�\���p�E�B���h�E
/// </summary>
class MessageWindow :
	public PopupWindow
{
private:
	/// <summary>
	/// �ʏ�`��
	/// </summary>
	/// <param name="input">���͏��</param>
	void NormalUpdate(const Input& input)override;

public:
	/// <param name="messageStr">���b�Z�[�W�e�L�X�g</param>
	/// <param name="uiDeque">UI�̐e�q�֌W�Ǘ�Deque</param>
	MessageWindow(const std::string& messageStr, std::deque<std::shared_ptr<UI>>* uiDeque);
	~MessageWindow();
};