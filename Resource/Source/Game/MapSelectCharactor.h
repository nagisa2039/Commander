#pragma once
#include "Actor.h"
#include <memory>
#include "CharactorType.h"

class Animator;

class MapSelectCharactor :
    public Actor
{
private:
    std::unique_ptr<Animator> _animator;
    Vector2Int _targetPos;
    int _speed;
    bool _isMove;

public:
    MapSelectCharactor(Camera& camera, const CharactorType& charactorType);
    ~MapSelectCharactor();

    void Update(const Input& input)override;
    void Draw()override;

    void SetTargetPos(const Vector2Int& targetPosX);
    bool GetIsMove()const;
};

