#include "DxLibUtility.h"
#include <DxLib.h>

void GetGraphSize(const int handle, Size& size)
{
	DxLib::GetGraphSize(handle, &size.w, &size.h);
}

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

void DrawRotaGraph(const Vector2Int& center, const double ExRate, const double Angle, const int GrHandle, const int TransFlag, const int ReverseXFlag, const int ReverseYFlag)
{
	DxLib::DrawRotaGraph(center.x, center.y, ExRate, Angle, GrHandle, TransFlag, ReverseXFlag, ReverseYFlag);
}

void DrawRectRotaGraph(const Vector2Int& center, const Vector2Int& srcLeftup, const Size& size, const double ExtRate, const double Angle, const int GraphHandle, const int TransFlag, const int ReverseXFlag, const int ReverseYFlag)
{
	DxLib::DrawRectRotaGraph(center.x, center.y, srcLeftup.x, srcLeftup.y, size.w, size.h, ExtRate, Angle, GraphHandle, TransFlag, ReverseXFlag, ReverseYFlag);
}

void DrawCircle(const Vector2Int& center, const int radius, const unsigned int color, const bool fillFlag)
{
	DxLib::DrawCircle(center.x, center.y, radius, color, fillFlag);
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

void DrawStringToHandle(const Vector2Int& drawPos, const Anker anker, const unsigned color, const int fontHandle, const char* string)
{
	Size strSize;
	int lineCnt;
	GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &lineCnt, fontHandle, string);
	auto calPos = GetDrawPos(drawPos, strSize, anker);
	DrawFormatStringToHandle(calPos.x, calPos.y, color, fontHandle, string);
}