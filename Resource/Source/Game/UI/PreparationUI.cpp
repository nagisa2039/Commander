#include "PreparationUI.h"
#include "Input.h"
#include "Application.h"
#include "FileSystem.h"
#include "DxLibUtility.h"
#include "MapChip.h"
#include "Camera.h"
#include "MapCtrl.h"
#include "../BattlePreparationCursor.h"
#include "WarSituation.h"
#include "CheckWindow.h"
#include "DxLib.h"

using namespace std;

int PreparationUI::_itemScreenH = -1;

void PreparationUI::CloseUpdate(const Input& input)
{
	if (_execution && _selectItem == Item::placement)
	{
		_battlePreparationCursor->Update(input);
		if (_battlePreparationCursor->GetEnd())
		{
			Open(true);
			_execution = false;
		}
		return;
	}

	if(_execution);
	{
		Open(true);
		_execution = false;
	}
}

void PreparationUI::OpenUpdate(const Input& input)
{
	if (_backMapSelect)return;

	if (input.GetButtonDown(0, "ok") || input.GetButtonDown(1, "ok"))
	{
		Close(true);
		_execution = true;
		return;
	}

	_selectExRateTrack->Update();
	if (input.GetButtonDown(0, "up") || input.GetButtonDown(1, "up"))
	{
		if (_selectItem > static_cast<Item>(0))
		{
			_selectItem = static_cast<Item>(static_cast<int>(_selectItem) - 1);
			_selectExRateTrack->Reset();
		}
	}
	if (input.GetButtonDown(0, "down") || input.GetButtonDown(1, "down"))
	{
		if (_selectItem < static_cast<Item>(static_cast<int>(Item::max) - 1))
		{
			_selectItem = static_cast<Item>(static_cast<int>(_selectItem) + 1);
			_selectExRateTrack->Reset();
		}
	}
	if (input.GetButtonDown(0, "back") || input.GetButtonDown(1, "back"))
	{
		_itemInfTable[static_cast<size_t>(Item::back)].func();
	}
}

void PreparationUI::CloseAnimUpdate(const Input& input)
{
	_animTrack->Update();
	if (_animTrack->GetEnd())
	{
		if (_execution)
		{
			_itemInfTable[static_cast<size_t>(_selectItem)].func();
		}
		_updater = &PreparationUI::CloseUpdate;
		_isOpen = false;
	}
}

void PreparationUI::OpenAnimUpdate(const Input& input)
{
	_animTrack->Update();
	if (_animTrack->GetEnd())
	{
		_updater = &PreparationUI::OpenUpdate;
		_isOpen = true;
	}
}

PreparationUI::PreparationUI(std::deque<std::shared_ptr<UI>>& uiDeque, Camera& camera, MapCtrl& mapCtrl)
	:UI(uiDeque), _mapCtrl(mapCtrl), _camera(camera)
{
	_updater = &PreparationUI::CloseUpdate;
	auto screenCenter = Application::Instance().GetWindowSize().ToVector2Int() * 0.5f;

	int spaceY = 150;
	Vector2Int currentDrawPos = screenCenter - Vector2Int(0, (static_cast<int>(Item::max)-1) / 2.0f * spaceY);
	_itemInfTable[static_cast<size_t>(Item::start)].name		= "戦闘開始";
	_itemInfTable[static_cast<size_t>(Item::placement)].name	= "マップ・配置";
	_itemInfTable[static_cast<size_t>(Item::warsituation)].name = "戦況確認";
	_itemInfTable[static_cast<size_t>(Item::back)].name			= "退却";

	_itemInfTable[static_cast<size_t>(Item::start)].func = [&]()
	{
		_delete = true;
	}; 
	_itemInfTable[static_cast<size_t>(Item::placement)].func = [&]()
	{
		_battlePreparationCursor->Start();
	}; 
	_itemInfTable[static_cast<size_t>(Item::warsituation)].func = [&]()
	{
		_uiDeque.emplace_front(make_shared<WarSituation>(_uiDeque, _mapCtrl));
	};
	_itemInfTable[static_cast<size_t>(Item::back)].func = [&]()
	{
		_uiDeque.emplace_front(make_shared<CheckWindow>("退却しますか？", _uiDeque, [&](){SetBackMapSelect(true);}));
	};

	for (auto& itemInf : _itemInfTable)
	{
		itemInf.pos = currentDrawPos;
		currentDrawPos.y += spaceY;
	}

	_animTrack = make_unique<Track<float>>();
	_animTrack->AddKey(0, 0.0f);
	_animTrack->AddKey(15, 1.0f);

	_battlePreparationCursor = make_unique<BattlePreparationCursor>(_mapCtrl, _camera);
	_battlePreparationCursor->SetCursorStartPosition();

	_selectItem = Item::start;
	_execution = false;
	_backMapSelect = false;

	_selectExRateTrack = make_unique<Track<float>>(true);
	_selectExRateTrack->AddKey(0, 1.0f);
	_selectExRateTrack->AddKey(30, 1.2f);
	_selectExRateTrack->AddKey(60, 1.0f);

	if (_itemScreenH == -1)
	{
		int windowH = Application::Instance().GetFileSystem().GetImageHandle("Resource/Image/UI/window0.png");
		Size windowSize;
		GetGraphSize(windowH, windowSize);
		_itemScreenH = MakeScreen(windowSize.w, windowSize.h, true);
	}
}

