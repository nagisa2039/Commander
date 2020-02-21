#include <cmath>
#include "Geometry.h"
#include <DxLib.h>

Vector2 Vector2::operator-(void)
{
	return Vector2(-this->x,-this->y);
}

void Vector2::operator+=(const Vector2& pos)
{
	this->x += pos.x;
	this->y += pos.y;
}

void Vector2::operator-=(const Vector2& pos)
{
	this->x -= pos.x;
	this->y -= pos.y;
}
void Vector2::operator*=(const Vector2& pos)
{
	this->x *= pos.x;
	this->y *= pos.y;
}
void Vector2::operator/=(const Vector2& pos)
{
	this->x /= pos.x;
	this->y /= pos.y;
}

void Vector2::operator+=(float scale)
{
	this->x += scale;
	this->y += scale;
}
void Vector2::operator-=(float scale)
{
	this->x -= scale;
	this->y -= scale;
}

void Vector2::operator*=(float scale)
{
	this->x *= scale;
	this->y *= scale;
}

void Vector2::operator/=(float scale)
{
	this->x /= scale;
	this->y /= scale;
}

float Vector2::Length() const
{
	return static_cast<float>(hypot(this->x, this->y));
}

Vector2 Vector2::Normalized() const
{
	auto mag = this->Length();
	return Vector2(this->x/mag, this->y/mag);
}

Vector2Int Vector2::ToIntVector()const
{
	return Vector2Int(static_cast<int>(this->x),static_cast<int>(this->y));
}

Vector2Int operator+(const Vector2Int& lval, const float rval)
{
	return Vector2Int(lval.x + rval, lval.y + rval);
}

Vector2Int operator+(const Vector2Int & lval, const Vector2Int & rval)
{
	return Vector2Int(lval.x + rval.x, lval.y + rval.y);
}

Vector2Int operator-(const Vector2Int & lval, const Vector2Int & rval)
{
	return Vector2Int(lval.x - rval.x, lval.y - rval.y);
}

Vector2Int operator*(const Vector2Int & lval, const Vector2Int & rval)
{
	return Vector2Int(lval.x * rval.x, lval.y * rval.y);
}

Vector2Int operator%(const Vector2Int & lval, const Vector2Int & rval)
{
	return Vector2Int(lval.x % rval.x, lval.y % rval.y);
}

Vector2Int operator/(const Vector2Int & lval, const Vector2Int & rval)
{
	return Vector2Int(lval.x / rval.x, lval.y / rval.y);
}

Vector2Int operator*(const Vector2Int & lval, const float & rval)
{
	return Vector2Int(static_cast<int>(lval.x * rval), static_cast<int>(lval.y * rval));
}

bool operator==(const Vector2Int & lval, const Vector2Int & rval)
{
	return lval.x == rval.x && lval.y == rval.y;
}

bool operator!=(const Vector2Int & lval, const Vector2Int & rval)
{
	return lval.x != rval.x || lval.y != rval.y;
}

float Dot(const Vector2 & lval, const Vector2 & rval)
{
	return lval.x * rval.x + lval.y * rval.y;
}

float Cross(const Vector2 & lval, const Vector2 & rval)
{
	return lval.x * rval.y - lval.y * rval.x;;
}

Vector2 operator+(const Vector2 & lval, const Vector2 & rval)
{
	return Vector2(lval.x + rval.x, lval.y + rval.y);
}

Vector2 operator-(const Vector2 & lval, const Vector2 & rval)
{
	return Vector2(lval.x - rval.x, lval.y - rval.y);
}

Vector2 operator*(const Vector2 & lval, const Vector2 & rval)
{
	return Vector2(lval.x * rval.x, lval.y * rval.y);
}

Vector2 operator/(const Vector2 & lval, const Vector2 & rval)
{
	return Vector2(lval.x / rval.x, lval.y / rval.y);
}

Vector2 operator+(const Vector2 & lval, const float & rval)
{
	return Vector2(lval.x + rval, lval.y + rval);
}

Vector2 operator-(const Vector2 & lval, const float & rval)
{
	return Vector2(lval.x - rval, lval.y - rval);
}

Vector2 operator*(const Vector2 & lval, const float & rval)
{
	return Vector2(lval.x * rval, lval.y * rval);
}

Vector2 operator/(const Vector2 & lval, const float & rval)
{
	return Vector2(lval.x / rval, lval.y / rval);
}

Rect::Rect():center(Vector2Int()),size(Size())
{
}

Rect::Rect(const int x, const int y, const int w, const int h) : center(Vector2Int(x,y)), size(Size(w,h))
{
}

Rect::Rect(const Vector2Int & pos, const Size & sz) : center(Vector2Int(pos)), size(Size(sz))
{
}

const int Rect::Left() const
{
	return center.x - size.w/2;
}

const int Rect::Top() const
{
	return center.y - size.h/2;
}

const int Rect::Right() const
{
	return center.x + size.w/2;
}

const int Rect::Botton() const
{
	return center.y + size.h/2;
}

void Rect::Draw(unsigned int color, const bool fill)const
{
	DrawBox(this->Left(),this->Top(),this->Right(),this->Botton(),color, fill);
}

void Rect::Draw(const Vector2Int& offset, int color, const bool fill)
{
	DrawBox(offset.x + this->Left(), offset.y + this->Top(), offset.x + this->Right(), offset.y + this->Botton(), color, fill);
}

void Rect::Draw(const Vector2& offset, int color, const bool fill)
{
	DrawBox(static_cast<int>(offset.x + this->Left()), static_cast<int>(offset.y + this->Top()), 
		static_cast<int>(offset.x + this->Right()), static_cast<int>(offset.y + this->Botton()), 
		color, fill);
}

