#pragma once
#include "UI.h"
#include "TimeLine.h"

class SelectPen :
    public UI
{
private:
    Vector2Int _pos;
    std::unique_ptr<Track<float>> _penAnimTrack;

public:
    SelectPen(std::deque<std::shared_ptr<UI>>* uiDeque);
    ~SelectPen();

    void Update(const Input& input)override;
    void Draw()override;
    void Draw(const Vector2Int& pos);
};

