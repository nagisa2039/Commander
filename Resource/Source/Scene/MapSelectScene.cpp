#include "MapSelectScene.h"
#include "Application.h"
#include "DataBase.h"
#include "Camera.h"
#include "Input.h"
#include "DxLibUtility.h"
#include <Dxlib.h>
#include "FileSystem.h"
#include "PlayScene.h"
#include "TitleScene.h"
#include "SaveData.h"
#include "MapSelectCharactor.h"
#include "Fade.h"
#include "SoundLoader.h"
#include "Map.h"

using namespace std;

void MapSelectScene::NormalUpdate(const Input& input)
{
	bool click = input.GetMouseInput(MOUSE_INPUT_LEFT);
	bool debug = input.GetButton("debug");

	if (click)
	{
		auto& mousePos = input.GetMousePos();
		auto offset = _camera->GetCameraOffset();
		int mapnum = Application::Instance().GetSaveData().GetMapNum();
		for (int i = 0; const auto & pos : _contentPosVec)
		{
			if (Rect(pos + offset, _contentSize).IsHit(Rect(mousePos, Size(1, 1)))
				&& i <= mapnum)
			{
				_selectIdx = i;
				Decide(debug);
				return;
			}
			i++;
		}
	}
	else if (input.GetButtonDown("ok"))
	{
		Decide(debug);
		return;
	}

	if (input.GetButtonDown("back"))
	{
		_controller.GetFade().StartFadeOut();
		_updater = &MapSelectScene::FadeUpdate;
		_fadeEndFunc = [&controller = _controller]()
		{controller.ChangeScene(make_shared<TitleScene>(controller)); };
		return;
	}

	MoveUpdate(input);

	for (auto& mapSelectCharactor : _mapSelectCharactors)
	{
		mapSelectCharactor->Update(input);
	}
	_camera->Update();
}

void MapSelectScene::FadeUpdate(const Input& input)
{
	if (_controller.GetFade().GetEnd())
	{
		_fadeEndFunc();
		return;
	}
}

MapSelectScene::MapSelectScene(SceneController& controller):Scene(controller), _contentSize(300,200)
{
	auto screenSize = Application::Instance().GetWindowSize();
	auto screenCenter = screenSize.ToVector2Int() * 0.5f;

	_camera = make_unique<Camera>(Rect(screenCenter, screenSize));
	_moveStartTrack = make_unique<Track<int>>();
	_moveStartTrack->AddKey(0, 0);
	_moveStartTrack->AddKey(10, 0);

	_updater = &MapSelectScene::FadeUpdate;
	_fadeEndFunc = [&updater = _updater]() {updater = &MapSelectScene::NormalUpdate; };
	_selectIdx = 0;

	_contentPosVec.clear();
	int idx = 0;
	int space = 500;
	for (const auto& mapData : Application::Instance().GetDataBase().GetMapDataTable())
	{
		_contentPosVec.emplace_back(screenCenter + Vector2Int(space * idx, 0));
		idx++;
	}
	assert(_contentPosVec.size() > 0);

	/*auto& charactorDataVec = Application::Instance().GetSaveData().GetCharactorDataVec();
	assert(charactorDataVec.size() > 0);
	for (const auto& charactorData : charactorDataVec)
	{
		_mapSelectCharactors.emplace_back(make_unique<MapSelectCharactor>(*_camera, charactorData.charType));
		(*_mapSelectCharactors.rbegin())->SetPos(_contentPosVec.at(_selectIdx).ToVector2());
		(*_mapSelectCharactors.rbegin())->SetTargetPos(_contentPosVec.at(_selectIdx));
	}*/
	for (int charType = 0; charType < static_cast<int>(CharactorType::max); ++charType)
	{
		_mapSelectCharactors.emplace_back(make_unique<MapSelectCharactor>(*_camera, static_cast<CharactorType>(charType)));
		(*_mapSelectCharactors.rbegin())->SetPos(_contentPosVec.at(_selectIdx).ToVector2());
		(*_mapSelectCharactors.rbegin())->SetTargetPos(_contentPosVec.at(_selectIdx));
	}
	_camera->SetPos(Vector3(_contentPosVec.at(_selectIdx).ToVector2()));

	_camera->AddTargetActor(&**_mapSelectCharactors.begin());

	_dir = 1;
	_charactorIdx = 0;

	_debug = false;

	_controller.GetFade().StartFadeIn();

	_bgmH = SoundHandle("Resource/Sound/BGM/mapSelect.mp3");
	SoundL.PlayBGM(_bgmH);
}