const bool Rect::IsHit(const Rect & aRect, const Rect & bRect)
{
	return abs(aRect.center.x - bRect.center.x) < (aRect.size.w + bRect.size.w) / 2 &&
		abs(aRect.center.y - bRect.center.y) <= (aRect.size.h + bRect.size.h) / 2;
}

const Size Rect::OverlapSize(const Rect & aRect, const Rect & bRect)
{
	int w = min(aRect.Right(), bRect.Right()) - max(aRect.Left(), bRect.Left());
	int h = min(aRect.Botton(), bRect.Botton()) - max(aRect.Top(), bRect.Top());
	return Size(w,h);
}

Circle::Circle():centor(0,0),radius(0)
{
}

Circle::Circle(const float x, const float y, const float r) : centor(x, y), radius(r)
{
}

Circle::Circle(const Vector2& centor, const float r) : centor(centor), radius(r)
{
}

void Circle::Draw(int color)
{
	DrawCircle(static_cast<int>(this->centor.x), static_cast<int>(this->centor.y), static_cast<int>(radius), color, false);
}

Segment::Segment():posA(Vector2()), posB(Vector2())
{
}

Segment::Segment(const Vector2 & posA, const Vector2 & posB):posA(posA),posB(posB)
{
}

Segment::Segment(const float ax, const float ay, const float bx, const float by) : posA(Vector2(ax,ay)), posB(Vector2(bx,by))
{
}

void Segment::Draw(unsigned int color)
{
	DrawLine(
		static_cast<int>(posA.x), static_cast<int>(posA.y),
		static_cast<int>(posB.x), static_cast<int>(posB.y),
		color);
}

float Segment::Grad(void)const
{
	return (posB.y - posA.y) / (posB.x - posA.x);
}

void Vector2Int::operator*=(const float scale)
{
	this->x = static_cast<int>(x * scale);
	this->y = static_cast<int>(y * scale);
}

void Vector2Int::operator+=(const Vector2Int & pos)
{
	this->x += pos.x;
	this->y += pos.y;
}

void Vector2Int::operator-=(const Vector2Int & pos)
{
	this->x -= pos.x;
	this->y -= pos.y;
}

Vector2 Vector2Int::ToFloatVector() const
{
	return Vector2(static_cast<float>(this->x), static_cast<float>(this->y));
}

Vector2Int Size::ToIntVector() const
{
	return Vector2Int(this->w,this->h);
}

Vector3 Vector3::operator-(void)
{
	return Vector3(-x,-y,-z);
}

void Vector3::operator+=(const Vector3 & val)
{
	this->x += val.x;
	this->y += val.y;
	this->z += val.z;
}

void Vector3::operator-=(const Vector3 & val)
{
	this->x -= val.x;
	this->y -= val.y;
	this->z -= val.z;
}

void Vector3::operator*=(const Vector3 & val)
{
	this->x *= val.x;
	this->y *= val.y;
	this->z *= val.z;
}

void Vector3::operator/=(const Vector3 & val)
{
	this->x /= val.x;
	this->y /= val.y;
	this->z /= val.z;
}

void Vector3::operator+=(float scale)
{
	this->x += scale;
	this->y += scale;
	this->z += scale;
}
void Vector3::operator-=(float scale)
{
	this->x -= scale;
	this->y -= scale;
	this->z -= scale;
}
void Vector3::operator*=(float scale)
{
	this->x *= scale;
	this->y *= scale;
	this->z *= scale;
}
void Vector3::operator/=(float scale)
{
	this->x /= scale;
	this->y /= scale;
	this->z /= scale;
}

float Vector3::Length() const
{
	return std::hypot(this->x, this->y, this->z);
}

void Vector3::Normalize()
{
	auto len = this->Length();
	this->x/=len;
	this->y/=len;
	this->z/=len;
}

Vector3 Vector3::Normalized() const
{
	auto len = this->Length();
	return Vector3(this->x / len, this->y / len, this->z / len);
}

float Dot(const Vector3 & lval, const Vector3 & rval)
{
	return lval.x * rval.x + lval.y * rval.y + lval.z * rval.z;
}

Vector3 Cross(const Vector3 & lval, const Vector3 & rval)
{
	return Vector3(lval.y * rval.z - lval.z * rval.y, 
		lval.z * rval.x - lval.x * rval.z, 
		lval.x * rval.y - lval.y * rval.x);
}

Vector3 operator+(const Vector3 & lval, const Vector3 & rval)
{
	return Vector3(lval.x + rval.x, lval.y + rval.y, lval.z + rval.z);
}

Vector3 operator-(const Vector3 & lval, const Vector3 & rval)
{
	return Vector3(lval.x - rval.x, lval.y - rval.y, lval.z - rval.z);
}

Vector3 operator*(const Vector3 & lval, const Vector3 & rval)
{
	return Vector3(lval.x * rval.x, lval.y * rval.y, lval.z * rval.z);
}

Vector3 operator/(const Vector3 & lval, const Vector3 & rval)
{
	return Vector3(lval.x / rval.x, lval.y / rval.y, lval.z / rval.z);
}

Vector3 operator+(const Vector3 & lval, const float & rval)
{
	return Vector3(lval.x + rval, lval.y + rval, lval.z + rval);
}

Vector3 operator-(const Vector3 & lval, const float & rval)
{
	return Vector3(lval.x - rval, lval.y - rval, lval.z - rval);
}

Vector3 operator*(const Vector3 & lval, const float & rval)
{
	return Vector3(lval.x * rval, lval.y * rval, lval.z * rval);
}

Vector3 operator/(const Vector3 & lval, const float & rval)
{
	return Vector3(lval.x / rval, lval.y / rval, lval.z / rval);
}
