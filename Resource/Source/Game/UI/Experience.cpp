#include <dxlib.h>
#include <algorithm>
#include "Experience.h"
#include "Application.h"
#include "Geometry.h"
#include "Charactor.h"
#include "BattleCharactor.h"
#include "DataBase.h"
#include "DxLibUtility.h"
#include "FileSystem.h"
#include "Input.h"

namespace
{
	constexpr unsigned int EXP_ANIM_MAX = 5;
	constexpr unsigned int ITEM_ANIM_MAX = 60;
	constexpr unsigned int WAIT_MAX = 60;

	constexpr unsigned int STATUS_ITEM_SIZE_W = 300;
	constexpr unsigned int STATUS_ITEM_SIZE_H = 60;
	constexpr unsigned int STATUS_ROW_CNT = 2;

	constexpr unsigned int NUM_GRAPH_SIZE = 128;
}

int Experience::_windowStatusH = -1;

using namespace std;

void Experience::WaitUpdate(const Input& input)
{
	_waitCnt++;
	if (_waitCnt > WAIT_MAX)
	{
		if (_nextUpdater == nullptr)
		{
			End();
		}
		else
		{
			_updater = _nextUpdater;
		}
	}
}

void Experience::ExpBerUpdate(const Input& input)
{
	bool skip = false;
	if (input.GetButtonDown(0, "ok"))
	{
		_itemAnimCnt = ITEM_ANIM_MAX;
		skip = true;
	}

	_expAnimCnt++;
	if (_expAnimCnt < EXP_ANIM_MAX && !skip)return;

	if (skip)
	{
		_currentExp += _currentAddExp;
	}
	else
	{
		_expAnimCnt = 0;
		_currentAddExp--;
		_currentExp++;
	}

	// レベルアップ
	if (_currentExp >= _maxExp)
	{
		if (_windowStatusH == -1)
		{
			_windowStatusH = MakeScreen(STATUS_ITEM_SIZE_W * STATUS_ROW_CNT, _drawDatas.size() / STATUS_ROW_CNT * STATUS_ITEM_SIZE_H, true);
		}

		auto& charactor = _battleChar.GetCharacotr();
		auto startStatus = charactor.GetStartStatus();
		_addStatus = charactor.GetLevelUpStatus();

		_drawDatas[static_cast<size_t>(Item::health)].current = startStatus.health;
		_drawDatas[static_cast<size_t>(Item::power)].current = startStatus.power;
		_drawDatas[static_cast<size_t>(Item::magic_power)].current = startStatus.magic_power;
		_drawDatas[static_cast<size_t>(Item::skill)].current = startStatus.skill;
		_drawDatas[static_cast<size_t>(Item::speed)].current = startStatus.speed;
		_drawDatas[static_cast<size_t>(Item::defence)].current = startStatus.defense;
		_drawDatas[static_cast<size_t>(Item::magic_defence)].current = startStatus.magic_defense;
		_drawDatas[static_cast<size_t>(Item::luck)].current = startStatus.luck;

		_drawDatas[static_cast<size_t>(Item::health)].add = _addStatus.health;
		_drawDatas[static_cast<size_t>(Item::power)].add = _addStatus.power;
		_drawDatas[static_cast<size_t>(Item::magic_power)].add = _addStatus.magic_power;
		_drawDatas[static_cast<size_t>(Item::skill)].add = _addStatus.skill;
		_drawDatas[static_cast<size_t>(Item::speed)].add = _addStatus.speed;
		_drawDatas[static_cast<size_t>(Item::defence)].add = _addStatus.defense;
		_drawDatas[static_cast<size_t>(Item::magic_defence)].add = _addStatus.magic_defense;
		_drawDatas[static_cast<size_t>(Item::luck)].add = _addStatus.luck;

		_scaleTrack->Reset();
		_updater = &Experience::ScaleUpdate;
		_drawer = &Experience::LevelUpDraw;

		_itemAnimCnt = 0;

		_drawIdx = -1;
		_drawIdx = GetNextDrawIdx();

		return;
	}
	// 終了
	if (_currentAddExp <= 0)
	{
		Wait(nullptr);
		return;
	}
}

void Experience::ScaleUpdate(const Input& input)
{
	_scaleTrack->Update();
	if (_scaleTrack->GetEnd())
	{
		if (_scaleTrack->GetReverse())
		{
			End();
		}
		else
		{
			_updater = &Experience::LevelUpUpdate;
		}
	}
}

