#pragma once
#include <memory>
#include <list>
#include <vector>
#include "Scene.h"
#include "../Utility/Geometry.h"
#include "../Game/Team.h"
#include <list>
#include <string>
#include "TimeLine.h"

class Charactor;
class MapCtrl;
class Camera;
class Commander;
class Effect;
class TurnChangeAnim;
class PreparationUI;
class UI;
class Fade;

class PlayScene :
	public Scene
{
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

	std::unique_ptr<Fade> _fade;
	void(PlayScene::* _fadeEndFunc)();

	// 場面ごとの更新を行う関数ポインタ playSceneを継続するかを返す
	bool(PlayScene::*_uniqueUpdater)(const Input& input);
	bool(PlayScene::* _uniqueUpdaterOld)(const Input& input);

	// 場面ごとの更新を行う関数ポインタ
	void(PlayScene::* _uniqueDrawer)(const Camera& camera);

	void StartPlayerTurn();
	void StartEnemyTurn();

	// 準備画面
	bool PreparationUpdate(const Input& input);
	bool TurnChengeUpdate(const Input& input);
	bool PlayerTurnUpdate(const Input& input);
	bool CharactorUpdate(const Input& input);
	bool EnemyTurnUpdate(const Input& input);

	bool CharactorDyingUpdate(const Input& input);

	bool GameClearUpdate(const Input& input);
	bool GameOverUpdate(const Input& input);

	void StartFadeIn( void (PlayScene::*funcP)(), const unsigned int color = 0x000000);
	void StartFadeOut(void (PlayScene::* funcP)(), const unsigned int color = 0x000000);

	bool FadeUpdate(const Input& input);

	void PreparationDraw(const Camera& camera);
	void TurnChengeDraw(const Camera& camera);
	void PlayerTurnDraw(const Camera& camera);
	void EnemyTurnDraw(const Camera& camera);
	void GameOverDraw(const Camera& camera);
	void GameClearDraw(const Camera& camera);
	void FadeDraw(const Camera& camera);

	// Fade終了時に実行する関数
	void ChnageMapSelect();
	void ChangePreparation();
	void ChangeGameOver();

	/*int pshandle;
	VERTEX2DSHADER Vert[6];
	FLOAT4 _colorC;
	FLOAT4 _waveC;*/

public:
	PlayScene(SceneController & ctrl, const unsigned int mapId);
	~PlayScene();

	void Update(const Input& input) override final;
	void Draw(void) override final;

	void PushShopScene();
};