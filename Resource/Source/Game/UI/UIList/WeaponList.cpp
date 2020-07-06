#include "WeaponList.h"
#include "WeaponListItem.h"
#include "Application.h"
#include "DataBase.h"
#include "../StatusWindow/WeaponWindow.h"
#include "Input.h"
#include "SaveData.h"

using namespace std;

void WeaponList::Decision()
{
	auto& saveData = Application::Instance().GetSaveData();
	auto charactorData = saveData.GetCharactorDataVec()[_charactorDataIdx];

	auto weaponId = GetWeaponId();
	charactorData.status.weaponId = weaponId;

	saveData.SaveCharactorData(charactorData, _charactorDataIdx);
	_weaponWindowBefore->SetWeaponId(weaponId);
}

void WeaponList::Back()
{
	_uiDeque->pop_front();
}

void WeaponList::ChengeItem()
{
	WeaponListItem* listItem = dynamic_cast<WeaponListItem*>(GetListItem());
	assert(listItem);

	_weaponWindowAfter->SetWeaponId(listItem->GetWeaponId());
}

uint8_t WeaponList::GetWeaponId()
{
	WeaponListItem* listItem = dynamic_cast<WeaponListItem*>(GetListItem());
	assert(listItem);
	return listItem->GetWeaponId();
}

WeaponList::WeaponList(const Vector2Int& leftup, const unsigned int charactorDataIdx, std::deque<std::shared_ptr<UI>>* uiDeque)
	:_charactorDataIdx(charactorDataIdx), UIList(9, uiDeque)
{
	assert(uiDeque);

	for (int i = 0; i < 30; i++)
	{
		AddListItem(std::make_shared<WeaponListItem>(i%6, nullptr));
	}

	ListItemInit(leftup);

	auto& saveData = Application::Instance().GetSaveData();
	auto charactorData = saveData.GetCharactorDataVec()[_charactorDataIdx];
	_weaponWindowBefore = make_unique<WeaponWindow>(charactorData.status.weaponId, nullptr);
	_weaponWindowAfter	= make_unique<WeaponWindow>(GetWeaponId(), nullptr);
}

WeaponList::~WeaponList()
{
}

void WeaponList::Draw()
{
	UIList::Draw();

	auto rect = GetRect();
	const int space = 50;
	int drawY = rect.Top();

	auto weaponWindowSize = _weaponWindowBefore->GetSize();
	_weaponWindowBefore->Draw(	Vector2Int(rect.Right() + weaponWindowSize.w / 2 + space, drawY + weaponWindowSize.h / 2));
	drawY += weaponWindowSize.h + space*2;
	_weaponWindowAfter->Draw(	Vector2Int(rect.Right() + weaponWindowSize.w / 2 + space, drawY + weaponWindowSize.h / 2));
}
