#include "AIApplication.h"




int main()
{
	auto app = new AIApplication();
	app->run("AIE", 1280, 720, false);
	delete app;

    return 0;
}

