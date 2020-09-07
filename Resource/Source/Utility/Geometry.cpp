#include <cmath>
#include "Geometry.h"
#include <DxLib.h>
#include <algorithm>

Vector2::Vector2(const Vector3& vec3): x(vec3.x), y(vec3.y)
{
}

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

Vector2Int Vector2::ToVector2Int()const
{
	return Vector2Int(static_cast<int>(this->x),static_cast<int>(this->y));
}

DirectX::XMFLOAT2 Vector2::ToXMFLOAT2() const
{
	return DirectX::XMFLOAT2(x,y);
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

Vector2Int operator+(const Vector2Int& lval, const int& rval)
{
	return Vector2Int(lval.x + rval, lval.y + rval);
}

bool operator==(const Vector2Int & lval, const Vector2Int & rval)
{
	return lval.x == rval.x && lval.y == rval.y;
}

bool operator!=(const Vector2Int & lval, const Vector2Int & rval)
{
	return lval.x != rval.x || lval.y != rval.y;
}

bool operator<(const Vector2Int& lval, const Vector2Int& rval)
{
	return lval.x < rval.x && lval.y < rval.y;
}

bool operator>(const Vector2Int& lval, const Vector2Int& rval)
{
	return lval.x > rval.x&& lval.y > rval.y;
}

bool operator<=(const Vector2Int& lval, const Vector2Int& rval)
{
	return lval.x <= rval.x&& lval.y <= rval.y;
}

bool operator>=(const Vector2Int& lval, const Vector2Int& rval)
{
	return lval.x >= rval.x && lval.y >= rval.y;
}

Vector2Int operator+(const Vector2Int& lval, const Size& rval)
{
	return Vector2Int(lval.x + rval.w, lval.y + rval.h);
}

Vector2Int operator*(const Vector2Int& lval, const Size& rval)
{
	return Vector2Int(lval.x * rval.w, lval.y * rval.h);
}

Vector2Int operator*(const Vector2Int& lval, const Vector2& rval)
{
	return (lval.ToVector2() * rval).ToVector2Int();
}

Vector2Int Lerp(const Vector2Int& start, const Vector2Int& end, const float parsent)
{
	return start + (end - start) * parsent;
}

Vector2Int Clamp(const Vector2Int& in, const Vector2Int& min, const Vector2Int& max)
{
	return Vector2Int(std::clamp(in.x, min.x, max.x), std::clamp(in.y, min.y, max.y));
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

Vector2 Lerp(const Vector2& start, const Vector2& end, const float parsent)
{
	return start + (end - start) * parsent;
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
	int rightbuttomOffset = fill ? 0 : -1;
	DxLib::DrawBox(this->Left(),this->Top(),this->Right() + rightbuttomOffset,this->Botton() + rightbuttomOffset,color, fill);
}

void Rect::Draw(const Vector2Int& offset, int color, const bool fill)const
{
	int rightbuttomOffset = fill ? 0 : -1;
	DxLib::DrawBox(static_cast<int>(offset.x + this->Left()), static_cast<int>(offset.y + this->Top()),
		static_cast<int>(offset.x + this->Right() + rightbuttomOffset), static_cast<int>(offset.y + this->Botton() + rightbuttomOffset),
		color, fill);
}

void Rect::DrawGraph(const int graphH, const Vector2Int& offset)const
{
	DxLib::DrawExtendGraph(offset.x + Left(), offset.y + Top(), offset.x + Right(), offset.y + Botton(), graphH, true);
}

void Rect::DrawRectGraph(const Vector2Int& leftup, const Size& rectSize, const int graphH, const Vector2Int& offset)const
{
	DxLib::DrawRectExtendGraph(offset.x + Left(), offset.y + Top(), offset.x + Right(), offset.y + Botton(),
		leftup.x, leftup.y, rectSize.w, rectSize.h, graphH, true);
}

void Rect::DrawExtendGraph(const Vector2Int& leftup, const Vector2Int& rightdown, const int graphH, const Vector2Int& offset)const
{
	DxLib::DrawRectExtendGraph(offset.x + leftup.x, offset.y + leftup.y, offset.x + rightdown.x, offset.y + rightdown.y,
		Left(), Top(), Width(), Height(), graphH, true);
}

bool Rect::IsHit(const Rect& rect)
{
	return abs(center.x - rect.center.x) < (size.w + rect.size.w) / 2 &&
		abs(center.y - rect.center.y) <= (size.h + rect.size.h) / 2;
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

void Vector2Int::operator=(const Vector2& pos)
{
	this->x = static_cast<int>(pos.x);
	this->y = static_cast<int>(pos.y);
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

Vector2 Vector2Int::ToVector2() const
{
	return Vector2(static_cast<float>(this->x), static_cast<float>(this->y));
}

Size Vector2Int::ToSize() const
{
	return Size(this->x, this->y);
}

Vector2Int Vector2Int::Zero()
{
	return Vector2Int(0,0);
}

Vector2Int Vector2Int::One()
{
	return Vector2Int(1,1);
}

Vector2Int Size::ToVector2Int() const
{
	return Vector2Int(this->w,this->h);
}

Vector2 Size::ToVector2() const
{
	return Vector2(static_cast<float>(w), static_cast<float>(h));
}

Size Size::operator-(const Size& size)
{
	return Size(-size.w, -size.h);
}

void Size::operator*=(const float scale)
{
	this->w = static_cast<int>(w * scale);
	this->h = static_cast<int>(h * scale);
}

Vector3 Vector3::operator-(void)const
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
	return std::hypot((const float)this->x, (const float)this->y, (const float)this->z);
}

Vector3 Vector3::Normalize()
{
	auto len = this->Length();
	this->x/=len;
	this->y/=len;
	this->z/=len;
	return *this;
}

Vector3 Vector3::Normalized() const
{
	auto len = this->Length();
	return Vector3(this->x / len, this->y / len, this->z / len);
}

DirectX::XMVECTOR Vector3::ToXMVECTOR() const
{
	return DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(x,y,z));
}

DirectX::XMFLOAT3 Vector3::ToXMFLOAT3() const
{
	return DirectX::XMFLOAT3(x,y,z);
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

Vector3 ReflectionVector(const Vector3 & baseVector, const Vector3 & norm)
{
	return baseVector + norm * (2 * Dot(baseVector*-1, norm));
}

Vector3 RefractionVector(const Vector3 & baseVector, const Vector3 & norm, float parsent)
{
	float vn = Dot(baseVector, norm);
	float d = 1 - pow((1 + vn), 2) / pow(parsent,2);
	return norm * (-vn/parsent - sqrtf(d)) + baseVector * (1 / parsent);
}

Vector3 Lerp(const Vector3 & start, const Vector3 & end, const float parsent)
{
	return start + (end - start) * parsent;
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

Vector3 operator+(const Vector3& lval, const Vector2& rval)
{
	return Vector3(lval.x + rval.x, lval.y + rval.y, lval.z);
}

Vector3 RotateVector(const Vector3& baseVector, const Vector3& rotate)
{
	auto DxVECTOR = VGet(baseVector.x, baseVector.y, baseVector.z);
	auto DxMATRIX = MGetIdent();

	const float deg2rad = static_cast<float>(DX_PI / 180.0f);
	auto rotaX = MGetRotX(rotate.x * deg2rad);
	auto rotaY = MGetRotY(rotate.y * deg2rad);
	auto rotaZ = MGetRotZ(rotate.z * deg2rad);

	DxMATRIX = MMult(MMult(rotaX, rotaY), rotaZ);
	
	DxVECTOR = VTransform(DxVECTOR, DxMATRIX);

	return Vector3(DxVECTOR.x, DxVECTOR.y, DxVECTOR.z);
}

Vector3 XMVECTORtoVec3(DirectX::XMVECTOR & vec)
{
	DirectX::XMFLOAT3 f3;
	DirectX::XMStoreFloat3(&f3, vec);
	return Vector3(f3.x, f3.y, f3.z);
}

DirectX::XMFLOAT2 Lerp(const DirectX::XMFLOAT2 lval, const DirectX::XMFLOAT2 rval, const float parsent)
{
	return DirectX::XMFLOAT2(Lerp(lval.x, rval.y, parsent), Lerp(lval.y, rval.y, parsent));
}

float TriangleArea(const Vector3 & p0, const Vector3 & p1, const Vector3 & p2)
{
	// éOï”ÇÃí∑Ç≥ÇãÅÇﬂÇÈ
	auto e0 = (p1 - p0).Length();
	auto e1 = (p2 - p0).Length();
	auto e2 = (p2 - p1).Length();

	// ÉwÉçÉìÇÃåˆéÆÇ≈ñ êœÇåvéZ
	auto s = (e0 + e1 + e2) / 2.0f;

	auto area = sqrtf(s*(s - e0)*(s - e1)*(s - e2));
	return area;
}

Size operator+(const Size& lval, const Size& rval)
{
	return Size(lval.w + rval.w, lval.h + rval.h);
}

Size operator*(const Size& lval, const Size& rval)
{
	return Size(lval.w * rval.w, lval.h * rval.h);
}

Size operator/(const Size& lval, const Size& rval)
{
	return Size(lval.w / rval.w, lval.h / rval.h);
}

Size operator/(const Size& lval, const Vector2Int& rval)
{
	return Size(lval.w / rval.x, lval.h / rval.y);
}

Size operator+(const Size& lval, const int& rval)
{
	return Size(lval.w + rval, lval.h + rval);
}

Size operator*(const Size& lval, const float& rval)
{
	return Size(static_cast<int>(lval.w * rval), static_cast<int>(lval.h * rval));
}

bool operator==(const Size& lval, const Size& rval)
{
	return lval.w == rval.w && lval.h == rval.h;
}

float Clamp(const float in, const float min, const float max)
{
	return min(max(in, min), max);
}

Vector3 Clamp(const Vector3 & in, const float min, const float max)
{
	return Vector3(Clamp(in.x, min, max), Clamp(in.y, min, max), Clamp(in.z, min, max));
}

bool operator==(const Range& lv, const Range& rv)
{
	return lv.min == rv.min && lv.max == rv.max;
}

Range::Range()
{
	this->min = 0;
	this->max = 0;
}

Range::Range(const int min, const int max)
{
	this->min = min;
	this->max = max;
}

bool Range::Hit(const int value) const
{
	return this->min <= value && this->max >= value;
}

Range Range::GetCriticalRange(const Range& target) const
{
	Range critical = *this;
	if (target == *this)
	{
		return Range(0,0);
	}

	bool start = false;
	for (int i = this->min; i <= this->max; i++)
	{
		if (!target.Hit(i))
		{
			start = true;
			critical.min = i;
		}
		else
		{
			if (start)
			{
				critical.max = i - 1;
				return critical;
			}
		}
	}
	return start ? critical : Range(0,0);
}
