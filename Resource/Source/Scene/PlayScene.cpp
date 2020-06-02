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

#include "Charactor.h"

#include "PlayerCommander.h"
#include "EnemyCommander.h"
#include "Effect/Effect.h"
#include "Effect/FlyText.h"
#include "TurnChangeAnim.h"

using namespace std;

PlayScene::PlayScene(SceneController & ctrl):Scene(ctrl)
{
	Application::Instance().GetFileSystem()->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin20", 20, 1, true, true);
	Application::Instance().GetFileSystem()->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin30", 30, 1, true, true);
	Application::Instance().GetFileSystem()->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin40", 40, 1, true, true);
	Application::Instance().GetFileSystem()->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin50", 50, 1, true, true);
	Application::Instance().GetFileSystem()->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin60", 60, 1, true, true);
	Application::Instance().GetFileSystem()->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin70", 70, 1, true, true);
	Application::Instance().GetFileSystem()->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin80", 80, 1, true, true);
	Application::Instance().GetFileSystem()->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin100", 100, 1, true, true);
	Application::Instance().GetFileSystem()->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin200", 200, 1, true, true);
	Application::Instance().GetFileSystem()->FontInit("Resource/Font/ShipporiMincho-TTF-Regular.ttf", "ShipporiMincho", "shippori20", 20, 1, true, true);
	Application::Instance().GetFileSystem()->FontInit("Resource/Font/ShipporiMincho-TTF-Regular.ttf", "ShipporiMincho", "shippori30", 30, 1, true, true);
	Application::Instance().GetFileSystem()->FontInit("Resource/Font/ShipporiMincho-TTF-Regular.ttf", "ShipporiMincho", "shippori40", 40, 1, true, true);
	Application::Instance().GetFileSystem()->FontInit("Resource/Font/ShipporiMincho-TTF-Regular.ttf", "ShipporiMincho", "shippori200", 200, 1, true, true);
	Application::Instance().GetFileSystem()->FontInit("Resource/Font/Oshidashi - M - Gothic - TT.ttf", "Oshidasi", "oshidashi200", 200, 1, true, true);

	_effects.clear();
	_charactors.clear();
	_charactors.reserve(30);

	debug = true;

	_mapCtrl = make_shared<MapCtrl>(_charactors);
	_camera = make_shared<Camera>(Rect(Vector2Int(), Application::Instance().GetWindowSize()));

	_turnChangeAnim = make_shared<TurnChangeAnim>();

	_playerCommander = make_shared<PlayerCommander>(_charactors, *_mapCtrl, Team::player, *_camera);
	_enemyCommander = make_shared<EnemyCommander>(_charactors, *_mapCtrl, Team::enemy, *_camera);

	list<shared_ptr<PlayerCommander>> testList;

	testList.emplace_back(make_shared<PlayerCommander>(_charactors, *_mapCtrl, Team::player, *_camera));
	PlayerCommander& test2 = **testList.begin();
	testList.clear();

	test2.CheckEnd();

	_camera->AddTargetActor(&*_playerCommander);

	auto mapSize = _mapCtrl->GetMapSize() * _mapCtrl->GetChipSize();
	_camera->SetLimitRect(Rect(mapSize.ToVector2Int() * 0.5, mapSize));

	_mapCtrl->LoadMap("map0.map");
	_mapCtrl->LoadCharactorData();
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
	_uniqueUpdater = &PlayScene::PreparationUpdate;
	_uniqueDrawer = &PlayScene::PreparationDraw;
	Vector2 cameraPos2D = (cursorMapPos * _mapCtrl->GetChipSize()).ToVector2();
	_camera->SetPos(Vector3(cameraPos2D.x, cameraPos2D.y, 0));

	// �s�N�Z���V�F�[�_�[�o�C�i���R�[�h�̓ǂݍ���
	pshandle = LoadPixelShader("Resource/Source/Shader/HPBer.cso");

	// ���_�f�[�^�̏���
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
	// �f�o�b�O
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

void PlayScene::PreparationUpdate(const Input& input)
{
	(*_preparationDeque.begin())->Update(input);
	if (_preparationUI->GetDelete())
	{
		// �v���C���[�^�[�����J�n
		StartPlayerTurn();
	}
}

void PlayScene::TurnChengeUpdate(const Input& input)
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
}

void PlayScene::PlayerTurnUpdate(const Input& input)
{
	CharactorUpdate(input);

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
	// dying���I����Ă��邩�m�F
	if (!(*_dyingCharItr)->GetDelete())
	{
		// �I����Ă��Ȃ��̂ōX�V����
		(*_dyingCharItr)->Update(input);
		return;
	}

	// �폜����
	_charactors.erase(_dyingCharItr);
	_dyingCharItr = _charactors.end();

	array<int, static_cast<size_t>(Team::max)> teamCntArray;
	teamCntArray.fill(0);
	for (auto& charactor : _charactors)
	{
		teamCntArray[static_cast<size_t>(charactor->GetTeam())]++;
	}

	// �Q�[���N���A�ƃQ�[���I�[�o�[�̔���
	for (int i = 0; i < teamCntArray.size(); i++)
	{
		if (teamCntArray[i] > 0)
		{
			continue;
		}

		if (static_cast<Team>(i) == Team::player)
		{
			// �Q�[���I�[�o�[
			_uniqueUpdater = &PlayScene::GameOverUpdate;
			_uniqueDrawer = &PlayScene::GameOverDraw;
			return;
		}
		else
		{
			// �Q�[���N���A
			_uniqueUpdater = &PlayScene::GameClearUpdate;
			_uniqueDrawer = &PlayScene::GameClearDraw;
			return;
		}
	}

	_uniqueUpdater = _uniqueUpdaterOld;
}

void PlayScene::GameClearUpdate(const Input& input)
{
	_mapCtrl->SaveCharactorData();
}

void PlayScene::GameOverUpdate(const Input& input)
{
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
	// �v���C���[Cursor�̕`��
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
	_mapCtrl->Draw(*_camera);

	for (auto& charactor : _charactors)
	{
		charactor->Draw();
	}
	for (auto& effect : _effects)
	{
		effect->Draw();
	}

	auto str = "GAME OVER";
	auto fontHandle = Application::Instance().GetFileSystem()->GetFontHandle("choplin200");
	auto wsize = Application::Instance().GetWindowSize();
	auto drawPos = GetDrawPos(wsize.ToVector2Int() * 0.5f, GetStringSizseToHandle(str, fontHandle), Anker::center);
	DrawFormatStringToHandle(drawPos.x, drawPos.y, 0x4000ff, fontHandle, str);
}

void PlayScene::GameClearDraw(const Camera& camera)
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
	// ��ʂ��Ƃ̕`��
	(this->*_uniqueDrawer)(*_camera);

	// �g�p����s�N�Z���V�F�[�_�[���Z�b�g
	SetUsePixelShader(pshandle);

	// �萔�̐ݒ�
	SetPSConstF(0, _colorC);
	SetPSConstF(1, _waveC);

	// �`��
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
