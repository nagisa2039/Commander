#pragma once
#include <memory>
#include <list>
#include <vector>
#include <array>
#include "Scene.h"
#include "../Utility/Geometry.h"
#include "../Game/Team.h"
#include <list>
#include <string>
#include "TimeLine.h"
#include <functional>

class Charactor;
class MapCtrl;
class Camera;
class Commander;
class Effect;
class TurnChangeAnim;
class PreparationUI;
class UI;

class PlayScene :
	public Scene
{
public:
	enum class FilterType
	{
		none,
		gauss,
		max
	};
private:
	bool debug;
	int _gameH;
	
	std::unique_ptr<MapCtrl> _mapCtrl;
	std::unique_ptr<Camera> _camera;

	unsigned char _turnCnt;
	const unsigned char _lastTurnCnt;

	std::unique_ptr<TurnChangeAnim> _turnChangeAnim;

	std::vector<std::shared_ptr<Charactor>> _charactors;
	std::vector<std::shared_ptr<Charactor>>::iterator _dyingCharItr;
	std::unique_ptr<Commander> _playerCommander;
	std::unique_ptr<Commander> _enemyCommander;
	std::vector<std::shared_ptr<Effect>> _effects;

	std::deque<std::shared_ptr<UI>> _preparationDeque;
	std::shared_ptr<PreparationUI> _preparationUI;

	std::unique_ptr<Track<float>> _clearAnimTrack;

	std::function<void()>_fadeEndFunc;

	FilterType _filterType;
	std::array<std::function<void()>, static_cast<size_t>(FilterType::max)> _filterFuncs;

	// 場面ごとの更新を行う関数ポインタ playSceneを継続するかを返す
	bool(PlayScene::*_updater)(const Input& input);
	bool(PlayScene::* _updaterOld)(const Input& input);

	// 場面ごとの更新を行う関数ポインタ
	void(PlayScene::* _drawer)();
	void(PlayScene::* _UIDrawer)();

	void StartPlayerTurn();
	void StartEnemyTurn();

	// 準備画面
	bool PreparationUpdate(const Input& input);
	bool TurnChengeUpdate(const Input& input);
	bool PlayerTurnUpdate(const Input& input);
	bool CharactorUpdate(const Input& input);
	bool EnemyTurnUpdate(const Input& input);

	bool CharactorDyingUpdate(const Input& input);

	void GameClear();

	void GameOver();

	bool GameClearUpdate(const Input& input);
	bool GameOverUpdate(const Input& input);

	void StartFadeIn(std::function<void()>, const unsigned int color = 0x000000, void(PlayScene::* nextDrawer)() = &PlayScene::BaseDraw);
	void StartFadeOut(std::function<void()>, const unsigned int color = 0x000000, void(PlayScene::* nextDrawer)() = &PlayScene::BaseDraw);

	bool FadeUpdate(const Input& input);

	void PreparationDraw();
	void TurnChengeDraw();
	void PlayerTurnDraw();
	void EnemyTurnDraw();
	void GameOverDraw();
	void GameClearDraw();
	void BaseDraw();

	void PreparationUIDraw();
	void PlayerUIDraw();
	void NoneUIDraw();

	// Fade終了時に実行する関数
	void ChnageMapSelect();
	void ChangePreparation();
	void ChangeGameOver();

	/*int pshandle;
	VERTEX2DSHADER Vert[6];
	FLOAT4 _colorC;
	FLOAT4 _waveC;*/

public:
	PlayScene(SceneController & ctrl, const unsigned int mapId, const bool ai = false);
	~PlayScene();

	void Update(const Input& input) override final;
	void Draw(void) override final;

	void On()override;
	void Off()override;

	void PushShopScene();

	// ショップなどで装備品などが変更されたときに呼ぶ
	void CharactorDataUpdate();

	void SetFilter(const FilterType type);
};