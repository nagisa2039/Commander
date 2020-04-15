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
class PlayerCommander;
class EnemyCommander;
class Effect;
class TurnChangeAnim;

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
	std::shared_ptr<PlayerCommander> _playerCommander;
	std::shared_ptr<EnemyCommander> _enemyCommander;
	std::vector<std::shared_ptr<Effect>> _effects;

	// 場面ごとの更新を行う関数ポインタ
	void(PlayScene::*_uniqueUpdater)(const Input& input);
	void(PlayScene::* _uniqueUpdaterOld)(const Input& input);

	// 場面ごとの更新を行う関数ポインタ
	void(PlayScene::* _uniqueDrawer)(const Camera& camera);

	// ポストエフェクト実験
	int _targetBf;
	int _shrinkBf;


	void MakePSTBuffer(const int targetHandle);
	void DrawPSTBuffer();

	void StartPlayerTurn();
	void StartEnemyTurn();

	void PlayerTurnUpdate(const Input& input);
	void CharactorUpdate(const Input& input);
	void EnemyTurnUpdate(const Input& input);

	void CharactorDyingUpdate(const Input& input);

	void GameClearUpdate(const Input& input);
	void GameOverUpdate(const Input& input);

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
};
