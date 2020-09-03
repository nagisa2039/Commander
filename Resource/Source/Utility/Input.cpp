#include <DxLib.h>
#include <algorithm>
#include <cassert>
#include "Input.h"

using namespace std;

Input::Input()
{
	_currentInputStateIdx = 0;

	_InputState = {};
	_keystate = {};

	_mouseState = {};
	_padState = {};

	_mousePosOld = {};
	_mousePos = {};

	// ゲームパッド
	_peripheralInfFuncs[static_cast<size_t>(PeripheralType::gamepad)] = [this](const string& cmd, const int code)
	{
		_InputState[_currentInputStateIdx][cmd]|= static_cast<bool>(code & _padState[_currentInputStateIdx]);
	};

	// キーボード
	_peripheralInfFuncs[static_cast<size_t>(PeripheralType::keybord)] = [this](const string& cmd, const int code)
	{
		_InputState[_currentInputStateIdx][cmd] |= static_cast<bool>(_keystate[_currentInputStateIdx][code]);
	};

	// マウス
	_peripheralInfFuncs[static_cast<size_t>(PeripheralType::mouse)] = [this](const string& cmd, const int code)
	{
		_InputState[_currentInputStateIdx][cmd] |= static_cast<bool>(code & _mouseState[_currentInputStateIdx]);
	};
}

Input::~Input()
{
}

void Input::Update(void)
{
	_mousePosOld = _mousePos;
	_currentInputStateIdx = GetNextInputBufferIndex();

	// キーボードの入力状態更新
	GetHitKeyStateAll(_keystate[_currentInputStateIdx].data());

	// マウスの入力情報更新
	_mouseState[_currentInputStateIdx] = GetMouseInput();
	GetMousePoint(&_mousePos.x, &_mousePos.y);

	_padState[_currentInputStateIdx] = GetJoypadInputState(DX_INPUT_PAD1);

	// 状態のリセット
	for (auto& state : _InputState[_currentInputStateIdx])
	{
		state.second = false;
	}

	// pair = <コマンド文字列, PeripheralInfo(code = 入力コード, periNo = 入力機器番号)>
	for (auto pair : _inputTbl)
	{
		for (int i = 0; i < pair.second.size();++i)
		{
			_peripheralInfFuncs[i](pair.first, pair.second[i]);
		}
	}
}

void Input::AddCommand(const std::string& cmd, const Input::PeripheralType perType, const int code)
{
	if (!_inputTbl.contains(cmd))
	{
		_inputTbl[cmd] = {0,0,0};
	}
	_inputTbl[cmd][static_cast<size_t>(perType)] = code;

	// 押下情報を初期化
	for (auto& inputState : _InputState)
	{
		inputState[cmd] = false;
	}
}

bool Input::GetButton(const std::string& cmd) const
{
	// 範囲外制御
	if (!CheckCommand(cmd))
	{
		return false;
	}

	return CurrentInput(cmd);
}

bool Input::GetButtonDown(const std::string& cmd) const
{
	// 範囲外制御
	if (!CheckCommand(cmd))
	{
		assert(0);
		return false;
	}

	return CurrentInput(cmd) && !LastInput(cmd);
}

bool Input::GetButtonUp(const std::string& cmd) const
{
	// 範囲外制御
	if (!CheckCommand(cmd))
	{
		return false;
	}

	return !CurrentInput(cmd) && LastInput(cmd);
}

bool Input::CheckCommand(const std::string& cmd) const
{
	size_t lastIdx = GetLastInputBufferIndex();

	return (_InputState[_currentInputStateIdx].contains(cmd)
		&& _InputState[lastIdx].contains(cmd));
}

const Vector2Int& Input::GetMousePos()const
{
	return _mousePos;
}

bool Input::GetButton(const char keycode)const
{
	return _keystate[_currentInputStateIdx][keycode];
}

bool Input::GetButtonDown(const char keycode)const
{
	size_t lastIdx = (_currentInputStateIdx - 1 + INPUT_RECORD_SIZE) % INPUT_RECORD_SIZE;
	return _keystate[_currentInputStateIdx][keycode] && !_keystate[lastIdx][keycode];
}

bool Input::GetButtonUp(const char keycode)const
{
	size_t lastIdx = (_currentInputStateIdx - 1 + INPUT_RECORD_SIZE) % INPUT_RECORD_SIZE;
	return !_keystate[_currentInputStateIdx][keycode] && _keystate[lastIdx][keycode];
}

bool Input::GetAnyMouseInput() const
{
	return static_cast<bool>(_mouseState[_currentInputStateIdx]) || GetMouseMove() != Vector2Int(0,0);
}

bool Input::GetAnyKeybordInput() const
{
	return DxLib::CheckHitKeyAll(DX_CHECKINPUT_KEY);
}

bool Input::GetAnyPadInput() const
{
	return static_cast<bool>(_padState[_currentInputStateIdx]);
}

Vector2Int Input::GetMouseMove() const
{
	return _mousePos - _mousePosOld;
}

const std::map<std::string, std::array<int, static_cast<size_t>(Input::PeripheralType::max)>>& Input::GetInputTable() const
{
	return _inputTbl;
}

void Input::SetRowInputTable(const std::map<std::string, std::array<int, static_cast<size_t>(PeripheralType::max)>>& rowInputTable) const
{
	_inputTbl = rowInputTable;
}

bool Input::ChangeCommand(const std::string& cmd, const PeripheralType perType, const int code)const
{
	if (!_inputTbl.contains(cmd))
	{
		return false;
	}

	_inputTbl[cmd][static_cast<size_t>(perType)] = code;
	return true;
}

size_t Input::GetNextInputBufferIndex()const
{
	return (_currentInputStateIdx + 1) % INPUT_RECORD_SIZE;
}

size_t Input::GetLastInputBufferIndex()const
{
	return (_currentInputStateIdx - 1 + INPUT_RECORD_SIZE) % INPUT_RECORD_SIZE;
}

const bool& Input::CurrentInput(const std::string& cmd)const
{
	return _InputState[_currentInputStateIdx].at(cmd);
}

const bool& Input::LastInput(const std::string& cmd)const
{
	return _InputState[GetLastInputBufferIndex()].at(cmd);
}

std::array<char, 256> Input::GetKeybordState() const
{
	return _keystate[_currentInputStateIdx];
}

std::array<char, 256> Input::GetKeybordDownState() const
{
	auto downState = std::array<char, INPUT_KEY_SIZE>();
	auto lastIdx = GetLastInputBufferIndex();
	for (int i = 0; i < INPUT_KEY_SIZE; ++i)
	{
		bool current = static_cast<bool>(_keystate[_currentInputStateIdx][i]);
		bool last = static_cast<bool>(_keystate[lastIdx][i]);
		downState[i] = static_cast<char>(current && !last);
	}
	return downState;
}

int Input::GetPadState() const
{
	return _padState[_currentInputStateIdx];
}

int Input::GetPadDownState() const
{
	int downState = 0;
	auto lastIdx = GetLastInputBufferIndex();
	for (int i = 0; i < sizeof(downState) * 8; ++i)
	{
		auto bit = 1 << i;
		bool current = static_cast<bool>(_padState[_currentInputStateIdx] & bit);
		bool last = static_cast<bool>(_padState[lastIdx] & bit);
		if (current && !last)
		{
			downState |= bit;
		}
	}
	return downState;
}

int Input::GetMouseState() const
{
	return _mouseState[_currentInputStateIdx];
}
