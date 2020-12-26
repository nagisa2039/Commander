#pragma once
#include "Effect.h"

/// <summary>
/// 地形回復エフェクト
/// </summary>
class TerrainRecoverEffect :
    public Effect
{
public:
    /// <param name="pos">座標</param>
    /// <param name="camera">カメラ</param>
    TerrainRecoverEffect(const Vector2Int& pos, Camera& camera);
    ~TerrainRecoverEffect();
};

