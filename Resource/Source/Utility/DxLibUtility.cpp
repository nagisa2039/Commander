#include "DxLibUtility.h"
#include <DxLib.h>
#include <stdarg.h>
#include "Application.h"

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
	return Application::Instance().GetAnkerCalculation().GetDrawPos(drawPos, size, anker);
}

void DrawStringToHandle(const Vector2Int& drawPos, const Anker anker, const unsigned color, const int fontHandle, const char* format, ...)
{
	va_list list;
	va_start(list, format);
	char str[256];
	_vstprintf_s(str, 256, format, list);
	va_end(list);

	Size strSize;
	int lineCnt;
	GetDrawExtendFormatStringSizeToHandle(&strSize.w, &strSize.h, &lineCnt, 1.0, 1.0, fontHandle, str);
	auto calPos = GetDrawPos(drawPos, strSize, anker).ToVector2();
	DrawExtendFormatStringFToHandle(calPos.x, calPos.y, 1.0, 1.0, color, fontHandle, str);
}

void DrawStringToHandle(const Vector2Int& drawPos, const Anker anker, const unsigned color, const double exRate, const int fontHandle, const char* format, ...)
{
	va_list list;
	va_start(list, format);
	char str[256];
	_vstprintf_s(str, 256, format, list);
	va_end(list);

	Size strSize;
	int lineCnt;
	GetDrawExtendFormatStringSizeToHandle(&strSize.w, &strSize.h, &lineCnt, exRate, exRate, fontHandle, str);
	auto calPos = GetDrawPos(drawPos, strSize, anker).ToVector2();
	DrawExtendFormatStringFToHandle(calPos.x, calPos.y, exRate, exRate, color, fontHandle, str);
}