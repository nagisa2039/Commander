#include "CharactorList.h"
#include "CharactorListItem.h"
#include "SaveData.h"
#include "../StatusWindow/WeaponWindow.h"
#include "../StatusWindow/ItemWindow.h"
#include "Input.h"
#include "WeaponList.h"
#include "Application.h"
#include "DataBase.h"
#include "../MoneyUI.h"

using namespace std;

void CharactorList::Decision()
{
	auto rect = GetRect();

	auto& charactorData = Application::Instance().GetSaveData().GetCharactorData(GetListIdx());

	auto weaponType = 1 << Application::Instance().GetDataBase().GetCharactorData(charactorData.charType).weaponType;
	_uiDeque->push_front(make_shared<WeaponList>(Vector2Int(rect.Left(), rect.Top()), charactorData.status.weaponId, weaponType, _uiDeque, []() {}));
}

void CharactorList::Back()
{
	_uiDeque->pop_front();
}

void CharactorList::ChengeItem()
{
	_weaponWindow->SetWeaponId(GetWeaponId());
	//_itemWindow->SetHoge();
}

uint8_t CharactorList::GetWeaponId()
{
	CharactorListItem* charactorListItem = dynamic_cast<CharactorListItem*>(GetListItem());
	assert(charactorListItem != nullptr);
	return charactorListItem->GetCharactorData().status.weaponId;
}

CharactorList::CharactorList(const Vector2Int& leftup, const std::vector<SaveDataCharactor>& charactorDatas, std::deque<std::shared_ptr<UI>>* uiDeque)
	:UIList(6, uiDeque), _charactorDatas(charactorDatas)
{
	assert(_uiDeque);

	for (const auto& charactorData : _charactorDatas)
	{
		AddListItem(std::make_shared<CharactorListItem>(charactorData, nullptr));
	}

	_weaponWindow = std::make_unique<WeaponWindow>(GetWeaponId(), nullptr);
	//_itemWindow = std::make_unique<ItemWindow>(nullptr);

	auto wsize = Application::Instance().GetWindowSize();
	_moneyUI = make_unique<MoneyUI>(Vector2Int(wsize.w - 120, 40), nullptr);

	ListItemInit(leftup);
}

CharactorList::~CharactorList()
{
}

void CharactorList::Draw()
{
	UIList::Draw();

	auto listRect = GetRect();
	auto weaponRectSize = _weaponWindow->GetSize();

	int space = 50;
	int drawY = listRect.Top();
	auto weaponWindowSizeH = _weaponWindow->GetSize().h;
	_weaponWindow->Draw(Vector2Int(listRect.Right() + weaponRectSize.w / 2 + space, drawY + weaponWindowSizeH/2));
	drawY += (weaponWindowSizeH + space);
	/*auto itemWindowSizeH = _itemWindow->GetSize().h;
	_itemWindow->Draw(	Vector2Int(listRect.Right() + weaponRectSize.w / 2 + space, drawY + itemWindowSizeH/2));*/

	_moneyUI->Draw();
}

void CharactorList::OnActive()
{
	_weaponWindow->SetWeaponId(GetWeaponId());
	//_itemWindow->SetHoge();
}
