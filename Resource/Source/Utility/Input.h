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
	// プレイヤー番号と、入力文字列から
	// 元の入力がわかる。つまりキーコンフィグ一覧
	// を表示するときにつかう

	mutable std::map<std::string, std::array<int, static_cast<size_t>(PeripheralType::max)>> _inputTbl;

	using inputFunc_t = std::function<void(const std::string&, const int)>;
	/// <summary>
	/// 機器ごとに入力情報を更新する関数テーブル
	/// </summary>
	std::array<inputFunc_t, static_cast<size_t>(PeripheralType::max)> _peripheralInfFuncs;

	Vector2Int _mousePosOld;	// 1フレーム前のマウス座標
	Vector2Int _mousePos;		// マウスの座標

	using InputState = std::map<std::string, bool>;
	std::array<InputState, 2> _InputState;
	size_t _currentInputStateIdx;

	static const int INPUT_RECORD_SIZE = 2;
	static const int INPUT_KEY_SIZE = 256;
	std::array<std::array<char, INPUT_KEY_SIZE>, INPUT_RECORD_SIZE> _keystate;		//キーボードの状況

	std::array<int, INPUT_RECORD_SIZE> _padState;				// 現在のパッドの状況
	std::array<int, INPUT_RECORD_SIZE> _mouseState;					// 現在のマウス入力情報

	/// <summary>
	/// 入力テーブルを取得する
	/// </summary>
	const std::map<std::string, std::array<int, static_cast<size_t>(PeripheralType::max)>>& GetInputTable()const;
	
	/// <summary>
	/// 生の入力テーブルを書き換える
	/// </summary>
	/// <param name="rowInputTable">書き換える値</param>
	void SetRowInputTable(const std::map<std::string, std::array<int, static_cast<size_t>(PeripheralType::max)>>& rowInputTable)const;

	/// <summary>
	/// 登録済みのコマンドを書き換える
	/// </summary>
	/// <param name="cmd">コマンド名</param>
	/// <param name="perType">機器種</param>
	/// <param name="code">入力マスク等</param>
	/// <returns>書き換え成功か</returns>
	bool ChangeCommand(const std::string& cmd, const PeripheralType perType, const int code)const;

	/// <summary>
	/// 次の入力バッファインデックスを返す
	/// </summary>
	/// <returns> 次の入力バッファインデックス </returns>
	size_t GetNextInputBufferIndex()const;

	// 1ﾌﾚｰﾑ前の入力バッファインデックスを返す
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

	// 毎フレーム入力情報を更新する
	void Update(void);

	// コマンド設定
	// @param pno	プレイヤー番号
	// @param cmd	コマンド文字列
	// @param periNo	入力番号
	// @param code	入力コード
	void AddCommand(const std::string& cmd, const PeripheralType perType, const int code);

	// 今、押されています
	// @param pno プレイヤー番号
	// @param cmd	コマンド文字列
	bool GetButton(const std::string& cmd) const;

	// 今まさに押されました
	// @param pno プレイヤー番号
	// @param cmd	コマンド文字列
	bool GetButtonDown(const std::string& cmd) const;

	// ボタンを押すのをやめた瞬間
	// @param pno プレイヤー番号
	// @param cmd	コマンド文字列
	bool GetButtonUp(const std::string& cmd) const;

	// _inputTblに指定したコマンドが存在するか確認 true	存在する	false 存在しない
	// @param pno プレイヤー番号
	// @param cmd	コマンド文字列
	bool CheckCommand(const std::string& cmd) const;

	// マウスの座標を返す
	const Vector2Int& GetMousePos()const;

	bool GetButton(const char keycode)const;
	bool GetButtonDown(const char keycode)const;
	bool GetButtonUp(const char keycode)const;

	bool GetAnyMouseInput()const;
	bool GetAnyKeybordInput()const;
	bool GetAnyPadInput()const;

	Vector2Int GetMouseMove()const;
};