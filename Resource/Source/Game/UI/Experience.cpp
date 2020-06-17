#include "Experience.h"
#include "Application.h"
#include "Geometry.h"
#include <dxlib.h>
#include "Charactor.h"
#include "BattleCharactor.h"
#include "DataBase.h"
#include "DxLibUtility.h"
#include "FileSystem.h"
#include <algorithm>

using namespace std;

void Experience::AddExpUpdate(const Input& input)
{
	_expAnimTrack->Update();
	if (!_expAnimTrack->GetEnd())return;

	_expAnimTrack->Reset();

	_currentAddExp--;
	_currentExp++;
	// レベルアップ
	if (_currentExp >= _maxExp)
	{
		auto& charactor = _battleChar.GetCharacotr();
		charactor.AddExp(_addExp, _maxExp);
		_addStatus = charactor.GetLevelUpStatus();
		auto startStatus = charactor.GetStartStatus();

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

		_updater = &Experience::LevelUpUpdate;
		_itemAnimTrack->Reset();
		return;
	}
	// 終了
	if (_currentAddExp <= 0)
	{
		StartEndUpdate();
		return;
	}
}

void Experience::StartEndUpdate()
{
	_endCnt = 60;
	_updater = &Experience::EndUpdate;
}

void Experience::LevelUpUpdate(const Input& input)
{
	_itemAnimTrack->Update();
	if (_itemAnimTrack->GetEnd())
	{
		_drawIdx++;
		if (_drawIdx >= _drawDatas.size())
		{
			StartEndUpdate();
			return;
		}
		else
		{
			_itemAnimTrack->Reset();
		}
	}
}

void Experience::EndUpdate(const Input& input)
{
	if (--_endCnt > 0) return;

	_battleChar.GetCharacotr().AddExp(_addExp, _maxExp);
	_uiDeque.pop_front();
}

Experience::Experience(BattleCharactor& battleChar, const bool kill, std::deque<std::shared_ptr<UI>>& uiDeque)
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
	_currentExp = battleStatus.status.exp;
	_currentAddExp = _addExp;

	_updater = &Experience::AddExpUpdate;

	_expAnimTrack = make_unique<Track<int>>();
	_expAnimTrack->AddKey(0, 0);
	_expAnimTrack->AddKey(5, 0);

	_itemAnimTrack = make_unique<Track<int>>();
	_itemAnimTrack->AddKey(0, 0);
	_itemAnimTrack->AddKey(30, 0);

	_drawIdx = -1;
	_drawDatas[static_cast<size_t>(Item::health)].name			= "HP";
	_drawDatas[static_cast<size_t>(Item::power)].name			= "力";
	_drawDatas[static_cast<size_t>(Item::magic_power)].name		= "魔力";
	_drawDatas[static_cast<size_t>(Item::skill)].name			= "技";
	_drawDatas[static_cast<size_t>(Item::speed)].name			= "速さ";
	_drawDatas[static_cast<size_t>(Item::defence)].name			= "守備";
	_drawDatas[static_cast<size_t>(Item::magic_defence)].name	= "魔防";
	_drawDatas[static_cast<size_t>(Item::luck)].name			= "幸運";
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
	Vector2Int space(20,20);
	auto screenCenter = Application::Instance().GetWindowSize().ToVector2Int() * 0.5f;
	Size berSize = Size(300, 50);
	Rect expBer = Rect(screenCenter, berSize);
	Rect base = Rect(screenCenter, expBer.size + (space*2).ToSize());

	base.Draw(0x000000);
	expBer.Draw(0x888888);
	
	DrawBox(expBer.Left(), expBer.Top(), expBer.Left() + berSize.w * (_currentExp%_maxExp) / static_cast<float>(_maxExp), expBer.Botton(), 0x00ff00, true);

	if (_updater == &Experience::LevelUpUpdate)
	{
		if (_drawIdx < 0 || _drawIdx >= _drawDatas.size())return;
		auto status = _battleChar.GetCharacotr().GetStatus();
		auto levelUpCenter = Vector2Int(screenCenter.x, 100);
		DrawCircle(levelUpCenter, 30, 0x0000ff);
		auto fontH = Application::Instance().GetFileSystem().GetFontHandle("choplin40edge");
		char str[10];
		sprintf_s(str, 10, "%d", static_cast<int>(status.level + _addStatus.level));
		DrawStringToHandle(levelUpCenter, Anker::center, 0xffffff, fontH, str);

		auto itemSize = Size(400, 50);
		auto statusDrawRect = Rect(screenCenter, Size(itemSize.w, _drawDatas.size() * itemSize.h));
		statusDrawRect.Draw(0xffffff);

		Rect itemRect(Vector2Int(screenCenter.x, statusDrawRect.Top() + itemSize.h / 2), itemSize);
		for (int idx = 0; idx < _drawDatas.size(); idx++)
		{
			int space = 20;
			DrawStringToHandle(Vector2Int(itemRect.Left() + space, itemRect.center.y), Anker::leftcenter, 0xffffff, fontH, _drawDatas[idx].name.c_str());
			DrawStringToHandle(itemRect.center, Anker::center, 0xffffff, fontH, "%d", _drawDatas[idx].current);
			if (_drawIdx >= idx && _drawDatas[idx].add != 0)
			{
				DrawStringToHandle(Vector2Int(itemRect.Right() - space, itemRect.center.y), Anker::rightcenter, 0x88ff00, fontH, "+%d", _drawDatas[idx].add);
			}
			itemRect.center.y += itemSize.h;
		}

		statusDrawRect.Draw(0x0000ff, false);
	}
}
