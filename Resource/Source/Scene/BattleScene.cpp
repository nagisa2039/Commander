#include "BattleScene.h"
#include "../Game/Charactor.h"
#include "DxLibUtility.h"
#include "../System/Application.h"
#include "../Utility/Input.h"
#include "../System/FileSystem.h"
#include "Effect/Effect.h"
#include "Camera.h"
#include <DxLib.h>
#include "Effect/FlyText.h"
#include "BattleCharactor.h"
#include "SoundLoader.h"
#include "CutIn.h"
#include "PlayScene.h"

using namespace std;

void BattleScene::SceneStartAnim(const Input& input)
{
	if (_exRateTL->GetEnd())
	{
		_leftBC.StartAttackAnim(*this);
		_updater = &BattleScene::LeftTurn;
	}
}

void BattleScene::SceneEndAnim(const Input& input)
{
	_brightTL->Update();
	if (_brightTL->GetEnd())
	{
		End();
		return;
	}
}

void BattleScene::LeftTurn(const Input& input)
{
	_leftBC.Update(*this);
	if (_leftBC.GetAttackAnimEnd())
	{
		_rightBC.StartHPAnim();
		_updater = &BattleScene::RightHPAnim;
	}
}

void BattleScene::LeftHPAnim(const Input& input)
{
	_leftBC.UIAnimUpdate();
	if (_leftBC.GetHPAnimEnd())
	{
		// ����ł�����I���
		if (_leftBC.GetCharacotr().GetIsDying())
		{
			_updater = &BattleScene::SceneEndAnim;
			return;
		}

		// �ǌ�����
		if (_pursuit)
		{
			// �ǌ��ς݂Ȃ̂Ő퓬���I������
			_updater = &BattleScene::SceneEndAnim;
			return;
		}
		else
		{
			PursuitAttack();
		}
	}
}

void BattleScene::RightTurn(const Input& input)
{
	_rightBC.Update(*this);
	if (_rightBC.GetAttackAnimEnd())
	{
		_leftBC.StartHPAnim();
		_updater = &BattleScene::LeftHPAnim;
	}
}

void BattleScene::RightHPAnim(const Input& input)
{
	_rightBC.UIAnimUpdate();
	if (_rightBC.GetHPAnimEnd())
	{
		_rightBC.SetReceiveDamageType(BattleCharactor::damageType::none);
		if (_leftBC.GetCharacotr().GetBattleStatus().CheckHeal())
		{
			_updater = &BattleScene::SceneEndAnim;
			return;
		}

		// ����ł�����I���
		if (_rightBC.GetCharacotr().GetIsDying())
		{
			_updater = &BattleScene::SceneEndAnim;
			return;
		}

		// �ǌ�����
		if (_pursuit)
		{
			_updater = &BattleScene::SceneEndAnim;
			return;
		}

		// �U���͈͓����m�F
		auto mapPosSub = _leftBC.GetCharacotr().GetMapPos() - _rightBC.GetCharacotr().GetMapPos();
		if (_rightBC.GetCharacotr().GetAttackRange().Hit(abs(mapPosSub.x) + abs(mapPosSub.y)) && !_rightBC.GetCharacotr().GetBattleStatus().CheckHeal())
		{
			_rightBC.StartAttackAnim(*this);
			_updater = &BattleScene::RightTurn;
		}
		else
		{
			PursuitAttack(false);
		}
	}
}

bool BattleScene::PursuitAttack(const bool rightAttack)
{
	auto leftBattleStatus = _leftBC.GetCharacotr().GetBattleStatus();
	auto rightBattleStatus = _rightBC.GetCharacotr().GetBattleStatus();
	if (leftBattleStatus.CheckPursuit(rightBattleStatus))
	{
		_leftBC.StartAttackAnim(*this);
		_updater = &BattleScene::LeftTurn;
		_pursuit = true;
		return true;
	}
	else
	{
		if (rightAttack && rightBattleStatus.CheckPursuit(leftBattleStatus))
		{
			_rightBC.StartAttackAnim(*this);
			_updater = &BattleScene::RightTurn;
			_pursuit = true;
			return true;
		}
	}

	// �ǌ��s�Ȃ̂ŏI��
	_updater = &BattleScene::SceneEndAnim;
	return false;
}

void BattleScene::End()
{
	SoundL.StopAllSound();

	_leftBC.GetCharacotr().MoveEnd();
	_controller.PopScene();
}

