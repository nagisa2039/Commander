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
#include "ShopScene.h"
#include "Tool.h"

using namespace std;

PlayScene::PlayScene(SceneController & ctrl, const unsigned int mapId):Scene(ctrl), _lastTurnCnt(100)
{
	_effects.clear();
	_charactors.clear();
	_charactors.reserve(30);

	auto wsize = Application::Instance().GetWindowSize();
	_gameH = FileSystem::Instance().MakeScreen("play_scene_screen", wsize, true);

	debug = true;
	_turnCnt = 0;

	_mapCtrl = make_unique<MapCtrl>(mapId, _charactors);
	_camera = make_unique<Camera>(Rect(Vector2Int(), wsize));
	_turnChangeAnim = make_unique<TurnChangeAnim>();

	_playerCommander = make_unique<PlayerCommander>(_charactors, *_mapCtrl, Team::player, *_camera, _turnCnt);
	//_playerCommander = make_unique<EnemyCommander>(_charactors, *_mapCtrl, Team::player, *_camera);

	_enemyCommander = make_unique<EnemyCommander>(_charactors, *_mapCtrl, Team::enemy, *_camera);

	_camera->AddTargetActor(&*_playerCommander);

	auto mapSize = _mapCtrl->GetMapSize() * _mapCtrl->GetChipSize();
	_camera->SetLimitRect(Rect(mapSize.ToVector2Int() * 0.5, mapSize));

	_mapCtrl->CreateCharactor(ctrl, _effects, *_camera);

	_dyingCharItr = _charactors.end();

	_playerCommander->TurnReset();
	_enemyCommander->TurnReset();

	_mapCtrl->CreateWarSituation();

	bool getCursorMapPos = false;
	Vector2Int cursorMapPos = Vector2Int(0, 0);
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
	_camera->SetPos(Vector3(cameraPos2D.x, cameraPos2D.y, 0));

	_clearAnimTrack = make_unique<Track<float>>();
	_clearAnimTrack->AddKey(0, 0.0f);
	_clearAnimTrack->AddKey(30, 1.0f);

	_updater = &PlayScene::PreparationUpdate;
	_drawer = &PlayScene::PreparationDraw;
	_UIDrawer = &PlayScene::PreparationUIDraw;
	_UIDrawer = &PlayScene::PreparationUIDraw;

	StartFadeIn(&PlayScene::ChangePreparation);

	_filterType = FilterType::none;
	_filterFuncs[Size_t(FilterType::none)] = []() {};
	_filterFuncs[Size_t(FilterType::gauss)] = [&graphH = _gameH]() 
	{GraphFilter(graphH, DX_GRAPH_FILTER_GAUSS, 16, 1000); };

	//// ピクセルシェーダーバイナリコードの読み込み
	//pshandle = LoadPixelShader("Resource/Source/Shader/HPBer.cso");

	//// 頂点データの準備
	//Vert[0].pos = VGet(0.0f, 0.0f, 0.0f);
	//Vert[1].pos = VGet(512.0f, 0.0f, 0.0f);
	//Vert[2].pos = VGet(0.0f, 512.0f, 0.0f);
	//Vert[3].pos = VGet(512.0f, 512.0f, 0.0f);
	//Vert[0].dif = GetColorU8(255, 255, 255, 255);
	//Vert[0].spc = GetColorU8(0, 0, 0, 0);
	//Vert[0].u = 0.0f; Vert[0].v = 0.0f;
	//Vert[1].u = 1.0f; Vert[1].v = 0.0f;
	//Vert[2].u = 0.0f; Vert[2].v = 1.0f;
	//Vert[3].u = 1.0f; Vert[3].v = 1.0f;
	//Vert[0].su = 0.0f; Vert[0].sv = 0.0f;
	//Vert[1].su = 1.0f; Vert[1].sv = 0.0f;
	//Vert[2].su = 0.0f; Vert[2].sv = 1.0f;
	//Vert[3].su = 1.0f; Vert[3].sv = 1.0f;
	//Vert[0].rhw = 1.0f;
	//Vert[1].rhw = 1.0f;
	//Vert[2].rhw = 1.0f;
	//Vert[3].rhw = 1.0f;
	//Vert[4] = Vert[2];
	//Vert[5] = Vert[1];

	//_colorC = { 0.0f,0.0f,1.0f,1.0f };
	//_waveC = { 0.5f, 0.0f, 0.0f, 0.0f };
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update(const Input & input)
{
	// デバッグ
	if (input.GetButtonDown("debug"))
	{
		debug = !debug;
	}

	if (debug)
	{
	}


	_camera->Update();
	if (!(this->*_updater)(input))
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

bool PlayScene::PreparationUpdate(const Input& input)
{
	(*_preparationDeque.begin())->Update(input);
	if (_preparationUI->GetBackMapSelect())
	{
		StartFadeOut(&PlayScene::ChnageMapSelect);
		return false;
	}

	// 戦闘開始
	if (_preparationUI->GetDelete())
	{
		StartPlayerTurn();
		_mapCtrl->GetMap()->StartBGM();
	}
	return true;
}

bool PlayScene::TurnChengeUpdate(const Input& input)
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
	return true;
}

bool PlayScene::PlayerTurnUpdate(const Input& input)
{
	CharactorUpdate(input);
	if (_playerCommander->GetBackMapSelect())
	{
		StartFadeOut(&PlayScene::ChnageMapSelect);
		return false;
	}

	if (_playerCommander->CheckEnd())
	{
		StartEnemyTurn();
		return true;
	}
	_playerCommander->Update(input);
	return true;
}

bool PlayScene::CharactorUpdate(const Input& input)
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
	return true;
}

