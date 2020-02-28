#pragma once
#include <array>
#include <functional>
#include "Geometry.h"

void DrawBox(const Vector2Int& leftup, const Vector2Int& rightdown, const unsigned int color, const bool fillFlag = true);

void DrawLine(const Vector2Int& start, const Vector2Int& end, const unsigned int color, const int thickness = 1);

void DrawGraph(const Vector2Int& leftup, const int handle, const bool transFlag = true);

void DrawCircle(const Vector2Int& center, const int radius, const unsigned int color, const bool fillFlag = true);

Vector2Int GetDrawPos(const Vector2Int& drawPos, const Size& size, const Anker anker);