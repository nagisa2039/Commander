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

CalDrawPos::CalDrawPos()
{
	_calFunArray[static_cast<int>(Anker::leftup)] = [](const Vector2Int& drawPos, const Size& size) 
	{
		return drawPos;
	};

	_calFunArray[static_cast<int>(Anker::leftcenter)] = [](const Vector2Int& drawPos, const Size& size)
	{
		return drawPos + Vector2Int(0, - size.h/2);
	};

	_calFunArray[static_cast<int>(Anker::leftdown)] = [](const Vector2Int& drawPos, const Size& size)
	{
		return drawPos + Vector2Int(0, -size.h);
	};

	_calFunArray[static_cast<int>(Anker::centerup)] = [](const Vector2Int& drawPos, const Size& size)
	{
		return drawPos + Vector2Int(-size.w/2, 0);
	};

	_calFunArray[static_cast<int>(Anker::center)] = [](const Vector2Int& drawPos, const Size& size)
	{
		return drawPos + Vector2Int(-size.w / 2, -size.h / 2);
	};

	_calFunArray[static_cast<int>(Anker::centerdown)] = [](const Vector2Int& drawPos, const Size& size)
	{
		return drawPos + Vector2Int(-size.w / 2, -size.h);
	};

	_calFunArray[static_cast<int>(Anker::rightup)] = [](const Vector2Int& drawPos, const Size& size)
	{
		return drawPos + Vector2Int(-size.w, 0);
	};

	_calFunArray[static_cast<int>(Anker::rightcenter)] = [](const Vector2Int& drawPos, const Size& size)
	{
		return drawPos + Vector2Int(-size.w, -size.h / 2);
	}; 

	_calFunArray[static_cast<int>(Anker::rightdown)] = [](const Vector2Int& drawPos, const Size& size)
	{
		return drawPos + Vector2Int(-size.w, -size.h);
	}; 
}

CalDrawPos::~CalDrawPos()
{
}

Vector2Int CalDrawPos::GetDrawPos(const Vector2Int& drawPos, const Size& size, const Anker anker)
{
	return _calFunArray[static_cast<int>(anker)](drawPos, size);
}
