#include "Menu.h"
#include "Input.h"
#include "DxLibUtility.h"
#include "Application.h"
#include "FileSystem.h"
#include <Dxlib.h>
#include "CheckWindow.h"

using namespace std;

Menu::Menu(std::deque<std::shared_ptr<UI>>& uiDeque, PlayerCommander& playerCom): _playerCommander(playerCom), UI(uiDeque)
{
	auto wsize = Application::Instance().GetWindowSize();
	Vector2Int offset(-30, 60);

	// 行間
	int lineSpace = 30;
	auto menuFrameH = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/menuFrame.png");
	Size menuFrameSize;
	GetGraphSize(menuFrameH, menuFrameSize);
	Vector2Int center = Vector2Int(offset.x + wsize.w - menuFrameSize.w / 2, offset.y + menuFrameSize.h / 2);

	for (auto& centerPos : _menuCenterPoss)
	{
		centerPos = center;
		center.y += lineSpace + menuFrameSize.h;
	}

	_penAnimTrack = make_unique<Track<float>>(true);
	_penAnimTrack->AddKey(0, 0.0f);
	_penAnimTrack->AddKey(30, 1.0f);
	_penAnimTrack->AddKey(60, 0.0f);

	_menuOpenAnimTrack = make_unique<Track<float>>();
	_menuOpenAnimTrack->AddKey(0, 0.0f);
	_menuOpenAnimTrack->AddKey(15, 1.0f);

	_menuCloseAnimTrack = make_unique<Track<float>>();
	_menuCloseAnimTrack->AddKey(0, 0.0f);
	_menuCloseAnimTrack->AddKey(15, 1.0f);
	_menuUpdater = &Menu::MenuCloseUpdate;
	_menuDrawer = &Menu::MenuCloseDraw;

	_menuContentNames[static_cast<size_t>(MenuContent::situation)] = "戦況";
	_menuContentNames[static_cast<size_t>(MenuContent::retreat)] = "退却";
	_menuContentNames[static_cast<size_t>(MenuContent::turnEnd)] = "ターン終了";

	_menuContentFunc[static_cast<size_t>(MenuContent::situation)] = [&]()
	{
		_uiDeque.emplace_front(make_shared<CheckWindow>("戦況確認Windowが出る予定", _uiDeque, _playerCommander));
	};
	_menuContentFunc[static_cast<size_t>(MenuContent::retreat)] = [&]()
	{
		_uiDeque.emplace_front(make_shared<CheckWindow>("退却しますか？", _uiDeque, _playerCommander));
	};
	_menuContentFunc[static_cast<size_t>(MenuContent::turnEnd)] = [&]()
	{
		_uiDeque.emplace_front(make_shared<CheckWindow>("ターンを終了しますか？", _uiDeque, _playerCommander));
	};

	_selectMenuContent = MenuContent::situation;
}

Menu::~Menu()
{
}

void Menu::Update(const Input& input)
{
	(this->*_menuUpdater)(input);
}

void Menu::Draw()
{
	(this->*_menuDrawer)();
}

void Menu::Open(bool animation)
{
	if (animation)
	{
		_menuUpdater = &Menu::MenuOpenAnimUpdate;
		_menuDrawer = &Menu::MenuOpenAnimDraw;
	}
	else
	{
		_menuUpdater = &Menu::MenuOpenUpdate;
		_menuDrawer = &Menu::MenuOpenDraw;
	}
	_menuOpenAnimTrack->Reset();
	_selectMenuContent = MenuContent::situation;
}

void Menu::Close(bool animation)
{
	if (animation)
	{
		_menuUpdater = &Menu::MenuCloseAnimUpdate;
		_menuDrawer = &Menu::MenuCloseAnimDraw;
	}
	else
	{
		_menuUpdater = &Menu::MenuCloseUpdate;
		_menuDrawer = &Menu::MenuCloseDraw;
	}
	_menuCloseAnimTrack->Reset();
}

bool Menu::GetIsOpen() const
{
	return _menuUpdater != &Menu::MenuCloseUpdate;
}

