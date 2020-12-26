#pragma once
#include "TimeLine.h"
#include "FlyText.h"
#include <memory>

/// <summary>
/// ���˂鎸�s�G�t�F�N�g
/// </summary>
class PopupMissEffect :
	public PopupText
{
public:
	/// <param name="pos">���W</param>
	/// <param name="camera">�J����</param>
	/// <param name="cameraActive">�J�������l�����ĕ`�悷�邩</param>
	/// <param name="critical">�N���e�B�J���t���O</param>
	PopupMissEffect(const Vector2Int& pos,
		Camera& camera, bool cameraActive, bool critical = false);
	~PopupMissEffect() = default;
};