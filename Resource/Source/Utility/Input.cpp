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
	// 0ならキーボード分の1を入れる
	playerCnt = pcount <= 0 ? 1 : pcount;
	playerCnt = 5;

	// キー情報格納用変数をプレイヤー分格納
	_inputTbl.resize(playerCnt);
	_currentInputState.resize(playerCnt);
	_lastInputState.resize(playerCnt);
	_padState.resize(playerCnt);
}

void Input::Update(void)
{
	// キーボードの入力状態更新
	_lastKeystate = _keystate;
	GetHitKeyStateAll(_keystate.data());

	// マウスの入力情報更新
	_lastMouseState = _mouseState;
	_mouseState = GetMouseInput();
	GetMousePoint(&_mousePos.x, &_mousePos.y);

	// 接続数分回す
	for (int j = 0; j < GetJoypadNum(); j++)
	{
		// 各パッドの入力状態更新
		_padState[j] = GetJoypadInputState(DX_INPUT_PAD1 + j);
	}

	// 入力の状態更新
	_lastInputState = _currentInputState;

	// プレイヤー数分回す
	for (int pno = 0; pno < static_cast<int>(_inputTbl.size());pno++)
	{
		// 状態のリセット
		for (auto& state : _currentInputState[pno])
		{
			state.second = false;
		}

		// pair = <コマンド文字列, PeripheralInfo(code = 入力コード, periNo = 入力機器番号)>
		for (auto pair : _inputTbl[pno])
		{
			if (pair.second.periNo == 0)
			{
				// キーボード
				_currentInputState[pno][pair.first] |= static_cast<bool>(_keystate[pair.second.code]);
			}
			else if (pair.second.periNo == 5)
			{
				// マウス
				_currentInputState[pno][pair.first] |= static_cast<bool>(pair.second.code & _mouseState);
			}
			else
			{
				// ゲームパッド
				_currentInputState[pno][pair.first] |= static_cast<bool>(pair.second.code & _padState[pair.second.periNo - 1]);
			}
		}
	}

}

void Input::AddCommand(int pno, std::string cmd, int periNo, int code)
{
	_inputTbl[pno].emplace(cmd, PeripheralInfo(periNo,code));				// コマンドに対する入力を設定
	_inputMap.emplace(PeripheralInfo(periNo,code), make_pair(pno, cmd));	// 入力に対するプレイヤー番号とコマンドを設定

	// 押下情報を初期化
	_currentInputState[pno][cmd]	= false;
	_lastInputState[pno][cmd]		= false;
}

bool Input::GetButton(int pno, std::string cmd) const
{
	// 範囲外制御
	if (!CheckCommand(pno, cmd))
	{
		return false;
	}

	return _currentInputState[pno].at(cmd);
}

bool Input::GetButtonDown(int pno, std::string cmd) const
{
	// 範囲外制御
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
	// 範囲外制御
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
