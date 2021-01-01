#pragma once
#include <DirectXMath.h>

struct Vector2;
struct Vector3;
struct Size;

/// <summary>
/// 整数2次元ベクトル
/// </summary>
struct Vector2Int
{
	int x, y;
	void operator*=(const float scale);

	void operator=(const Vector2& pos);
	void operator+=(const Vector2Int& pos);
	void operator-=(const Vector2Int& pos);

	Vector2 ToVector2() const;
	Size ToSize() const;

	static Vector2Int Zero();
	static Vector2Int One();
};

Vector2Int operator+(const Vector2Int &lval, const Vector2Int& rval);
Vector2Int operator-(const Vector2Int &lval, const Vector2Int& rval);
Vector2Int operator*(const Vector2Int &lval, const Vector2Int& rval);
Vector2Int operator%(const Vector2Int &lval, const Vector2Int& rval);
Vector2Int operator/(const Vector2Int &lval, const Vector2Int& rval);
Vector2Int operator*(const Vector2Int &lval, const float& rval);
Vector2Int operator+(const Vector2Int& lval, const int& rval);
bool operator==(const Vector2Int &lval, const Vector2Int& rval);
bool operator!=(const Vector2Int &lval, const Vector2Int& rval);
bool operator<(const Vector2Int& lval, const Vector2Int& rval);
bool operator>(const Vector2Int& lval, const Vector2Int& rval);
bool operator<=(const Vector2Int& lval, const Vector2Int& rval);
bool operator>=(const Vector2Int& lval, const Vector2Int& rval);

Vector2Int operator+(const Vector2Int& lval, const Size& rval);
Vector2Int operator*(const Vector2Int& lval, const Size& rval);
Vector2Int operator*(const Vector2Int& lval, const Vector2& rval);

/// <summary>
/// 入力した値を一定範囲に収める
/// </summary>
/// <param name="in">元の値</param>
/// <param name="min">下限</param>
/// <param name="max">上限</param>
Vector2Int Clamp(const Vector2Int& in, const Vector2Int& min, const Vector2Int& max);

/// <summary>
/// 2次元ベクトル
/// </summary>
struct Vector2
{
	float x, y;
	Vector2 operator-(void);

	void operator+=(const Vector2& pos);
	void operator-=(const Vector2& pos);
	void operator*=(const Vector2& pos);
	void operator/=(const Vector2& pos);

	void operator+=(float scale);
	void operator-=(float scale);
	void operator*=(float scale);
	void operator/=(float scale);

	float Length()const;
	Vector2 Normalized()const;

	Vector2Int ToVector2Int()const;
	DirectX::XMFLOAT2 ToXMFLOAT2()const;
};

/// <summary>
/// 内積
/// </summary>
/// <param name="lval">ベクトルA</param>
/// <param name="rval">ベクトルB</param>
float Dot(const Vector2 &lval, const Vector2& rval);

/// <summary>
/// 外積
/// </summary>
/// <param name="lval">ベクトルA</param>
/// <param name="rval">ベクトルB</param>
/// <returns></returns>
float Cross(const Vector2 &lval, const Vector2& rval);

Vector2 operator+(const Vector2 &lval, const Vector2& rval);
Vector2 operator-(const Vector2 &lval, const Vector2& rval);
Vector2 operator*(const Vector2 &lval, const Vector2& rval);
Vector2 operator/(const Vector2 &lval, const Vector2& rval);

Vector2 operator+(const Vector2 &lval, const float& rval);
Vector2 operator-(const Vector2 &lval, const float& rval);
Vector2 operator*(const Vector2 &lval, const float& rval);
Vector2 operator/(const Vector2 &lval, const float& rval);

/// <summary>
/// 線型補完
/// </summary>
/// <typeparam name="T">補完する型</typeparam>
/// <param name="lval">始点</param>
/// <param name="rval">終点</param>
/// <param name="parsent">割合</param>
template <typename T>
inline T Lerp(const T lval, const T rval, const float parsent)
{
	return static_cast<T>(lval + (rval - lval) * parsent);
}

