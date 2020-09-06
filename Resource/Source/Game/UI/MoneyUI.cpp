#include "MoneyUI.h"
#include "Application.h"
#include "FileSystem.h"
#include "DxLibUtility.h"
#include "SaveData.h"

MoneyUI::MoneyUI(const Vector2Int& pos, std::deque<std::shared_ptr<UI>>* uiDeque)
	:UI(uiDeque), _money(/*Application::Instance().GetSaveData().GetMoney()*/0)
{
	_drawRect = {pos, Size(200, 40)};
	_isOpen = true;
}

void MoneyUI::Update(const Input& input)
{
}

void MoneyUI::Draw()
{
	auto& fileSystem = FileSystem::Instance();
	_drawRect.DrawGraph(fileSystem.GetImageHandle("Resource/Image/UI/window0.png"));

	int fontH = fileSystem.GetFontHandle("choplin30");
	DrawStringToHandle(Vector2Int(_drawRect.Right() - 20, _drawRect.center.y), Anker::rightcenter, 0x000000, fontH, "%d G", _money);
}
