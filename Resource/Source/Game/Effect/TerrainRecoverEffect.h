#pragma once
#include "Effect.h"

/// <summary>
/// �n�`�񕜃G�t�F�N�g
/// </summary>
class TerrainRecoverEffect :
    public Effect
{
public:
    /// <param name="pos">���W</param>
    /// <param name="camera">�J����</param>
    TerrainRecoverEffect(const Vector2Int& pos, Camera& camera);
    ~TerrainRecoverEffect();
};

