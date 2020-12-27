#pragma once
#include "Commander.h"
#include <memory>
#include <deque>
#include <list>

class PlayerUI;
class UI;

class PlayerCommander :
	public Commander
{
private:
	// UI管理用クラス
	std::unique_ptr<PlayerUI> _playerUI;
	// マップ選択に移行するか
	bool _backMapSelect;

	// 決定時SE
	int _okH;
	// キャンセル時SE
	int _canselH;

	void(PlayerCommander::* _uniqueUpdater)(const Input& input);

	/// <summary>
	///  _mapPosが_selectCharの行動範囲内かの確認
	/// </summary>
	bool CheckMoveRange();

	/// <summary>
	/// _mapPosが_selectCharの攻撃マスかどうかの確認
	/// </summary>
	bool CheckAttackMass();

	/// <summary>
	/// 戦闘予測画面
	/// </summary>
	/// <param name="input">入力情報</param>
	void BattlePredictionUpdate(const Input& input);

	/// <summary>
	/// 地形効果演出中
	/// </summary>
	/// <param name="input">入力情報</param>
	void TerrainEffectUpdate(const Input& input);

	/// <summary>
	/// 通常時
	/// </summary>
	/// <param name="input">入力情報</param>
	void NormalUpdate(const Input& input);

	/// <summary>
	/// キャラクター選択中
	/// </summary>
	/// <param name="input">入力情報</param>
	void SelectUpdate(const Input& input);

	/// <summary>
	/// 戦闘中
	/// </summary>
	/// <param name="input">入力情報</param>
	void BattaleUpdate(const Input& input);

public:
	/// <param name="charactors">キャラクター管理配列</param>
	/// <param name="mapCtrl">マップ管理クラス</param>
	/// <param name="ctrlTeam">操作するチーム</param>
	/// <param name="camera">カメラ</param>
	/// <param name="turnCnt">ターン数</param>
	PlayerCommander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam, Camera& camera, const unsigned char & turnCnt);
	~PlayerCommander();

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

	/// <summary>
	/// 選択中のキャラクター移動範囲を描画する
	/// </summary>
	void DrawMovableMass()override;

	/// <summary>
	/// 行動を終了する
	/// </summary>
	void End()override;

	/// <summary>
	/// キャラクターを選択する
	/// </summary>
	/// <param name="charactor">選択するキャラクター</param>
	/// <param name="nextTarget"></param>
	void SelectCharactor(Charactor* charactor, const bool nextTarget);

	/// <summary>
	/// 指定した座標を基に戦闘準備へ移行する
	/// </summary>
	/// <param name="mapPos">戦闘準備へ移行する座標</param>
	bool AttackPrePos(const Vector2Int& mapPos);

	/// <summary>
	/// 戦闘開始
	/// </summary>
	void BattleStart();

	/// <summary>
	/// 戦闘準備画面に戻る
	/// </summary>
	void BackBattalePrediction();

	/// <summary>
	/// 移動を取り消す
	/// </summary>
	/// <returns>成否</returns>
	bool MoveCancel();

	/// <summary>
	/// マップ選択へ移行するかを確認
	/// </summary>
	virtual bool GetBackMapSelect()const;

	/// <summary>
	/// マップ選択へ移行するかを設定
	/// </summary>
	/// <param name="value">マップ選択へ移行するか</param>
	void SetBackMapSelect(const bool value);
};

