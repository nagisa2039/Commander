#pragma once
#include <map>
#include <string>
#include <vector>
#include <array>
#include <functional>
#include "Geometry.h"

/// <summary>
/// 入力情報管理クラス
/// </summary>
class Input
{
public:
	/// <summary>
	/// 機種
	/// </summary>
	enum class PeripheralType
	{
		// キーボード
		keybord,
		// パッド
		gamepad,
		// マウス
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

	// 1フレーム前のマウス座標
	Vector2Int _mousePosOld;
	// マウスの座標
	Vector2Int _mousePos;

	// 入力情報の記憶バッファ数
	static const int INPUT_RECORD_SIZE = 2;
	using InputState = std::map<std::string, bool>;
	// 入力状態
	std::array<InputState, INPUT_RECORD_SIZE> _InputState;
	// 現在の入力バッファインデックス
	size_t _currentInputStateIdx;

	// キー数
	static const int INPUT_KEY_SIZE = 256;
	// キーボードの状況
	std::array<std::array<char, INPUT_KEY_SIZE>, INPUT_RECORD_SIZE> _keystate;

	// 現在のパッドの状況
	std::array<int, INPUT_RECORD_SIZE> _padState;
	// 現在のマウス入力情報
	std::array<int, INPUT_RECORD_SIZE> _mouseState;

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

	/// <summary>
	/// 1フレーム前の入力バッファインデックスを返す
	/// </summary>
	size_t GetLastInputBufferIndex()const;

	/// <summary>
	/// 指定したコマンドの現在の入力状況を取得
	/// </summary>
	/// <param name="cmd">コマンド</param>
	const bool& CurrentInput(const std::string& cmd)const;

	/// <summary>
	/// 指定したコマンドの1フレーム前の入力状況を取得
	/// </summary>
	/// <param name="cmd">コマンド</param>
	const bool& LastInput(const std::string& cmd)const;

	/// <summary>
	/// キーボードの入力状況を取得
	/// </summary>
	std::array<char, INPUT_KEY_SIZE> GetKeybordState()const;

	/// <summary>
	/// キーボードの押した瞬間を取得
	/// </summary>
	std::array<char, INPUT_KEY_SIZE> GetKeybordDownState()const;

	/// <summary>
	/// パッドの入力状況取得
	/// </summary>
	int GetPadState()const;

	/// <summary>
	/// パッドの押された瞬間取得
	/// </summary>
	int GetPadDownState()const;

	/// <summary>
	/// マウスの入力状況取得
	/// </summary>
	int GetMouseState()const;

public:
	Input();
	~Input();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(void);

	/// <summary>
	/// コマンド設定
	/// </summary>
	/// <param name="cmd">コマンド文字列</param>
	/// <param name="perType">機種</param>
	/// <param name="code">入力コード</param>
	void AddCommand(const std::string& cmd, const PeripheralType perType, const int code);

	/// <summary>
	/// 現在押されているか
	/// </summary>
	/// <param name="cmd">コマンド文字列</param>
	bool GetButton(const std::string& cmd) const;

	/// <summary>
	/// 押された瞬間か
	/// </summary>
	/// <param name="cmd">コマンド文字列</param>
	bool GetButtonDown(const std::string& cmd) const;

	/// <summary>
	/// ボタンを押すのをやめた瞬間
	/// </summary>
	/// <param name="cmd">コマンド文字列</param>
	bool GetButtonUp(const std::string& cmd) const;

	/// <summary>
	/// _inputTblに指定したコマンドが存在するか確認
	/// </summary>
	/// <param name="cmd">コマンド文字列</param>
	bool CheckCommand(const std::string& cmd) const;

	/// <summary>
	/// マウスの座標を返す
	/// </summary>
	/// <returns></returns>
	const Vector2Int& GetMousePos()const;

	/// <summary>
	/// ボタンの押下情報取得
	/// </summary>
	/// <param name="keycode">キーコード</param>
	bool GetButton(const char keycode)const;

	/// <summary>
	/// ボタンを押下した瞬間情報取得
	/// </summary>
	/// <param name="keycode">キーコード</param>
	bool GetButtonDown(const char keycode)const;

	/// <summary>
	/// ボタンを離した瞬間情報取得
	/// </summary>
	/// <param name="keycode">キーコード</param>
	bool GetButtonUp(const char keycode)const;

	/// <summary>
	/// 何かしらのマウスボタン入力があったか取得
	/// </summary>
	bool GetAnyMouseInput()const;

	/// <summary>
	/// 何かしらのキーボード入力があったか取得
	/// </summary>
	bool GetAnyKeybordInput()const;

	/// <summary>
	/// 何かしらのパッド入力があったか取得
	/// </summary>
	bool GetAnyPadInput()const;

	/// <summary>
	/// マウスの移動量取得
	/// </summary>
	Vector2Int GetMouseMove()const;

	/// <summary>
	/// マウスのボタン入力状況取得
	/// </summary>
	/// <param name="checkInput">マウスボタンコード</param>
	bool GetMouseInput(const unsigned int checkInput)const;
};