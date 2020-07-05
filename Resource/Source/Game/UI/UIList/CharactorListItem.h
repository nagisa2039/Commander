#pragma once
#include "UIListItem.h"

struct CharactorData;

class CharactorListItem :
    public UIListItem
{
private:
    const CharactorData& _charactorData;

public:
    CharactorListItem(const CharactorData& charactorData, std::deque<std::shared_ptr<UI>>* uiDeque);
    ~CharactorListItem();

    void Update(const Input& input)override;
    void Draw(const Vector2Int& offset);

    const CharactorData& GetCharactorData()const;
};

