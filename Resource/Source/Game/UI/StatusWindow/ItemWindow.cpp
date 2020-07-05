#include <string>
#include <Dxlib.h>
#include "Application.h"
#include "FileSystem.h"
#include "DxLibUtility.h"
#include "ItemWindow.h"
#include "Geometry.h"

namespace
{
	constexpr unsigned int WINDOW_SIZE_W = 250;
	constexpr unsigned int WINDOW_SIZE_H = 250;
}

int ItemWindow::_windowH = -1;

void ItemWindow::DrawToWindow()
{
	int currentScreen = GetDrawScreen();
	SetDrawScreen(_windowH);

	auto & fileSystem = Application::Instance().GetFileSystem();

	auto rectSize = GetSize();
	Rect rect = Rect(rectSize.ToVector2Int()*0.5f, rectSize);
	rect.DrawGraph(fileSystem.GetImageHandle("Resource/Image/UI/statusWindow1.png"));

	auto itemSize = Size(rect.size.w, 40);

	std::string itemNames[] = { "傷薬", "凄い傷薬", "ボロの釣り竿", "探検セット" };

	auto choplin30 = fileSystem.GetFontHandle("choplin30edge");
	Vector2Int center = Vector2Int(rect.center.x, rect.Top() + itemSize.h / 2);
	for (const auto& itemName : itemNames)
	{
		Rect(center, itemSize).DrawGraph(fileSystem.GetImageHandle("Resource/Image/UI/itemFrame.png"));
		DrawStringToHandle(center, Anker::center, 0xffffff, choplin30, itemName.c_str());
		center.y += itemSize.h;
	}

	SetDrawScreen(currentScreen);
}

ItemWindow::ItemWindow(std::deque<std::shared_ptr<UI>>* uiDeque):UI(uiDeque)
{
	if (_windowH == -1)
	{
		_windowH = MakeScreen(WINDOW_SIZE_W, WINDOW_SIZE_H, true);
	}
	DrawToWindow();
}

ItemWindow::~ItemWindow()
{
}

Size ItemWindow::GetSize() const
{
	return Size(WINDOW_SIZE_W, WINDOW_SIZE_H);
}

void ItemWindow::Update(const Input& input)
{
}

void ItemWindow::Draw()
{
}

void ItemWindow::Draw(const Vector2Int& pos)
{
	Rect(pos, GetSize()).DrawGraph(_windowH);
}

void ItemWindow::SetHoge()
{
	DrawToWindow();
}
