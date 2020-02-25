#pragma once
#include <DirectXMath.h>

struct Vector2;
struct Vector3;

struct Vector2Int
{
	int x, y;
	Vector2Int() :x(0), y(0) {}
	Vector2Int(int x, int y) :x(x), y(y) {}
	void operator*=(const float scale);

	void operator+=(const Vector2Int& pos);
	void operator-=(const Vector2Int& pos);

	Vector2 ToVector2() const;
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

struct Vector2
{
	float x, y;
	Vector2():x(0),y(0) {}
	Vector2(const float x, const float y) :x(x), y(y) {}
	Vector2(const DirectX::XMFLOAT2& xy): x(xy.x), y(xy.y){}
	Vector2(const Vector3& vec3);

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

float Dot(const Vector2 &lval, const Vector2& rval);
float Cross(const Vector2 &lval, const Vector2& rval);

Vector2 operator+(const Vector2 &lval, const Vector2& rval);
Vector2 operator-(const Vector2 &lval, const Vector2& rval);
Vector2 operator*(const Vector2 &lval, const Vector2& rval);
Vector2 operator/(const Vector2 &lval, const Vector2& rval);

Vector2 operator+(const Vector2 &lval, const float& rval);
Vector2 operator-(const Vector2 &lval, const float& rval);
Vector2 operator*(const Vector2 &lval, const float& rval);
Vector2 operator/(const Vector2 &lval, const float& rval);

Vector2 Lerp(const Vector2& start, const Vector2& end, const float parsent);

struct Vector3
{
	float x, y, z;
	Vector3():x(0),y(0),z(0) {}
	Vector3(const float x, const float y, const float z) :x(x), y(y), z(z) {}
	Vector3(const DirectX::XMFLOAT3& xyz):x(xyz.x), y(xyz.y), z(xyz.z) {}
	Vector3(const DirectX::XMFLOAT4& xyzw) :x(xyzw.x), y(xyzw.y), z(xyzw.z) {}
	Vector3(const Vector2& vec2): x(vec2.x), y(vec2.y), z(0) {}

	Vector3 operator-(void)const;

	void operator+=(const Vector3& val);
	void operator-=(const Vector3& val);
	void operator*=(const Vector3& val);
	void operator/=(const Vector3& val);

	void operator+=(float scale);
	void operator-=(float scale);
	void operator*=(float scale);
	void operator/=(float scale);

	float Length()const;
	Vector3 Normalize();
	Vector3 Normalized()const;

	DirectX::XMVECTOR ToXMVECTOR()const;
	DirectX::XMFLOAT3 ToXMFLOAT3()const;
};

float Dot(const Vector3 &lval, const Vector3& rval);
Vector3 Cross(const Vector3 &lval, const Vector3& rval);

// 反射ベクトルを作る
Vector3 ReflectionVector(const Vector3& baseVector, const Vector3& norm);

// 屈折ベクトルを作る
Vector3 RefractionVector(const Vector3& baseVector, const Vector3& norm, float parsent);

// 線形補完
Vector3 Lerp(const Vector3& start, const Vector3& end, const float parsent);

Vector3 operator+(const Vector3 &lval, const Vector3& rval);
Vector3 operator-(const Vector3 &lval, const Vector3& rval);
Vector3 operator*(const Vector3 &lval, const Vector3& rval);
Vector3 operator/(const Vector3 &lval, const Vector3& rval);

Vector3 operator+(const Vector3 &lval, const float& rval);
Vector3 operator-(const Vector3 &lval, const float& rval);
Vector3 operator*(const Vector3 &lval, const float& rval);
Vector3 operator/(const Vector3 &lval, const float& rval);

Vector3 operator+(const Vector3& lval, const Vector2& rval);

Vector3 RotateVector(const Vector3& baseVector, const Vector3& rotate);

Vector3 XMVECTORtoVec3(DirectX::XMVECTOR& vec);

float Lerp(const float lval, const float rval, const float parsent);
DirectX::XMFLOAT2 Lerp(const DirectX::XMFLOAT2 lval, const DirectX::XMFLOAT2 rval, const float parsent);

// 三角形の面積の算出
float TriangleArea(const Vector3& p0, const Vector3& p1, const Vector3& p2);

struct Vector4
{
	float x, y, z, w;
	Vector4() {};
	Vector4(const float x, const float y, const float z, const float w) :x(x), y(y), z(z), w(w){}
};

struct Size
{
	int w, h;
	Size() :w(0), h(0) {}
	Size(const int w, const int h) :w(w), h(h) {}
	Vector2Int ToVector2Int()const;
	Size operator-(const Size& size);
};

Size operator+(const Size& lval, const Size& rval);
Size operator*(const Size& lval, const Size& rval);

Size operator+(const Size& lval, const float& rval);
Size operator*(const Size& lval, const float& rval);

enum class Anker
{
	leftup,
	leftcenter,
	leftdown,
	centerup,
	center,
	centerdown,
	rightup,
	rightcenter,
	rightdown,
	max,
};

struct Rect 
{
	Vector2Int center;
	Size size;
	Rect();
	Rect(const int x, const int y, const int w, const int h);
	Rect(const Vector2Int& pos, const Size& sz);

	const int Left()const;
	const int Top() const;
	const int Right()const;
	const int Botton()const;
	const int Width()const { return size.w; }
	const int Height()const{ return size.h; }

	void Draw(const unsigned int color = 0xffffff, const bool fill = true)const;
	void Draw(const Vector2Int&  offset, const int color = 0xffffff, const bool fill = true);
	void Draw(const Vector2& offset, const int color = 0xffffff, const bool fill = true);

	static const bool IsHit(const Rect& aRect, const Rect& bRect);
	static const Size OverlapSize(const Rect& aRect, const Rect& bRect);
};

struct Circle
{
	Vector2 centor;
	float radius;
	Circle();
	Circle(const float x, const float y, const float r);
	Circle(const Vector2& centor, const float r);

	void Draw(int color = 0xffffff);
};

struct Segment 
{
	Vector2 posA;
	Vector2 posB;
	Segment();
	Segment(const Vector2& posA, const Vector2& posB);
	Segment(const float ax, const float ay, const float bx, const float by);
	void Draw(unsigned int color = 0xffffff);
	float Grad(void)const;
};


// 値を最大値と最小値を考慮した値に加工する
float Clamp(const float in, const float min = 0.0f, const float max = 1.0f);

Vector3 Clamp(const Vector3& in, const float min = 0.0f, const float max = 1.0f);