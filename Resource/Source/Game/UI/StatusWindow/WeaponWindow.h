#pragma once
#include "../UI.h"
#include "Geometry.h"

struct WeaponData;

/// <summary>
/// ����E�B���h�E
/// </summary>
class WeaponWindow :
    public UI
{
private:
    // �`���X�N���[���n���h��
    int _windowH;
    // ����ID
    uint8_t _weaponId;
    // �\�����W
    const Vector2Int _drawPos;

    /// <summary>
    /// �ŗL�̕`���ւ̕`��
    /// </summary>
    void DrawToWindow();

public:
    /// <param name="weaponId">����ID</param>
    /// <param name="drawPos">�\�����W</param>
	/// <param name="uiDeque">UI�̐e�q�֌W�Ǘ�Deque</param>
    WeaponWindow(const uint8_t weaponId, const Vector2Int& drawPos, std::deque<std::shared_ptr<UI>>* uiDeque);
    ~WeaponWindow();

    /// <summary>
    /// �`��
    /// </summary>
    void Draw()override;

    /// <summary>
    /// �\���T�C�X�̎擾
    /// </summary>
    Size GetSize()const;

    /// <summary>
    /// 
    /// </summary>
    /// <param name="weaponId"></param>
    void SetWeaponId(const uint8_t weaponId);
};

