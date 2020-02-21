#include <DxLib.h>
#include <algorithm>
#include <cassert>
#include "Input.h"

using namespace std;

bool operator<(const PeripheralInfo& lval, const PeripheralInfo& rval)
{
	if (lval.periNo == rval.periNo)
	{
		return lval.code < rval.code;
	}
	return lval.periNo < rval.periNo;
}

bool operator==(const PeripheralInfo & lval, const PeripheralInfo & rval)
{
	return (lval.code == rval.code && lval.periNo == rval.periNo);
}

bool operator!=(const PeripheralInfo & lval, const PeripheralInfo & rval)
{
	return !(lval.code == rval.code && lval.periNo == rval.periNo);
}

Input::Input()
{
}


Input::~Input()
{
}

int Input::GetConnectPadCnt(void) const
{
	return GetJoypadNum();
}

void Input::SetPlayerCnt(const int pcount)
{
	// 0�Ȃ�L�[�{�[�h����1������
	playerCnt = pcount <= 0 ? 1 : pcount;
	playerCnt = 5;

	// �L�[���i�[�p�ϐ����v���C���[���i�[
	_inputTbl.resize(playerCnt);
	_currentInputState.resize(playerCnt);
	_lastInputState.resize(playerCnt);
	_padState.resize(playerCnt);
}

void Input::Update(void)
{
	// �L�[�{�[�h�̓��͏�ԍX�V
	_lastKeystate = _keystate;
	GetHitKeyStateAll(_keystate.data());

	// �}�E�X�̓��͏��X�V
	_lastMouseState = _mouseState;
	_mouseState = GetMouseInput();
	GetMousePoint(&_mousePos.x, &_mousePos.y);

	// �ڑ�������
	for (int j = 0; j < GetJoypadNum(); j++)
	{
		// �e�p�b�h�̓��͏�ԍX�V
		_padState[j] = GetJoypadInputState(DX_INPUT_PAD1 + j);
	}

	// ���͂̏�ԍX�V
	_lastInputState = _currentInputState;

	// �v���C���[������
	for (int pno = 0; pno < static_cast<int>(_inputTbl.size());pno++)
	{
		// ��Ԃ̃��Z�b�g
		for (auto& state : _currentInputState[pno])
		{
			state.second = false;
		}

		// pair = <�R�}���h������, PeripheralInfo(code = ���̓R�[�h, periNo = ���͋@��ԍ�)>
		for (auto pair : _inputTbl[pno])
		{
			if (pair.second.periNo == 0)
			{
				// �L�[�{�[�h
				_currentInputState[pno][pair.first] |= static_cast<bool>(_keystate[pair.second.code]);
			}
			else if (pair.second.periNo == 5)
			{
				// �}�E�X
				_currentInputState[pno][pair.first] |= static_cast<bool>(pair.second.code & _mouseState);
			}
			else
			{
				// �Q�[���p�b�h
				_currentInputState[pno][pair.first] |= static_cast<bool>(pair.second.code & _padState[pair.second.periNo - 1]);
			}
		}
	}

}

void Input::AddCommand(int pno, std::string cmd, int periNo, int code)
{
	_inputTbl[pno].emplace(cmd, PeripheralInfo(periNo,code));				// �R�}���h�ɑ΂�����͂�ݒ�
	_inputMap.emplace(PeripheralInfo(periNo,code), make_pair(pno, cmd));	// ���͂ɑ΂���v���C���[�ԍ��ƃR�}���h��ݒ�

	// ��������������
	_currentInputState[pno][cmd]	= false;
	_lastInputState[pno][cmd]		= false;
}

bool Input::GetButton(int pno, std::string cmd) const
{
	// �͈͊O����
	if (!CheckCommand(pno, cmd))
	{
		return false;
	}

	return _currentInputState[pno].at(cmd);
}

bool Input::GetButtonDown(int pno, std::string cmd) const
{
	// �͈͊O����
	if (!CheckCommand(pno, cmd))
	{
		return false;
	}

	bool currnt = _currentInputState[pno].at(cmd);
	bool last = _lastInputState[pno].at(cmd);

	return currnt && !last;
}

bool Input::GetButtonUp(int pno, std::string cmd) const
{
	// �͈͊O����
	if (!CheckCommand(pno, cmd))
	{
		return false;
	}

	bool currnt = _currentInputState[pno].at(cmd);
	bool last = _lastInputState[pno].at(cmd);

	return !currnt && last;
}

std::vector<std::map<std::string, bool>> Input::GetCurrnetInputState(void) const
{
	return _currentInputState;
}

int Input::GetPlayerCnt(void) const
{
	return playerCnt;
}

bool Input::CheckCommand(int pno, std::string cmd) const
{
	assert(pno < static_cast<int>(_inputTbl.size()));
	return (_currentInputState[pno].find(cmd)	!= _currentInputState[pno].end()
		 && _lastInputState[pno].find(cmd)		!= _lastInputState[pno].end());
}

const Vector2Int& Input::GetMousePos()const
{
	return _mousePos;
}

bool Input::GetButton(const char keycode)const
{
	return _keystate[keycode];
}

bool Input::GetButtonDown(const char keycode)const
{
	return _keystate[keycode] && !_lastKeystate[keycode];
}

bool Input::GetButtonUp(const char keycode)const
{
	return !_keystate[keycode] && _lastKeystate[keycode];
}
