#include<DxLib.h>
#include <algorithm>
#include "PlayScene.h"
#include "../Utility/Input.h"
#include "../System/Application.h"
#include "../System/FileSystem.h"
#include "../Game/Camera.h"
#include "../Game/MapCtrl.h"
#include "MapEditScene.h"
#include "UI/PreparationUI.h"
#include "SaveData.h"

#include "Charactor.h"

#include "PlayerCommander.h"
#include "EnemyCommander.h"
#include "Effect/Effect.h"
#include "Effect/FlyText.h"
#include "TurnChangeAnim.h"
#include "MapSelectScene.h"

using namespace std;

PlayScene::PlayScene(SceneController & ctrl, const unsigned int mapId):Scene(ctrl)
{
	_effects.clear();
	_charactors.clear();
	_charactors.reserve(30);

	debug = true;
	_mapId = mapId;

	auto wsize = Application::Instance().GetWindowSize();

	_mapCtrl = make_shared<MapCtrl>(_charactors);
	_camera = make_shared<Camera>(Rect(Vector2Int(), wsize));

	_turnChangeAnim = make_shared<TurnChangeAnim>();

	_playerCommander = make_shared<PlayerCommander>(_charactors, *_mapCtrl, Team::player, *_camera);
	_enemyCommander = make_shared<EnemyCommander>(_charactors, *_mapCtrl, Team::enemy, *_camera);

	_camera->AddTargetActor(&*_playerCommander);

	auto mapSize = _mapCtrl->GetMapSize() * _mapCtrl->GetChipSize();
	_camera->SetLimitRect(Rect(mapSize.ToVector2Int() * 0.5, mapSize));

	_mapCtrl->LoadMap(Application::Instance().GetDataBase().GetMapData(_mapId).fileName);
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

	_preparationDeque.clear();
	_preparationUI = make_shared<PreparationUI>(_preparationDeque, *_camera, *_mapCtrl);
	_preparationDeque.emplace_back(_preparationUI);

	_preparationUI->Open(true);
	Vector2 cameraPos2D = (cursorMapPos * _mapCtrl->GetChipSize()).ToVector2();
	_camera->SetPos(Vector3(cameraPos2D.x, cameraPos2D.y, 0));

	_clearAnimTrack = make_unique<Track<float>>();
	_clearAnimTrack->AddKey(0, 0.0f);
	_clearAnimTrack->AddKey(30, 1.0f);

	_uniqueUpdater = &PlayScene::PreparationUpdate;
	_uniqueDrawer = &PlayScene::PreparationDraw;

	_fadeTrack = make_unique<Track<float>>();
	_fadeTrack->AddKey(0, 1.0f);
	_fadeTrack->AddKey(60, 0.0f);
	_fadeColor = 0x000000;

	StartFadeIn();
	_fadeEndFunc = &PlayScene::ChangePreparation;

	// ピクセルシェーダーバイナリコードの読み込み
	pshandle = LoadPixelShader("Resource/Source/Shader/HPBer.cso");

	// 頂点データの準備
	Vert[0].pos = VGet(0.0f, 0.0f, 0.0f);
	Vert[1].pos = VGet(512.0f, 0.0f, 0.0f);
	Vert[2].pos = VGet(0.0f, 512.0f, 0.0f);
	Vert[3].pos = VGet(512.0f, 512.0f, 0.0f);
	Vert[0].dif = GetColorU8(255, 255, 255, 255);
	Vert[0].spc = GetColorU8(0, 0, 0, 0);
	Vert[0].u = 0.0f; Vert[0].v = 0.0f;
	Vert[1].u = 1.0f; Vert[1].v = 0.0f;
	Vert[2].u = 0.0f; Vert[2].v = 1.0f;
	Vert[3].u = 1.0f; Vert[3].v = 1.0f;
	Vert[0].su = 0.0f; Vert[0].sv = 0.0f;
	Vert[1].su = 1.0f; Vert[1].sv = 0.0f;
	Vert[2].su = 0.0f; Vert[2].sv = 1.0f;
	Vert[3].su = 1.0f; Vert[3].sv = 1.0f;
	Vert[0].rhw = 1.0f;
	Vert[1].rhw = 1.0f;
	Vert[2].rhw = 1.0f;
	Vert[3].rhw = 1.0f;
	Vert[4] = Vert[2];
	Vert[5] = Vert[1];

	_colorC = { 0.0f,0.0f,1.0f,1.0f };
	_waveC = { 0.5f, 0.0f, 0.0f, 0.0f };
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update(const Input & input)
{
	// デバッグ
	if (input.GetButtonDown(0, "debug"))
	{
		debug = !debug;
	}

	if (debug)
	{
	}

	if (input.GetButtonDown(0, "F1"))
	{
		_controller.ChangeScene(make_shared<MapEditScene>(_controller));
		return;
	}


	_camera->Update();
	if (!(this->*_uniqueUpdater)(input))
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
		StartFadeOut();
		return false;
	}
	if (_preparationUI->GetDelete())
	{
		// プレイヤーターンを開始
		StartPlayerTurn();
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
			_uniqueUpdater = &PlayScene::PlayerTurnUpdate;
			_uniqueDrawer = &PlayScene::PlayerTurnDraw;
			_playerCommander->StartTerrainEffect();
		}
		else
		{
			_uniqueUpdater = &PlayScene::EnemyTurnUpdate;
			_uniqueDrawer = &PlayScene::EnemyTurnDraw;
			_enemyCommander->StartTerrainEffect();
		}
	}
	return true;
}