void Experience::LevelUpUpdate(const Input& input)
{
	_levlAnimTrack->Update();
	if (_levlAnimTrack->GetEnd())
	{
		_updater = &Experience::StatusUpdate;
	}
}

void Experience::StatusUpdate(const Input& input)
{
	_itemAnimCnt++;
	if (input.GetButtonDown(0, "ok"))
	{
		_itemAnimCnt = ITEM_ANIM_MAX;
	}

	if (_itemAnimCnt > ITEM_ANIM_MAX)
	{
		// 終了
		if (NextDrawItem())
		{
			Wait(&Experience::ScaleUpdate);
			_scaleTrack->Reset();
			_scaleTrack->SetReverse(true);
			return;
		}
	}
}

bool Experience::NextDrawItem()
{
	_drawIdx = GetNextDrawIdx();

	if (_drawIdx >= _drawDatas.size())
	{
		return true;
	}
	else
	{
		_itemAnimCnt = 0;
	}
	return false;
}

int Experience::GetNextDrawIdx()
{
	for (int i = _drawIdx+1; i < _drawDatas.size(); i++)
	{
		if (_drawDatas[i].add > 0)
		{
			return i;
		}
	}
}

void Experience::End()
{
	auto status = _battleChar.GetCharacotr().GetStartStatus();
	if (status.exp + _addExp >= _maxExp)
	{
		status.AddStatus(_addStatus);
		_battleChar.GetCharacotr().SetStatus(status);
	}
	_battleChar.GetCharacotr().AddExp(_addExp, _maxExp);

	assert(_uiDeque != nullptr);
	_uiDeque->pop_front();
}

void Experience::Wait(void(Experience::* nextUpdate)(const Input&))
{
	_waitCnt = 0;
	_nextUpdater = nextUpdate;

	_updater = &Experience::WaitUpdate;
}

void Experience::ExpBerDraw()
{
	auto screenCenter = Application::Instance().GetWindowSize().ToVector2Int() * 0.5f;
	Vector2Int space(20, 20);
	Size berSize = Size(300, 50);
	Rect expBer = Rect(screenCenter, berSize);
	Rect base = Rect(screenCenter, expBer.size + (space * 2).ToSize());

	base.Draw(0x000000);
	expBer.Draw(0x888888);

	int berWidth = static_cast<int>(berSize.w * (_currentExp % _maxExp) / static_cast<float>(_maxExp));
	DrawBox(expBer.Left(), expBer.Top(), expBer.Left() + berWidth, expBer.Botton(), 0x00ff00, true);
}

void Experience::LevelUpDraw()
{
	int drawDataCnt = static_cast<int>(_drawDatas.size());
	if (_drawIdx < 0)return;

	auto screenCenter = Application::Instance().GetWindowSize().ToVector2Int() * 0.5f;
	auto status = _battleChar.GetCharacotr().GetStatus();
	auto levelUpCenter = Vector2Int(screenCenter.x, 120);

	DrawLevel(levelUpCenter, status);

	DrawToStatusWindow();

	DrawRotaGraph(screenCenter, _scaleTrack->GetValue(), 0.0f, _windowStatusH, false);
}

