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
    Vector2Int _targetPos;  // 移動目的地
    int _speed; // 移動速度
    bool _isMove;   // 移動中
    int _moveSEH;
    int _fontChangeCnt; // 正面向くまでの時間
    bool _begin; // 列の先頭か

public:
    MapSelectCharactor(Camera& camera, const CharactorType& charactorType, const bool begin);
    ~MapSelectCharactor();

    void Update(const Input& input)override;
    void Draw()override;

    void SetTargetPos(const Vector2Int& targetPosX);
    bool GetIsMove()const;
};

