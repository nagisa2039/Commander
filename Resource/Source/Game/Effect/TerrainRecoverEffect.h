#pragma once
#include "Effect.h"
class TerrainRecoverEffect :
    public Effect
{
public:
    TerrainRecoverEffect(const Vector2Int& pos, Camera& camera);
    ~TerrainRecoverEffect();

    void Draw() override;
};

