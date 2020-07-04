#include "WeaponData.h"
#include "Application.h"
#include "DataBase.h"

void WeaponData::DrawWeaponIcon(const Rect& rect)const
{
	auto drawRect = rect;
	drawRect.Draw(0x000000);
	drawRect.size *= 0.8f;
	drawRect.Draw(Application::Instance().GetDataBase().GetAttributeData(attribute).color);
}

std::string WeaponData::GetRengeString() const
{
	char str[256];
	if (range.min == range.max)
	{
		sprintf_s(str, 256, "%d", range.max);
	}
	else
	{
		sprintf_s(str, 256, "%d Å` %d", range.min, range.max);
	}
	return str;
}
