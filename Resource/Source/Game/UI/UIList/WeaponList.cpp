#include "WeaponList.h"
#include "WeaponListItem.h"
#include "Application.h"
#include "DataBase.h"
#include "../StatusWindow/WeaponWindow.h"
#include "Input.h"
#include "SaveData.h"
#include "../MoneyUI.h"
#include "../CheckWindow.h"

using namespace std;

void WeaponList::Decision()
{
	_uiDeque->push_front(make_shared<CheckWindow>("購入しますか？", _uiDeque, [this]() {Buy(); }));
}

void WeaponList::Buy()
{
	auto& saveData = Application::Instance().GetSaveData();
	_weaponId = GetWeaponId();
	auto money = saveData.GetMoney();
	auto price = Application::Instance().GetDataBase().GetWeaponData(_weaponId).price;

	if (money >= price)
	{
		_weaponWindowBefore->SetWeaponId(_weaponId);
		_func();

		saveData.SetMoney(money - price);
	}
}

void WeaponList::Back()
{
	if (_uiDeque && _uiDeque->size() > 0)
	{
		if (_uiDeque->size() > 1)
		{
			(*(_uiDeque->begin() + 1))->OnActive();
		}

		_uiDeque->pop_front();
	}
}

void WeaponList::ChengeItem()
{
	WeaponListItem* listItem = dynamic_cast<WeaponListItem*>(GetListItem());
	assert(listItem);

	_weaponWindowAfter->SetWeaponId((*listItem).GetWeaponId());
}

uint8_t WeaponList::GetWeaponId()
{
	WeaponListItem* listItem = dynamic_cast<WeaponListItem*>(GetListItem());
	assert(listItem);
	return (*listItem).GetWeaponId();
}

WeaponList::WeaponList(const Vector2Int& leftup, uint8_t& weaponId, const uint8_t typeFilter, std::deque<std::shared_ptr<UI>>* uiDeque, std::function<void()> func)
	:_weaponId(weaponId), _func(func), UIList(9, uiDeque)
{
	auto& weaponDataVec = Application::Instance().GetDataBase().GetWeaponDataTable();
	for(int weaponId = 0; weaponId < weaponDataVec.size(); weaponId++)
	{
		// typeID分シフトしてbitfieldを作成しfilterと&演算を行う
		if (1 << weaponDataVec[weaponId].typeId & typeFilter)
		{
			AddListItem(std::make_shared<WeaponListItem>(weaponId, nullptr));
		}
	}

	ListItemInit(leftup);

	auto& saveData = Application::Instance().GetSaveData();
	_weaponWindowBefore = make_unique<WeaponWindow>(_weaponId, nullptr);
	_weaponWindowAfter	= make_unique<WeaponWindow>(GetWeaponId(), nullptr);

	auto wsize = Application::Instance().GetWindowSize();
	_moneyUI = make_unique<MoneyUI>(Vector2Int(wsize.w - 120, 40), nullptr);
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
	drawY += weaponWindowSize.h + space;
	_weaponWindowAfter->Draw(	Vector2Int(rect.Right() + weaponWindowSize.w / 2 + space, drawY + weaponWindowSize.h / 2));

	_moneyUI->Draw();
}
