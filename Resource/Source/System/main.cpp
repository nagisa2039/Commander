#include <DxLib.h>
#include "Application.h"

using namespace std;

// ---------- WinMainä÷êî
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	auto& app = Application::Instance();
	app.Initialize();
	app.Run();
	app.Terminate();

	return 0;
}