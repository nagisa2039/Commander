#pragma once
#include <map>
#include <string>
#include <vector>
#include <array>
#include "Geometry.h"

struct PeripheralInfo
{
	PeripheralInfo() :periNo(0), code(0) {};
	PeripheralInfo(int pno, int cd) :periNo(pno), code(cd) {}
	int periNo;		// ���Ӌ@��ԍ�
	int code;		// ���Ӌ@�퐶���͏�� (��FPAD_INPUT_1)
};

struct MY_XINPUT_STATE
{
	unsigned char			Buttons[16];					// �{�^���P�U��( �Y���ɂ� XINPUT_BUTTON_DPAD_UP �����g�p����A0:������Ă��Ȃ�  1:������Ă��� )
	unsigned char			LeftTrigger;					// ���g���K�[( 0�`255 )
	unsigned char			RightTrigger;					// �E�g���K�[( 0�`255 )
	short					ThumbLX;						// ���X�e�B�b�N�̉����l( -32768 �` 32767 )
	short					ThumbLY;						// ���X�e�B�b�N�̏c���l( -32768 �` 32767 )
	short					ThumbRX;						// �E�X�e�B�b�N�̉����l( -32768 �` 32767 )
	short					ThumbRY;						// �E�X�e�B�b�N�̏c���l( -32768 �` 32767 )
};
 
bool operator< (const PeripheralInfo& lval, const PeripheralInfo& rval);
bool operator==(const PeripheralInfo& lval, const PeripheralInfo& rval);
bool operator!=(const PeripheralInfo& lval, const PeripheralInfo& rval);

class Input
{
private:
	// �v���C���[�ԍ��ƁA���͕����񂩂�
	// ���̓��͂��킩��B�܂�L�[�R���t�B�O�ꗗ
	// ��\������Ƃ��ɂ���
	std::vector<std::multimap<std::string, PeripheralInfo>> _inputTbl;

	// ���͂���A�v���C���[�ԍ������
	// ���͕�������������邽�߂Ɏg��
	std::map<PeripheralInfo, std::pair<int, std::string>> _inputMap;

	// ���݂̉��������i�[
	// �v���C���[�ԍ��A�R�}���h������
	std::vector <std::map<std::string, bool>> _currentInputState;	

	// 1�t���[���O�̉��������i�[
	// �v���C���[�ԍ��A�R�}���h������
	std::vector <std::map<std::string, bool>> _lastInputState;

	std::array<char, 256> _keystate;		// ���݂̃L�[�{�[�h�̏�
	std::array<char, 256> _lastKeystate;	// 1�t���[���O�̃L�[�{�[�h�̏�
	std::vector<int> _padState;				// ���݂̃p�b�h�̏�
	
	std::vector<MY_XINPUT_STATE> _currentXInputState;	// Xinput�̓��͏��
	std::vector<MY_XINPUT_STATE> _lastXInputState;	// Xinput�̓��͏��
	int _mouseState;						// ���݂̃}�E�X���͏��
	int _lastMouseState;					// 1�t���[���O��ϳ��̏�
	Vector2Int _mousePos;					// �}�E�X�̍��W
	Vector2Int _lastMousePos;				// 1�t���[���O�̃}�E�X���W
	int playerCnt;							// �v���C���[�̐l��

public:
	Input();
	~Input();

	// �ڑ����̃p�b�h����Ԃ�
	int GetConnectPadCnt(void) const;

	// �v���C���[����ݒ�
	// @param pcount �v���C���[��
	void SetPlayerCnt(const int pcount);

	// ���t���[�����͏����X�V����
	void Update(void);

	// �R�}���h�ݒ�
	// @param pno	�v���C���[�ԍ�
	// @param cmd	�R�}���h������
	// @param periNo	���͔ԍ�
	// @param code	���̓R�[�h
	void AddCommand(int pno, std::string cmd, int periNo, int code);

	// ���A������Ă��܂�
	// @param pno �v���C���[�ԍ�
	// @param cmd	�R�}���h������
	bool GetButton(int pno, std::string cmd) const;

	// ���܂��ɉ�����܂���
	// @param pno �v���C���[�ԍ�
	// @param cmd	�R�}���h������
	bool GetButtonDown(int pno, std::string cmd) const;

	// �{�^���������̂���߂��u��
	// @param pno �v���C���[�ԍ�
	// @param cmd	�R�}���h������
	bool GetButtonUp(int pno, std::string cmd) const;

	// �S�v���C���[�̓��͏󋵂�Ԃ�
	std::vector <std::map<std::string, bool>> GetCurrnetInputState(void) const;

	// �v���C���[�l����Ԃ�
	int GetPlayerCnt(void) const;

	// _inputTbl�Ɏw�肵���R�}���h�����݂��邩�m�F true	���݂���	false ���݂��Ȃ�
	// @param pno �v���C���[�ԍ�
	// @param cmd	�R�}���h������
	bool CheckCommand(int pno, std::string cmd) const;

	// �}�E�X�̍��W��Ԃ�
	const Vector2Int& GetMousePos()const;
	// 1�t���[���O�Ƃ̃}�E�X�ړ��ʂ�Ԃ�
	const Vector2Int GetMouseMove()const;

	bool GetButton(const char keycode)const;
	bool GetButtonDown(const char keycode)const;
	bool GetButtonUp(const char keycode)const;

	bool GetXInputButtonDown(const int padNum, const int keycode)const;

	// �L�[�{�[�h���牽������̓��͂����邩
	bool GetAnyKeybordInput()const;
	// �}�E�X���牽������̓��͂����邩
	bool GetAnyMouseInput()const;
};

