#include "StatusInfomation.h"
#include "Input.h"
#include "MapCtrl.h"
#include "Application.h"
#include "DxLibUtility.h"
#include <Dxlib.h>
#include "MapCursor.h"
#include "FileSystem.h"
#include "Charactor.h"
#include "PlayerUI.h"

StatusInfomation::StatusInfomation(std::deque<std::shared_ptr<UI>>& uiDeque, const MapCtrl& mapCtrl, const MapCursor& cursor, const PlayerUI& playerUI)
	:UI(uiDeque), _mapCtrl(mapCtrl), _cursor(cursor), _playerUI(playerUI)
{
	_moveAnimTrack = std::make_unique<Track<float>>();
	_moveAnimTrack->AddKey(0, 0.0f);
	_moveAnimTrack->AddKey(15, 1.0f);

	_targetPosList.clear();
}

StatusInfomation::~StatusInfomation()
{
}

void StatusInfomation::Update(const Input& input)
{
	if (_playerUI.GetUIIsOpen())
	{
		if (!_moveAnimTrack->GetReverse())
		{
			_moveAnimTrack->SetReverse(true);
			_moveAnimTrack->Reset();
		}
	}
	else
	{
		if (_moveAnimTrack->GetReverse())
		{
			_moveAnimTrack->SetReverse(false);
			_moveAnimTrack->Reset();
		}

		auto currentMapPos = _cursor.GetMapPos();
		if (_targetPosList.size() <= 0 || *_targetPosList.begin() != currentMapPos)
		{
			_targetPosList.emplace_front(currentMapPos);
			_moveAnimTrack->SetReverse(false);
			_moveAnimTrack->Reset();
		}
	}

	_moveAnimTrack->Update();
}

void StatusInfomation::Draw()
{
	if (_targetPosList.size() <= 0) return;
	auto charactor = _mapCtrl.GetMapPosChar(*_targetPosList.begin());
	if (charactor == nullptr)return;

	auto fontH = Application::Instance().GetFileSystem()->GetFontHandle("choplin20");
	auto wsize = Application::Instance().GetWindowSize();
	auto space = Vector2Int(20, -20);
	Size windowSize(400, 100);
	auto windowCenter = Vector2Int(0, wsize.h) + Vector2Int(windowSize.w, -windowSize.h) * 0.5f + space;
	Rect windowRect(Lerp(Vector2Int(-windowSize.w/2, windowCenter.y), windowCenter, _moveAnimTrack->GetValue()), windowSize);
	windowRect.Draw(0xffffff);

	auto teamColor = GetTeamColorBattle(charactor->GetTeam());
	Rect nameRect(windowRect.center - Vector2Int(0, (windowSize.h - 30)/2), Size(windowSize.w, 30));
	nameRect.Draw(teamColor);
	DrawStringToHandle(nameRect.center, Anker::center, 0xffffff, fontH, charactor->GetName().c_str());

	Rect textRect(nameRect.center + Vector2Int(0, (nameRect.Height() + 70)/2), Size(windowSize.w, 70));
	int drawY = textRect.Top() + 30 / 2;

	auto startStatus = charactor->GetStartStatus();
	auto status = charactor->GetStatus();
	
	int spaceX = 25;
	char str[256];
	sprintf_s(str, 256, "Lv. %d", status.level);
	DrawStringToHandle(Vector2Int(textRect.Left()+ spaceX, drawY), Anker::leftcenter, 0xffffff, fontH, str);
	sprintf_s(str, 256, "HP %d/%d", status.health, startStatus.health);
	DrawStringToHandle(Vector2Int(textRect.Left()+300 - spaceX, drawY), Anker::rightcenter, 0xffffff, fontH, str);

	drawY += 30;
	Size hpSize = Size(250, 20);
	auto drawPos = GetDrawPos(Vector2Int(textRect.Left() + spaceX, drawY), hpSize, Anker::leftcenter);
	DrawBox(drawPos, drawPos + hpSize, 0xaaaaaa);
	DrawBox(drawPos, drawPos + Size(hpSize.w * static_cast<float>(status.health) / static_cast<float>(startStatus.health), hpSize.h), teamColor);

	Size iconSize(100, 100);
	charactor->DrawCharactorIcon(Rect(Vector2Int(windowRect.Right() - iconSize.w/2, windowRect.Botton() - iconSize.h/2), iconSize));
}
