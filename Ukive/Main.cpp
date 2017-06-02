#include "UCommon.h"
#include "UApplication.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR pCmdLine, int iCmdShow)
{
	UApplication *app = new UApplication();
	app->run();

	delete app;

	return 0;
}