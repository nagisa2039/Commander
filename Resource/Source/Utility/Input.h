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
	int periNo;		// 周辺機器番号
	int code;		// 周辺機器生入力情報 (例：PAD_INPUT_1)
};

struct MY_XINPUT_STATE
{
	unsigned char			Buttons[16];					// ボタン１６個( 添字には XINPUT_BUTTON_DPAD_UP 等を使用する、0:押されていない  1:押されている )
	unsigned char			LeftTrigger;					// 左トリガー( 0～255 )
	unsigned char			RightTrigger;					// 右トリガー( 0～255 )
	short					ThumbLX;						// 左スティックの横軸値( -32768 ～ 32767 )
	short					ThumbLY;						// 左スティックの縦軸値( -32768 ～ 32767 )
	short					ThumbRX;						// 右スティックの横軸値( -32768 ～ 32767 )
	short					ThumbRY;						// 右スティックの縦軸値( -32768 ～ 32767 )
};
 
bool operator< (const PeripheralInfo& lval, const PeripheralInfo& rval);
bool operator==(const PeripheralInfo& lval, const PeripheralInfo& rval);
bool operator!=(const PeripheralInfo& lval, const PeripheralInfo& rval);

class Input
{
private:
	// プレイヤー番号と、入力文字列から
	// 元の入力がわかる。つまりキーコンフィグ一覧
	// を表示するときにつかう
	std::vector<std::multimap<std::string, PeripheralInfo>> _inputTbl;

	// 入力から、プレイヤー番号および
	// 入力文字列を検索するために使う
	std::map<PeripheralInfo, std::pair<int, std::string>> _inputMap;

	// 現在の押下情報を格納
	// プレイヤー番号、コマンド文字列
	std::vector <std::map<std::string, bool>> _currentInputState;	

	// 1フレーム前の押下情報を格納
	// プレイヤー番号、コマンド文字列
	std::vector <std::map<std::string, bool>> _lastInputState;

	std::array<char, 256> _keystate;		// 現在のキーボードの状況
	std::array<char, 256> _lastKeystate;	// 1フレーム前のキーボードの状況
	std::vector<int> _padState;				// 現在のパッドの状況
	
	std::vector<MY_XINPUT_STATE> _currentXInputState;	// Xinputの入力情報
	std::vector<MY_XINPUT_STATE> _lastXInputState;	// Xinputの入力情報
	int _mouseState;						// 現在のマウス入力情報
	int _lastMouseState;					// 1フレーム前のﾏｳｽの状況
	Vector2Int _mousePos;					// マウスの座標
	Vector2Int _lastMousePos;				// 1フレーム前のマウス座標
	int playerCnt;							// プレイヤーの人数

public:
	Input();
	~Input();

	// 接続中のパッド数を返す
	int GetConnectPadCnt(void) const;

	// プレイヤー数を設定
	// @param pcount プレイヤー数
	void SetPlayerCnt(const int pcount);

	// 毎フレーム入力情報を更新する
	void Update(void);

	// コマンド設定
	// @param pno	プレイヤー番号
	// @param cmd	コマンド文字列
	// @param periNo	入力番号
	// @param code	入力コード
	void AddCommand(int pno, std::string cmd, int periNo, int code);

	// 今、押されています
	// @param pno プレイヤー番号
	// @param cmd	コマンド文字列
	bool GetButton(int pno, std::string cmd) const;

	// 今まさに押されました
	// @param pno プレイヤー番号
	// @param cmd	コマンド文字列
	bool GetButtonDown(int pno, std::string cmd) const;

	// ボタンを押すのをやめた瞬間
	// @param pno プレイヤー番号
	// @param cmd	コマンド文字列
	bool GetButtonUp(int pno, std::string cmd) const;

	// 全プレイヤーの入力状況を返す
	std::vector <std::map<std::string, bool>> GetCurrnetInputState(void) const;

	// プレイヤー人数を返す
	int GetPlayerCnt(void) const;

	// _inputTblに指定したコマンドが存在するか確認 true	存在する	false 存在しない
	// @param pno プレイヤー番号
	// @param cmd	コマンド文字列
	bool CheckCommand(int pno, std::string cmd) const;

	// マウスの座標を返す
	const Vector2Int& GetMousePos()const;
	// 1フレーム前とのマウス移動量を返す
	const Vector2Int GetMouseMove()const;

	bool GetButton(const char keycode)const;
	bool GetButtonDown(const char keycode)const;
	bool GetButtonUp(const char keycode)const;

	bool GetXInputButtonDown(const int padNum, const int keycode)const;

	// キーボードから何かしらの入力があるか
	bool GetAnyKeybordInput()const;
	// マウスから何かしらの入力があるか
	bool GetAnyMouseInput()const;
};

