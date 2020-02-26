#include "DxLibUtility.h"
#include <DxLib.h>

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

void DrawCircle(const Vector2Int& center, const int radius, const unsigned int color, const bool fillFlag)
{
	DxLib::DrawCircle(center.x, center.y, radius, color, fillFlag);
}

void DrawFormatStringToHandle(const Vector2Int& leftup, unsigned int Color, const int FontHandle, const char* FormatString)
{
	DxLib::DrawFormatStringToHandle(leftup.x, leftup.y, Color, FontHandle, FormatString);
}

Vector2Int GetDrawPos(const Vector2Int& drawPos, const Size& size, const Anker anker)
{
	switch (anker)
	{
	case Anker::leftup:
		return drawPos;
	case Anker::leftcenter:
		return drawPos + Vector2Int(0, -size.h / 2);
	case Anker::leftdown:
		return drawPos + Vector2Int(0, -size.h);
	case Anker::centerup:
		return drawPos + Vector2Int(-size.w / 2, 0);
	case Anker::center:
		return drawPos + Vector2Int(-size.w / 2, -size.h / 2);
	case Anker::centerdown:
		return drawPos + Vector2Int(-size.w / 2, -size.h);
	case Anker::rightup:
		return drawPos + Vector2Int(-size.w, 0);
	case Anker::rightcenter:
		return drawPos + Vector2Int(-size.w, -size.h / 2);
	case Anker::rightdown:
		return drawPos + Vector2Int(-size.w, -size.h);
	default:
		return Vector2Int();
	}
}