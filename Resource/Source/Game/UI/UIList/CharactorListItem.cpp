#include <Dxlib.h>
#include "CharactorListItem.h"
#include "Charactor.h"
#include "DxLibUtility.h"
#include "Application.h"
#include "FileSystem.h"
#include "SaveData.h"
#include "DataBase.h"

CharactorListItem::CharactorListItem(const CharactorData& charactorData, std::deque<std::shared_ptr<UI>>* uiDeque)
	:UIListItem(Rect(Vector2Int(), Size(350, 60)), uiDeque), _charactorData(charactorData)
{
	
}

CharactorListItem::~CharactorListItem()
{
}

void CharactorListItem::Update(const Input& input)
{
}

void CharactorListItem::Draw(const Vector2Int& offset)
{
	auto drawRect = GetRect();
	drawRect.center += offset;
	drawRect.Draw(0xffffff);

	const int offsetX = 10;
	int drawX = drawRect.Left() + offsetX;

	auto& dataBase_charactorData = Application::Instance().GetDataBase().GetCharactorData(_charactorData.charType);

	// キャラアイコンの描画
	auto charIconSize = Size(drawRect.size.h * 0.9f, drawRect.size.h * 0.9f);
	char charIconPath[256];
	sprintf_s(charIconPath, 256, "%s_player.png", dataBase_charactorData.imagePath.c_str());

	Rect(Vector2Int(drawX + charIconSize.w / 2, drawRect.center.y), charIconSize).DrawRectGraph(Vector2Int(32, 0), Size(32,32),
		Application::Instance().GetFileSystem().GetImageHandle(charIconPath));
	drawX += charIconSize.w + offsetX;

	// 名前の描画
	int choplin30 = Application::Instance().GetFileSystem().GetFontHandle("choplin30edge");
	DrawStringToHandle(Vector2Int(drawX, drawRect.center.y), Anker::leftcenter, 0xffffff, choplin30, dataBase_charactorData.name.c_str());
}

const CharactorData& CharactorListItem::GetCharactorData() const
{
	return _charactorData;
}
