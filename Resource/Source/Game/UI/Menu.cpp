#include "Menu.h"
#include "Input.h"
#include "DxLibUtility.h"
#include "Application.h"
#include "FileSystem.h"
#include <Dxlib.h>
#include "CheckWindow.h"
#include "PlayerCommander.h"

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

	for (auto& centerPos : _centerPoss)
	{
		centerPos = center;
		center.y += lineSpace + menuFrameSize.h;
	}

	_penAnimTrack = make_unique<Track<float>>(true);
	_penAnimTrack->AddKey(0, 0.0f);
	_penAnimTrack->AddKey(30, 1.0f);
	_penAnimTrack->AddKey(60, 0.0f);

	_openAnimTrack = make_unique<Track<float>>();
	_openAnimTrack->AddKey(0, 0.0f);
	_openAnimTrack->AddKey(15, 1.0f);

	_closeAnimTrack = make_unique<Track<float>>();
	_closeAnimTrack->AddKey(0, 0.0f);
	_closeAnimTrack->AddKey(15, 1.0f);
	_updater = &Menu::CloseUpdate;
	_drawer = &Menu::CloseDraw;

	_contentNames[static_cast<size_t>(Content::situation)] = "戦況";
	_contentNames[static_cast<size_t>(Content::retreat)] = "退却";
	_contentNames[static_cast<size_t>(Content::turnEnd)] = "ターン終了";

	_contentFunc[static_cast<size_t>(Content::situation)] = [&]()
	{
		_uiDeque.emplace_front(make_shared<CheckWindow>("戦況確認Windowが出る予定", _uiDeque, [&](){_playerCommander.End();}));
	};
	_contentFunc[static_cast<size_t>(Content::retreat)] = [&]()
	{
		_uiDeque.emplace_front(make_shared<CheckWindow>("退却しますか？", _uiDeque, [&](){_playerCommander.End();}));
	};
	_contentFunc[static_cast<size_t>(Content::turnEnd)] = [&]()
	{
		_uiDeque.emplace_front(make_shared<CheckWindow>("ターンを終了しますか？", _uiDeque, [&](){_playerCommander.End();}));
	};

	_selectContent = Content::situation;
}

Menu::~Menu()
{
}

void Menu::Update(const Input& input)
{
	(this->*_updater)(input);
}

void Menu::Draw()
{
	(this->*_drawer)();
}

void Menu::Open(bool animation)
{
	if (animation)
	{
		_updater = &Menu::OpenAnimUpdate;
		_drawer = &Menu::OpenAnimDraw;
	}
	else
	{
		_updater = &Menu::OpenUpdate;
		_drawer = &Menu::OpenDraw;
	}
	_openAnimTrack->Reset();
	_selectContent = Content::situation;
}

void Menu::Close(bool animation)
{
	if (animation)
	{
		_updater = &Menu::CloseAnimUpdate;
		_drawer = &Menu::CloseAnimDraw;
	}
	else
	{
		_updater = &Menu::CloseUpdate;
		_drawer = &Menu::CloseDraw;
	}
	_closeAnimTrack->Reset();
}

bool Menu::GetIsOpen() const
{
	return _updater != &Menu::CloseUpdate;
}

void Menu::OpenUpdate(const Input& input)
{
	if (input.GetButtonDown(0, "back"))
	{
		Close();
	}

	if (input.GetButtonDown(0, "space"))
	{
		_contentFunc[static_cast<size_t>(_selectContent)]();
	}

	if (input.GetButtonDown(0, "up") && static_cast<int>(_selectContent) > 0)
	{
		_selectContent = static_cast<Content>(static_cast<int>(_selectContent) - 1);
	}

	if (input.GetButtonDown(0, "down") && static_cast<int>(_selectContent) < static_cast<int>(Content::max) - 1)
	{
		_selectContent = static_cast<Content>(static_cast<int>(_selectContent) + 1);
	}
	_penAnimTrack->Update();
}

void Menu::CloseUpdate(const Input& input)
{
}

void Menu::OpenAnimUpdate(const Input& input)
{
	_openAnimTrack->Update();
	if (_openAnimTrack->GetEnd())
	{
		_updater = &Menu::OpenUpdate;
		_drawer = &Menu::OpenDraw;
	}
}

void Menu::CloseAnimUpdate(const Input& input)
{
	_closeAnimTrack->Update();
	if (_closeAnimTrack->GetEnd())
	{
		_updater = &Menu::CloseUpdate;
		_drawer = &Menu::CloseDraw;
	}
}

void Menu::OpenDraw()
{
	for (int idx = 0; idx < static_cast<int>(Content::max); idx++)
	{
		DrawContent(_centerPoss[idx], idx);
	}

	DrawPen();
}

void Menu::CloseDraw()
{
}

void Menu::OpenAnimDraw()
{
	auto menuFrameH = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/menuFrame.png");
	Size menuFrameSize;
	GetGraphSize(menuFrameH, menuFrameSize);

	auto GetCenterPos = [&](const unsigned int idx)
	{

		return Lerp(Vector2Int(_centerPoss[idx].x, -menuFrameSize.h / 2), _centerPoss[idx], _openAnimTrack->GetValue());
	};

	for (int idx = 0; idx < static_cast<int>(Content::max); idx++)
	{
		DrawContent(GetCenterPos(idx), idx);
	}
}

void Menu::CloseAnimDraw()
{
	auto wsize = Application::Instance().GetWindowSize();
	auto menuFrameH = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/menuFrame.png");
	Size menuFrameSize;
	GetGraphSize(menuFrameH, menuFrameSize);

	auto GetCenterPos = [&](const unsigned int idx)
	{
		return Lerp(_centerPoss[idx], Vector2Int(wsize.w + menuFrameSize.w / 2, _centerPoss[idx].y), _closeAnimTrack->GetValue());
	};

	for (int idx = 0; idx < static_cast<int>(Content::max); idx++)
	{
		DrawContent(GetCenterPos(idx), idx);
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

	Vector2Int penDrawPos = penMove * _penAnimTrack->GetValue() + _centerPoss[static_cast<size_t>(_selectContent)] - Vector2Int(menuFrameSize.w / 2 - 10, -10);
	DrawGraph(GetDrawPos(penDrawPos, penSize, Anker::rightdown), penH, true);
}

void Menu::DrawContent(const Vector2Int& drawCenterPos, const unsigned int idx)
{
	int choplin30 = Application::Instance().GetFileSystem()->GetFontHandle("choplin30");
	auto menuFrameH = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/menuFrame.png");
	Size menuFrameSize;
	GetGraphSize(menuFrameH, menuFrameSize);
	DrawGraph(GetDrawPos(drawCenterPos, menuFrameSize, Anker::center), menuFrameH, true);
	Size strSize;
	int lineCnt;
	Vector2Int namePos = Vector2Int();
	GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &lineCnt, choplin30, _contentNames[idx].c_str());

	auto drawPos = GetDrawPos(drawCenterPos, strSize, Anker::center);
	DrawFormatStringToHandle(drawPos.x, drawPos.y, 0xffffff, choplin30, _contentNames[idx].c_str());
}
