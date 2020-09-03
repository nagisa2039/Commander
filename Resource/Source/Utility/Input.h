#pragma once
#include <map>
#include <string>
#include <vector>
#include <array>
#include <functional>
#include "Geometry.h"

class KeyConfigScene;

class Input
{
	friend KeyConfigScene;
public:
	enum class PeripheralType
	{
		keybord,
		gamepad,
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

	Vector2Int _mousePosOld;	// 1�t���[���O�̃}�E�X���W
	Vector2Int _mousePos;		// �}�E�X�̍��W

	using InputState = std::map<std::string, bool>;
	std::array<InputState, 2> _InputState;
	size_t _currentInputStateIdx;

	static const int INPUT_RECORD_SIZE = 2;
	static const int INPUT_KEY_SIZE = 256;
	std::array<std::array<char, INPUT_KEY_SIZE>, INPUT_RECORD_SIZE> _keystate;		//�L�[�{�[�h�̏�

	std::array<int, INPUT_RECORD_SIZE> _padState;				// ���݂̃p�b�h�̏�
	std::array<int, INPUT_RECORD_SIZE> _mouseState;					// ���݂̃}�E�X���͏��

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

	// 1�ڰёO�̓��̓o�b�t�@�C���f�b�N�X��Ԃ�
	size_t GetLastInputBufferIndex()const;

	const bool& CurrentInput(const std::string& cmd)const;

	const bool& LastInput(const std::string& cmd)const;

	std::array<char, 256> GetKeybordState()const;
	std::array<char, 256> GetKeybordDownState()const;
	int GetPadState()const;
	int GetPadDownState()const;
	int GetMouseState()const;

public:
	Input();
	~Input();

	// ���t���[�����͏����X�V����
	void Update(void);

	// �R�}���h�ݒ�
	// @param pno	�v���C���[�ԍ�
	// @param cmd	�R�}���h������
	// @param periNo	���͔ԍ�
	// @param code	���̓R�[�h
	void AddCommand(const std::string& cmd, const PeripheralType perType, const int code);

	// ���A������Ă��܂�
	// @param pno �v���C���[�ԍ�
	// @param cmd	�R�}���h������
	bool GetButton(const std::string& cmd) const;

	// ���܂��ɉ�����܂���
	// @param pno �v���C���[�ԍ�
	// @param cmd	�R�}���h������
	bool GetButtonDown(const std::string& cmd) const;

	// �{�^���������̂���߂��u��
	// @param pno �v���C���[�ԍ�
	// @param cmd	�R�}���h������
	bool GetButtonUp(const std::string& cmd) const;

	// _inputTbl�Ɏw�肵���R�}���h�����݂��邩�m�F true	���݂���	false ���݂��Ȃ�
	// @param pno �v���C���[�ԍ�
	// @param cmd	�R�}���h������
	bool CheckCommand(const std::string& cmd) const;

	// �}�E�X�̍��W��Ԃ�
	const Vector2Int& GetMousePos()const;

	bool GetButton(const char keycode)const;
	bool GetButtonDown(const char keycode)const;
	bool GetButtonUp(const char keycode)const;

	bool GetAnyMouseInput()const;
	bool GetAnyKeybordInput()const;
	bool GetAnyPadInput()const;

	Vector2Int GetMouseMove()const;
};