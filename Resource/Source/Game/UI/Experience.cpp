#include "Experience.h"
#include "Application.h"
#include "Geometry.h"
#include <dxlib.h>
#include "Charactor.h"
#include "BattleCharactor.h"
#include "DataBase.h"
#include "DxLibUtility.h"
#include "FileSystem.h"

using namespace std;

void Experience::AddExpUpdate(const Input& input)
{
	_currentAddExp--;
	_currentExp++;
	// レベルアップ
	if (_currentExp >= _maxExp)
	{
		auto& charactor = _battleChar.GetCharacotr();
		charactor.AddExp(_addExp);
		_addStatus = charactor.GetLevelUpStatus();
		auto startStatus = charactor.GetStartStatus();

		char str[10];
		sprintf_s(str, 10, "%d", static_cast<int>(startStatus.health));
		_drawDatas[static_cast<size_t>(Item::health)].current = str;
		sprintf_s(str, 10, "%d", static_cast<int>(startStatus.power));
		_drawDatas[static_cast<size_t>(Item::power)].current = str;
		sprintf_s(str, 10, "%d", static_cast<int>(startStatus.skill));
		_drawDatas[static_cast<size_t>(Item::skill)].current = str;
		sprintf_s(str, 10, "%d", static_cast<int>(startStatus.speed));
		_drawDatas[static_cast<size_t>(Item::speed)].current = str;
		sprintf_s(str, 10, "%d", static_cast<int>(startStatus.defense));
		_drawDatas[static_cast<size_t>(Item::defence)].current = str;
		sprintf_s(str, 10, "%d", static_cast<int>(startStatus.magic_defense));
		_drawDatas[static_cast<size_t>(Item::magic_defence)].current = str;

		sprintf_s(str, 10, "+ %d", static_cast<int>(_addStatus.health));
		_drawDatas[static_cast<size_t>(Item::health)].add =	str;
		sprintf_s(str, 10, "+ %d", static_cast<int>(_addStatus.power));
		_drawDatas[static_cast<size_t>(Item::power)].add = str;
		sprintf_s(str, 10, "+ %d", static_cast<int>(_addStatus.skill));
		_drawDatas[static_cast<size_t>(Item::skill)].add = str;
		sprintf_s(str, 10, "+ %d", static_cast<int>(_addStatus.speed));
		_drawDatas[static_cast<size_t>(Item::speed)].add = str;
		sprintf_s(str, 10, "+ %d", static_cast<int>(_addStatus.defense));
		_drawDatas[static_cast<size_t>(Item::defence)].add = str;
		sprintf_s(str, 10, "+ %d", static_cast<int>(_addStatus.magic_defense));
		_drawDatas[static_cast<size_t>(Item::magic_defence)].add = str;

		_updater = &Experience::LevelUpUpdate;
		_itemAnimTrack->Reset();
		return;
	}
	// 終了
	if (_currentAddExp <= 0)
	{
		_battleChar.GetCharacotr().AddExp(_addExp);
		_uiDeque.pop_front();
	}
}

void Experience::LevelUpUpdate(const Input& input)
{
	_itemAnimTrack->Update();
	if (_itemAnimTrack->GetEnd())
	{
		_drawIdx++;
		if (_drawIdx >= _drawDatas.size())
		{
			_battleChar.GetCharacotr().AddStatus(_addStatus);
			_uiDeque.pop_front();
		}
		else
		{
			_itemAnimTrack->Reset();
		}
	}
}

Experience::Experience(BattleCharactor& battleChar, std::deque<std::shared_ptr<UI>>& uiDeque)
	: _battleChar(battleChar), UI(uiDeque)
{
	auto status = _battleChar.GetCharacotr().GetStatus();
	auto targetStatus = _battleChar.GetTargetBattleCharactor()->GetCharacotr().GetStartStatus();
	auto dataBase = Application::Instance().GetDataBase();
	auto expData = dataBase.GetExpData(status.level);
	auto targetExpData = dataBase.GetExpData(targetStatus.level);
	// 回復なら固定値で10point, レベルごとの基本値 * 攻撃なら与えたダメージ割合で算出
	_addExp = status.heal ? 10 : targetExpData.getPoint * (battleChar.GetGivenDamage() / static_cast<float>(targetStatus.health));
	_currentExp = status.exp;
	_currentAddExp = _addExp;
	_maxExp = expData.maxPoint;

	_updater = &Experience::AddExpUpdate;

	_itemAnimTrack = make_unique<Track<int>>();
	_itemAnimTrack->AddKey(0, 0);
	_itemAnimTrack->AddKey(30, 0);

	_drawIdx = -1;
	_drawDatas[static_cast<size_t>(Item::health)].name =		"HP";
	_drawDatas[static_cast<size_t>(Item::power)].name =			"力";
	_drawDatas[static_cast<size_t>(Item::skill)].name =			"技";
	_drawDatas[static_cast<size_t>(Item::speed)].name =			"速さ";
	_drawDatas[static_cast<size_t>(Item::defence)].name =		"守備";
	_drawDatas[static_cast<size_t>(Item::magic_defence)].name = "魔防";
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
	DrawBox(expBer.Left(), expBer.Top(), expBer.Left() + berSize.w * _currentExp / static_cast<float>(_maxExp), expBer.Botton(), 0x00ff00, true);

	if (_updater == &Experience::LevelUpUpdate)
	{
		if (_drawIdx < 0 || _drawIdx >= _drawDatas.size())return;
		auto status = _battleChar.GetCharacotr().GetStatus();
		auto levelUpCenter = Vector2Int(screenCenter.x, 100);
		DrawCircle(levelUpCenter, 30, 0x0000ff);
		auto fontH = Application::Instance().GetFileSystem()->GetFontHandle("choplin40edge");
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
			DrawStringToHandle(itemRect.center, Anker::center, 0xffffff, fontH, _drawDatas[idx].current.c_str());
			if (_drawIdx >= idx && _drawDatas[idx].add != "+ 0")
			{
				DrawStringToHandle(Vector2Int(itemRect.Right() - space, itemRect.center.y), Anker::rightcenter, 0x88ff00, fontH, _drawDatas[idx].add.c_str());
			}
			itemRect.center.y += itemSize.h;
		}

		statusDrawRect.Draw(0x0000ff, false);
	}
}
