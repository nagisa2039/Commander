#pragma once
#include <vector>
#include <map>
#include "../Utility/Geometry.h"
#include <string>

/// <summary>
/// アニメーション管理クラス
/// </summary>
class Animator
{
public:
	/// <summary>
	/// アニメーション情報
	/// </summary>
	struct AnimInfo
	{
		// 表示矩形配列
		std::vector<Rect> rectVec;
		// 切り替え時間
		int itv;
		// ループするか
		bool isLoop;
	};

private:
	// 画像の格納
	int _imageH;
	// 画像サイズ
	Size _imageSize;
	// アニメーション配列
	std::map<std::string, AnimInfo> _animInfoMap;

	// アニメーション名
	std::string _animName;
	// 現在のフレーム数
	int _animFrame;
	// 終了判定
	bool _animEnd;
	// Y軸反転
	bool _reverseY;	

public:
	Animator();
	~Animator();

	/// <summary>
	/// 画像ハンドルの設定
	/// </summary>
	/// <param name="path">パス</param>
	void SetImage(const char* path);

	/// <summary>
	/// 画像ハンドルの設定
	/// </summary>
	/// <param name="handle">ハンドル</param>
	void SetImageHandle(const int handle);

	/// <summary>
	/// Y軸反転設定
	/// </summary>
	/// <param name="value">Y軸反転</param>
	void SetReverseY(const bool value);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="pos">表示座標</param>
	void Draw(const Vector2Int& pos);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="pos">表示座標</param>
	/// <param name="size">表示サイズ</param>
	void Draw(const Vector2Int& pos, Size size);

	/// <summary>
	/// アニメーションの追加
	/// </summary>
	/// <param name="animName">名前</param>
	/// <param name="animInfo">アニメーション情報</param>
	void AddAnim(const std::string& animName, const AnimInfo& animInfo);

	/// <summary>
	/// アニメーションの追加
	/// </summary>
	/// <param name="animName">名前</param>
	/// <param name="rectvec">矩形配列</param>
	/// <param name="animItv">間隔</param>
	/// <param name="loop">ループ</param>
	void AddAnim(const std::string& animName, const std::vector<Rect>& rectvec, const int animItv, const bool loop);

	/// <summary>
	/// アニメーションの追加
	/// </summary>
	/// <param name="animName">名前</param>
	/// <param name="startRect">矩形開始位置</param>
	/// <param name="animCnt">コマ数</param>
	/// <param name="animItv">間隔</param>
	/// <param name="loop">ループ</param>
	/// <param name="vertical">縦並びか</param>
	void AddAnimDiv(const std::string& animName, const Rect& startRect, const int animCnt, const int animItv, const bool loop, const bool vertical = true);

	/// <summary>
	/// アニメーションの追加
	/// </summary>
	/// <param name="animName">名前</param>
	/// <param name="animCnt">コマ数</param>
	/// <param name="animItv">間隔</param>
	/// <param name="loop">ループ</param>
	void AddAnimDiv(const std::string& animName, const Vector2Int& animCnt, const int animItv, const bool loop);

	/// <summary>
	/// アニメーションの変更
	/// </summary>
	/// <param name="animName">アニメーション名</param>
	void ChangeAnim(std::string animName);

	/// <summary>
	/// アニメーションをはじめからやり直す
	/// </summary>
	void AnimRestart();

	/// <summary>
	/// アニメーション名取得
	/// </summary>
	const std::string& GetAnimName()const;

	/// <summary>
	/// 終了判定
	/// </summary>
	const bool GetAnimEnd()const;

	/// <summary>
	/// 画像ハンドルの取得
	/// </summary>
	const int GetAnimHandle()const;

	/// <summary>
	/// アニメーション矩形取得
	/// </summary>
	const Rect& GetAnimRect()const;

	/// <summary>
	/// 画像ハンドルの取得
	/// </summary>
	const int GetImageH()const;

	/// <summary>
	/// 画像サイズの取得
	/// </summary>
	const Size& GetImageSize()const;
};

