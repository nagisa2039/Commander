#include "Menu.h"
#include "Input.h"
#include "DxLibUtility.h"
#include "Application.h"
#include "FileSystem.h"
#include <Dxlib.h>

using namespace std;

Menu::Menu(std::deque<std::shared_ptr<UI>>& uiDeque, PlayerCommander& playerCom, const MapCtrl& mapCtrl)
	: _playerCommander(playerCom), _mapCtrl(mapCtrl), UI(uiDeque)
{
	_selectContent = 0;
}

void Menu::Init(const size_t contentNum, const int frameH)
{
	_contentInfs.resize(contentNum);

	auto wsize = Application::Instance().GetWindowSize();
	Vector2Int offset(-30, 60);

	// çsä‘
	int lineSpace = 30;
	Size menuFrameSize;
	GetGraphSize(frameH, menuFrameSize);
	Vector2Int center = Vector2Int(offset.x + wsize.w - menuFrameSize.w / 2, offset.y + menuFrameSize.h / 2);

	for (auto& contentInf : _contentInfs)
	{
		contentInf.centerPos = center;
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
	_isOpen = false;
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
	if (_isOpen) return;

	_isOpen = true;
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
	_selectContent = 0;
}

void Menu::Close(bool animation)
{
	if (!_isOpen || _updater != &Menu::OpenUpdate) return;

	if (animation)
	{
		_updater = &Menu::CloseAnimUpdate;
		_drawer = &Menu::CloseAnimDraw;
	}
	else
	{
		_updater = &Menu::CloseUpdate;
		_drawer = &Menu::CloseDraw;
		_isOpen = false;
	}
	_closeAnimTrack->Reset();
}

void Menu::Back()
{
	Close();
}

void Menu::Decision()
{
	_contentInfs[_contentList[_selectContent]].func();
}

void Menu::OpenUpdate(const Input& input)
{
	if (input.GetButtonDown(0, "back") || input.GetButtonDown(1, "back"))
	{
		Back();
	}

	if (input.GetButtonDown(0, "ok") || input.GetButtonDown(1, "ok"))
	{
		Decision();
	}

	if ((input.GetButtonDown(0, "up") || input.GetButtonDown(1, "up")) && _selectContent > 0)
	{
		_selectContent = _selectContent - 1;
	}

	if ((input.GetButtonDown(0, "down") || input.GetButtonDown(1, "down")) && _selectContent < _contentList.size() - 1)
	{
		_selectContent = _selectContent + 1;
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
		_isOpen = false;
	}
}

void Menu::OpenDraw()
{
	for (int idx = 0; idx < _contentList.size(); idx++)
	{
		DrawContent(_contentInfs[idx].centerPos, _contentList[idx]);
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
		return Lerp(Vector2Int(_contentInfs[idx].centerPos.x, -menuFrameSize.h / 2), _contentInfs[idx].centerPos, _openAnimTrack->GetValue());
	};

	for (int idx = 0; idx < _contentList.size(); idx++)
	{
		DrawContent(GetCenterPos(idx), _contentList[idx]);
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
		return Lerp(_contentInfs[idx].centerPos, Vector2Int(wsize.w + menuFrameSize.w / 2, _contentInfs[idx].centerPos.y), _closeAnimTrack->GetValue());
	};

	for (int idx = 0; idx < _contentList.size(); idx++)
	{
		DrawContent(GetCenterPos(idx), _contentList[idx]);
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

	Vector2Int penDrawPos = penMove * _penAnimTrack->GetValue() + _contentInfs[static_cast<size_t>(_selectContent)].centerPos - Vector2Int(menuFrameSize.w / 2 - 10, -10);
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
	GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &lineCnt, choplin30, _contentInfs[idx].name.c_str());

	auto drawPos = GetDrawPos(drawCenterPos, strSize, Anker::center);
	DrawFormatStringToHandle(drawPos.x, drawPos.y, 0xffffff, choplin30, _contentInfs[idx].name.c_str());
}