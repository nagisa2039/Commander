#include "DxLibUtility.h"
#include "Geometry.h"

void DrawBox(const Vector2Int& leftup, const Vector2Int& rightdown, const unsigned int color, const bool fillFlag)
{
	DxLib::DrawBox(leftup.x, leftup.y, rightdown.x, rightdown.y, color, fillFlag);
}

void DrawLine(const Vector2Int& start, const Vector2Int& end, const unsigned int color, const int thickness)
{
	DxLib::DrawLine(start.x, start.y, end.x, end.y, color, thickness);
}

void DrawGraph(const Vector2Int& leftup, const int handle, const bool transFlag)
{
	DxLib::DrawGraph(leftup.x, leftup.y, handle, transFlag);
}