BattleScene::BattleScene(BattleCharactor& leftBC, BattleCharactor& rightBC, SceneController& ctrl, const Vector2Int& cameraOffset)
	: Scene(ctrl), _screenSize(Application::Instance().GetWindowSize()), _leftBC(leftBC), _rightBC(rightBC), 
	_startPos(cameraOffset + (leftBC.GetCharacotr().GetCenterPos() + rightBC.GetCharacotr().GetCenterPos()).ToVector2Int()*0.5f)
{
	auto wsize = Application::Instance().GetWindowSize();
	auto cameraPos = wsize.ToVector2Int() * 0.5;
	_camera = make_shared<Camera>(Rect{ cameraPos, wsize });
	_camera->SetPos(Vector2{ (cameraPos).ToVector2() });

	_screenH = FileSystem::Instance().
		MakeScreen("battle_scene_screen", _screenSize, true);

	_effects.clear();

	_playScene = &dynamic_cast<PlayScene&>(Application::Instance().GetSceneController().GetCurrentScene());
	assert(_playScene);
	_playScene->SetFilter(PlayScene::FilterType::gauss);

	_groundY = _screenSize.h / 2.0f;
	auto screenCenter = _screenSize.ToVector2Int() * 0.5f;

	auto distance = 0;
	auto mapPosSub = _leftBC.GetCharacotr().GetMapPos() - _rightBC.GetCharacotr().GetMapPos();
	distance = abs(mapPosSub.x) + abs(mapPosSub.y) <= 1 ? 200 : 300;

	_leftBC.Init(Vector2{ static_cast<float>(screenCenter.x - distance), _groundY }, Dir::left, &rightBC);
	_rightBC.Init(Vector2{ static_cast<float>(screenCenter.x + distance), _groundY }, Dir::right, &leftBC);

	_pursuit = false;

	_exRateTL = make_unique<Track<float>>();
	_exRateTL->ClearKey();
	_exRateTL->AddKey(0,0.0f);
	_exRateTL->AddKey(60, 1.0f);

	_brightTL = make_unique<Track<float>>();
	_brightTL->AddKey(0, 1.0f);
	_brightTL->AddKey(30, 0.0f);

	_updater = &BattleScene::SceneStartAnim;
	
	auto& soundLoader = SoundL;
	if (_leftBC.GetCharacotr().GetBattleStatus().CheckHeal()
		&& _rightBC.GetCharacotr().GetTeam() == _leftBC.GetCharacotr().GetTeam())
	{
		_bgmH = soundLoader.GetSoundHandle("Resource/Sound/BGM/heal.mp3");
	}
	else
	{
		_bgmH = soundLoader.GetSoundHandle("Resource/Sound/BGM/battle.mp3");
	}
	soundLoader.PlayBGM(_bgmH);
}

BattleScene::~BattleScene()
{
	_playScene->SetFilter(PlayScene::FilterType::none);
	SoundL.StopSound(_bgmH);
}

void BattleScene::Update(const Input& input)
{
	_exRateTL->Update();

	_camera->Update();

	_leftBC.AnimUpdate();
	_rightBC.AnimUpdate();

	if (_cutIn)
	{
		_cutIn->Update();
		if (_cutIn->GetEnd())
		{
			_cutIn = nullptr;
		}
		return;
	}

	(this->*_updater)(input);

	if (_effects.size() > 0)
	{
		for (auto& effect : _effects)
		{
			effect->Update(input);
		}

		auto newEnd = remove_if(_effects.begin(), _effects.end(),
			[](const std::shared_ptr<Effect>& effect) { return effect->GetDelete(); });
		_effects.erase(newEnd, _effects.end());
	}
}

void BattleScene::Draw(void)
{
	int currentScreen = GetDrawScreen();
	SetDrawScreen(_screenH);
	ClsDrawScreen();

	auto screenCenter = _screenSize.ToVector2Int() * 0.5f;

	// ���̕`��
	DrawFloor(screenCenter);

	auto charSize = Size{ 128, 128 };

	_leftBC.Draw();
	_rightBC.Draw();

	for (auto& effect : _effects)
	{
		effect->Draw();
	}

	if (_cutIn)
	{
		_cutIn->Draw();
	}

	auto wsize = Application::Instance().GetWindowSize();
	auto exRateValue = _exRateTL->GetValue();

	SetDrawScreen(currentScreen);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>((exRateValue * 128)));
	DrawBox(Vector2Int{ 0, 0 }, wsize.ToVector2Int(), 0x000000);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	DrawRotaGraph(Lerp(_startPos, Vector2Int{ wsize.w / 2, wsize.h / 2 }, exRateValue), exRateValue, exRateValue * 4.0 * DX_PI, _screenH, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>((1.0f - _brightTL->GetValue()) * 255));
	DrawBox(0,0, wsize.w, wsize.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void BattleScene::DrawFloor(Vector2Int& screenCenter)
{
	auto mapPosSub = _leftBC.GetCharacotr().GetMapPos() - _rightBC.GetCharacotr().GetMapPos();
	if (abs(mapPosSub.x) + abs(mapPosSub.y) <= 1)
	{
		int floorH = ImageHandle("Resource/Image/Battle/floor_big.png");
		DrawRotaGraph(Vector2Int{ screenCenter.x, static_cast<int>(_groundY) }, 1.0, 0.0, floorH, true);
	}
	else
	{
		int floorH = ImageHandle("Resource/Image/Battle/floor_small.png");
		DrawRotaGraph(_leftBC.GetStartPos(), 1.0, 0.0, floorH, true);
		DrawRotaGraph(_rightBC.GetStartPos(), 1.0, 0.0, floorH, true);
	}
}

std::vector<std::shared_ptr<Effect>>& BattleScene::GetEffectVec()
{
	return _effects;
}

void BattleScene::SetCutIn(std::shared_ptr<CutIn> c)
{
	_cutIn = c;
}
