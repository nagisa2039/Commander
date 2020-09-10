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
    Vector2Int _targetPos;  // ˆÚ“®–Ú“I’n
    int _speed; // ˆÚ“®‘¬“x
    bool _isMove;   // ˆÚ“®’†
    int _moveSEH;
    int _fontChangeCnt; // ³–ÊŒü‚­‚Ü‚Å‚ÌŠÔ
    bool _begin; // —ñ‚Ìæ“ª‚©

public:
    MapSelectCharactor(Camera& camera, const CharactorType& charactorType, const bool begin);
    ~MapSelectCharactor();

    void Update(const Input& input)override;
    void Draw()override;

    void SetTargetPos(const Vector2Int& targetPosX);
    bool GetIsMove()const;
};

