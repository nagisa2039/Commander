#pragma once

#include <stdint.h>

// 符号なし1バイト整数にキャスト
#define Uint8(X)	static_cast<uint8_t> (X)
// 符号なし2バイト整数にキャスト
#define Uint16(X)	static_cast<uint16_t>(X)
// 符号なし4バイト整数にキャスト
#define Uint32(X)	static_cast<uint32_t>(X)
// 符号なし8バイト整数にキャスト
#define Uint64(X)	static_cast<uint64_t>(X)

// 符号あり1バイト整数にキャスト
#define Int8 (X)	static_cast<int8_t> (X)
// 符号あり2バイト整数にキャスト
#define Int16(X)	static_cast<int16_t>(X)
// 符号あり4バイト整数にキャスト
#define Int32(X)	static_cast<int32_t>(X)
// 符号あり8バイト整数にキャスト
#define Int64(X)	static_cast<int64_t>(X)

// floatキャスト
#define Float(X)	static_cast<float> (X)
// doubleにキャスト
#define Double(X)	static_cast<double>(X)

// YをXにキャスト
#define Cast(X, Y)	static_cast<X>(Y)