MapSelectScene::~MapSelectScene()
{
	SoundL.StopSound(_bgmH);
}

void MapSelectScene::Update(const Input& input)
{
	(this->*_updater)(input);
}

void MapSelectScene::Decide(const bool degub)
{
	_controller.GetFade().StartFadeOut();
	_updater = &MapSelectScene::FadeUpdate;
	_fadeEndFunc = [&coontroller = _controller, selectIdx = _selectIdx, degub]()
		{coontroller.ChangeScene(make_shared<PlayScene>(coontroller, selectIdx, degub)); };
}

void MapSelectScene::MoveUpdate(const Input& input)
{
	CursorMove(input);

	MapSelectCharactorUpdate();
}

void MapSelectScene::MapSelectCharactorUpdate()
{
	if (_charactorIdx >= _mapSelectCharactors.size()) return;

	_moveStartTrack->Update();
	if (_moveStartTrack->GetEnd())
	{
		int charactorSpace = 50;
		_mapSelectCharactors[_charactorIdx]->SetTargetPos(
			_contentPosVec[_selectIdx] + Vector2Int(charactorSpace * _charactorIdx, 0) * static_cast<float>(_dir));
		_charactorIdx++;
		_moveStartTrack->Reset();
	}
}

void MapSelectScene::CursorMove(const Input& input)
{
	if ((*_mapSelectCharactors.begin())->GetIsMove())return;

	// 後続のキャラクターを今までの目的地に移動させる
	auto successorMove = [&]()
	{
		for (unsigned int idx = 1; idx < _mapSelectCharactors.size(); idx++)
		{
			_mapSelectCharactors[idx]->SetTargetPos(_contentPosVec.at(_selectIdx));
		}
	};

	auto& mousePos = input.GetMousePos();
	auto wsize = Application::Instance().GetWindowSize();
	float range = 0.1f;
	if (input.GetButton("right") ||mousePos.x >= wsize.w*(1.0f - range))
	{
		if (_selectIdx < _contentPosVec.size() - 1 
			&& static_cast<size_t>(_selectIdx) + 1 <= (_debug ? _contentPosVec.size() : Application::Instance().GetSaveData().GetMapNum()))
		{
			_dir = -1;
			successorMove();
			_selectIdx++;
			_charactorIdx = 0;
			_mapSelectCharactors[_charactorIdx]->SetTargetPos(_contentPosVec.at(_selectIdx));

			if (_mapSelectCharactors.size() > 1)
			{
				_charactorIdx++;
				_moveStartTrack->Reset();
			}
		}
	}
	if (input.GetButton("left") || mousePos.x <= wsize.w * range)
	{
		_dir = 1;
		if (_selectIdx > 0)
		{
			successorMove();
			_selectIdx--;
			_charactorIdx = 0;
			_mapSelectCharactors[_charactorIdx]->SetTargetPos(_contentPosVec.at(_selectIdx));
			if (_mapSelectCharactors.size() > 1)
			{
				_moveStartTrack->Reset();
				_charactorIdx++;
			}
		}
	}
}

void MapSelectScene::Draw()
{
	auto offset = _camera->GetCameraOffset();
	auto wsize = Application::Instance().GetWindowSize();
	DrawBox(Vector2Int(0,0), wsize.ToVector2Int(), 0x364364);

	auto& mapDataVec = Application::Instance().GetDataBase().GetMapDataTable();
	int fontH = FontHandle("choplin40edge");
	auto mapNum = _debug ? _contentPosVec.size() : Application::Instance().GetSaveData().GetMapNum();
	auto& cameraRect = _camera->GetRect();
	for (size_t idx = 0; auto& map : mapDataVec)
	{
		auto contentRect = Rect(_contentPosVec[idx], _contentSize);

		if (!contentRect.IsHit(cameraRect))
		{
			idx++;
			continue;
		}

		if (idx > mapNum)
		{
			SetDrawBright(64,64,64);
		}
		contentRect.DrawGraph(map->GetMapGraphH(), offset);
		//contentRect.Draw(offset, 0x000000, false);

		DrawStringToHandle(offset + contentRect.center, Anker::center, 0xffffff, fontH, map->GetName().c_str());
		SetDrawBright(255, 255, 255);
		idx++;
	}

	for (auto rItr = _mapSelectCharactors.rbegin(); rItr != _mapSelectCharactors.rend(); rItr++)
	{
		(*rItr)->Draw();
	}
}