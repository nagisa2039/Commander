#pragma once

#include <stdint.h>

// �����Ȃ�1�o�C�g�����ɃL���X�g
#define Uint8(X)	static_cast<uint8_t> (X)
// �����Ȃ�2�o�C�g�����ɃL���X�g
#define Uint16(X)	static_cast<uint16_t>(X)
// �����Ȃ�4�o�C�g�����ɃL���X�g
#define Uint32(X)	static_cast<uint32_t>(X)
// �����Ȃ�8�o�C�g�����ɃL���X�g
#define Uint64(X)	static_cast<uint64_t>(X)

// ��������1�o�C�g�����ɃL���X�g
#define Int8 (X)	static_cast<int8_t> (X)
// ��������2�o�C�g�����ɃL���X�g
#define Int16(X)	static_cast<int16_t>(X)
// ��������4�o�C�g�����ɃL���X�g
#define Int32(X)	static_cast<int32_t>(X)
// ��������8�o�C�g�����ɃL���X�g
#define Int64(X)	static_cast<int64_t>(X)

// float�L���X�g
#define Float(X)	static_cast<float> (X)
// double�ɃL���X�g
#define Double(X)	static_cast<double>(X)

// Y��X�ɃL���X�g
#define Cast(X, Y)	static_cast<X>(Y)