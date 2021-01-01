#pragma once
#include "Geometry.h"
#include "AnkerCalculation.h"

/// <summary>
/// 画像サイズ取得
/// </summary>
/// <param name="handle">画像ハンドル</param>
/// <param name="size">画像サイズ格納用変数</param>
void GetGraphSize(const int handle, Size& size);

/// <summary>
/// 矩形描画
/// </summary>
/// <param name="leftup">左上座標</param>
/// <param name="rightdown">右下座標</param>
/// <param name="color">色</param>
/// <param name="fillFlag">塗りつぶすか</param>
void DrawBox(const Vector2Int& leftup, const Vector2Int& rightdown, const unsigned int color, const bool fillFlag = true);

/// <summary>
/// 線分の描画
/// </summary>
/// <param name="start">始点</param>
/// <param name="end">終点</param>
/// <param name="color">色</param>
/// <param name="thickness">太さ</param>
void DrawLine(const Vector2Int& start, const Vector2Int& end, const unsigned int color, const int thickness = 1);

/// <summary>
/// 画像描画
/// </summary>
/// <param name="leftup">左上</param>
/// <param name="handle">ハンドル</param>
/// <param name="transFlag">透明度を有効化するか</param>
void DrawGraph(const Vector2Int& leftup, const int handle, const bool transFlag = true);

/// <summary>
/// 画像回転描画
/// </summary>
/// <param name="center">中心座標</param>
/// <param name="ExRate">拡大率</param>
/// <param name="Angle">回転(ラジアン)</param>
/// <param name="GrHandle">ハンドル</param>
/// <param name="TransFlag">透明度を有効化するか</param>
/// <param name="ReverseXFlag">X軸反転するか</param>
/// <param name="ReverseYFlag">Y軸反転するか</param>
void DrawRotaGraph(const Vector2Int& center, const double ExRate, const double Angle, const int GrHandle, const int TransFlag, const int ReverseXFlag = false, const int ReverseYFlag = false);

/// <summary>
/// 画像回転切り抜き描画
/// </summary>
/// <param name="center">中心</param>
/// <param name="srcLeftup">切り抜き矩形の左上</param>
/// <param name="size">切り抜きサイズ</param>
/// <param name="ExtRate">拡大率</param>
/// <param name="Angle">回転(ラジアン)</param>
/// <param name="GrHandle">ハンドル</param>
/// <param name="TransFlag">透明度を有効化するか</param>
/// <param name="ReverseXFlag">X軸反転するか</param>
/// <param name="ReverseYFlag">Y軸反転するか</param>
void DrawRectRotaGraph(const Vector2Int& center, const Vector2Int& srcLeftup, const Size& size, const double ExtRate, const double Angle, const int GraphHandle, const int TransFlag = true, const int ReverseXFlag = false, const int ReverseYFlag = false);

/// <summary>
/// 円を描画
/// </summary>
/// <param name="center">中心</param>
/// <param name="radius">半径</param>
/// <param name="color">色</param>
/// <param name="fillFlag">塗りつぶすか</param>
void DrawCircle(const Vector2Int& center, const int radius, const unsigned int color, const bool fillFlag = true);

/// <summary>
/// 描画座標(左上座標)の取得
/// 例: 右下(アンカー指定)座標(drawPos指定)を左上座標へ変換する
/// </summary>
/// <param name="drawPos">描画座標</param>
/// <param name="size">サイズ</param>
/// <param name="anker">アンカー</param>
Vector2Int GetDrawPos(const Vector2Int& drawPos, const Size& size, const Anker anker);

/// <summary>
/// フォントを使用した文字列描画
/// </summary>
/// <param name="drawPos">描画座標</param>
/// <param name="anker">アンカー</param>
/// <param name="color">色</param>
/// <param name="fontHandle">フォントハンドル</param>
/// <param name="format">文字列フォーマット</param>
void DrawStringToHandle(const Vector2Int& drawPos, const Anker anker, const unsigned color, const int fontHandle, const char* format, ...);

/// <summary>
/// フォントを使用した文字列描画
/// </summary>
/// <param name="drawPos">描画座標</param>
/// <param name="anker">アンカー</param>
/// <param name="color">色</param>
/// <param name="exRate">拡大率</param>
/// <param name="fontHandle">フォントハンドル</param>
/// <param name="format">文字列フォーマット</param>
void DrawStringToHandle(const Vector2Int& drawPos, const Anker anker, const unsigned color, const double exRate, const int fontHandle, const char* format, ...);