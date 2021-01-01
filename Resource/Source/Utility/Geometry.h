#pragma once
#include <DirectXMath.h>

struct Vector2;
struct Vector3;
struct Size;

/// <summary>
/// ����2�����x�N�g��
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
/// ���͂����l�����͈͂Ɏ��߂�
/// </summary>
/// <param name="in">���̒l</param>
/// <param name="min">����</param>
/// <param name="max">���</param>
Vector2Int Clamp(const Vector2Int& in, const Vector2Int& min, const Vector2Int& max);

/// <summary>
/// 2�����x�N�g��
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
/// ����
/// </summary>
/// <param name="lval">�x�N�g��A</param>
/// <param name="rval">�x�N�g��B</param>
float Dot(const Vector2 &lval, const Vector2& rval);

/// <summary>
/// �O��
/// </summary>
/// <param name="lval">�x�N�g��A</param>
/// <param name="rval">�x�N�g��B</param>
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
/// ���^�⊮
/// </summary>
/// <typeparam name="T">�⊮����^</typeparam>
/// <param name="lval">�n�_</param>
/// <param name="rval">�I�_</param>
/// <param name="parsent">����</param>
template <typename T>
inline T Lerp(const T lval, const T rval, const float parsent)
{
	return static_cast<T>(lval + (rval - lval) * parsent);
}

/// <summary>
/// �T�C�Y
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
/// ��`
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
	/// �`��
	/// </summary>
	/// <param name="color">�F</param>
	/// <param name="fill">�h��Ԃ���</param>
	void Draw(const unsigned int color = 0xffffff, const bool fill = true)const;

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="offset">�`��I�t�Z�b�g</param>
	/// <param name="color">�F</param>
	/// <param name="fill">�h��Ԃ���</param>
	void Draw(const Vector2Int& offset, const int color = 0xffffff, const bool fill = true)const;

	/// <summary>
	/// �摜�`��
	/// </summary>
	/// <param name="graphH">�摜�n���h��</param>
	/// <param name="offset">�`��I�t�Z�b�g</param>
	void DrawGraph(const int graphH, const Vector2Int& offset = Vector2Int{0,0})const;

	/// <summary>
	/// ��`�`��
	/// </summary>
	/// <param name="leftup">�؂蔲����`����</param>
	/// <param name="rectSize">�؂蔲����`�T�C�Y</param>
	/// <param name="graphH">�摜�n���h��</param>
	/// <param name="offset">�`��I�t�Z�b�g</param>
	void DrawRectGraph(const Vector2Int& leftup, const Size& rectSize, const int graphH, const Vector2Int& offset = Vector2Int{ 0,0 })const;

	/// <summary>
	/// ���g��؂蔲����`�Ƃ��Ďg���w�肵���͈͂ɕ`��
	/// </summary>
	/// <param name="leftup">�`�捶����W</param>
	/// <param name="rightdown">�`��E����W</param>
	/// <param name="graphH">�摜�n���h��</param>
	/// <param name="offset">�`��I�t�Z�b�g</param>
	void DrawExtendGraph(const Vector2Int& leftup, const Vector2Int& rightdown, const int graphH, const Vector2Int& offset = Vector2Int{ 0,0 })const;

	/// <summary>
	/// ��`���m�̓����蔻��
	/// </summary>
	/// <param name="rect">����̋�`</param>
	bool IsHit(const Rect& rect);

	/// <summary>
	/// ��`���m�̓����蔻��
	/// </summary>
	/// <param name="aRect">��`A</param>
	/// <param name="bRect">��`B</param>
	static const bool IsHit(const Rect& aRect, const Rect& bRect);

	/// <summary>
	/// ��`���m�̏d�Ȃ��Ă����
	/// </summary>
	/// <param name="aRect">��`A</param>
	/// <param name="bRect">��`B</param>
	static const Size OverlapSize(const Rect& aRect, const Rect& bRect);
};

/// <summary>
/// �͈�
/// </summary>
struct Range
{
	// �ŏ��l
	int min;
	// �ő�l
	int max;

	/// <summary>
	/// �w�肵���l���͈͓���
	/// </summary>
	/// <param name="value">�l</param>
	bool Hit(const int value)const;

	/// <summary>
	/// �^�[�Q�b�g�ɑ΂��Ĕ͈͊ORange���擾
	/// �U������Ƃ��Ɉ���I�ɉ����͈�
	/// </summary>
	/// <param name="target">�^�[�Q�b�g�̍U���͈�</param>
	Range GetCriticalRange(const Range& target)const;
};
bool operator==(const Range& lv, const Range& rv);