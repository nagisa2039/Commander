#pragma once
class Application;

/// <summary>
/// FPS�Ǘ��N���X
/// </summary>
class FPSManager
{
	friend Application;

private:
	/// <summary>
	/// FPS�Ǘ��N���X
	/// </summary>
	/// <param name="fps">�Œ肵�����t���[����
	/// ���̒l�Ȃ疳����</param>
	FPSManager(const int fps);
	FPSManager(const FPSManager&) = delete;
	FPSManager& operator=(const FPSManager&) = delete;

	// �Œ肷��FPS
	const int fixedFPS_;
	// 1�t���[���ɂ�����������
	float deltaTime_;

	/// <summary>
	/// �J�n�~���b
	/// </summary>
	unsigned int startTime_;

	/// <summary>
	/// �O�t���[���̊J�n�~���b
	/// </summary>
	unsigned int prevFrameStartTime_;

	/// <summary>
	/// �w�肵���t���[�����ɂȂ�܂ő҂�
	/// </summary>
	void Wait();

public:
	~FPSManager() = default;

	/// <summary>
	/// �Œ肵���t���[�������擾
	/// </summary>
	int FixedFPS();

	/// <summary>
	/// ���݂̃t���[�����̎擾
	/// </summary>
	float GetFPS();

	/// <summary>
	/// 1�t���[�����̌o�ߎ��Ԃ̎擾
	/// </summary>
	float GetDeltaTime();
};

