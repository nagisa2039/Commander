#pragma once
#include "Actor.h"
#include <memory>
#include "CharactorType.h"

class Animator;

/// <summary>
/// �}�b�v�I�����̃L�����N�^�[
/// </summary>
class MapSelectCharactor :
    public Actor
{
private:
    // �A�j���[�V�����Ǘ��N���X
    std::unique_ptr<Animator> _animator;
    // �ړ��ړI�n
    Vector2Int _targetPos; 
    // �ړ����x
    int _speed; 
    // �ړ���
    bool _isMove;
    // �ړ�SE�n���h��
    int _moveSEH;
    // ���ʌ����܂ł̎���
    int _frontChangeCnt; 
    // ��̐擪��
    bool _begin; 

public:
    /// <param name="camera">�J����</param>
    /// <param name="charactorType">�L�����N�^�[��</param>
    /// <param name="begin">�퓬����t���O</param>
    MapSelectCharactor(Camera& camera, const CharactorType& charactorType, const bool begin);
    ~MapSelectCharactor();

    /// <summary>
    /// �X�V
    /// </summary>
    /// <param name="input">���͏��</param>
    void Update(const Input& input)override;

    /// <summary>
    /// �`��
    /// </summary>
    void Draw()override;

    /// <summary>
    /// �ړ��ڕWX���W��ݒ�
    /// </summary>
    /// <param name="targetPosX">�ړ��ڕWX���W</param>
    void SetTargetPos(const Vector2Int& targetPosX);

    /// <summary>
    /// �ړ��������擾
    /// </summary>
    bool GetIsMove()const;
};

