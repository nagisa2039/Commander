#pragma once
#include <DxLib.h>

struct Vector2Int;

void DrawBox(const Vector2Int& leftup, const Vector2Int& rightdown, const unsigned int color, const bool fillFlag = true);

void DrawLine(const Vector2Int& start, const Vector2Int& end, const unsigned int color, const int thickness = 1);

void DrawGraph(const Vector2Int& leftup, const int handle, const bool transFlag = true);