void Menu::MenuOpenUpdate(const Input& input)
{
	if (input.GetButtonDown(0, "back"))
	{
		Close();
	}

	if (input.GetButtonDown(0, "space"))
	{
		_menuContentFunc[static_cast<size_t>(_selectMenuContent)]();
	}

	if (input.GetButtonDown(0, "up") && static_cast<int>(_selectMenuContent) > 0)
	{
		_selectMenuContent = static_cast<MenuContent>(static_cast<int>(_selectMenuContent) - 1);
	}

	if (input.GetButtonDown(0, "down") && static_cast<int>(_selectMenuContent) < static_cast<int>(MenuContent::max) - 1)
	{
		_selectMenuContent = static_cast<MenuContent>(static_cast<int>(_selectMenuContent) + 1);
	}
}

void Menu::MenuCloseUpdate(const Input& input)
{
}

void Menu::MenuOpenAnimUpdate(const Input& input)
{
	_menuOpenAnimTrack->Update();
	if (_menuOpenAnimTrack->GetEnd())
	{
		_menuUpdater = &Menu::MenuOpenUpdate;
		_menuDrawer = &Menu::MenuOpenDraw;
	}
}

void Menu::MenuCloseAnimUpdate(const Input& input)
{
	_menuCloseAnimTrack->Update();
	if (_menuCloseAnimTrack->GetEnd())
	{
		_menuUpdater = &Menu::MenuCloseUpdate;
		_menuDrawer = &Menu::MenuCloseDraw;
	}
}

void Menu::MenuOpenDraw()
{
	for (int idx = 0; idx < static_cast<int>(MenuContent::max); idx++)
	{
		DrawMenuContent(_menuCenterPoss[idx], idx);
	}

	DrawPen();
}

void Menu::MenuCloseDraw()
{
}

void Menu::MenuOpenAnimDraw()
{
	auto menuFrameH = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/menuFrame.png");
	Size menuFrameSize;
	GetGraphSize(menuFrameH, menuFrameSize);

	auto GetCenterPos = [&](const unsigned int idx)
	{

		return Lerp(Vector2Int(_menuCenterPoss[idx].x, -menuFrameSize.h / 2), _menuCenterPoss[idx], _menuOpenAnimTrack->GetValue());
	};

	for (int idx = 0; idx < static_cast<int>(MenuContent::max); idx++)
	{
		DrawMenuContent(GetCenterPos(idx), idx);
	}
}

void Menu::MenuCloseAnimDraw()
{
	auto wsize = Application::Instance().GetWindowSize();
	auto menuFrameH = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/menuFrame.png");
	Size menuFrameSize;
	GetGraphSize(menuFrameH, menuFrameSize);

	auto GetCenterPos = [&](const unsigned int idx)
	{
		return Lerp(_menuCenterPoss[idx], Vector2Int(wsize.w + menuFrameSize.w / 2, _menuCenterPoss[idx].y), _menuCloseAnimTrack->GetValue());
	};

	for (int idx = 0; idx < static_cast<int>(MenuContent::max); idx++)
	{
		DrawMenuContent(GetCenterPos(idx), idx);
	}
}

void Menu::DrawPen()
{
	auto penH = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/quillPen.png");
	Size penSize;
	GetGraphSize(penH, penSize);
	Vector2Int penMove = penSize.ToVector2Int() * -0.2f;

	auto menuFrameH = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/menuFrame.png");
	Size menuFrameSize;
	GetGraphSize(menuFrameH, menuFrameSize);

	Vector2Int penDrawPos = penMove * _penAnimTrack->GetValue() + _menuCenterPoss[static_cast<size_t>(_selectMenuContent)] - Vector2Int(menuFrameSize.w / 2 - 10, -10);
	DrawGraph(GetDrawPos(penDrawPos, penSize, Anker::rightdown), penH, true);
}

void Menu::DrawMenuContent(const Vector2Int& drawCenterPos, const unsigned int idx)
{
	int choplin30 = Application::Instance().GetFileSystem()->GetFontHandle("choplin30");
	auto menuFrameH = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/menuFrame.png");
	Size menuFrameSize;
	GetGraphSize(menuFrameH, menuFrameSize);
	DrawGraph(GetDrawPos(drawCenterPos, menuFrameSize, Anker::center), menuFrameH, true);
	Size strSize;
	int lineCnt;
	Vector2Int namePos = Vector2Int();
	GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &lineCnt, choplin30, _menuContentNames[idx].c_str());

	auto drawPos = GetDrawPos(drawCenterPos, strSize, Anker::center);
	DrawFormatStringToHandle(drawPos.x, drawPos.y, 0xffffff, choplin30, _menuContentNames[idx].c_str());
}
