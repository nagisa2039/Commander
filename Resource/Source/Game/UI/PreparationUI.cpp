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
#include "../Scene/PlayScene.h"

using namespace std;

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

	if(_execution)
	{
		Open(true);
		_execution = false;
		_playScene.CharactorDataUpdate();
	}
}

void PreparationUI::OpenUpdate(const Input& input)
{
	if (_backMapSelect)return;

	auto select = [this](const Item item)
	{
		_selectItem = item;
		_selectExRateTrack->Reset();
	};

	if(input.GetAnyMouseInput() || input.GetMouseMove() != Vector2Int(0,0))
	{
		auto mouseRect = Rect(input.GetMousePos(), Size(1,1));
		auto click = input.GetButtonDown(0, "mouseLeft");
		for (int idx = 0; idx < _itemInfTable.size(); ++idx)
		{
			if (Rect(_itemInfTable[idx].pos, _itemSize).IsHit(mouseRect))
			{
				select(static_cast<Item>(idx));
				if (click)
				{
					Close(true);
				}
				break;
			}
		}
		if (input.GetButtonDown(0, "mouseRight"))
		{
			_itemInfTable[static_cast<size_t>(Item::back)].func();
		}
		return;
	}

	if (input.GetButtonDown(0, "ok") || input.GetButtonDown(1, "ok"))
	{
		Close(true);
		return;
	}

	_selectExRateTrack->Update();
	if (input.GetButtonDown(0, "up") || input.GetButtonDown(1, "up"))
	{
		if (_selectItem > static_cast<Item>(0))
		{
			select(static_cast<Item>(static_cast<int>(_selectItem) - 1));
		}
	}
	if (input.GetButtonDown(0, "down") || input.GetButtonDown(1, "down"))
	{
		if (_selectItem < static_cast<Item>(static_cast<int>(Item::max) - 1))
		{
			select(static_cast<Item>(static_cast<int>(_selectItem) + 1));
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

PreparationUI::PreparationUI(std::deque<std::shared_ptr<UI>>* uiDeque, Camera& camera, MapCtrl& mapCtrl, PlayScene& playScene)
	:UI(uiDeque), _mapCtrl(mapCtrl), _camera(camera), _playScene(playScene)
{
	assert(_uiDeque != nullptr);
	_updater = &PreparationUI::CloseUpdate;
	auto screenCenter = Application::Instance().GetWindowSize().ToVector2Int() * 0.5f;

	_itemInfTable[static_cast<size_t>(Item::start)].name		= "戦闘開始";
	_itemInfTable[static_cast<size_t>(Item::placement)].name	= "マップ・配置";
	_itemInfTable[static_cast<size_t>(Item::warsituation)].name = "戦況確認";
	//_itemInfTable[static_cast<size_t>(Item::shop)].name			= "購入";
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
		_uiDeque->emplace_front(make_shared<WarSituation>(_uiDeque, _mapCtrl));
	};
	/*_itemInfTable[static_cast<size_t>(Item::shop)].func = [&]()
	{
		_playScene.PushShopScene();
	};*/
	_itemInfTable[static_cast<size_t>(Item::back)].func = [&]()
	{
		_uiDeque->emplace_front(make_shared<CheckWindow>("退却しますか？", _uiDeque, [&](){BackMapSelect();}));
	};

	const int spaceY = 120;
	int currentScreen = GetDrawScreen();
	Vector2Int currentDrawPos = screenCenter - Vector2Int(0, static_cast<int>((static_cast<int>(Item::max)-1) / 2.0f * spaceY));
	int windowH = Application::Instance().GetFileSystem().GetImageHandle("Resource/Image/UI/window0.png");
	GetGraphSize(windowH, _itemSize);
	int fontH = Application::Instance().GetFileSystem().GetFontHandle("choplin40edge");
	for (auto& itemInf : _itemInfTable)
	{
		itemInf.pos = currentDrawPos;
		currentDrawPos.y += spaceY;
		itemInf.graphH = MakeScreen(_itemSize.w, _itemSize.h, true);
		SetDrawScreen(itemInf.graphH);
		ClsDrawScreen();
		DrawGraph(0,0, windowH, true);
		DrawStringToHandle(_itemSize.ToVector2Int()*0.5f, Anker::center, 0xffffff, fontH, itemInf.name.c_str());
	}
	SetDrawScreen(currentScreen);

	_animTrack = make_unique<Track<float>>();
	_animTrack->AddKey(0, 0.0f);
	_animTrack->AddKey(15, 1.0f);

	_battlePreparationCursor = make_unique<BattlePreparationCursor>(_mapCtrl, _camera);
	_battlePreparationCursor->SetCursorStartPosition();

	_selectItem = Item::start;
	_execution = false;
	_backMapSelect = false;

	_selectExRateTrack = make_unique<Track<float>>(true);
	_selectExRateTrack->AddKey(0, 0.9);
	_selectExRateTrack->AddKey(15, 1.0f);
	_selectExRateTrack->AddKey(30, 0.9);
}

PreparationUI::~PreparationUI()
{
	for (auto& item : _itemInfTable)
	{
		DeleteGraph(item.graphH);
		item.graphH = -1;
	}
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
	for (int idx = 0; idx < _itemInfTable.size(); idx++)
	{
		auto exRate = idx == static_cast<int>(_selectItem) ? _selectExRateTrack->GetValue() : 0.9f;
		DrawRotaGraph(_itemInfTable[idx].pos, exRate, 0.0f, _itemInfTable[idx].graphH, true);
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
	_execution = true;
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

void PreparationUI::BackMapSelect()
{
	_backMapSelect = true;
}

bool PreparationUI::GetBackMapSelect()
{
	return _backMapSelect;
}
