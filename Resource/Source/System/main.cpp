#include <DxLib.h>
#include "Application.h"

using namespace std;

// ---------- WinMain�֐�
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	auto& app = Application::Instance();
	app.Initialize();
	app.Run();
	app.Terminate();

	return 0;
}