#pragma once

class PlayerCommander;
class MapCtrl;
class Input;

class PlayerUI
{
private:
	const PlayerCommander& _playerCommander;
	const MapCtrl& _mapCtrl;

public:
	PlayerUI(const PlayerCommander& playerCommander, const MapCtrl& mapCtrl);
	~PlayerUI();

	void Update(const Input& input);
	void Draw();
};

