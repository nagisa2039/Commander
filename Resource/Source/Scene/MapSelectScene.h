#pragma once
#include <vector>
#include <memory>
#include <deque>
#include <functional>
#include "Scene.h"
#include "Geometry.h"
#include "TimeLine.h"

class Camera;
class MapSelectCharactor;
class UI;

/// <summary>
/// マップ選択シーン
/// </summary>
class MapSelectScene :
    public Scene
{
private:
	// 項目サイズ
	const Size _contentSize;
	// 項目座標配列
	std::vector<Vector2Int> _contentPosVec;
	// カメラ
	std::unique_ptr<Camera> _camera;
	// キャラクター管理配列
	std::vector<std::unique_ptr<MapSelectCharactor>> _mapSelectCharactors;
	// フェード終了時に呼ばれる関数
	std::function<void()> _fadeEndFunc;
	// UI親子関係管理Deque
	std::deque<std::shared_ptr<UI>> _uiDeque;

	// 選択中のインデックス
	unsigned int _selectIdx;

	// キャラクター移動開始までのインターバルトラック
	std::unique_ptr<Track<int>> _moveStartTrack;
	// 選択中項目アニメーショントラック
	std::unique_ptr<Track<float>> _selectAnimTrack;
	// 移動させるキャラクターインデックス
	unsigned int _charactorIdx;

	// キャラクター移動方向 -1 or +1
	int _dir;
	// BGMハンドル
	int _bgmH;

	// マップタイトル下地ハンドル
	int _uiH;

	// 描画先スクリーン
	int _screenH;

	// 更新関数ポインタ
	void(MapSelectScene::* _updater)(const Input&);

	/// <summary>
	/// 通常更新
	/// </summary>
	/// <param name="input">更新</param>
	void NormalUpdate(const Input& input);

	/// <summary>
	/// フェード時更新
	/// </summary>
	/// <param name="input">更新</param>
	void FadeUpdate(const Input& input);

	/// <summary>
	/// 移動更新
	/// </summary>
	/// <param name="input">更新</param>
	void MoveUpdate(const Input& input);

	/// <summary>
	/// カーソル移動更新
	/// </summary>
	/// <param name="input">更新</param>
	void CursorMove(const Input& input);

	/// <summary>
	/// キャラクター移動更新
	/// </summary>
	void MapSelectCharactorUpdate();

	/// <summary>
	/// 決定
	/// </summary>
	/// <param name="debug">デバッグか</param>
	void Decide(const bool debug = false);

	/// <summary>
	/// キャラクター選択インデックスの設定
	/// </summary>
	/// <param name="value">キャラクター選択インデックス</param>
	void SetSelectIdx(const unsigned int value);

public:
	/// <param name="controller">シーン管理クラス</param>
	MapSelectScene(SceneController& controller);
	~MapSelectScene();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void Update(const Input& input) override final;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(void) override final;
};