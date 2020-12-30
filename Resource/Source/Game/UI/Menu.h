#pragma once
#include "UI.h"
#include <array>
#include <functional>
#include <vector>
#include "Geometry.h"
#include "TimeLine.h"

class SelectPen;
class PlayerCommander;
class MapCtrl;

/// <summary>
/// メニュー表示用基底クラス
/// </summary>
class Menu :
	public UI
{
protected:
	// プレイヤー指揮官
	PlayerCommander& _playerCommander;
	// マップ管理クラス
	const MapCtrl& _mapCtrl;

	// 項目サイズ
	Size _contentSize;
	// 選択中の項目インデックス
	size_t _selectIdx;

	// 開くSE
	int _openSEH;
	// 閉じるSE
	int _closeSEH;
	// 移動SE
	int _moveSEH;

	// 開くアニメーショントラック
	std::unique_ptr<Track<float>> _openAnimTrack;
	// 閉じるアニメーショントラック
	std::unique_ptr<Track<float>> _closeAnimTrack;

	// 選択中の矢印
	std::unique_ptr<SelectPen> _selectPen;

	/// <summary>
	/// 項目情報
	/// </summary>
	struct ContentInf
	{
		// 項目名
		std::string name;
		// 表示座標
		Vector2Int centerPos;
		// 選択時に呼ぶ関数
		std::function<void()> func;
	};

	// 項目管理配列
	std::vector<ContentInf> _contentInfs;
	// 項目インデックス管理リスト
	std::vector<unsigned int> _contentList;

	// 更新関数ポインタ
	void (Menu::* _updater)(const Input& input);
	// 描画関数ポインタ
	void (Menu::* _drawer)();

	/// <summary>
	/// 開かれた状態更新
	/// </summary>
	/// <param name="input">入力情報</param>
	virtual void OpenUpdate(const Input& input);

	/// <summary>
	/// 閉じた状態更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void CloseUpdate(const Input& input);

	/// <summary>
	/// 開くアニメーション更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void OpenAnimUpdate(const Input& input);

	/// <summary>
	/// 閉じるアニメーション更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void CloseAnimUpdate(const Input& input);

	/// <summary>
	/// 開かれた状態描画
	/// </summary>
	void OpenDraw();

	/// <summary>
	/// 閉じた状態描画
	/// </summary>
	void CloseDraw();

	/// <summary>
	/// 開くアニメーション描画
	/// </summary>
	void OpenAnimDraw();

	/// <summary>
	/// 閉じるアニメーション描画
	/// </summary>
	void CloseAnimDraw();

	/// <summary>
	/// 項目描画
	/// </summary>
	/// <param name="drawCenterPos">表示中心座標</param>
	/// <param name="idx">項目インデックス</param>
	void DrawContent(const Vector2Int& drawCenterPos, const unsigned int idx);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="contentNum">項目数</param>
	/// <param name="frameH">項目表示枠画像ハンドル</param>
	void Init(const size_t contentNum, const int frameH);

	/// <summary>
	/// 戻る
	/// </summary>
	virtual void Back();

	/// <summary>
	/// 決定
	/// </summary>
	virtual void Decision();

public:
	/// <param name="uiDeque">UIの親子関係管理Deque</param>
	/// <param name="playerCom">プレイヤー指揮官</param>
	/// <param name="mapCtrl">マップ管理クラス</param>
	Menu(std::deque<std::shared_ptr<UI>>* uiDeque, PlayerCommander& playerCom, const MapCtrl& mapCtrl);
	~Menu();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void Update(const Input& input)override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// 開く
	/// </summary>
	/// <param name="animation">アニメーションをするか</param>
	void Open(bool animation = true);

	/// <summary>
	/// 閉じる
	/// </summary>
	/// <param name="animation">アニメーションをするか</param>
	void Close(bool animation = true);
};