void PlayScene::StartPlayerTurn()
{
	if (++_turnCnt >= _lastTurnCnt)
	{
		// ゲームオーバー
		StartFadeOut(&PlayScene::ChangeGameOver);
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

	_camera->ClearTargetActor();
	_camera->AddTargetActor(&*_enemyCommander);
	_camera->SetPos(_enemyCommander->GetCenterPos());
	for (auto& charactor : _charactors)
	{
		charactor->RouteSearch();
	}
}

bool PlayScene::EnemyTurnUpdate(const Input& input)
{
	CharactorUpdate(input);

	_turnChangeAnim->Update(input);
	if (!_turnChangeAnim->GetAnimEnd()) return true;

	if (_enemyCommander->CheckEnd())
	{
		StartPlayerTurn();
		return true;
	}
	_enemyCommander->Update(input);
	return true;
}

bool PlayScene::CharactorDyingUpdate(const Input& input)
{
	// dyingが終わっているか確認
	if (!(*_dyingCharItr)->GetDelete())
	{
		// 終わっていないので更新する
		(*_dyingCharItr)->Update(input);
		return true;
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
			StartFadeOut(&PlayScene::ChangeGameOver);
			return true;
		}
		else
		{
			// ゲームクリア
			Application::Instance().GetSaveData().Save(/*_charactors, */_mapCtrl->GetMap()->GetMapID());
			_updater = &PlayScene::GameClearUpdate;
			_drawer = &PlayScene::GameClearDraw;
			return true;
		}
	}

	_mapCtrl->AllCharactorRouteSearch();
	_updater = _updaterOld;
	return true;
}

bool PlayScene::GameClearUpdate(const Input& input)
{
	_clearAnimTrack->Update();
	if (!_clearAnimTrack->GetEnd())return true;

	if(input.GetButtonDown("ok"))
	{
		StartFadeOut(&PlayScene::ChnageMapSelect);
		return false;
	}

	return true;
}

bool PlayScene::GameOverUpdate(const Input& input)
{
	if (input.GetButtonDown("ok"))
	{
		StartFadeOut(&PlayScene::ChnageMapSelect);
		return false;
	}
	return true;
}

void PlayScene::StartFadeIn(void (PlayScene::* funcP)(), const unsigned int color)
{
	_controller.GetFade().StartFadeIn(color);
	_fadeEndFunc = funcP;
	_updater = &PlayScene::FadeUpdate;
	_drawer = &PlayScene::FadeDraw;
	_UIDrawer = &PlayScene::NoneUIDraw;
}

void PlayScene::StartFadeOut(void (PlayScene::* funcP)(), const unsigned int color)
{
	_controller.GetFade().StartFadeOut(color);
	_fadeEndFunc = funcP;
	_updater = &PlayScene::FadeUpdate;
	_drawer = &PlayScene::FadeDraw;
	_UIDrawer = &PlayScene::NoneUIDraw;
}

bool PlayScene::FadeUpdate(const Input& input)
{
	if (_controller.GetFade().GetEnd())
	{
		if (_fadeEndFunc != nullptr)
		{
			(this->*_fadeEndFunc)();
		}
		return false;
	}
	return true;
}