void Experience::DrawLevel(const Vector2Int& levelUpCenter, const Status& status)
{
	auto& fileSystem = Application::Instance().GetFileSystem();
	DrawRotaGraph(levelUpCenter + Vector2Int(0, -40), 1.0f, 0.0f, fileSystem.GetImageHandle("Resource/Image/UI/levelUp.png"), true);

	float animVel = _levlAnimTrack->GetValue();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(animVel * 255));

	auto fontH = Application::Instance().GetFileSystem().GetFontHandle("choplin50edge");
	uint8_t level = _battleChar.GetCharacotr().GetStartStatus().level + _addStatus.level;
	DrawStringToHandle(levelUpCenter, Anker::center, 0xffff00, Lerp(5.0f, 1.0f, animVel), fontH, "%d", level);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void Experience::DrawToStatusWindow()
{
	int drawDataCnt = static_cast<int>(_drawDatas.size());
	auto fontH = Application::Instance().GetFileSystem().GetFontHandle("choplin30edge");

	int currentScreen = GetDrawScreen();
	SetDrawScreen(_windowStatusH);
	auto itemSize = Size(STATUS_ITEM_SIZE_W, STATUS_ITEM_SIZE_H);
	auto statusDrawRectSize = Size(itemSize.w * STATUS_ROW_CNT, (drawDataCnt + 1) / STATUS_ROW_CNT * itemSize.h);
	auto statusDrawRect = Rect(statusDrawRectSize.ToVector2Int() * 0.5f, statusDrawRectSize);
	statusDrawRect.DrawGraph(Application::Instance().GetFileSystem().GetImageHandle("Resource/Image/UI/statusWindow2.png"));

	Rect itemRect = Rect(Vector2Int(0, 0), itemSize);
	for (int idx = 0; idx < drawDataCnt; idx++)
	{
		if (idx % (drawDataCnt / STATUS_ROW_CNT) == 0)
		{
			itemRect.center = Vector2Int(statusDrawRect.Left() + itemSize.w * (idx / (drawDataCnt / STATUS_ROW_CNT) + 0.5f),
				statusDrawRect.Top() + itemSize.h / 2);
		}
		int space = 20;
		DrawStringToHandle(Vector2Int(itemRect.Left() + space, itemRect.center.y), Anker::leftcenter, 0xffffff, fontH, _drawDatas[idx].name.c_str());
		DrawStringToHandle(itemRect.center, Anker::center, 0xffffff, fontH, "%d", _drawDatas[idx].current);
		if (_drawIdx > idx && _drawDatas[idx].add != 0)
		{
			DrawStringToHandle(Vector2Int(itemRect.Right() - space, itemRect.center.y), Anker::rightcenter, 0x88ff00, fontH, "+%d", _drawDatas[idx].add);
		}
		itemRect.center.y += itemSize.h;
	}
	SetDrawScreen(currentScreen);
}

Experience::Experience(BattleCharactor& battleChar, const bool kill, std::deque<std::shared_ptr<UI>>* uiDeque)
	: _battleChar(battleChar), UI(uiDeque), _maxExp(100)
{
	auto battleStatus = _battleChar.GetCharacotr().GetBattleStatus();
	auto targetStatus = _battleChar.GetTargetBattleCharactor()->GetCharacotr().GetStartStatus();
	auto dataBase = Application::Instance().GetDataBase();
	
	// ダメージ割合
	const float damageRate = battleChar.GetGivenDamage() / static_cast<float>(targetStatus.health);
	// レベル差
	const float levelDifference = 1.0f + (targetStatus.level - battleStatus.status.level) * 0.01f;
	_addExp = static_cast<uint8_t>( battleStatus.CheckHeal() ? 20 : min(100.0f, max(0.0f, 40 * damageRate * levelDifference)) );
	if (kill)
	{
		_addExp *= 2;
	}

	// DEBUG-------------------------------------------------
	_addExp = 100;
	//--------------------------------------------------------------

	_currentExp = battleStatus.status.exp;
	_currentAddExp = _addExp;

	_updater = &Experience::ExpBerUpdate;
	_drawer = &Experience::ExpBerDraw;

	_drawDatas[static_cast<size_t>(Item::health)].name			= "HP";
	_drawDatas[static_cast<size_t>(Item::power)].name			= "力";
	_drawDatas[static_cast<size_t>(Item::magic_power)].name		= "魔力";
	_drawDatas[static_cast<size_t>(Item::skill)].name			= "技";
	_drawDatas[static_cast<size_t>(Item::speed)].name			= "速さ";
	_drawDatas[static_cast<size_t>(Item::defence)].name			= "守備";
	_drawDatas[static_cast<size_t>(Item::magic_defence)].name	= "魔防";
	_drawDatas[static_cast<size_t>(Item::luck)].name			= "幸運";

	_expAnimCnt = 0;
	_itemAnimCnt = 0;
	_waitCnt = 0;

	_scaleTrack = make_unique<Track<float>>();
	_scaleTrack->AddKey(0, 0.0f);
	_scaleTrack->AddKey(20, 1.0f);

	_levlAnimTrack = make_unique<Track<float>>();
	_levlAnimTrack->AddKey(0, 0.0f);
	_levlAnimTrack->AddKey(60, 1.0f);
}

Experience::~Experience()
{
}

void Experience::Update(const Input& input)
{
	(this->*_updater)(input);
}

void Experience::Draw()
{
	(this->*_drawer)();
}
