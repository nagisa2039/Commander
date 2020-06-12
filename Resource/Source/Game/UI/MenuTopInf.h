#pragma once
#include "UI.h"
#include "TimeLine.h"

class MapCtrl;

class MenuTopInf :
    public UI
{
private:
    const MapCtrl& _mapCtrl;
    const unsigned char& _turnCnt;

    std::unique_ptr<Track<Vector2Int>> _moveAnimTrack;

public:
    MenuTopInf(const MapCtrl& mapCtrl, const unsigned char& turnCnt, std::deque<std::shared_ptr<UI>> uiDeque);
    ~MenuTopInf();

    void Update(const Input& input) override;
    void Draw() override;

    void Open();
    void Close();
};

