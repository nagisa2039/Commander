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
    Vector2Int _targetPos;  // �ړ��ړI�n
    int _speed; // �ړ����x
    bool _isMove;   // �ړ���
    int _moveSEH;
    int _fontChangeCnt; // ���ʌ����܂ł̎���
    bool _begin; // ��̐擪��

public:
    MapSelectCharactor(Camera& camera, const CharactorType& charactorType, const bool begin);
    ~MapSelectCharactor();

    void Update(const Input& input)override;
    void Draw()override;

    void SetTargetPos(const Vector2Int& targetPosX);
    bool GetIsMove()const;
};

