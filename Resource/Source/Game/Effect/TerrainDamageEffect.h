#pragma once
#include "Effect.h"

/// <summary>
/// �n�`�_���[�W�G�t�F�N�g
/// </summary>
class TerrainDamageEffect :
    public Effect
{
public:
    /// <param name="pos">���W</param>
    /// <param name="camera">�J����</param>
    TerrainDamageEffect(const Vector2Int& pos, Camera& camera);
    ~TerrainDamageEffect();
};

