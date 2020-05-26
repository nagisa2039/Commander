#include "PlayerMenu.h"
#include "Application.h"
#include "FileSystem.h"
#include "CheckWindow.h"
#include "PlayerCommander.h"
#include "WarSituation.h"

PlayerMenu::PlayerMenu(std::deque<std::shared_ptr<UI>>& uiDeque, PlayerCommander& playerCom, const MapCtrl& mapCtrl)
	:Menu(uiDeque, playerCom, mapCtrl)
{
	auto menuFrameH = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/menuFrame.png");
	Init(static_cast<size_t>(Content::max), menuFrameH);

	_contentInfs[static_cast<size_t>(Content::situation)].name = "戦況";
	_contentInfs[static_cast<size_t>(Content::retreat)].name = "退却";
	_contentInfs[static_cast<size_t>(Content::turnEnd)].name = "ターン終了";

	_contentInfs[static_cast<size_t>(Content::situation)].func = [&]()
	{
		_uiDeque.emplace_front(make_shared<WarSituation>(_uiDeque, _mapCtrl));
	};
	_contentInfs[static_cast<size_t>(Content::retreat)].func = [&]()
	{
		_uiDeque.emplace_front(make_shared<CheckWindow>("退却しますか？", _uiDeque, [&]() {_playerCommander.End(); }));
	};
	_contentInfs[static_cast<size_t>(Content::turnEnd)].func = [&]()
	{
		_uiDeque.emplace_front(make_shared<CheckWindow>("ターンを終了しますか？", _uiDeque, [&]() {_playerCommander.End(); }));
	};

	for (int idx = 0; idx != static_cast<size_t>(Content::max); idx++)
	{
		_contentList.emplace_back(idx);
	}
}

PlayerMenu::~PlayerMenu()
{
}
