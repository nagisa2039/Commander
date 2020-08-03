#pragma once
#include "UIListItem.h"

struct SaveDataCharactor;

class CharactorListItem :
    public UIListItem
{
private:
    const SaveDataCharactor& _charactorData;

public:
    CharactorListItem(const SaveDataCharactor& charactorData, std::deque<std::shared_ptr<UI>>* uiDeque);
    ~CharactorListItem();

    void Update(const Input& input)override;
    void Draw(const Vector2Int& offset);

    const SaveDataCharactor& GetCharactorData()const;
};

