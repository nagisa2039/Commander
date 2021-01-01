#include<DxLib.h>
#include <algorithm>
#include "PlayScene.h"
#include "../Utility/Input.h"
#include "../System/Application.h"
#include "../System/FileSystem.h"
#include "../Game/Camera.h"
#include "../Game/MapCtrl.h"
#include "UI/PreparationUI.h"
#include "SaveData.h"
#include "Fade.h"
#include "Map.h"
#include "Charactor.h"
#include "PlayerCommander.h"
#include "EnemyCommander.h"
#include "Effect/Effect.h"
#include "Effect/FlyText.h"
#include "TurnChangeAnim.h"
#include "MapSelectScene.h"
#include "Tool.h"
#include "SoundLoader.h"
#include "UI/CheckWindow.h"
#include "TitleScene.h"
#include "Cast.h"

using namespace std;

namespace
{
	constexpr unsigned int AUTO_SCENE_CHANGE_FRAME_CNT = 180;
}

PlayScene::PlayScene(SceneController & ctrl, const unsigned int mapId, const bool ai):Scene(ctrl), _limitTurnCnt(100), _aiMode(ai)
{
	_effects.clear();
	_charactors.clear();
	_charactors.reserve(30);

	auto wsize = Application::Instance().GetWindowSize();
	_gameH = FileSystem::Instance().MakeScreen("play_scene_screen", wsize, true);

	_turnCnt = 0;

	_mapCtrl = make_unique<MapCtrl>(mapId, _charactors);
	_camera = make_unique<Camera>(Rect{ Vector2Int(), wsize });
	_turnChangeAnim = make_unique<TurnChangeAnim>();

	auto mapSize = _mapCtrl->GetMapSize() * _mapCtrl->GetChipSize();
	_camera->SetLimitRect(Rect{ mapSize.ToVector2Int() * 0.5, mapSize });

	_mapCtrl->CreateCharactor(ctrl, _effects, *_camera);

	if (ai)
	{
		_playerCommander = make_unique<EnemyCommander>(_charactors, *_mapCtrl, Team::player, *_camera);
	}
	else
	{
		_playerCommander = make_unique<PlayerCommander>(_charactors, *_mapCtrl, Team::player, *_camera, _turnCnt);
	}
	_enemyCommander = make_unique<EnemyCommander>(_charactors, *_mapCtrl, Team::enemy, *_camera);

	_camera->AddTargetActor(&*_playerCommander);

	_dyingCharItr = _charactors.end();

	_playerCommander->TurnReset();
	_enemyCommander->TurnReset();

	_mapCtrl->CreateWarSituation();

	bool getCursorMapPos = false;
	Vector2Int cursorMapPos = Vector2Int{ 0, 0 };
	for (auto& charactor : _charactors)
	{
		charactor->RouteSearch();
		if (!getCursorMapPos && charactor->GetTeam() == Team::player)
		{
			getCursorMapPos = true;
			cursorMapPos = charactor->GetMapPos();
		}
	}

	_preparationUI = make_shared<PreparationUI>(&_preparationDeque, *_camera, *_mapCtrl, *this);
	_preparationDeque.emplace_back(_preparationUI);

	Vector2 cameraPos2D = (cursorMapPos * _mapCtrl->GetChipSize()).ToVector2();
	_camera->SetPos(Vector2{ cameraPos2D.x, cameraPos2D.y});

	_clearAnimTrack = make_unique<Track<float>>();
	_clearAnimTrack->AddKey(0, 0.0f);
	_clearAnimTrack->AddKey(30, 1.0f);

	_autoSceneChangeTrack = make_unique<Track<int>>();
	_autoSceneChangeTrack->AddKey(0, 0);
	_autoSceneChangeTrack->AddKey(AUTO_SCENE_CHANGE_FRAME_CNT, 0);

	_updater = &PlayScene::PreparationUpdate;
	_drawer = &PlayScene::PreparationDraw;
	_UIDrawer = &PlayScene::PreparationUIDraw;
	_UIDrawer = &PlayScene::PreparationUIDraw;


	_filterType = FilterType::none;
	_filterFuncs[Uint64(FilterType::none)] = []() {};
	_filterFuncs[Uint64(FilterType::gauss)] = [&graphH = _gameH]()
	{GraphFilter(graphH, DX_GRAPH_FILTER_GAUSS, 16, 1000); };

	if (_aiMode)
	{
		_demoAnimTrack = make_unique<Track<float>>(true);
		_demoAnimTrack->AddKey(0, 1.0f);
		_demoAnimTrack->AddKey(30, 0.0f);
		_demoAnimTrack->AddKey(60, 1.0f);
		StartFadeIn([this]() {GameStart(); }); 
	}
	else
	{
		StartFadeIn([this]() {ChangePreparation(); });
	}
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update(const Input & input)
{
	_camera->Update();

	if (_aiMode)
	{
		_demoAnimTrack->Update();
	}

	for (auto& endUI : _endUIDeque)
	{
		endUI->Update(input);
		return;
	}
	erase_if(_endUIDeque, [](const std::shared_ptr<UI>& endUI) {return endUI->GetDelete(); });

	(this->*_updater)(input);

	if (_effects.size() <= 0)
	{
		return;
	}

	for (auto& effect : _effects)
	{
		effect->Update(input);
	}
	auto newEffectEnd = remove_if(_effects.begin(), _effects.end(),
		[](const std::shared_ptr<Effect>& effect) { return effect->GetDelete(); });
	_effects.erase(newEffectEnd, _effects.end());
}

void PlayScene::PreparationUpdate(const Input& input)
{
	(*_preparationDeque.begin())->Update(input);
	if (_preparationUI->GetBackMapSelect())
	{
		StartFadeOut([this]() {ChengeMapSelect(); });
		return;
	}

	// 戦闘開始
	if (_preparationUI->GetDelete())
	{
		GameStart();
	}
	return;
}

void PlayScene::GameStart()
{
	StartPlayerTurn();
	_mapCtrl->GetMap()->StartBGM();
}

void PlayScene::TurnChengeUpdate(const Input& input)
{
	_turnChangeAnim->Update(input);
	if (_turnChangeAnim->GetAnimEnd())
	{
		if (_turnChangeAnim->GetCurrentTeam() == Team::player)
		{
			_updater = &PlayScene::PlayerTurnUpdate;
			_drawer = &PlayScene::PlayerTurnDraw;
			_UIDrawer = &PlayScene::PlayerUIDraw;
			if (_turnCnt == 1)
			{
				_playerCommander->StartNormalUpdate();
			}
			else
			{
				_playerCommander->StartTerrainEffectUpdate();
			}
		}
		else
		{
			_updater = &PlayScene::EnemyTurnUpdate;
			_drawer = &PlayScene::EnemyTurnDraw;
			_UIDrawer = &PlayScene::NoneUIDraw;
			if (_turnCnt == 1)
			{
				_enemyCommander->StartNormalUpdate();
			}
			else
			{
				_enemyCommander->StartTerrainEffectUpdate();
			}
		}
	}
	return;
}

void PlayScene::PlayerTurnUpdate(const Input& input)
{
	if (BackSceneWindow(input))return;

	CharactorUpdate(input);
	if (_playerCommander->GetBackMapSelect())
	{
		StartFadeOut([this]() {ChengeMapSelect(); });
		return;
	}

	if (_playerCommander->CheckEnd())
	{
		StartEnemyTurn();
		return;
	}
	_playerCommander->Update(input);
	return;
}

void PlayScene::CharactorUpdate(const Input& input)
{
	for (auto itr = _charactors.begin(); itr != _charactors.end(); itr++)
	{
		(*itr)->Update(input);
		if ((*itr)->GetIsDying())
		{
			_dyingCharItr = itr;
			_updaterOld = _updater;
			_updater = &PlayScene::CharactorDyingUpdate;
		}
	}
	return;
}

void PlayScene::StartPlayerTurn()
{
	if (++_turnCnt >= _limitTurnCnt)
	{
		// ゲームオーバー
		GameOver();
		return;
	}

	_updater = &PlayScene::TurnChengeUpdate;
	_drawer = &PlayScene::TurnChengeDraw;
	_UIDrawer = &PlayScene::NoneUIDraw;
	_turnChangeAnim->TurnStart(Team::player);
	_enemyCommander->TurnReset();
	_playerCommander->TurnReset();

	_updaterOld = _updater;

	_camera->ClearTargetActor();
	_camera->AddTargetActor(&*_playerCommander);
	_camera->SetPos(_playerCommander->GetCenterPos());
	_mapCtrl->AllCharactorRouteSearch();
}

void PlayScene::StartEnemyTurn()
{
	_updater = &PlayScene::TurnChengeUpdate;
	_drawer = &PlayScene::TurnChengeDraw;
	_UIDrawer = &PlayScene::NoneUIDraw;
	_turnChangeAnim->TurnStart(Team::enemy);
	_enemyCommander->TurnReset();
	_playerCommander->TurnReset();

	_updaterOld = _updater;

	_camera->SetLooseFollow(false);
	_camera->ClearTargetActor();
	_camera->AddTargetActor(&*_enemyCommander);
	_camera->SetPos(_enemyCommander->GetCenterPos());
	for (auto& charactor : _charactors)
	{
		charactor->RouteSearch();
	}
}

void PlayScene::EnemyTurnUpdate(const Input& input)
{
	if (BackSceneWindow(input))return;

	CharactorUpdate(input); 

	_turnChangeAnim->Update(input);
	if (!_turnChangeAnim->GetAnimEnd()) return;

	if (_enemyCommander->CheckEnd())
	{
		StartPlayerTurn();
		return;
	}
	_enemyCommander->Update(input);
	return;
}

bool PlayScene::BackSceneWindow(const Input& input)
{
	if (input.GetButtonDown("F1"))
	{
		_endUIDeque.emplace_back(make_shared<CheckWindow>("タイトルに戻りますか？", &_endUIDeque,
			[this]() {	StartFadeOut([this]() {ChengeTitle(); }); }));
		return true;
	}
	if (input.GetButtonDown("F2"))
	{
		_endUIDeque.emplace_back(make_shared<CheckWindow>("マップ選択に戻りますか？", &_endUIDeque,
			[this]() {	StartFadeOut([this]() {ChengeMapSelect(); }); }));
		return true;
	}
	return false;
}

void PlayScene::CharactorDyingUpdate(const Input& input)
{
	// dyingが終わっているか確認
	if (!(*_dyingCharItr)->GetDelete())
	{
		// 終わっていないので更新する
		(*_dyingCharItr)->Update(input);
		return;
	}

	// 削除処理
	_charactors.erase(_dyingCharItr);
	_dyingCharItr = _charactors.end();

	array<int, static_cast<size_t>(Team::max)> teamCntArray;
	teamCntArray.fill(0);
	for (auto& charactor : _charactors)
	{
		teamCntArray[static_cast<size_t>(charactor->GetTeam())]++;
	}

	// ゲームクリアとゲームオーバーの判定
	for (int i = 0; i < teamCntArray.size(); i++)
	{
		if (teamCntArray[i] > 0)
		{
			continue;
		}

		if (static_cast<Team>(i) == Team::player)
		{
			// ゲームオーバー
			GameOver();
			return;
		}
		else
		{
			// ゲームクリア
			GameClear();
			return;
		}
	}

	_mapCtrl->AllCharactorRouteSearch();
	_updater = _updaterOld;
	return;
}

void PlayScene::GameClear()
{
	if (!_aiMode)
	{
		Application::Instance().GetSaveData().Save(_mapCtrl->GetMap()->GetMapID(), _turnCnt);
	}
	_autoSceneChangeTrack->Reset();
	_updater = &PlayScene::GameClearUpdate;
	_drawer = &PlayScene::GameClearDraw;
	_mapCtrl->GetMap()->StopBGM();
	SoundL.PlaySE("Resource/Sound/SE/shakin1.mp3");
}

void PlayScene::GameOver()
{
	StartFadeOut([this]() {ChangeGameOver(); });
}

void PlayScene::GameClearUpdate(const Input& input)
{
	_clearAnimTrack->Update();
	if (!_clearAnimTrack->GetEnd())return;

	_autoSceneChangeTrack->Update();
	if(input.GetButtonDown("ok") || _autoSceneChangeTrack->GetEnd())
	{
		SoundL.PlaySE("Resource/Sound/SE/ok2.mp3");
		StartFadeOut([this]() {_aiMode ? ChengeTitle() : ChengeMapSelect(); }, 0, &PlayScene::GameClearDraw);
		return;
	}

	return;
}

void PlayScene::GameOverUpdate(const Input& input)
{
	_autoSceneChangeTrack->Update();
	if (input.GetButtonDown("ok") || _autoSceneChangeTrack->GetEnd())
	{
		SoundL.PlaySE("Resource/Sound/SE/ok2.mp3");
		StartFadeOut([this]() {_aiMode ? ChengeTitle() : ChengeMapSelect(); }, 0, &PlayScene::GameOverDraw);
		return;
	}
	return;
}

void PlayScene::StartFadeIn(std::function<void()> funcP, const unsigned int color, void(PlayScene::* nextDrawer)())
{
	_controller.GetFade().StartFadeIn(color);
	_fadeEndFunc = funcP;
	_updater = &PlayScene::FadeUpdate;
	_drawer = nextDrawer;
	_UIDrawer = &PlayScene::NoneUIDraw;
}

void PlayScene::StartFadeOut(std::function<void()> funcP, const unsigned int color, void(PlayScene::* nextDrawer)())
{
	_controller.GetFade().StartFadeOut(color);
	_fadeEndFunc = funcP;
	_updater = &PlayScene::FadeUpdate;
	_drawer = nextDrawer;
	_UIDrawer = &PlayScene::NoneUIDraw;
}

void PlayScene::FadeUpdate(const Input& input)
{
	if (_controller.GetFade().GetEnd())
	{
		if (_fadeEndFunc != nullptr)
		{
			_fadeEndFunc();
		}
		return;
	}
	return;
}

void PlayScene::PreparationDraw()
{
	_mapCtrl->Draw(*_camera);

	_preparationUI->BeginDraw();

	for (auto& charactor : _charactors)
	{
		charactor->Draw();
	}
	for (auto& effect : _effects)
	{
		effect->Draw();
	}
}

void PlayScene::TurnChengeDraw()
{
	BaseDraw();
	_turnChangeAnim->Draw();
}

void PlayScene::PlayerTurnDraw()
{
	_mapCtrl->Draw(*_camera);

	_playerCommander->DrawMovableMass();
	for (auto& charactor : _charactors)
	{
		charactor->Draw();
	}
	for (auto& effect : _effects)
	{
		effect->Draw();
	}
}

void PlayScene::EnemyTurnDraw()
{
	BaseDraw();
}

void PlayScene::GameOverDraw()
{
	auto wsize = Application::Instance().GetWindowSize();
	DrawGraph(0,0,ImageHandle("Resource/Image/Battle/game_over.jpg"), false);
	auto fontHandle = FontHandle("choplin200edge");
	DrawStringToHandle(wsize.ToVector2Int() * 0.5f, Anker::center, 0x000088, fontHandle, "GAME OVER");
}

void PlayScene::GameClearDraw()
{
	BaseDraw();

	auto wsize = Application::Instance().GetWindowSize();
	auto& fileSystem = FileSystem::Instance();
	auto animValue = _clearAnimTrack->GetValue();
	SetDrawBlendMode(DX_BLENDMODE_ADD, static_cast<int>(animValue * 255));
	DrawRotaGraph(wsize.ToVector2Int() * 0.5f, 1.0f, 0.0f, fileSystem.GetImageHandle("Resource/Image/Battle/light.png"), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	auto fontHandle = fileSystem.GetFontHandle("choplin200edge");
	DrawStringToHandle(wsize.ToVector2Int() * 0.5f, Anker::center, 0xffff00, fontHandle, "GAME CLEAR");
}

void PlayScene::BaseDraw()
{
	_mapCtrl->Draw(*_camera);

	for (auto& charactor : _charactors)
	{
		charactor->Draw();
	}
	for (auto& effect : _effects)
	{
		effect->Draw();
	}
}

void PlayScene::PreparationUIDraw()
{
	(*_preparationDeque.begin())->Draw();
}

void PlayScene::PlayerUIDraw()
{
	// プレイヤーCursorの描画
	_playerCommander->Draw();
}

void PlayScene::NoneUIDraw()
{
}

void PlayScene::ChengeTitle()
{
	SoundL.StopAllSound();
	_controller.ChangeScene(make_shared<TitleScene>(_controller));
}

void PlayScene::ChengeMapSelect()
{
	SoundL.StopAllSound();
	_controller.ChangeScene(make_shared<MapSelectScene>(_controller));
}

void PlayScene::ChangePreparation()
{
	_updater = &PlayScene::PreparationUpdate;
	_drawer = &PlayScene::PreparationDraw;
	_UIDrawer = &PlayScene::PreparationUIDraw;

	_preparationUI->Open(true);
	_preparationUI->StartBGM();
}

void PlayScene::ChangeGameOver()
{
	_controller.GetFade().StartFadeIn();
	_updater = &PlayScene::FadeUpdate;
	_drawer = &PlayScene::GameOverDraw;
	_UIDrawer = &PlayScene::NoneUIDraw;
	_autoSceneChangeTrack->Reset();
	_fadeEndFunc = [&updater = _updater]() {updater = &PlayScene::GameOverUpdate;};
	SoundL.PlaySE("Resource/Sound/SE/shock1.mp3");
	SoundL.PlayBGM("Resource/Sound/BGM/game_over.mp3");
}

void PlayScene::Draw(void)
{
	// 場面ごとの描画
	SetDrawScreen(_gameH);
	ClsDrawScreen();

	(this->*_drawer)();
	SetDrawScreen(DX_SCREEN_BACK);

	_filterFuncs[Uint64(_filterType)]();

	DrawGraph(0, 0, _gameH, true);

	(this->*_UIDrawer)();

	if (_aiMode)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(_demoAnimTrack->GetValue() * 255));
		Rect demoRect{ Vector2Int{ 220, 45 }, Size{ 400, 50 } };
		demoRect.DrawGraph(ImageHandle("Resource/Image/UI/checkWindowSelect.png"));
		DrawStringToHandle(demoRect.center, Anker::center, 0xffffff, FontHandle("choplin20edge"), "デモプレイ中  F1で終了");
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	for (auto rItr = _endUIDeque.rbegin(); rItr != _endUIDeque.rend(); ++rItr)
	{
		(*rItr)->Draw();
	}
}

void PlayScene::On()
{
	if (_preparationUI->GetDelete() || _aiMode)
	{
		_mapCtrl->GetMap()->StartBGM();
	}
}

void PlayScene::Off()
{
	_mapCtrl->GetMap()->StopBGM();
}

void PlayScene::SetFilter(const FilterType type)
{
	_filterType = type;
}