/// <summary>
/// サイズ
/// </summary>
struct Size
{
	int w, h;
	Vector2Int ToVector2Int()const;
	Vector2 ToVector2()const;
	Size operator-(const Size& size); 
	void operator*=(const float scale);
	void operator/=(const float scale);
};

Size operator+(const Size& lval, const Size& rval);
Size operator*(const Size& lval, const Size& rval);
Size operator/(const Size& lval, const Size& rval);
Size operator/(const Size& lval, const Vector2Int& rval);

Size operator+(const Size& lval, const int& rval);
Size operator*(const Size& lval, const float& rval);

bool operator==(const Size & lval, const Size& rval);

/// <summary>
/// 矩形
/// </summary>
struct Rect 
{
	// 
	Vector2Int center;
	// 
	Size size;

	const int Left()const;
	const int Top() const;
	const int Right()const;
	const int Botton()const;
	const int Width()const { return size.w; }
	const int Height()const{ return size.h; }

	Vector2Int Leftup();
	Vector2Int Leftcenter();
	Vector2Int Leftdown();
	Vector2Int Centerup();
	Vector2Int Center();
	Vector2Int Centerdown();
	Vector2Int Rightup();
	Vector2Int Rightcenter();
	Vector2Int Rightdown();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="color">色</param>
	/// <param name="fill">塗りつぶすか</param>
	void Draw(const unsigned int color = 0xffffff, const bool fill = true)const;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="offset">描画オフセット</param>
	/// <param name="color">色</param>
	/// <param name="fill">塗りつぶすか</param>
	void Draw(const Vector2Int& offset, const int color = 0xffffff, const bool fill = true)const;

	/// <summary>
	/// 画像描画
	/// </summary>
	/// <param name="graphH">画像ハンドル</param>
	/// <param name="offset">描画オフセット</param>
	void DrawGraph(const int graphH, const Vector2Int& offset = Vector2Int{0,0})const;

	/// <summary>
	/// 矩形描画
	/// </summary>
	/// <param name="leftup">切り抜き矩形左上</param>
	/// <param name="rectSize">切り抜き矩形サイズ</param>
	/// <param name="graphH">画像ハンドル</param>
	/// <param name="offset">描画オフセット</param>
	void DrawRectGraph(const Vector2Int& leftup, const Size& rectSize, const int graphH, const Vector2Int& offset = Vector2Int{ 0,0 })const;

	/// <summary>
	/// 自身を切り抜き矩形として使い指定した範囲に描画
	/// </summary>
	/// <param name="leftup">描画左上座標</param>
	/// <param name="rightdown">描画右上座標</param>
	/// <param name="graphH">画像ハンドル</param>
	/// <param name="offset">描画オフセット</param>
	void DrawExtendGraph(const Vector2Int& leftup, const Vector2Int& rightdown, const int graphH, const Vector2Int& offset = Vector2Int{ 0,0 })const;

	/// <summary>
	/// 矩形同士の当たり判定
	/// </summary>
	/// <param name="rect">相手の矩形</param>
	bool IsHit(const Rect& rect);

	/// <summary>
	/// 矩形同士の当たり判定
	/// </summary>
	/// <param name="aRect">矩形A</param>
	/// <param name="bRect">矩形B</param>
	static const bool IsHit(const Rect& aRect, const Rect& bRect);

	/// <summary>
	/// 矩形同士の重なっている量
	/// </summary>
	/// <param name="aRect">矩形A</param>
	/// <param name="bRect">矩形B</param>
	static const Size OverlapSize(const Rect& aRect, const Rect& bRect);
};

/// <summary>
/// 範囲
/// </summary>
struct Range
{
	// 最小値
	int min;
	// 最大値
	int max;

	/// <summary>
	/// 指定した値が範囲内か
	/// </summary>
	/// <param name="value">値</param>
	bool Hit(const int value)const;

	/// <summary>
	/// ターゲットに対して範囲外Rangeを取得
	/// 攻撃するときに一方的に殴れる範囲
	/// </summary>
	/// <param name="target">ターゲットの攻撃範囲</param>
	Range GetCriticalRange(const Range& target)const;
};
bool operator==(const Range& lv, const Range& rv);