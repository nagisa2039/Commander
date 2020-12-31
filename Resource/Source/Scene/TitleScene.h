#pragma once
#include <functional>
#include <vector>
#include <memory>
#include "Scene.h"
#include "TimeLine.h"
#include "CharactorType.h"
#include "Team.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene :
    public Scene
{
private:
	// 背景画像
	int _bgH;
	// 開始アニメーション画像用ハンドル
	int _start_bksH;
	// タイトルロゴ画像ハンドル
	int _titleH;
	// BGM
	int _bgmH;

	/// <summary>
	/// 背景に降らすキャラクター情報
	/// </summary>
	struct CharInf
	{
		// 描画するか
		bool active;
		// 座標
		Vector2 pos;
		// チーム
		Team team;
		// キャラクター種
		CharactorType type;
		// 回転
		float angle;
	};

	// キャラクター情報管理配列
	std::vector<CharInf> _charInfVec;
	// キャラクター生成インターバルトラック
	std::unique_ptr<Track_f> _charCreateTrack;

	// 開始アニメーション用トラック
	std::unique_ptr<Track_f> _animTrack;
	// デモシーン閉校するまでのカウント
	std::unique_ptr<Track_f> _demoSceneCnt;

	// 更新関数ポインタ
	void(TitleScene::* _updater)(const Input&);

	// フェート終了時に呼ばれる関数
	std::function<void()> _fadeEndFunc;

	/// <summary>
	/// 通常時の更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void NormalUpdate(const Input& input);

	/// <summary>
	/// フェート時の更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void FadeUpdate(const Input& input);

	/// <summary>
	/// キャラクター更新
	/// </summary>
	void CharactorUpdate();

public:
	/// <param name="controller">シーン管理クラス</param>
	TitleScene(SceneController& controller);
	~TitleScene();

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