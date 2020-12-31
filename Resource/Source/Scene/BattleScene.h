#pragma once
#include "Scene.h"
#include "../Utility/Dir.h"
#include "../Utility/Geometry.h"
#include <vector>
#include <memory>
#include <deque>
#include "TimeLine.h"

class UI;
class Camera;
class Effect;
class Charactor;
class BattleCharactor;
class CutIn;
class PlayScene;

/// <summary>
/// 戦闘シーン
/// </summary>
class BattleScene :
	public Scene
{
private:
	//攻撃を仕掛けた側のキャラクター
	BattleCharactor& _leftBC;
	//攻撃を仕掛けられた側のキャラクター
	BattleCharactor& _rightBC;

	// 描画先スクリーンサイズ
	const Size _screenSize;
	// 描画先スクリーンハンドル
	int _screenH;
	// カメラ
	std::shared_ptr<Camera> _camera;

	// エフェクト管理配列
	std::vector<std::shared_ptr<Effect>> _effects;
	// プレイシーン
	PlayScene* _playScene;

	// PlaySceneで戦闘が起こったスクリーン座標
	Vector2Int _startPos;
	// 地面の高さ
	float _groundY;
	// 追撃判定を行ったか
	bool _pursuit;

	// BGMハンドル
	int _bgmH;

	// カットイン再生クラス
	std::shared_ptr<CutIn> _cutIn;

	// 拡縮アニメーショントラック
	std::unique_ptr<Track<float>> _exRateTL;
	// 画面高度アニメーショントラック
	std::unique_ptr<Track<float>> _brightTL;

	// 更新関数ポインタ
	void(BattleScene::* _updater)(const Input& input);

	/// <summary>
	/// 戦闘開始時のアニメーション
	/// </summary>
	/// <param name="input">入力情報</param>
	void SceneStartAnim(const Input& input);

	/// <summary>
	/// 終了時アニメーション
	/// </summary>
	/// <param name="input">入力情報</param>
	void SceneEndAnim(const Input& input);

	/// <summary>
	/// 左側ターン
	/// </summary>
	/// <param name="input">入力情報</param>
	void LeftTurn(const Input& input);

	/// <summary>
	/// 左側HPアニメーション
	/// </summary>
	/// <param name="input">入力情報</param>
	void LeftHPAnim(const Input& input);

	/// <summary>
	/// 右側ターン
	/// </summary>
	/// <param name="input">入力情報</param>
	void RightTurn(const Input& input);

	/// <summary>
	/// 右側HPアニメーション
	/// </summary>
	/// <param name="input">入力情報</param>
	void RightHPAnim(const Input& input);

	/// <summary>
	/// 追撃できるキャラがいれば追撃する
	/// </summary>
	/// <param name="rightAttack">右側の攻撃が当たる範囲か</param>
	/// <returns>追撃可能</returns>
	bool PursuitAttack(const bool rightAttack = true);

	/// <summary>
	/// シーン終了
	/// PlaySceneに戻る
	/// </summary>
	void End();

	/// <summary>
	/// 床の描画
	/// </summary>
	/// <param name="screenCenter">描画スクリーン中心座標</param>
	void DrawFloor(Vector2Int& screenCenter);

public:
	/// <param name="leftBC">攻撃を仕掛けた側のキャラクター</param>
	/// <param name="rightBC">攻撃を仕掛けられた側のキャラクター</param>
	/// <param name="ctrl">シーン管理クラス</param>
	/// <param name="cameraOffset">描画オフセット</param>
	BattleScene(BattleCharactor& leftBC, BattleCharactor& rightBC, SceneController& ctrl, const Vector2Int& cameraOffset);
	~BattleScene();

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
	/// エフェクト管理配列の取得
	/// </summary>
	std::vector<std::shared_ptr<Effect>>& GetEffectVec();

	/// <summary>
	/// クリティカルカットインを再生する
	/// </summary>
	/// <param name="c">クリティカルカットイン</param>
	void SetCutIn(std::shared_ptr<CutIn> c);
};