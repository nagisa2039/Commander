#pragma once
#include "UI.h"
#include "TimeLine.h"
#include <memory>
#include <array>
#include <functional>

class BattlePreparationCursor;
class MapCtrl;
class Camera;
class PlayScene;

/// <summary>
/// 戦闘準備画面
/// </summary>
class PreparationUI :
	public UI
{
private:
	/// <summary>
	/// 項目種類
	/// </summary>
	enum class Content
	{
		// 戦闘開始
		start,
		// 配置換え
		placement,	
		// 戦況確認
		warsituation,
		// 戻る
		back,
		max
	};

	/// <summary>
	/// 項目情報
	/// </summary>
	struct ContentInf
	{  
		// 名前
		std::string name;
		// 表示座標
		Vector2Int pos;
		// 決定実行関数
		std::function<void(void)> func;
		// 描画先画像ハンドル
		int graphH = -1;
	};

	// 項目サイズ
	Size _contentSize;

	// 選択中の項目
	Content _selectContent;

	// PlaySceneの参照
	PlayScene& _playScene;
	// カメラ
	Camera& _camera;
	// マップ管理クラス
	MapCtrl& _mapCtrl;

	// 開くSEハンドル
	int _openH;
	// 閉じるSEハンドル
	int _closeH;
	// 選択移動時SEハンドル
	int _moveH;
	// 決定時SEハンドル
	int _okH;
	// キャンセル時SEハンドル
	int _canselH;
	// BGMハンドル
	int _bgmH;

	// 何かしらの決定を行っているか
	bool _execution;
	// マップ選択に戻ってよいかのフラグ
	bool _backMapSelect;

	// 更新関数ポインタ
	void (PreparationUI::* _updater)(const Input& input);

	// 項目情報管理配列
	std::array<ContentInf, static_cast<size_t>(Content::max)> _contentInfTable;

	// 開閉アニメーション用トラック
	std::unique_ptr<Track<float>> _openAnimTrack;
	// 項目選択時拡縮アニメーショントラック
	std::unique_ptr<Track<float>> _selectExRateTrack;
	// 出撃マス選択時のカーソル
	std::unique_ptr<BattlePreparationCursor> _battlePreparationCursor;

	/// <summary>
	/// 閉じ切ったの更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void CloseUpdate(const Input& input);

	/// <summary>
	/// 開き切った時の更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void OpenUpdate(const Input& input);

	/// <summary>
	/// 閉じるアニメーション更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void CloseAnimUpdate(const Input& input);

	/// <summary>
	/// 開くアニメーション更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void OpenAnimUpdate(const Input& input);

	/// <summary>
	/// 閉じ切った瞬間に呼ばれる
	/// </summary>
	void CloseEnd();

public:
	/// <param name="uiDeque">UIの親子関係管理Deque</param>
	/// <param name="camera"></param>
	/// <param name="mapCtrl">マップ管理クラス</param>
	/// <param name="playScene"></param>
	PreparationUI(std::deque<std::shared_ptr<UI>>* uiDeque, Camera& camera, MapCtrl& mapCtrl, PlayScene& playScene);
	~PreparationUI();

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
	/// キャラクター描画の前に行う
	/// 移動可能マスや出撃可能マスの描画
	/// </summary>
	void BeginDraw();

	/// <summary>
	/// 開く
	/// </summary>
	/// <param name="animation">アニメーションを行うか</param>
	void Open(const bool animation);

	/// <summary>
	/// 閉じる
	/// </summary>
	/// <param name="animation">アニメーションを行うか</param>
	void Close(const bool animation);

	/// <summary>
	/// BGMの開始
	/// </summary>
	void StartBGM();

	/// <summary>
	/// マップ選択に戻るためのフラグセット
	/// </summary>
	void BackMapSelect();

	/// <summary>
	/// マップ選択に戻るかのフラグ取得
	/// </summary>
	/// <returns></returns>
	bool GetBackMapSelect();
};