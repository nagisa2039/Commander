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
#include "UI/CheckWindow.h"
#include "Cast.h"

using namespace std;

void MapSelectScene::NormalUpdate(const Input& input)
{
	if (_uiDeque.size() > 0)
	{
		_uiDeque.front()->Update(input);
		return;
	}

	bool click = input.GetMouseInput(MOUSE_INPUT_LEFT);
	bool debug = input.GetButton("debug");

	if (click)
	{
		auto& mousePos = input.GetMousePos();
		auto offset = _camera->GetCameraOffset();
		auto& clearMapDataVec = Application::Instance().GetSaveData().GetClearMapDataVec();
		
		if (Rect{ _contentPosVec[_selectIdx] + offset, _contentSize }.IsHit(Rect{ mousePos, Size{ 1, 1 } })
			&& clearMapDataVec[_selectIdx] >= 0)
		{
			Decide(debug);
			return;
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

	if (input.GetButtonDown("F1"))
	{
		_uiDeque.push_front(make_shared<CheckWindow>("セーブデータを初期化しますか？", &_uiDeque, 
			[this]()
			{
				Application::Instance().GetSaveData().Reset();
				SetSelectIdx(0);
			}));
		return;
	}
	if (input.GetButtonDown("F2"))
	{
		_uiDeque.push_front(make_shared<CheckWindow>("ステージをすべて開放しますか？", &_uiDeque,
			[]() 
			{
				Application::Instance().GetSaveData().AllOpen();
			}));
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

MapSelectScene::MapSelectScene(SceneController& controller) :Scene(controller), _contentSize({ 450,300 })
{
	auto screenSize = Application::Instance().GetWindowSize();
	auto screenCenter = screenSize.ToVector2Int() * 0.5f;

	_camera = make_unique<Camera>(Rect{ screenCenter, screenSize });
	_moveStartTrack = make_unique<Track<int>>();
	_moveStartTrack->AddKey(0, 0);
	_moveStartTrack->AddKey(10, 0);

	_selectAnimTrack = make_unique<Track<float>>(true);
	_selectAnimTrack->AddKey(0, 1.0f);
	_selectAnimTrack->AddKey(30, 1.1f);
	_selectAnimTrack->AddKey(60, 1.0f);

	_updater = &MapSelectScene::FadeUpdate;
	_fadeEndFunc = [&updater = _updater]() {updater = &MapSelectScene::NormalUpdate; };
	_selectIdx = 0;

	_contentPosVec.clear();
	int idx = 0;
	int space = 700;
	for (const auto& mapData : DataBase::Instance().GetMapDataTable())
	{
		_contentPosVec.emplace_back(screenCenter + Vector2Int{ space * idx, 0 });
		idx++;
	}
	assert(_contentPosVec.size() > 0);
	auto charactorTypeMax = DataBase::Instance().GetCharactorDataTable().size();
	for (unsigned int charType = 0; charType < charactorTypeMax; ++charType)
	{
		_mapSelectCharactors.emplace_back(make_unique<MapSelectCharactor>
			(*_camera, static_cast<CharactorType>(charType), charType == 0));
		(*_mapSelectCharactors.rbegin())->SetPos(_contentPosVec.at(_selectIdx).ToVector2());
		(*_mapSelectCharactors.rbegin())->SetTargetPos(_contentPosVec.at(_selectIdx));
	}
	_camera->SetPos(Vector2{ _contentPosVec.at(_selectIdx).ToVector2() });

	_camera->AddTargetActor(&**_mapSelectCharactors.begin());
	_camera->SetOffset(Vector2{ 0, -100 });

	_dir = 1;
	_charactorIdx = 0;

	_controller.GetFade().StartFadeIn();

	_bgmH = SoundHandle("Resource/Sound/BGM/mapSelect.mp3");
	SoundL.PlayBGM(_bgmH);

	_uiH = ImageHandle("Resource/Image/UI/checkWindowSelect.png");

	_screenH = FileSystem::Instance().MakeScreen("mapSelectScreen", Application::Instance().GetWindowSize());
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
	SoundL.PlaySE("Resource/Sound/SE/ok2.mp3");
	_controller.GetFade().StartFadeOut();
	_updater = &MapSelectScene::FadeUpdate;
	_fadeEndFunc = [&coontroller = _controller, selectIdx = _selectIdx, degub]()
		{coontroller.ChangeScene(make_shared<PlayScene>(coontroller, selectIdx, degub)); };
}

void MapSelectScene::SetSelectIdx(const unsigned int value)
{
	int sub =_selectIdx - value;
	_dir = sub / abs(sub);

	// 後続のキャラクターを今までの目的地に移動させる
	for (unsigned int idx = 1; idx < _mapSelectCharactors.size(); idx++)
	{
		_mapSelectCharactors[idx]->SetTargetPos(_contentPosVec.at(_selectIdx));
	}

	_selectIdx = value;
	_selectAnimTrack->Reset();
	_charactorIdx = 0;
	_mapSelectCharactors[_charactorIdx]->SetTargetPos(_contentPosVec.at(_selectIdx));

	if (_mapSelectCharactors.size() > 1)
	{
		_charactorIdx++;
		_moveStartTrack->Reset();
	}
}

void MapSelectScene::MoveUpdate(const Input& input)
{
	_selectAnimTrack->Update();
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
			_contentPosVec[_selectIdx] + Vector2Int{ charactorSpace * Int32(_charactorIdx), 0 } *static_cast<float>(_dir));
		_charactorIdx++;
		_moveStartTrack->Reset();
	}
}

void MapSelectScene::CursorMove(const Input& input)
{
	if ((*_mapSelectCharactors.begin())->GetIsMove())return;


	auto& mousePos = input.GetMousePos();
	auto wsize = Application::Instance().GetWindowSize();
	float range = 0.1f;

	bool mouseMove = input.GetMouseMove() != Vector2Int{ 0,0 };
	if (input.GetButton("right") || (mousePos.x >= wsize.w*(1.0f - range) && mouseMove))
	{
		auto nextIdx = static_cast<size_t>(_selectIdx) + 1;
		if (nextIdx < _contentPosVec.size()
			&& Application::Instance().GetSaveData().GetClearMapDataVec()[nextIdx] >= 0)
		{
			SetSelectIdx(_selectIdx+1);
		}
	}
	if (input.GetButton("left") || (mousePos.x <= wsize.w * range && mouseMove))
	{
		if (_selectIdx > 0)
		{
			SetSelectIdx(_selectIdx-1);
		}
	}
}

void MapSelectScene::Draw()
{
	int currentScreen = GetDrawScreen();
	SetDrawScreen(_screenH);

	auto offset = _camera->GetCameraOffset();
	auto wsize = Application::Instance().GetWindowSize();
	DrawGraph(0,0,ImageHandle("Resource/Image/UI/mapSelectBG.jpg"), false);

	auto& mapDataVec = DataBase::Instance().GetMapDataTable();
	auto& clearMapDataVec = Application::Instance().GetSaveData().GetClearMapDataVec();
	int fontH = FontHandle("choplin30edge");
	int clearH = ImageHandle("Resource/Image/UI/clear.png");
	auto& cameraRect = _camera->GetRect();
	Vector2Int shadowOffset = Vector2Int{ 10, 10 };
	for (size_t idx = 0; auto& map : mapDataVec)
	{
		auto contentRect = Rect{ _contentPosVec[idx], _contentSize * (idx == _selectIdx ? _selectAnimTrack->GetValue() : 1.0f) };

		if (!contentRect.IsHit(cameraRect))
		{
			idx++;
			continue;
		}

		if (clearMapDataVec[idx] < 0)
		{
			SetDrawBright(64,64,64);
		}
		float frameScale = 1.1f;
		contentRect.Draw(offset + shadowOffset, 0);
		contentRect.DrawGraph(map->GetMapGraphH(), offset);
		SetDrawBright(255, 255, 255);

		if (clearMapDataVec[idx] > 0)
		{
			contentRect.DrawGraph(clearH, offset);
		}
		idx++;
	}

	for (auto rItr = _mapSelectCharactors.rbegin(); rItr != _mapSelectCharactors.rend(); rItr++)
	{
		(*rItr)->Draw();
	}

	auto uiRect = Rect{ Vector2Int{ wsize.w / 2, 150 }, Size{ 600, 100 } };
	uiRect.Draw(shadowOffset, 0, true);
	uiRect.DrawGraph(_uiH);
	DrawStringToHandle(uiRect.center, Anker::center, 0xffffff, 
		fontH, mapDataVec[_selectIdx]->GetName().c_str());

	if (clearMapDataVec[_selectIdx] > 0)
	{
		fontH = FontHandle("choplin20edge");
		DrawStringToHandle(uiRect.Rightcenter() + Vector2Int{ -100, 30 }, Anker::rightcenter, 0xffffff,
			fontH, "クリアターン数%02d", clearMapDataVec[_selectIdx]);
	}

	SetDrawScreen(currentScreen);

	if (_uiDeque.size() > 0)
	{
		GraphFilter(_screenH, DX_GRAPH_FILTER_GAUSS, 16, 1000);
	}

	DrawGraph(0, 0, _screenH, false);
	for (auto rItr = _uiDeque.rbegin(); rItr != _uiDeque.rend(); ++rItr)
	{
		(*rItr)->Draw();
	}
}