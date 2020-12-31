#pragma once
#include <memory>
#include <list>
#include <vector>
#include <array>
#include "Scene.h"
#include "../Utility/Geometry.h"
#include "../Game/Team.h"
#include <list>
#include <string>
#include "TimeLine.h"
#include <functional>

class Charactor;
class MapCtrl;
class Camera;
class Commander;
class Effect;
class TurnChangeAnim;
class PreparationUI;
class UI;

/// <summary>
/// プレイシーン
/// </summary>
class PlayScene :
	public Scene
{
public:
	/// <summary>
	/// 描画時にかけるフィルター種
	/// </summary>
	enum class FilterType
	{
		// かけない
		none,
		// ぼかし
		gauss,
		max
	};
private:
	// AI同士の戦闘
	bool _aiMode;
	// 描画先スクリーンハンドル
	int _gameH;
	
	// マップ管理クラス
	std::unique_ptr<MapCtrl> _mapCtrl;
	// カメラ
	std::unique_ptr<Camera> _camera;

	// ターン数
	unsigned char _turnCnt;
	// 制限ターン数
	const unsigned char _limitTurnCnt;

	// ターン遷移アニメーション
	std::unique_ptr<TurnChangeAnim> _turnChangeAnim;

	// キャラクター管理配列
	std::vector<std::shared_ptr<Charactor>> _charactors;
	// 死亡キャラクターイテレーター
	std::vector<std::shared_ptr<Charactor>>::iterator _dyingCharItr;
	// プレイヤー指揮官
	std::unique_ptr<Commander> _playerCommander;
	// 敵指揮官
	std::unique_ptr<Commander> _enemyCommander;
	// エフェクト管理配列
	std::vector<std::shared_ptr<Effect>> _effects;

	// 強制的に終了するときのUI用
	std::deque<std::shared_ptr<UI>> _endUIDeque;
	// 戦闘準備UI親子関係管理Deque
	std::deque<std::shared_ptr<UI>> _preparationDeque;
	// 戦闘準備画面
	std::shared_ptr<PreparationUI> _preparationUI;

	// クリアアニメーション用トラック
	std::unique_ptr<Track_f> _clearAnimTrack;
	// デモアニメーション用トラック
	std::unique_ptr<Track_f> _demoAnimTrack;
	// 自動シーン切り替え用トラック
	std::unique_ptr<Track_i> _autoSceneChangeTrack;

	// フェード終了時に呼ばれる関数
	std::function<void()>_fadeEndFunc;

	// 表示時フィルター種
	FilterType _filterType;
	// フィルターごとの実行関数テーブル
	std::array<std::function<void()>, static_cast<size_t>(FilterType::max)> _filterFuncs;

	// 場面ごとの更新を行う関数ポインタ playSceneを継続するかを返す
	void(PlayScene::*_updater)(const Input& input);
	// 一時的にUpdate関数を変えるときに前Update関数を格納する
	void(PlayScene::* _updaterOld)(const Input& input);

	// 場面ごとの更新を行う関数ポインタ
	void(PlayScene::* _drawer)();
	// UI描画関数ポインタ
	void(PlayScene::* _UIDrawer)();

	/// <summary>
	/// プレイヤーターンを開始する
	/// </summary>
	void StartPlayerTurn();

	/// <summary>
	/// 敵ターンを開始する
	/// </summary>
	void StartEnemyTurn();

	/// <summary>
	/// ゲームを開始する
	/// </summary>
	void GameStart();

	/// <summary>
	/// 準備画面
	/// </summary>
	/// <param name="input">入力情報</param>
	void PreparationUpdate(const Input& input);

	/// <summary>
	/// ターン移動時更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void TurnChengeUpdate(const Input& input);

	/// <summary>
	/// プレイヤーターン時更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void PlayerTurnUpdate(const Input& input);

	/// <summary>
	/// キャラクター更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void CharactorUpdate(const Input& input);

	/// <summary>
	/// 敵ターン更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void EnemyTurnUpdate(const Input& input);

	/// <summary>
	/// PlaySceneから抜けるときの入力受付
	/// </summary>
	/// <param name="input">入力情報</param>
	/// <returns>抜ける</returns>
	bool BackSceneWindow(const Input& input);

	/// <summary>
	/// キャラクター死亡時更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void CharactorDyingUpdate(const Input& input);

	/// <summary>
	/// ゲームクリア時更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void GameClearUpdate(const Input& input);

	/// <summary>
	/// ゲームオーバー時更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void GameOverUpdate(const Input& input);

	/// <summary>
	/// ゲームクリアに移行
	/// </summary>
	void GameClear();

	/// <summary>
	/// ゲームオーバーに移行
	/// </summary>
	void GameOver();

	/// <summary>
	/// フェードインを開始する
	/// </summary>
	/// <param name="funcP">フェード終了時に呼ぶ関数</param>
	/// <param name="color">フェード時の色</param>
	/// <param name="nextDrawer">次の描画関数</param>
	void StartFadeIn(std::function<void()>funcP, const unsigned int color = 0x000000, void(PlayScene::* nextDrawer)() = &PlayScene::BaseDraw);

	/// <summary>
	/// フェードアウトを開始する
	/// </summary>
	/// <param name="funcP">フェード終了時に呼ぶ関数</param>
	/// <param name="color">フェード時の色</param>
	/// <param name="nextDrawer">次の描画関数</param>
	void StartFadeOut(std::function<void()> funcP, const unsigned int color = 0x000000, void(PlayScene::* nextDrawer)() = &PlayScene::BaseDraw);

	/// <summary>
	/// フェード更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void FadeUpdate(const Input& input);

	/// <summary>
	/// 戦闘準備時の描画
	/// </summary>
	void PreparationDraw();

	/// <summary>
	/// ターン移行のアニメーション描画
	/// </summary>
	void TurnChengeDraw();

	/// <summary>
	/// プレイヤーターン時の描画
	/// </summary>
	void PlayerTurnDraw();

	/// <summary>
	/// 敵ターン時の描画
	/// </summary>
	void EnemyTurnDraw();

	/// <summary>
	/// ゲームオーバー時の描画
	/// </summary>
	void GameOverDraw();

	/// <summary>
	/// ゲームクリア時の描画
	/// </summary>
	void GameClearDraw();

	/// <summary>
	/// 通常描画
	/// </summary>
	void BaseDraw();

	/// <summary>
	/// 戦闘準備の描画
	/// </summary>
	void PreparationUIDraw();

	/// <summary>
	/// プレイヤーUIの描画
	/// </summary>
	void PlayerUIDraw();

	/// <summary>
	/// 通常UIの描画
	/// </summary>
	void NoneUIDraw();

	// Fade終了時に実行する関数-------------------------------------------------------
	/// <summary>
	/// タイトルシーンへの移行
	/// </summary>
	void ChengeTitle();

	/// <summary>
	/// マップ選択シーンへの移行
	/// </summary>
	void ChengeMapSelect();

	/// <summary>
	/// 戦闘準備画面への移行
	/// </summary>
	void ChangePreparation();

	/// <summary>
	/// ゲームオーバーへの移行
	/// </summary>
	void ChangeGameOver();

public:
	/// <param name="ctrl">シーン管理クラス</param>
	/// <param name="mapId">マップID</param>
	/// <param name="ai">AI同士の対決か</param>
	PlayScene(SceneController & ctrl, const unsigned int mapId, const bool ai = false);
	~PlayScene();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void Update(const Input& input) override final;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(void) override final;

	/// <summary>
	/// シーンがアクティブになったら呼ばれる
	/// </summary>
	void On()override;

	/// <summary>
	/// シーンが非アクティブになったら呼ばれる
	/// </summary>
	void Off()override;

	/// <summary>
	/// 描画時にフィルターをかける
	/// </summary>
	/// <param name="type">フィルター種</param>
	void SetFilter(const FilterType type);
};