bool PlayScene::PlayerTurnUpdate(const Input& input)
{
	CharactorUpdate(input);
	if (_playerCommander->GetBackMapSelect())
	{
		StartFadeOut();
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
			_uniqueUpdaterOld = _uniqueUpdater;
			_uniqueUpdater = &PlayScene::CharactorDyingUpdate;
		}
	}
	return true;
}

void PlayScene::StartPlayerTurn()
{
	_uniqueUpdater = &PlayScene::TurnChengeUpdate;
	_uniqueDrawer = &PlayScene::TurnChengeDraw;
	_turnChangeAnim->TurnStart(Team::player);
	_enemyCommander->TurnReset();
	_playerCommander->TurnReset();

	_uniqueUpdaterOld = _uniqueUpdater;

	_camera->ClearTargetActor();
	_camera->AddTargetActor(&*_playerCommander);
	_camera->SetPos(_playerCommander->GetCenterPos());
	for (auto& charactor : _charactors)
	{
		charactor->RouteSearch();
	}
}

void PlayScene::StartEnemyTurn()
{
	_uniqueUpdater = &PlayScene::TurnChengeUpdate;
	_uniqueDrawer = &PlayScene::TurnChengeDraw;
	_turnChangeAnim->TurnStart(Team::enemy);
	_enemyCommander->TurnReset();
	_playerCommander->TurnReset();

	_uniqueUpdaterOld = _uniqueUpdater;

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
			_fadeEndFunc = &PlayScene::ChangeGameOver;
			StartFadeOut();
			return true;
		}
		else
		{
			// ゲームクリア
			Application::Instance().GetSaveData()->Save(_charactors, _mapId);
			_uniqueUpdater = &PlayScene::GameClearUpdate;
			_uniqueDrawer = &PlayScene::GameClearDraw;
			return true;
		}
	}

	_uniqueUpdater = _uniqueUpdaterOld;
	return true;
}

bool PlayScene::GameClearUpdate(const Input& input)
{
	_clearAnimTrack->Update();
	if (!_clearAnimTrack->GetEnd())return true;

	if(input.GetButtonDown(0, "ok") || input.GetButtonDown(1, "ok"))
	{
		_fadeEndFunc = &PlayScene::ChnageMapSelect;
		StartFadeOut();
		return false;
	}

	return true;
}