void PlayScene::PreparationDraw(const Camera& camera)
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

void PlayScene::TurnChengeDraw(const Camera& camera)
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
	_turnChangeAnim->Draw();
}

void PlayScene::PlayerTurnDraw(const Camera& camera)
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

void PlayScene::EnemyTurnDraw(const Camera& camera)
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

void PlayScene::GameOverDraw(const Camera& camera)
{
	auto wsize = Application::Instance().GetWindowSize();
	_mapCtrl->Draw(*_camera);

	for (auto& charactor : _charactors)
	{
		charactor->Draw();
	}
	for (auto& effect : _effects)
	{
		effect->Draw();
	}

	auto fontHandle = FontHandle("choplin200edge");
	DrawStringToHandle(wsize.ToVector2Int() * 0.5f, Anker::center, 0x000088, fontHandle, "GAME OVER");
}

void PlayScene::GameClearDraw(const Camera& camera)
{
	auto wsize = Application::Instance().GetWindowSize();

	_mapCtrl->Draw(*_camera);

	for (auto& charactor : _charactors)
	{
		charactor->Draw();
	}
	for (auto& effect : _effects)
	{
		effect->Draw();
	}

	auto& fileSystem = FileSystem::Instance();
	auto animValue = _clearAnimTrack->GetValue();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(animValue * 255));
	DrawRotaGraph(wsize.ToVector2Int() * 0.5f, 1.0f, 0.0f, fileSystem.GetImageHandle("Resource/Image/Battle/light.png"), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	auto fontHandle = fileSystem.GetFontHandle("choplin200edge");
	DrawStringToHandle(wsize.ToVector2Int() * 0.5f, Anker::center, 0xffff00, fontHandle, "GAME CLEAR");
}

void PlayScene::FadeDraw(const Camera& camera)
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

void PlayScene::ChnageMapSelect()
{
	_mapCtrl->GetMap()->StopBGM();
	_controller.ChangeScene(make_shared<MapSelectScene>(_controller));
}

void PlayScene::ChangePreparation()
{
	_updater = &PlayScene::PreparationUpdate;
	_drawer = &PlayScene::PreparationDraw;
	_UIDrawer = &PlayScene::PreparationUIDraw;

	_preparationUI->Open(true);
}

void PlayScene::ChangeGameOver()
{
	_updater = &PlayScene::GameOverUpdate;
	_drawer = &PlayScene::GameOverDraw;
	_UIDrawer = &PlayScene::NoneUIDraw;
	_fadeEndFunc = nullptr;
}

void PlayScene::Draw(void)
{
	// 場面ごとの描画
	SetDrawScreen(_gameH);
	ClsDrawScreen();

	(this->*_drawer)(*_camera);
	SetDrawScreen(DX_SCREEN_BACK);

	_filterFuncs[Size_t(_filterType)]();

	DrawGraph(0, 0, _gameH, true);

	(this->*_UIDrawer)();

	//// 使用するピクセルシェーダーをセット
	//SetUsePixelShader(pshandle);

	//// 定数の設定
	//SetPSConstF(0, _colorC);
	//SetPSConstF(1, _waveC);

	// 描画
	//DrawPrimitive2DToShader(Vert, 6, DX_PRIMTYPE_TRIANGLELIST);

	if (debug)
	{
	}

	//DrawPSTBuffer();
}

void PlayScene::On()
{
	if (_preparationUI->GetDelete())
	{
		_mapCtrl->GetMap()->StartBGM();
	}
}

void PlayScene::Off()
{
	_mapCtrl->GetMap()->StopBGM();
}

void PlayScene::PushShopScene()
{
	_controller.PushScene(make_shared<ShopScene>(_controller));
}

void PlayScene::CharactorDataUpdate()
{
	/*int idx = 0;
	for (auto& charactorData : Application::Instance().GetSaveData().GetCharactorDataVec())
	{
		for (int i = idx; i < _charactors.size(); i++)
		{
			if (_charactors[i]->GetTeam() != Team::player)continue;

			_charactors[i]->InitStatus(charactorData.status);
			idx = i+1;
			break;
		}
	}

	_mapCtrl->AllCharactorRouteSearch();*/
}

void PlayScene::SetFilter(const FilterType type)
{
	_filterType = type;
}
