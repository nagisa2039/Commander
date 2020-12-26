#pragma once
#include "Effect.h"

/// <summary>
/// 地形ダメージエフェクト
/// </summary>
class TerrainDamageEffect :
    public Effect
{
public:
    /// <param name="pos">座標</param>
    /// <param name="camera">カメラ</param>
    TerrainDamageEffect(const Vector2Int& pos, Camera& camera);
    ~TerrainDamageEffect();
};

