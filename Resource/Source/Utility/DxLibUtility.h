#pragma once
#include "Geometry.h"
#include "AnkerCalculation.h"

void GetGraphSize(const int handle, Size& size);

void DrawBox(const Vector2Int& leftup, const Vector2Int& rightdown, const unsigned int color, const bool fillFlag = true);

void DrawLine(const Vector2Int& start, const Vector2Int& end, const unsigned int color, const int thickness = 1);

void DrawGraph(const Vector2Int& leftup, const int handle, const bool transFlag = true);

void DrawRotaGraph(const Vector2Int& center, const double ExRate, const double Angle, const int GrHandle, const int TransFlag, const int ReverseXFlag = false, const int ReverseYFlag = false);

void DrawRectRotaGraph(const Vector2Int& center, const Vector2Int& srcLeftup, const Size& size, const double ExtRate, const double Angle, const int GraphHandle, const int TransFlag = true, const int ReverseXFlag = false, const int ReverseYFlag = false);

void DrawCircle(const Vector2Int& center, const int radius, const unsigned int color, const bool fillFlag = true);

Vector2Int GetDrawPos(const Vector2Int& drawPos, const Size& size, const Anker anker);

void DrawStringToHandle(const Vector2Int& drawPos, const Anker anker, const unsigned color, const int fontHandle, const char* format, ...);