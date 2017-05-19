#pragma once

class AIApplication;
class Agent;
class UI
{
public:
	UI();
	~UI();

	static void run(bool* open, Agent* agent, AIApplication* app);

	static void showHelp();

	static void showUI(bool* open, Agent* agent, AIApplication* app);

	static void showMenu();

	

};