PreparationUI::~PreparationUI()
{
}

void PreparationUI::Update(const Input& input)
{
	(this->*_updater)(input);
}

void PreparationUI::Draw()
{
	if (_execution && _selectItem == Item::placement)
	{
		_battlePreparationCursor->Draw();
		return;
	}
	auto fileSystem = Application::Instance().GetFileSystem();
	int fontH = fileSystem.GetFontHandle("choplin30edge");
	int windowH = fileSystem.GetImageHandle("Resource/Image/UI/window0.png");
	Size graphSize;
	auto wsize = Application::Instance().GetWindowSize();
	GetGraphSize(windowH, graphSize);
	for (int idx = 0; idx < _itemInfTable.size(); idx++)
	{
		int currentDrawScreen = GetDrawScreen();
		SetDrawScreen(_itemScreenH);
		ClsDrawScreen();
		DrawGraph(0,0, windowH, true);
		DrawStringToHandle(graphSize.ToVector2Int() * 0.5f, Anker::center, 0xffffff, fontH, _itemInfTable[idx].name.c_str());

		auto centerPos = Lerp(Vector2Int(wsize.w + graphSize.w / 2, _itemInfTable[idx].pos.y), _itemInfTable[idx].pos, _animTrack->GetValue());
		SetDrawScreen(currentDrawScreen);
		DrawRotaGraph(centerPos, idx == static_cast<int>(_selectItem) ? _selectExRateTrack->GetValue() : 1.0f, 0.0f, _itemScreenH, true);
	}
}

void PreparationUI::BeginDraw()
{
	_battlePreparationCursor->DrawMovableMass();
	_battlePreparationCursor->DrawSortieMass();
}

void PreparationUI::Open(const bool animation)
{
	if (_updater == &PreparationUI::OpenAnimUpdate || _updater == &PreparationUI::OpenUpdate)
	{
		return;
	}
	_animTrack->SetReverse(false);
	if (animation)
	{
		_updater = &PreparationUI::OpenAnimUpdate;
		_animTrack->Reset();
	}
	else
	{
		_updater = &PreparationUI::OpenUpdate;
		_animTrack->End();
	}
}

void PreparationUI::Close(const bool animation)
{
	if (_updater == &PreparationUI::CloseAnimUpdate || _updater == &PreparationUI::CloseUpdate)
	{
		return;
	}
	_animTrack->SetReverse(true);
	if (animation)
	{
		_updater = &PreparationUI::CloseAnimUpdate;
		_animTrack->Reset();
	}
	else
	{
		_updater = &PreparationUI::CloseUpdate;
		_animTrack->End();
	}
	_updater = &PreparationUI::CloseAnimUpdate;
}

void PreparationUI::SetBackMapSelect(const bool backMapSelect)
{
	_backMapSelect = backMapSelect;
}

bool PreparationUI::GetBackMapSelect()
{
	return _backMapSelect;
}
