#pragma once
struct Vector2;

struct Vector2Int
{
	int x, y;
	Vector2Int() {}
	Vector2Int(int x, int y) :x(x), y(y) {}
	void operator*=(const float scale);

	void operator+=(const Vector2Int& pos);
	void operator-=(const Vector2Int& pos);

	Vector2 ToFloatVector() const;
};

Vector2Int operator+(const Vector2Int& lval, const float rval);
Vector2Int operator+(const Vector2Int &lval, const Vector2Int& rval);
Vector2Int operator-(const Vector2Int &lval, const Vector2Int& rval);
Vector2Int operator*(const Vector2Int &lval, const Vector2Int& rval);
Vector2Int operator%(const Vector2Int &lval, const Vector2Int& rval);
Vector2Int operator/(const Vector2Int &lval, const Vector2Int& rval);
Vector2Int operator*(const Vector2Int &lval, const float& rval);
bool operator==(const Vector2Int &lval, const Vector2Int& rval);
bool operator!=(const Vector2Int &lval, const Vector2Int& rval);

struct Vector2
{
	float x, y;
	Vector2() {}
	Vector2(const float x, const float y) :x(x), y(y) {}

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

	Vector2Int ToIntVector()const;
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

struct Vector3
{
	float x, y, z;
	Vector3() {}
	Vector3(const float x, const float y, const float z) :x(x), y(y), z(z) {}

	Vector3 operator-(void);

	void operator+=(const Vector3& val);
	void operator-=(const Vector3& val);
	void operator*=(const Vector3& val);
	void operator/=(const Vector3& val);

	void operator+=(float scale);
	void operator-=(float scale);
	void operator*=(float scale);
	void operator/=(float scale);

	float Length()const;
	void Normalize();
	Vector3 Normalized()const;
};

float Dot(const Vector3 &lval, const Vector3& rval);
Vector3 Cross(const Vector3 &lval, const Vector3& rval);

Vector3 operator+(const Vector3 &lval, const Vector3& rval);
Vector3 operator-(const Vector3 &lval, const Vector3& rval);
Vector3 operator*(const Vector3 &lval, const Vector3& rval);
Vector3 operator/(const Vector3 &lval, const Vector3& rval);

Vector3 operator+(const Vector3 &lval, const float& rval);
Vector3 operator-(const Vector3 &lval, const float& rval);
Vector3 operator*(const Vector3 &lval, const float& rval);
Vector3 operator/(const Vector3 &lval, const float& rval);

struct Size
{
	int w, h;
	Size() {}
	Size(const int w, const int h) :w(w), h(h) {}
	Vector2Int ToIntVector()const;
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

