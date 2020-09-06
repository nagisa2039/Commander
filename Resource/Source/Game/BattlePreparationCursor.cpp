#include "BattlePreparationCursor.h"
#include "Input.h"
#include "Camera.h"
#include "MapCtrl.h"
#include "Map.h"
#include "Application.h"
#include "Charactor.h"
#include "FileSystem.h"
#include "DxLibUtility.h"
#include "UI/PreparationUI.h"
#include "UI/TerrainInf.h"
#include "UI/StatusInfomation.h"
#include "UI/StatusWindow/StatusWindow.h"

using namespace std;

namespace
{
	constexpr int NONE_SELECT = -1;
}

BattlePreparationCursor::BattlePreparationCursor(MapCtrl& mapCtrl, Camera& camera):MapCursor(mapCtrl, camera)
{
	_selectMapPos = Vector2Int(NONE_SELECT, NONE_SELECT);
	_end = false;

	_exRateTrack = std::make_unique<Track<float>>(true);
	_exRateTrack->AddKey(0, 0.8f);
	_exRateTrack->AddKey(30, 1.0f);
	_exRateTrack->AddKey(60, 0.8f);

	_terrainInf = make_shared<TerrainInf>(nullptr, _mapCtrl, _mapPos);
	_statusInf = make_shared<StatusInf>(nullptr, _mapCtrl, _mapPos);
	_statusWindow.reset();
}

BattlePreparationCursor::~BattlePreparationCursor()
{
}

void BattlePreparationCursor::Update(const Input& input)
{
	_exRateTrack->Update();
	if (_statusWindow)
	{
		_terrainInf->Close();
		_statusInf->Close();

		if (_statusWindow->GetDelete())
		{
			_statusWindow.reset();
		}
	}
	else
	{
		CursorMove(input);

		if (input.GetButtonDown("ok"))
		{
			Select();
		}

		if (input.GetButtonDown("back"))
		{
			_end = true;
		}

		if (input.GetButtonDown("status"))
		{
			auto charactor = _mapCtrl.GetMapPosChar(_mapPos);
			if (charactor == nullptr)return;
			_statusWindow = make_shared<StatusWindow>(nullptr, *charactor);
		}

		_terrainInf->Open();
		_statusInf->Open();
	}

	auto UIUpdate = [&input](std::shared_ptr<UI> ui)
	{
		if (!ui)return;
		ui->Update(input);
	};

	UIUpdate(_statusWindow);
	UIUpdate(_terrainInf);
	UIUpdate(_statusInf);
}

void BattlePreparationCursor::Select()
{
	auto charactorChips = _mapCtrl.GetMap()->GetCharactorChipInf(_mapPos);

	if(charactorChips.team != Team::player/* || charactorChips.type == CharactorType::max*/)return;

	// キャラクターの選択
	if (_selectMapPos.x == NONE_SELECT)
	{
		_selectMapPos = _mapPos;
		return;
	}

	// 選択解除
	if (_selectMapPos == _mapPos)
	{
		_selectMapPos.x = NONE_SELECT;
		return;
	}

	auto charactor = _mapCtrl.GetMapPosChar(_mapPos);
	auto selectcharactor = _mapCtrl.GetMapPosChar(_selectMapPos);

	//キャラクターがいれば入れ替え
	if (charactor != nullptr)
	{
		charactor->InitmapPos(_selectMapPos);
	}
	if(selectcharactor != nullptr)
	{
		selectcharactor->InitmapPos(_mapPos);
	}

	// 移動したのでルートの再検索
	for (auto& charactor : _mapCtrl.GetCharacots())
	{
		charactor->RouteSearch();
	}
	_selectMapPos.x = NONE_SELECT;
}

void BattlePreparationCursor::Draw()
{
	auto chipSize = _mapCtrl.GetChipSize();
	auto offset = _camera.GetCameraOffset();
	
	auto handle = ImageHandle("Resource/Image/MapChip/cursor.png");
	Size graphSize;
	GetGraphSize(handle, graphSize);

	DrawRectRotaGraph(offset + _mapPos * chipSize + chipSize * 0.5, Vector2Int(0, 0), graphSize,
		_exRateTrack->GetValue() * (chipSize.w / static_cast<float>(graphSize.w)), 0.0f, handle);

	auto dequeDraw = [](std::shared_ptr<UI> ui)
	{
		if (ui)
		{
			ui->Draw();
		}
	};

	dequeDraw(_terrainInf);
	dequeDraw(_statusInf);
	dequeDraw(_statusWindow);
}

void BattlePreparationCursor::DrawSortieMass()
{
	auto offset = _camera.GetCameraOffset();

	for (const auto& mapDataVec : _mapCtrl.GetMap()->GetMapData())
	{
		for (const auto& mapData : mapDataVec)
		{
			if (mapData.charactorChip.team != Team::player)continue;

			if (mapData.charactorChip.mapPos == _selectMapPos)
			{
				_mapCtrl.DrawSortieMass(offset, mapData.charactorChip, 0xffff00, 0xffff00);
			}
			else
			{
				_mapCtrl.DrawSortieMass(offset, mapData.charactorChip);
			}
		}
	}
}

void BattlePreparationCursor::DrawMovableMass()
{
	auto charactor = _mapCtrl.GetMapPosChar(_mapPos);
	if (charactor == nullptr) return;

	charactor->DrawMovableMass(128);
}

bool BattlePreparationCursor::GetEnd() const
{
	return _end;
}

void BattlePreparationCursor::Start()
{
	_end = false;
	_camera.AddTargetActor(this);
	SetCursorStartPosition();
}

void BattlePreparationCursor::SetCursorStartPosition()
{
	for (const auto& charactor : _mapCtrl.GetCharacots())
	{
		if (charactor->GetTeam() == Team::player)
		{
			_mapPos = charactor->GetMapPos();
			_camera.SetPos(charactor->GetCenterPos());
			break;
		}
	}
}
