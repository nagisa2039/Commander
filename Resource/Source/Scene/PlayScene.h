#pragma once
#include <memory>
#include <list>
#include <vector>
#include "Scene.h"
#include "../Utility/Geometry.h"
#include "../Game/Team.h"
#include <list>
#include <string>

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
private:
	bool debug;
	int _gameH;	// ゲーム画面のハンドル
	
	std::shared_ptr<MapCtrl> _mapCtrl;
	std::shared_ptr<Camera> _camera;

	std::shared_ptr<TurnChangeAnim> _turnChangeAnim;

	std::vector<std::shared_ptr<Charactor>> _charactors;
	std::vector<std::shared_ptr<Charactor>>::iterator _dyingCharItr;
	std::shared_ptr<Commander> _playerCommander;
	std::shared_ptr<Commander> _enemyCommander;
	std::vector<std::shared_ptr<Effect>> _effects;

	std::deque<std::shared_ptr<UI>> _preparationDeque;
	std::shared_ptr<PreparationUI> _preparationUI;

	// 場面ごとの更新を行う関数ポインタ
	void(PlayScene::*_uniqueUpdater)(const Input& input);
	void(PlayScene::* _uniqueUpdaterOld)(const Input& input);

	// 場面ごとの更新を行う関数ポインタ
	void(PlayScene::* _uniqueDrawer)(const Camera& camera);

	void StartPlayerTurn();
	void StartEnemyTurn();

	// 準備画面
	void PreparationUpdate(const Input& input);
	void TurnChengeUpdate(const Input& input);
	void PlayerTurnUpdate(const Input& input);
	void CharactorUpdate(const Input& input);
	void EnemyTurnUpdate(const Input& input);

	void CharactorDyingUpdate(const Input& input);

	void GameClearUpdate(const Input& input);
	void GameOverUpdate(const Input& input);

	void PreparationDraw(const Camera& camera);
	void TurnChengeDraw(const Camera& camera);
	void PlayerTurnDraw(const Camera& camera);
	void EnemyTurnDraw(const Camera& camera);
	void GameOverDraw(const Camera& camera);
	void GameClearDraw(const Camera& camera);

	Size GetStringSizseToHandle(const std::string& str, const int fontHandle);

	int pshandle;
	VERTEX2DSHADER Vert[6];
	FLOAT4 _colorC;
	FLOAT4 _waveC;

public:
	PlayScene(SceneController & ctrl);
	~PlayScene();

	void Update(const Input& input) override final;
	void Draw(void) override final;

	virtual void DrawMovableMass();
};