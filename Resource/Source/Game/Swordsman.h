#pragma once
#include "Charactor.h"

class Swordsman :
	public Charactor
{
public:
	Swordsman(const Vector2Int& mapPos, const Team team, MapCtrl& mapCtrl, SceneController& ctrl);
	~Swordsman();

	void Update(const Input& input)override final;
	void Draw(const Camera& camera)override final;
};

