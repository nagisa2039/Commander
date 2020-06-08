#pragma once
#include "Effect.h"
class TerrainDamageEffect :
    public Effect
{
public:
    TerrainDamageEffect(const Vector2Int& pos, Camera& camera);
    ~TerrainDamageEffect();

    void Draw() override;
};

