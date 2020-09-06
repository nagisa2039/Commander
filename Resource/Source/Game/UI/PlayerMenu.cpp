#include "PlayerMenu.h"
#include "Application.h"
#include "FileSystem.h"
#include "CheckWindow.h"
#include "PlayerCommander.h"
#include "WarSituation.h"
#include "MenuTopInf.h"

PlayerMenu::PlayerMenu(std::deque<std::shared_ptr<UI>>* uiDeque, PlayerCommander& playerCom, const MapCtrl& mapCtrl, const unsigned char& turnCnt)
	:Menu(uiDeque, playerCom, mapCtrl)
{
	assert(_uiDeque != nullptr);

	auto menuFrameH = ImageHandle("Resource/Image/UI/menuFrame.png");
	Init(static_cast<size_t>(Content::max), menuFrameH);

	_contentInfs[static_cast<size_t>(Content::situation)].name = "戦況";
	_contentInfs[static_cast<size_t>(Content::retreat)].name = "退却";
	_contentInfs[static_cast<size_t>(Content::turnEnd)].name = "ターン終了";

	
	_contentInfs[static_cast<size_t>(Content::situation)].func = [this] ()
	{
		_uiDeque->emplace_front(make_shared<WarSituation>(_uiDeque, _mapCtrl));
	};
	_contentInfs[static_cast<size_t>(Content::retreat)].func = [this]()
	{
		_uiDeque->emplace_front(make_shared<CheckWindow>("退却しますか？", _uiDeque, [&]() {_playerCommander.SetBackMapSelect(true); }));
	};
	_contentInfs[static_cast<size_t>(Content::turnEnd)].func = [this]()
	{
		_uiDeque->emplace_front(make_shared<CheckWindow>("ターンを終了しますか？", _uiDeque, [&]() {_playerCommander.End(); }));
	};

	for (int idx = 0; idx != static_cast<size_t>(Content::max); idx++)
	{
		_contentList.emplace_back(idx);
	}

	_menuTopDeque.clear();
	_menuTop = make_shared<MenuTopInf>(_mapCtrl, turnCnt, &_menuTopDeque);
	_menuTopDeque.emplace_back(_menuTop);
}

PlayerMenu::~PlayerMenu()
{
}

void PlayerMenu::Update(const Input& input)
{
	Menu::Update(input);
	if (_isOpen)
	{
		_menuTop->Open();
	}
	else
	{
		_menuTop->Close();
	}
	if (_menuTopDeque.size() > 0)
	{
		(*_menuTopDeque.begin())->Update(input);
	}
}

void PlayerMenu::Draw()
{
	Menu::Draw();
	for (auto rItr = _menuTopDeque.rbegin(); rItr != _menuTopDeque.rend(); rItr++)
	{
		(*rItr)->Draw();
	}
}
