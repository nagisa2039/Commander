#include "MoveMenu.h"
#include "Application.h"
#include "FileSystem.h"
#include "PlayerCommander.h"
#include "Charactor.h"
#include "Input.h"

void MoveMenu::Decision()
{
	if (_isBattle)
	{
		_isBattle = false;
		_playerCommander.BattleStart();
		Close(false);
		return;
	}

	Menu::Decision();
}

void MoveMenu::Back()
{
	if (_isBattle)
	{
		_playerCommander.BackBattalePrediction();
		_isBattle = false;
		return;
	}

	_playerCommander.MoveCancel();
	Close();
}

void MoveMenu::OpenUpdate(const Input& input)
{
	if (_isBattle)
	{
		if (input.GetButtonDown("ok"))
		{
			Decision();
			return;
		}
		if (input.GetButtonDown("back"))
		{
			Back();
			return;
		}
		if (input.GetButtonDown("right"))
		{
			if (_attackPosListItr == _attackPosList.end())return;
			_attackPosListItr++;
			if (_attackPosListItr == _attackPosList.end())
			{
				_attackPosListItr = _attackPosList.begin();
			}
			_playerCommander.BackBattalePrediction();
			_playerCommander.AttackPrePos(*_attackPosListItr);
		}
		if (input.GetButtonDown("left"))
		{
			if (_attackPosListItr == _attackPosList.begin())
			{
				_attackPosListItr = _attackPosList.end();
			}
			_attackPosListItr--;
			_playerCommander.BackBattalePrediction();
			_playerCommander.AttackPrePos(*_attackPosListItr);
		}
		return;
	}
	Menu::OpenUpdate(input);
}

MoveMenu::MoveMenu(std::deque<std::shared_ptr<UI>>* uiDeque, PlayerCommander& playerCom, const MapCtrl& mapCtrl)
	:Menu(uiDeque, playerCom, mapCtrl)
{
	auto menuFrameH = ImageHandle("Resource/Image/UI/menuFrame.png");
	Init(static_cast<size_t>(Content::max), menuFrameH);

	_contentInfs[static_cast<size_t>(Content::battle)].name = "�퓬";
	_contentInfs[static_cast<size_t>(Content::wait)].name = "�ҋ@";
	_contentInfs[static_cast<size_t>(Content::close)].name = "����";

	_contentInfs[static_cast<size_t>(Content::battle)].func = [&]()
	{
		_isBattle = true;
		assert(_attackPosList.size() > 0);
		_attackPosListItr = _attackPosList.begin();
		_playerCommander.AttackPrePos(*_attackPosListItr);
	};
	_contentInfs[static_cast<size_t>(Content::wait)].func = [&]()
	{
		auto selectChar = _playerCommander.GetSelectCharactor();
		if (selectChar == nullptr)return;

		selectChar->MoveEnd(false, false);
		_playerCommander.SelectCharactor(nullptr, true);
		Close();
	};
	_contentInfs[static_cast<size_t>(Content::close)].func = [&]()
	{
		_playerCommander.MoveCancel();
		Close();
	};

	_contentList.clear();
	_contentList.reserve(static_cast<size_t>(Content::max));

	_isBattle = false;
}

MoveMenu::~MoveMenu()
{
}

void MoveMenu::SetContent(const std::list<Vector2Int>& attackPosList)
{
	_attackPosList = attackPosList;
	_attackPosListItr = _attackPosList.end();

	_contentList.clear();
	for (int idx = 0; idx != static_cast<size_t>(Content::max); idx++)
	{
		if (idx == static_cast<size_t>(Content::battle) && _attackPosList.size() <= 0)
		{
			continue;
		}
		_contentList.emplace_back(idx);
	}

	auto selectChar = _playerCommander.GetSelectCharactor();
	if (selectChar == nullptr)return;
	_contentInfs[static_cast<size_t>(Content::battle)].name = selectChar->GetBattleStatus().CheckHeal() ? "��" : "�퓬";
}

void MoveMenu::Draw()
{
	if (_isBattle)return;
	Menu::Draw();
}
