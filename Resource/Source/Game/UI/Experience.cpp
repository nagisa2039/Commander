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

using namespace std;

void Experience::AddExpUpdate(const Input& input)
{
	bool skip = false;
	if (input.GetButtonDown(0, "ok"))
	{
		_itemAnimTrack->End();
		skip = true;
	}

	_expAnimTrack->Update();
	if (!_expAnimTrack->GetEnd() && !skip)return;

	if (skip)
	{
		_currentExp += _currentAddExp;
	}
	else
	{
		_expAnimTrack->Reset();
		_currentAddExp--;
		_currentExp++;
	}

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
		_drawer = &Experience::LevelUpDraw;

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
	_drawer = &Experience::ExpBerDraw;
}

void Experience::LevelUpUpdate(const Input& input)
{
	_itemAnimTrack->Update();
	if (input.GetButtonDown(0, "ok"))
	{
		_itemAnimTrack->End();
	}

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
	if (_drawIdx < 0 || _drawIdx >= _drawDatas.size())return;

	auto screenCenter = Application::Instance().GetWindowSize().ToVector2Int() * 0.5f;
	auto status = _battleChar.GetCharacotr().GetStatus();
	auto levelUpCenter = Vector2Int(screenCenter.x, 120);

	auto& fileSystem = Application::Instance().GetFileSystem();
	DrawRotaGraph(levelUpCenter + Vector2Int(0, -45), 1.0f, 0.0f, fileSystem.GetImageHandle("Resource/Image/UI/levelUp.png"), true);
	auto fontH = Application::Instance().GetFileSystem().GetFontHandle("choplin40edge");
	char str[10];
	sprintf_s(str, 10, "%d", static_cast<int>(status.level + _addStatus.level));
	DrawStringToHandle(levelUpCenter, Anker::center, 0xffffff, fontH, str);

	const int rowCnt = 2;
	auto itemSize = Size(400, 50);
	auto statusDrawRect = Rect(screenCenter, Size(itemSize.w* rowCnt, static_cast<int>(_drawDatas.size())* itemSize.h));
	statusDrawRect.Draw(0xffffff);

	Rect itemRect = Rect(Vector2Int(statusDrawRect.Left() + itemSize.w / 2, statusDrawRect.Top() + itemSize.h / 2), itemSize);
	for (int idx = 0; idx < _drawDatas.size(); idx++)
	{
		if (idx == (_drawDatas.size() + rowCnt - 1) / rowCnt)
		{
			itemRect.center = Vector2Int(statusDrawRect.Left() + itemSize.w * (idx / (_drawDatas.size() / rowCnt) + 1.5f),
				statusDrawRect.Top() + itemSize.h / 2);
		}
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

	// DEBUG-------------------------------------------------
	_addExp = 100;
	//--------------------------------------------------------------

	_currentExp = battleStatus.status.exp;
	_currentAddExp = _addExp;

	_updater = &Experience::AddExpUpdate;
	_drawer = &Experience::ExpBerDraw;

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
	(this->*_drawer)();
}
