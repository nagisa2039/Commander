#include<DxLib.h>
#include <algorithm>
#include "PlayScene.h"
#include "../Utility/Input.h"
#include "../System/Application.h"
#include "../System/FileSystem.h"
#include "../Game/Camera.h"
#include "../Game/MapCtrl.h"

#include "Swordsman.h"
#include "Soldier.h"
#include "Warrior.h"
#include "Archer.h"
#include "Mage.h"

#include "PlayerCommander.h"
#include "EnemyCommander.h"
#include "Effect.h"
#include "FlyText.h"
#include "TurnChangeAnim.h"

using namespace std;

PlayScene::PlayScene(SceneController & ctrl):Scene(ctrl)
{
	_effects.clear();
	_charactors.clear();
	_charactors.reserve(30);


	debug = true;

	_mapCtrl = make_shared<MapCtrl>(_charactors);
	_camera = make_shared<Camera>(Rect(Vector2Int(), Application::Instance().GetWindowSize()));

	_turnChangeAnim = make_shared<TurnChangeAnim>();

	_playerCommander = make_shared<PlayerCommander>(_charactors, *_mapCtrl, Team::player);
	_enemyCommander = make_shared<EnemyCommander>(_charactors, *_mapCtrl, Team::enemy);

	_camera->AddTargetActor(_playerCommander);

	auto mapSize = _mapCtrl->GetMapCnt() * _mapCtrl->GetChipSize();
	_camera->SetLimitRect(Rect(mapSize.ToVector2Int() * 0.5, mapSize));

	_mapCtrl->LoadMap("map0");
	_mapCtrl->CreateCharactor(ctrl, _effects);

	_dyingCharItr = _charactors.end();

	_playerCommander->TurnReset();

	Application::Instance().GetFileSystem()->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin20", 20, 1, true, true);
	Application::Instance().GetFileSystem()->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin30", 30, 1, true, true);
	Application::Instance().GetFileSystem()->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin40", 40, 1, true, true);
	Application::Instance().GetFileSystem()->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin100", 100, 1, true, true);
	Application::Instance().GetFileSystem()->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin200", 200, 1, true, true);

	// プレイヤーターンを開始
	StartPlayerTurn();

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

	(this->*_uniqueUpdater)(input);

	_camera->Update();
	for (auto& effect : _effects)
	{
		effect->Update(input);
	}
	auto newEffectEnd = remove_if(_effects.begin(), _effects.end(),
		[](const std::shared_ptr<Effect>& effect) { return effect->GetDelete(); });
	_effects.erase(newEffectEnd, _effects.end());
}

void PlayScene::PlayerTurnUpdate(const Input& input)
{
	CharactorUpdate(input);

	_turnChangeAnim->Update(input);
	if (!_turnChangeAnim->GetAnimEnd()) return;

	if (_playerCommander->CheckEnd())
	{
		StartEnemyTurn();
		return;
	}
	_playerCommander->Update(input);
}

void PlayScene::CharactorUpdate(const Input& input)
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
}

void PlayScene::MakePSTBuffer(const int targetHandle)
{
	if (targetHandle == -1) return;

	Size buffSize;
	GetGraphSize(targetHandle, &buffSize.w, &buffSize.h);

	// 縮小バッファをtargetBfの半分のサイズで作成
	Size shrinkSize = Size(buffSize.w / 2, buffSize.h);
	_shrinkBf = MakeScreen(shrinkSize.w, shrinkSize.h, true);

	// _shrinkBfへの描画
	SetDrawScreen(_shrinkBf);
	ClsDrawScreen();

	int drawY = 0;
	int shrinkCnt = 4;

	shrinkSize.h /= 2;
	for (int i = 0; i < shrinkCnt; i++)
	{
		DrawExtendGraph(0, drawY, shrinkSize.w, drawY + shrinkSize.h, targetHandle, true);
		drawY += shrinkSize.h;
		shrinkSize *= 0.5f;
	}

	// 縮小バッファを拡大して加算
	SetDrawScreen(targetHandle);

	GraphFilter(_shrinkBf, DX_GRAPH_FILTER_GAUSS, 8, 100);

	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);

	shrinkSize = buffSize * 0.5f;
	drawY = 0;
	for (int i = 0; i < shrinkCnt; i++)
	{
		DrawRectExtendGraph(0, 0, buffSize.w - 1, buffSize.h - 1, 
			0, drawY, shrinkSize.w, shrinkSize.h, _shrinkBf, true);
		drawY += shrinkSize.h;
		shrinkSize *= 0.5f;
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	SetDrawScreen(DX_SCREEN_BACK);

	//GraphFilter(targetHandle, DX_GRAPH_FILTER_GAUSS, 8, 100);
}

