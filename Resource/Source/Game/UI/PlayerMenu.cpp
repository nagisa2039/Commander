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

	_contentInfs[static_cast<size_t>(Content::situation)].name = "�틵";
	_contentInfs[static_cast<size_t>(Content::retreat)].name = "�ދp";
	_contentInfs[static_cast<size_t>(Content::turnEnd)].name = "�^�[���I��";

	_contentInfs[static_cast<size_t>(Content::situation)].func = [&]()
	{
		_uiDeque.emplace_front(make_shared<WarSituation>(_uiDeque, _mapCtrl));
	};
	_contentInfs[static_cast<size_t>(Content::retreat)].func = [&]()
	{
		_uiDeque.emplace_front(make_shared<CheckWindow>("�ދp���܂����H", _uiDeque, [&]() {_playerCommander.End(); }));
	};
	_contentInfs[static_cast<size_t>(Content::turnEnd)].func = [&]()
	{
		_uiDeque.emplace_front(make_shared<CheckWindow>("�^�[�����I�����܂����H", _uiDeque, [&]() {_playerCommander.End(); }));
	};

	for (int idx = 0; idx != static_cast<size_t>(Content::max); idx++)
	{
		_contentList.emplace_back(idx);
	}
}

PlayerMenu::~PlayerMenu()
{
}
