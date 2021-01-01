#pragma once
#include <map>
#include <string>
#include <vector>
#include <array>
#include <functional>
#include "Geometry.h"

/// <summary>
/// ���͏��Ǘ��N���X
/// </summary>
class Input
{
public:
	/// <summary>
	/// �@��
	/// </summary>
	enum class PeripheralType
	{
		// �L�[�{�[�h
		keybord,
		// �p�b�h
		gamepad,
		// �}�E�X
		mouse,
		max
	};

private:
	// �v���C���[�ԍ��ƁA���͕����񂩂�
	// ���̓��͂��킩��B�܂�L�[�R���t�B�O�ꗗ
	// ��\������Ƃ��ɂ���
	mutable std::map<std::string, std::array<int, static_cast<size_t>(PeripheralType::max)>> _inputTbl;

	using inputFunc_t = std::function<void(const std::string&, const int)>;
	/// <summary>
	/// �@�킲�Ƃɓ��͏����X�V����֐��e�[�u��
	/// </summary>
	std::array<inputFunc_t, static_cast<size_t>(PeripheralType::max)> _peripheralInfFuncs;

	// 1�t���[���O�̃}�E�X���W
	Vector2Int _mousePosOld;
	// �}�E�X�̍��W
	Vector2Int _mousePos;

	// ���͏��̋L���o�b�t�@��
	static const int INPUT_RECORD_SIZE = 2;
	using InputState = std::map<std::string, bool>;
	// ���͏��
	std::array<InputState, INPUT_RECORD_SIZE> _InputState;
	// ���݂̓��̓o�b�t�@�C���f�b�N�X
	size_t _currentInputStateIdx;

	// �L�[��
	static const int INPUT_KEY_SIZE = 256;
	// �L�[�{�[�h�̏�
	std::array<std::array<char, INPUT_KEY_SIZE>, INPUT_RECORD_SIZE> _keystate;

	// ���݂̃p�b�h�̏�
	std::array<int, INPUT_RECORD_SIZE> _padState;
	// ���݂̃}�E�X���͏��
	std::array<int, INPUT_RECORD_SIZE> _mouseState;

	/// <summary>
	/// ���̓e�[�u�����擾����
	/// </summary>
	const std::map<std::string, std::array<int, static_cast<size_t>(PeripheralType::max)>>& GetInputTable()const;
	
	/// <summary>
	/// ���̓��̓e�[�u��������������
	/// </summary>
	/// <param name="rowInputTable">����������l</param>
	void SetRowInputTable(const std::map<std::string, std::array<int, static_cast<size_t>(PeripheralType::max)>>& rowInputTable)const;

	/// <summary>
	/// �o�^�ς݂̃R�}���h������������
	/// </summary>
	/// <param name="cmd">�R�}���h��</param>
	/// <param name="perType">�@���</param>
	/// <param name="code">���̓}�X�N��</param>
	/// <returns>��������������</returns>
	bool ChangeCommand(const std::string& cmd, const PeripheralType perType, const int code)const;

	/// <summary>
	/// ���̓��̓o�b�t�@�C���f�b�N�X��Ԃ�
	/// </summary>
	/// <returns> ���̓��̓o�b�t�@�C���f�b�N�X </returns>
	size_t GetNextInputBufferIndex()const;

	/// <summary>
	/// 1�t���[���O�̓��̓o�b�t�@�C���f�b�N�X��Ԃ�
	/// </summary>
	size_t GetLastInputBufferIndex()const;

	/// <summary>
	/// �w�肵���R�}���h�̌��݂̓��͏󋵂��擾
	/// </summary>
	/// <param name="cmd">�R�}���h</param>
	const bool& CurrentInput(const std::string& cmd)const;

	/// <summary>
	/// �w�肵���R�}���h��1�t���[���O�̓��͏󋵂��擾
	/// </summary>
	/// <param name="cmd">�R�}���h</param>
	const bool& LastInput(const std::string& cmd)const;

	/// <summary>
	/// �L�[�{�[�h�̓��͏󋵂��擾
	/// </summary>
	std::array<char, INPUT_KEY_SIZE> GetKeybordState()const;

	/// <summary>
	/// �L�[�{�[�h�̉������u�Ԃ��擾
	/// </summary>
	std::array<char, INPUT_KEY_SIZE> GetKeybordDownState()const;

	/// <summary>
	/// �p�b�h�̓��͏󋵎擾
	/// </summary>
	int GetPadState()const;

	/// <summary>
	/// �p�b�h�̉����ꂽ�u�Ԏ擾
	/// </summary>
	int GetPadDownState()const;

	/// <summary>
	/// �}�E�X�̓��͏󋵎擾
	/// </summary>
	int GetMouseState()const;

public:
	Input();
	~Input();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(void);

	/// <summary>
	/// �R�}���h�ݒ�
	/// </summary>
	/// <param name="cmd">�R�}���h������</param>
	/// <param name="perType">�@��</param>
	/// <param name="code">���̓R�[�h</param>
	void AddCommand(const std::string& cmd, const PeripheralType perType, const int code);

	/// <summary>
	/// ���݉�����Ă��邩
	/// </summary>
	/// <param name="cmd">�R�}���h������</param>
	bool GetButton(const std::string& cmd) const;

	/// <summary>
	/// �����ꂽ�u�Ԃ�
	/// </summary>
	/// <param name="cmd">�R�}���h������</param>
	bool GetButtonDown(const std::string& cmd) const;

	/// <summary>
	/// �{�^���������̂���߂��u��
	/// </summary>
	/// <param name="cmd">�R�}���h������</param>
	bool GetButtonUp(const std::string& cmd) const;

	/// <summary>
	/// _inputTbl�Ɏw�肵���R�}���h�����݂��邩�m�F
	/// </summary>
	/// <param name="cmd">�R�}���h������</param>
	bool CheckCommand(const std::string& cmd) const;

	/// <summary>
	/// �}�E�X�̍��W��Ԃ�
	/// </summary>
	/// <returns></returns>
	const Vector2Int& GetMousePos()const;

	/// <summary>
	/// �{�^���̉������擾
	/// </summary>
	/// <param name="keycode">�L�[�R�[�h</param>
	bool GetButton(const char keycode)const;

	/// <summary>
	/// �{�^�������������u�ԏ��擾
	/// </summary>
	/// <param name="keycode">�L�[�R�[�h</param>
	bool GetButtonDown(const char keycode)const;

	/// <summary>
	/// �{�^���𗣂����u�ԏ��擾
	/// </summary>
	/// <param name="keycode">�L�[�R�[�h</param>
	bool GetButtonUp(const char keycode)const;

	/// <summary>
	/// ��������̃}�E�X�{�^�����͂����������擾
	/// </summary>
	bool GetAnyMouseInput()const;

	/// <summary>
	/// ��������̃L�[�{�[�h���͂����������擾
	/// </summary>
	bool GetAnyKeybordInput()const;

	/// <summary>
	/// ��������̃p�b�h���͂����������擾
	/// </summary>
	bool GetAnyPadInput()const;

	/// <summary>
	/// �}�E�X�̈ړ��ʎ擾
	/// </summary>
	Vector2Int GetMouseMove()const;

	/// <summary>
	/// �}�E�X�̃{�^�����͏󋵎擾
	/// </summary>
	/// <param name="checkInput">�}�E�X�{�^���R�[�h</param>
	bool GetMouseInput(const unsigned int checkInput)const;
};