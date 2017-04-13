#include "AIOverlord.h"




int main()
{
	auto app = new AIOverlord();
	app->run("AIE", 1280, 720, false);
	delete app;

    return 0;
}

