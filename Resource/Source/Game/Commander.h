#pragma once
#include "MapCursor.h"
#include <memory>
#include <vector>
#include "Team.h"

class Charactor;
class Animator;

/// <summary>
/// 指揮官クラス
/// </summary>
class Commander :
	public MapCursor
{
protected:
	// キャラクター管理配列
	std::vector<std::shared_ptr<Charactor>>& _charactors;
	// 選択中のキャラクター
	Charactor* _selectChar;
	// 操作するチーム(自軍)
	const Team _ctrlTeam;
	// アニメーション用カウンター
	int _animCnt;
	// 硬直フレーム数
	int _rigid;
	// 行動終了判定用フラグ
	bool _end;

	/// <summary>
	/// 自軍の行動が終わったかを確認
	/// </summary>
	bool ChaeckCharactor();

	/// <summary>
	/// 地形効果エフェクトを開始する
	/// </summary>
	void StartTerrainEffect();

public:
	/// <param name="charactors">キャラクター管理配列</param>
	/// <param name="mapCtrl">マップ管理クラス</param>
	/// <param name="ctrlTeam">操作するチーム</param>
	/// <param name="camera">カメラ</param>
	Commander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam, Camera& camera);
	virtual ~Commander();

	/// <summary>
	/// 選択中のキャラクター移動範囲を描画する
	/// </summary>
	virtual void DrawMovableMass();

	/// <summary>
	/// マップ選択へ移行するかを確認
	/// </summary>
	virtual bool GetBackMapSelect()const;

	/// <summary>
	/// 行動を終了する
	/// </summary>
	virtual void End();

	/// <summary>
	/// 行動を終了したかを確認
	/// </summary>
	bool CheckEnd()const;

	/// <summary>
	/// ターンを開始するための初期化
	/// </summary>
	void TurnReset();

	/// <summary>
	/// 選択中のキャラクターのポインターを返す
	/// </summary>
	Charactor* GetSelectCharactor()const;

	/// <summary>
	/// 地形効果を受けてターンを開始する
	/// </summary>
	virtual void StartTerrainEffectUpdate() = 0;

	/// <summary>
	/// 地形効果を受けないでターンを開始する	1ターン目に使用
	/// </summary>
	virtual void StartNormalUpdate() = 0;
};

