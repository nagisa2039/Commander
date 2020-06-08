#pragma once
#include "UI.h"
#include "TimeLine.h"

class MapCtrl;

class TerrainInf :
    public UI
{
private:
    std::unique_ptr<Track<float>> _animTrack;
    const MapCtrl& _mapCtrl;
    const Vector2Int& _mapPos;

public:
    TerrainInf(std::deque<std::shared_ptr<UI>>& uiDeque, const MapCtrl& mapCtrl, const Vector2Int& mapPos);
    ~TerrainInf();

    void Update(const Input& input)override;
    void Draw()override;

    void Close();
    void Open();
};

