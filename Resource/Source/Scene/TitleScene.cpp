#include "TitleScene.h"
#include "FileSystem.h"
#include "SoundLoader.h"
#include <Dxlib.h>
#include "DxLibUtility.h"
#include "Fade.h"
#include "Input.h"
#include "Application.h"
#include "MapSelectScene.h"
#include "DataBase.h"
#include "Tool.h"
#include "PlayScene.h"

namespace
{
	constexpr float FALL_SPEED = 8.0f;
	constexpr float ROTATE_SPEED = 5.0f;
}

void TitleScene::NormalUpdate(const Input& input)
{
	// デモへの移行
	_demoSceneCnt->Update();
	if (_demoSceneCnt->GetEnd())
	{
		SoundL.PlaySE("Resource/Sound/SE/ok2.mp3");
		_fadeEndFunc = [&controller = _controller]()
		{
			auto mapMax = DataBase::Instance().GetMapDataTable().size();
			controller.ChangeScene(std::make_shared<PlayScene>(controller, mt()% mapMax, true));
		};
		_controller.GetFade().StartFadeOut();
		_updater = &TitleScene::FadeUpdate;
	}

	_animTrack->Update();
	if (input.GetButtonDown("ok"))
	{
		SoundL.PlaySE("Resource/Sound/SE/ok2.mp3");
		_fadeEndFunc = [&controller = _controller]() 
		{
			controller.ChangeScene(std::make_shared<MapSelectScene>(controller));
		};
		_controller.GetFade().StartFadeOut();
		_updater = &TitleScene::FadeUpdate;
	}
}

void TitleScene::FadeUpdate(const Input& input)
{
	if (_controller.GetFade().GetEnd())
	{
		_fadeEndFunc();
	}
}

TitleScene::TitleScene(SceneController& controller)
	:Scene(controller)
{
	_updater = &TitleScene::FadeUpdate;
	_fadeEndFunc = [&updater = _updater]() {updater = &TitleScene::NormalUpdate; };
	_controller.GetFade().StartFadeIn();
	_bgH = ImageHandle("Resource/Image/UI/mapSelectBG.jpg");
	_titleH = ImageHandle("Resource/Image/Title/commander.png");
	_bgmH = SoundHandle("Resource/Sound/BGM/title.mp3");
	_start_bksH = ImageHandle("Resource/Image/Title/title_bks.png");
	SoundL.PlayBGM(_bgmH);

	_animTrack = std::make_unique<Track<float>>(true);
	_animTrack->AddKey(0, 0.0f);
	_animTrack->AddKey(30, 1.0f);
	_animTrack->AddKey(60, 0.0f);

	_charCreateTrack = std::make_unique<Track<float>>();
	_charCreateTrack->AddKey(0, 0.0f);
	_charCreateTrack->AddKey(5, 0.0f);

	_demoSceneCnt = std::make_unique<Track<float>>();
	_demoSceneCnt->AddKey(0, 0.0f);
	_demoSceneCnt->AddKey(600, 0.0f);

	_charInfVec.resize(100);
	for (auto& charInf : _charInfVec)
	{
		charInf.active = false;
	}
}

TitleScene::~TitleScene()
{
	SoundL.StopSound(_bgmH);
}

void TitleScene::Update(const Input& input)
{
	CharactorUpdate();
	(this->*_updater)(input);
}

void TitleScene::CharactorUpdate()
{
	auto wsize = Application::Instance().GetWindowSize();
	for (auto& charInf : _charInfVec)
	{
		if (!charInf.active)continue;
		charInf.pos.y += FALL_SPEED;
		charInf.angle += ROTATE_SPEED * DX_PI_F / 180.0f;
		if (charInf.pos.y >= wsize.h + 200)
		{
			charInf.active = false;
		}
	}

	_charCreateTrack->Update();
	if (_charCreateTrack->GetEnd())
	{
		_charCreateTrack->Reset();
		auto charTypeMax = DataBase::Instance().GetCharactorDataTable().size();
		for (auto& charInf : _charInfVec)
		{
			if (charInf.active)continue;

			charInf.active = true;
			charInf.team = static_cast<Team>(mt() % static_cast<int>(Team::max));
			charInf.type = static_cast<CharactorType>(mt() % charTypeMax);
			charInf.pos = Vector2(static_cast<float>(mt() % wsize.w), -200.0);
			charInf.angle = (mt() % 360) * DX_PI_F / 180.0f;
			break;
		}
	}
	std::sort(_charInfVec.begin(), _charInfVec.end(), [](const CharInf& left, const CharInf& right)
		{
			return left.pos.y < right.pos.y;
		});
}

void TitleScene::Draw(void)
{
	auto wsize = Application::Instance().GetWindowSize();
	auto& dataBase = DataBase::Instance();
	auto drawObject = [&wsize, &charInfVec = _charInfVec, &dataBase, titleH = _titleH](const Vector2Int& offset = Vector2Int(0,0))
	{
		for (auto& charInf : charInfVec)
		{
			if (!charInf.active)continue;
			auto handle = dataBase.GetCharactorImageHandle(charInf.type, charInf.team);
			DrawRectRotaGraph(charInf.pos.ToVector2Int() + offset, Vector2Int(32, 0), Size(32, 32), 5.0, charInf.angle, handle, true);
			DrawRotaGraph(wsize.ToVector2Int() * 0.5f + offset, 1.0, 0.0, titleH, true);
		}
	};

	SetDrawBright(32, 32, 32);
	DrawGraph(0,0,_bgH, false);

	auto offset = Vector2Int(20, 20);
	SetDrawBright(0, 0, 0);
	SetDrawMode(DX_DRAWMODE_NEAREST);
	drawObject(offset);
	SetDrawBright(255, 255, 255);
	drawObject();
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	Vector2Int center = Vector2Int(wsize.w / 2, wsize.h - 150);
	SetDrawBlendMode(DX_BLENDMODE_ADD, static_cast<int>(_animTrack->GetValue() * 255));
	SetDrawBright(10, 10, 255);
	DrawRotaGraph(center, 1.5, 0.0, _start_bksH, true);
	SetDrawBright(255, 255, 255);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	int fontH = FontHandle("choplin60edge");
	DrawStringToHandle(center, Anker::center, 0xffffff, fontH, "開始");
}
