#include "UI.h"
#include "imgui\imgui.h"
#include "imgui_glfw3.h"
#include "Agent.h"
#include "AIApplication.h"
#include "Agent.h"

UI::UI()
{
}


UI::~UI()
{
}

void UI::run(bool* open, Agent* agent, AIApplication* app)
{
	ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
	
	showUI(open, agent, app);

}

void UI::showHelp()
{
	ImGui::BulletText("Double-click on title bar to collapse window.");
	ImGui::BulletText("Click and drag on lower right corner to resize window.");
	ImGui::BulletText("Click and drag on any empty space to move window.");
	ImGui::BulletText("Mouse Wheel to scroll.");
	if (ImGui::GetIO().FontAllowUserScaling)
		ImGui::BulletText("CTRL+Mouse Wheel to zoom window contents.");
	ImGui::BulletText("TAB/SHIFT+TAB to cycle through keyboard editable fields.");
	ImGui::BulletText("CTRL+Click on a slider or drag box to input text.");
	ImGui::BulletText(
		"While editing text:\n"
		"- Hold SHIFT or use mouse to select text\n"
		"- CTRL+Left/Right to word jump\n"
		"- CTRL+A or double-click to select all\n"
		"- CTRL+X,CTRL+C,CTRL+V clipboard\n"
		"- CTRL+Z,CTRL+Y undo/redo\n"
		"- ESCAPE to revert\n"
		"- You can apply arithmetic operators +,*,/ on numerical values.\n"
		"  Use +- to subtract.\n");
}

void UI::showUI(bool * open, Agent* agent, AIApplication* app)
{	
	static bool show_menu = false;

	static bool show_app_metrics = false;

	static bool show_app_style_editor = false;

	static bool show_app_about = false;

	if (show_menu) showMenu();

	static bool no_titlebar = false;
	static bool no_border = true;
	static bool no_resize = false;
	static bool no_move = false;
	static bool no_scrollbar = false;
	static bool no_collapse = false;
	static bool no_menu = false;

	ImGuiWindowFlags window_flags = 0;
	if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
	if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
	if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
	if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
	if (!no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiSetCond_FirstUseEver);
	
	if (!ImGui::Begin("Main", open, window_flags))
	{
		ImGui::End();
		return;
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			showMenu();
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	if (ImGui::CollapsingHeader("Help"))
	{
		showHelp();
	}

	if (ImGui::CollapsingHeader("Character"))
	{
		
	}

	ImGui::End();
}

void UI::showMenu()
{
	ImGui::MenuItem("(dummy menu)", NULL, false, false);
	if (ImGui::MenuItem("New")) {}
	if (ImGui::MenuItem("Open", "Ctrl+O")) {}
	if (ImGui::BeginMenu("Open Recent"))
	{
		ImGui::MenuItem("fish_hat.c");
		ImGui::MenuItem("fish_hat.inl");
		ImGui::MenuItem("fish_hat.h");
		if (ImGui::BeginMenu("More.."))
		{
			ImGui::MenuItem("Hello");
			ImGui::MenuItem("Sailor");
			if (ImGui::BeginMenu("Recurse.."))
			{
				showMenu();
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Save", "Ctrl+S")) {}
	if (ImGui::MenuItem("Save As..")) {}
	ImGui::Separator();
	if (ImGui::BeginMenu("Options"))
	{
		static bool enabled = true;
		ImGui::MenuItem("Enabled", "", &enabled);
		ImGui::BeginChild("child", ImVec2(0, 60), true);
		for (int i = 0; i < 10; i++)
			ImGui::Text("Scrolling Text %d", i);
		ImGui::EndChild();
		static float f = 0.5f;
		static int n = 0;
		ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
		ImGui::InputFloat("Input", &f, 0.1f);
		ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Colors"))
	{
		for (int i = 0; i < ImGuiCol_COUNT; i++)
			ImGui::MenuItem(ImGui::GetStyleColName((ImGuiCol)i));
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Disabled", false)) // Disabled
	{
		IM_ASSERT(0);
	}
	if (ImGui::MenuItem("Checked", NULL, true)) {}
	if (ImGui::MenuItem("Quit", "Alt+F4")) {}
}
