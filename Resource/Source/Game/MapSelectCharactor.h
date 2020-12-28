#pragma once
#include "Actor.h"
#include <memory>
#include "CharactorType.h"

class Animator;

/// <summary>
/// マップ選択時のキャラクター
/// </summary>
class MapSelectCharactor :
    public Actor
{
private:
    // アニメーション管理クラス
    std::unique_ptr<Animator> _animator;
    // 移動目的地
    Vector2Int _targetPos; 
    // 移動速度
    int _speed; 
    // 移動中
    bool _isMove;
    // 移動SEハンドル
    int _moveSEH;
    // 正面向くまでの時間
    int _frontChangeCnt; 
    // 列の先頭か
    bool _begin; 

public:
    /// <param name="camera">カメラ</param>
    /// <param name="charactorType">キャラクター種</param>
    /// <param name="begin">戦闘判定フラグ</param>
    MapSelectCharactor(Camera& camera, const CharactorType& charactorType, const bool begin);
    ~MapSelectCharactor();

    /// <summary>
    /// 更新
    /// </summary>
    /// <param name="input">入力情報</param>
    void Update(const Input& input)override;

    /// <summary>
    /// 描画
    /// </summary>
    void Draw()override;

    /// <summary>
    /// 移動目標X座標を設定
    /// </summary>
    /// <param name="targetPosX">移動目標X座標</param>
    void SetTargetPos(const Vector2Int& targetPosX);

    /// <summary>
    /// 移動中かを取得
    /// </summary>
    bool GetIsMove()const;
};

