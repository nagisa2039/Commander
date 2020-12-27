#pragma once
#include "Commander.h"
#include <vector>

class EnemyCommander :
	public Commander
{
private:
	// 状態更新関数ポインタ
	void(EnemyCommander::* _uniqueUpdater)(const Input& input);

	/// <summary>
	/// 通常更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void NormalUpdate(const Input& input);

	/// <summary>
	/// 地形効果演出時更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void TerrainEffectUpdate(const Input& input);

public:
	/// <param name="charactors">キャラクター管理配列</param>
	/// <param name="mapCtrl">マップ管理クラス</param>
	/// <param name="ctrlTeam">操作するチーム</param>
	/// <param name="camera">カメラ</param>
	EnemyCommander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam, Camera& camera);
	~EnemyCommander();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void Update(const Input& input)override final;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override final;

	/// <summary>
	/// 地形効果を受けてターンを開始する
	/// </summary>
	void StartTerrainEffectUpdate()override;

	/// <summary>
	/// 地形効果を受けないでターンを開始する	1ターン目に使用
	/// </summary>
	void StartNormalUpdate()override;
};

