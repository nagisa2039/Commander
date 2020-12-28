#pragma once
#include "MapCursor.h"
#include <memory>
#include <deque>
#include "TimeLine.h"

class Charactor;
class TerrainInf;
class StatusInf;
class StatusWindow;
class UI;

/// <summary>
/// 戦闘準備画面のカーソル
/// </summary>
class BattlePreparationCursor :
	public MapCursor
{
private:
	// 選択中のマップ座標  xが負の値の時は未選択
	Vector2Int _selectMapPos;
	// 拡縮アニメーショントラック
	std::unique_ptr<Track<float>> _exRateTrack;
	// 終了判定
	bool _end;
	// 決定時SE
	int _okH;
	// キャンセル時SE
	int _canselH;
	// 地形情報表示クラス
	std::shared_ptr<TerrainInf> _terrainInf;
	// ステータス情報表示クラス
	std::shared_ptr<StatusInf> _statusInf;
	// ステータスウィンドウ表示クラス
	std::shared_ptr<StatusWindow> _statusWindow;

	/// <summary>
	/// キャラクター選択
	/// </summary>
	void Select();

public:
	/// <param name="mapCtrl">マップ管理クラス</param>
	/// <param name="camera">カメラ</param>
	BattlePreparationCursor(MapCtrl& mapCtrl, Camera& camera);
	~BattlePreparationCursor();

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
	/// 出撃可能マスの表示
	/// </summary>
	void DrawSortieMass();

	/// <summary>
	/// 移動可能マスの表示
	/// </summary>
	void DrawMovableMass();

	/// <summary>
	/// 終了判定取得
	/// </summary>
	bool GetEnd()const;

	/// <summary>
	/// 開始時の初期化
	/// </summary>
	void Start();

	/// <summary>
	/// 最初のカーソル座標設定
	/// </summary>
	void SetCursorStartPosition();
};