void PlayScene::DrawPSTBuffer()
{
	auto wsize = Application::Instance().GetWindowSize();
	DrawRotaGraph(wsize.w/2, wsize.h/2, 1.0f, 0.0f, _targetBf, true);
	DrawGraph(0, 0, _shrinkBf, true);
}

void PlayScene::StartPlayerTurn()
{
	_uniqueUpdater = &PlayScene::PlayerTurnUpdate;
	_uniqueDrawer = &PlayScene::PlayerTurnDraw;
	_turnChangeAnim->TurnStart(Team::player);
	_enemyCommander->TurnReset();

	_uniqueUpdaterOld = _uniqueUpdater;
}

void PlayScene::StartEnemyTurn()
{
	_uniqueUpdater = &PlayScene::EnemyTurnUpdate;
	_uniqueDrawer = &PlayScene::EnemyTurnDraw;
	_turnChangeAnim->TurnStart(Team::enemy);
	_playerCommander->TurnReset();

	_uniqueUpdaterOld = _uniqueUpdater;
}

void PlayScene::EnemyTurnUpdate(const Input& input)
{
	CharactorUpdate(input);

	_turnChangeAnim->Update(input);
	if (!_turnChangeAnim->GetAnimEnd()) return;

	if (_enemyCommander->CheckEnd())
	{
		StartPlayerTurn();
		return;
	}
	_enemyCommander->Update(input);
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
			_uniqueUpdater = &PlayScene::GameOverUpdate;
			_uniqueDrawer = &PlayScene::GameOverDraw;
			return;
		}
		else
		{
			// ゲームクリア
			_uniqueUpdater = &PlayScene::GameClearUpdate;
			_uniqueDrawer = &PlayScene::GameClearDraw;
			return;
		}
	}

	_uniqueUpdater = _uniqueUpdaterOld;
}

void PlayScene::GameClearUpdate(const Input& input)
{
}

void PlayScene::GameOverUpdate(const Input& input)
{
}

void PlayScene::PlayerTurnDraw(const Camera& camera)
{
	// プレイヤーCursorの描画
	_playerCommander->Draw(*_camera);
}

void PlayScene::EnemyTurnDraw(const Camera& camera)
{
}

void PlayScene::GameOverDraw(const Camera& camera)
{
	auto str = "GAME OVER";
	auto fontHandle = Application::Instance().GetFileSystem()->GetFontHandle("choplin200");
	auto wsize = Application::Instance().GetWindowSize();
	auto drawPos = GetDrawPos(wsize.ToVector2Int() * 0.5f, GetStringSizseToHandle(str, fontHandle), Anker::center);
	DrawFormatStringToHandle(drawPos.x, drawPos.y, 0x4000ff, fontHandle, str);
}

void PlayScene::GameClearDraw(const Camera& camera)
{
	auto str = "GAME CLER";
	auto fontHandle = Application::Instance().GetFileSystem()->GetFontHandle("choplin200");
	auto wsize = Application::Instance().GetWindowSize();
	auto drawPos = GetDrawPos(wsize.ToVector2Int() * 0.5f, GetStringSizseToHandle(str, fontHandle), Anker::center);
	DrawFormatStringToHandle(drawPos.x, drawPos.y, 0xffff00, fontHandle, str);
}

Size PlayScene::GetStringSizseToHandle(const std::string& str, const int fontHandle)
{
	Size strSize;
	int lineCnt;
	GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &lineCnt, fontHandle, str.c_str());
	return strSize;
}

void PlayScene::Draw(void)
{
	_mapCtrl->Draw(*_camera);
	for (auto& charactor : _charactors)
	{
		charactor->Draw(*_camera);
	}
	for (auto& effect : _effects)
	{
		effect->Draw(*_camera);
	}

	// 場面ごとの描画
	(this->*_uniqueDrawer)(*_camera);

	// ターン交代のエフェクト描画
	_turnChangeAnim->Draw(*_camera);

	// 使用するピクセルシェーダーをセット
	SetUsePixelShader(pshandle);

	// 定数の設定
	SetPSConstF(0, _colorC);
	SetPSConstF(1, _waveC);

	// 描画
	//DrawPrimitive2DToShader(Vert, 6, DX_PRIMTYPE_TRIANGLELIST);

	if (debug)
	{
	}

	//DrawPSTBuffer();
}