bool PlayScene::GameOverUpdate(const Input& input)
{
	_fadeTrack->Update();
	if (!_fadeTrack->GetEnd())return true;

	if (input.GetButtonDown(0, "ok") || input.GetButtonDown(1, "ok"))
	{
		_fadeEndFunc = &PlayScene::ChnageMapSelect;
		StartFadeOut();
		return false;
	}
	return true;
}

void PlayScene::StartFadeIn(const unsigned int color)
{
	_fadeColor = color;
	_fadeTrack->SetReverse(false);
	_fadeTrack->Reset();
	_uniqueUpdater = &PlayScene::FadeUpdate;
	_uniqueDrawer = &PlayScene::FadeDraw;
}

void PlayScene::StartFadeOut(const unsigned int color)
{
	_fadeColor = color;
	_fadeTrack->SetReverse(true);
	_fadeTrack->Reset();
	_uniqueUpdater = &PlayScene::FadeUpdate;
	_uniqueDrawer = &PlayScene::FadeDraw;
}

bool PlayScene::FadeUpdate(const Input& input)
{
	_fadeTrack->Update();
	if (_fadeTrack->GetEnd())
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
	_mapCtrl->Draw(*_camera, false);

	_preparationUI->BeginDraw();

	for (auto& charactor : _charactors)
	{
		charactor->Draw();
	}
	for (auto& effect : _effects)
	{
		effect->Draw();
	}

	(*_preparationDeque.begin())->Draw();
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
	// プレイヤーCursorの描画
	_playerCommander->Draw();
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

	auto fontHandle = Application::Instance().GetFileSystem()->GetFontHandle("choplin200");
	DrawStringToHandle(wsize.ToVector2Int() * 0.5f, Anker::center, 0x000088, fontHandle, "GAME OVER");

	if (_fadeTrack->GetEnd())return;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _fadeTrack->GetValue() * 255);
	DrawBox(Vector2Int(0, 0), Application::Instance().GetWindowSize().ToVector2Int(), _fadeColor);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
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

	auto animValue = _clearAnimTrack->GetValue();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, animValue * 255);
	DrawRotaGraph(wsize.ToVector2Int() * 0.5f, 1.0f, 0.0f, 
		Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/Battle/light.png"), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	auto fontHandle = Application::Instance().GetFileSystem()->GetFontHandle("choplin200");
	DrawStringToHandle(wsize.ToVector2Int() * 0.5f, Anker::center, 0xffff00, fontHandle, "GAME CLEAR");

	SetDrawScreen(DX_SCREEN_BACK);
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
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _fadeTrack->GetValue() * 255);
	DrawBox(Vector2Int(0, 0), Application::Instance().GetWindowSize().ToVector2Int(), _fadeColor);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void PlayScene::ChnageMapSelect()
{
	_controller.ChangeScene(make_shared<MapSelectScene>(_controller));
}

void PlayScene::ChangePreparation()
{
	_uniqueUpdater = &PlayScene::PreparationUpdate;
	_uniqueDrawer = &PlayScene::PreparationDraw;
}

void PlayScene::ChangeGameOver()
{
	_uniqueUpdater = &PlayScene::GameOverUpdate;
	_uniqueDrawer = &PlayScene::GameOverDraw;
	_fadeEndFunc = nullptr;
	_fadeTrack->Reset();
	_fadeTrack->SetReverse(false);
	_fadeColor = 0x000000;
}

void PlayScene::Draw(void)
{
	// 場面ごとの描画
	(this->*_uniqueDrawer)(*_camera);

	// 使用するピクセルシェーダーをセット
	SetUsePixelShader(pshandle);

	// 定数の設定
	SetPSConstF(0, _colorC);
	SetPSConstF(1, _waveC);

	// 描画
	//DrawPrimitive2DToShader(Vert, 6, DX_PRIMTYPE_TRIANGLELIST);

	if (debug)
	{
		DrawFormatString(0, 0, 0x000000, "%d", _effects.size());
	}

	//DrawPSTBuffer();
}

void PlayScene::DrawMovableMass()
{
}
