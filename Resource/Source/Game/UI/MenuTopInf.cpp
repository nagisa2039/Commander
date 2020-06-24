#include "MenuTopInf.h"
#include "MapCtrl.h"
#include "Application.h"
#include "FileSystem.h"
#include "DataBase.h"
#include "DxLibUtility.h"

using namespace std;

MenuTopInf::MenuTopInf(const MapCtrl& mapCtrl, const unsigned char& turnCnt, std::deque<std::shared_ptr<UI>> uiDeque)
	: UI(uiDeque), _mapCtrl(mapCtrl), _turnCnt(turnCnt)
{
	auto fileSystem = Application::Instance().GetFileSystem();
	int nameFrameH = fileSystem.GetImageHandle("Resource/Image/UI/mapNameFrame.png");
	int turnCntFrameH = fileSystem.GetImageHandle("Resource/Image/UI/turnCntFrame.png");

	int spaceY = 20;
	Size nameFrameSize;
	GetGraphSize(nameFrameH, nameFrameSize);

	Size turnCntSize;
	GetGraphSize(turnCntFrameH, turnCntSize);

	_moveAnimTrack = make_unique<Track<Vector2Int>>();
	_moveAnimTrack->AddKey(0, Vector2Int(-nameFrameSize.w - turnCntSize.w, static_cast<int>(nameFrameSize.h * 0.5f) + spaceY));
	_moveAnimTrack->AddKey(30, Vector2Int(static_cast<int>(nameFrameSize.w * 0.5f), static_cast<int>(nameFrameSize.h * 0.5f) + spaceY));
	_moveAnimTrack->SetReverse(true);
	_moveAnimTrack->End();
}

MenuTopInf::~MenuTopInf()
{
}

void MenuTopInf::Update(const Input& input)
{
	_moveAnimTrack->Update();
}

void MenuTopInf::Draw()
{
	auto fileSystem = Application::Instance().GetFileSystem();
	int nameFrameH = fileSystem.GetImageHandle("Resource/Image/UI/mapNameFrame.png");
	int turnCntFrameH = fileSystem.GetImageHandle("Resource/Image/UI/turnCntFrame.png");
	int choplin20 = fileSystem.GetFontHandle("choplin20edge");
	int choplin30 = fileSystem.GetFontHandle("choplin30edge");

	Size nameFrameSize;
	GetGraphSize(nameFrameH, nameFrameSize);
	Size turnCntSize;
	GetGraphSize(turnCntFrameH, turnCntSize);

	Rect nameFrameRect(_moveAnimTrack->GetValue(), nameFrameSize);
	Rect turnCntFrameRect(Vector2Int(nameFrameRect.Right() + static_cast<int>(turnCntSize.w * 0.5f), nameFrameRect.center.y), turnCntSize);

	nameFrameRect.DrawGraph(nameFrameH);
	turnCntFrameRect.DrawGraph(turnCntFrameH);

	// マップ名の描画
	DrawStringToHandle(nameFrameRect.center, Anker::center, 0xffffff, choplin30,
		Application::Instance().GetDataBase().GetMapData(_mapCtrl.GetMapID()).name.c_str());

	// 経過ターン数の描画
	DrawStringToHandle(turnCntFrameRect.center - Vector2Int(0, 15), Anker::center, 0xffffff, choplin20, "TURN");
	DrawStringToHandle(turnCntFrameRect.center + Vector2Int(0, 15), Anker::center, 0xffffff, choplin20, "%d", _turnCnt);
}

void MenuTopInf::Open()
{
	if (!_moveAnimTrack->GetReverse()) return;

	_moveAnimTrack->SetReverse(false);
	_moveAnimTrack->Reset();
}

void MenuTopInf::Close()
{
	if (_moveAnimTrack->GetReverse()) return;

	_moveAnimTrack->SetReverse(true);
	_moveAnimTrack->Reset();
}
