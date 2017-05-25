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

void UI::run(bool& open, AIApplication* app)
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_FirstUseEver);	
	showUI(open, app);
}

void UI::showHelp()
{
	ImGui::BulletText(
		"To start simulation:\n"
		"- Allocate Character stats then apply\n"
		"- Select Skillset then apply\n"
		"- Press Start\n"
		);
	ImGui::BulletText(
		"Stats:\n"
		"- Strength: Increases damage and health\n"
		"- Intelligence: Increases damage, mana pool, and reduces cool down of actions\n"
		"- Agility: Increases damage and movement speed, and decreases cast time\n"
		"- Vitality: Increases Health"
		);
	ImGui::BulletText(
		"Camera Controls:\n"
		"- WASD: To move forward, left, back and right\n"
		"- QE: To move up and down y axis\n"
		"- Hold mouse right click and move to adjust view angle\n"
		);
}

void UI::showUI(bool & open, AIApplication* app)
{	
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_ShowBorders;
	window_flags |= ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_NoSavedSettings;

	ImGui::SetNextWindowSize(ImVec2(550, 680));	

	if (!ImGui::Begin("Main", &open, window_flags))
	{
		ImGui::End();
		return;
	}
	
	if (ImGui::CollapsingHeader("Help"))
	{
		ImGui::SetWindowSize(ImVec2( 400, 0));
		showHelp();
	}
	
	static bool pointsAvailable = false;

	if (ImGui::CollapsingHeader("Character"))
	{

		static int availablePoints = 0;
		static int selected_skillSet = 0;

		ImGui::BeginChild("Character Details", ImVec2(0, 250.f));

		if (ImGui::CollapsingHeader("Stats"))
		{
			static Stats tempStats;
			
			if (pointsAvailable)
			{
				tempStats = app->m_agent->getStats();
				availablePoints = app->m_agent->getStatPoints();
				pointsAvailable = false;
			}
			float str = tempStats.strength;
			float inte = tempStats.intelligence;
			float agi = tempStats.agility;
			float vit = tempStats.vitality;

			ImGui::PushItemWidth(200);
			ImGui::SliderInt("Available Points", &availablePoints, availablePoints, availablePoints);
			ImGui::InputFloat("Strength", &str, 1.0f, 1.0f, 0);
			ImGui::InputFloat("Intelligence", &inte, 1.0f, 1.0f, 0);
			ImGui::InputFloat("Agililty", &agi, 1.0f, 1.0f, 0);
			ImGui::InputFloat("Vitality", &vit, 1.0f, 1.0f, 0);


			if (str > tempStats.strength && availablePoints > 0)
			{
				tempStats.strength = str;
				--availablePoints;
			}

			if (inte > tempStats.intelligence && availablePoints > 0)
			{
				tempStats.intelligence = inte;
				--availablePoints;
			}

			if (agi > tempStats.agility && availablePoints > 0)
			{
				tempStats.agility = agi;
				--availablePoints;
			}

			if (vit > tempStats.vitality && availablePoints > 0)
			{
				tempStats.vitality = vit;
				--availablePoints;
			}

			if (str < app->m_agent->getStats().strength)
				str = app->m_agent->getStats().strength;

			if (inte < app->m_agent->getStats().intelligence)
				inte = app->m_agent->getStats().intelligence;

			if (agi < app->m_agent->getStats().agility)
				agi = app->m_agent->getStats().agility;

			if (vit < app->m_agent->getStats().vitality)
				vit = app->m_agent->getStats().vitality;

			if (str < tempStats.strength)
			{
				tempStats.strength = str;
				++availablePoints;
			}

			if (inte < tempStats.intelligence)
			{
				tempStats.intelligence = inte;
				++availablePoints;
			}

			if (agi < tempStats.agility)
			{
				tempStats.agility = agi;
				++availablePoints;
			}

			if (vit < tempStats.vitality)
			{
				tempStats.vitality = vit;
				++availablePoints;
			}

			if (ImGui::Button("Apply"))
			{
				app->m_agent->getStats() = tempStats;
				app->m_agent->setStatPoints(availablePoints);
				if (!app->m_start)
				{
					app->m_agent->updateStats();
				}
			}

		}
		else if (app->m_agent->getStatPoints() > 0)
		{
			pointsAvailable = true;
		}

		if (ImGui::CollapsingHeader("Skills"))
		{
			const char* names[] = { "None", "Melee", "Range", "Magic" };

			// Simple selection popup
			// (If you want to show the current selection inside the Button itself, you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
			if (ImGui::Button("Skill Sets"))
				ImGui::OpenPopup("select");
			ImGui::SameLine();
			ImGui::Text(names[selected_skillSet]);
			if (ImGui::BeginPopup("select"))
			{
				ImGui::Text("Skill Sets");
				ImGui::Separator();
				for (int i = 0; i < 4; i++)
					if (ImGui::Selectable(names[i]))
						selected_skillSet = i;
				ImGui::EndPopup();
			}

			if(ImGui::Button("Apply Skill Set"))
			{
				app->applySkillSet(app->m_agent, selected_skillSet);
			}
		}

		ImGui::EndChild();
	}	



	if(ImGui::Button("Start"))
	{
		if (app->m_agent->getStatPoints() > 0)
		{
			ImGui::OpenPopup("Allocate stat points");
		}
		else if (app->m_agent->getTActions().size() == 0)
		{
			ImGui::OpenPopup("Select skill set");
		}
		else
		{
			app->m_start = true;
			open = false;
			pointsAvailable = true;
		}
	}

	if (ImGui::BeginPopupModal("Allocate stat points"))
	{
		ImGui::Text("Allocate stat points!\n\n");
		if (ImGui::Button("Close"))
			ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("Select skill set"))
	{
		ImGui::Text("Select skill set!\n\n");
		if (ImGui::Button("Close"))
			ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}

	ImGui::End();
}

void UI::showHealthStatus(bool * open, AIApplication * app)
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoResize;
	ImGui::SetWindowPos(ImVec2(10, 10));
	ImGui::SetNextWindowSize(ImVec2(200, 100));
	ImGui::Begin("Health Status", &app->m_start, window_flags);
	char healthbuf[32];
	sprintf(healthbuf, "%d", (int)app->m_agent->getCurrentHealth());
	ImGui::Text("Health");
	ImGui::SameLine();
	ImGui::ProgressBar(app->m_agent->getHealthPercentage(), ImVec2(-1, 0), healthbuf);
	char manabuf[32];
	sprintf(manabuf, "%d", (int)app->m_agent->getCurrentMana());
	ImGui::Text("Mana");
	ImGui::SameLine();
	ImGui::ProgressBar(app->m_agent->getManaPercentage(), ImVec2(-1, 0), manabuf);
	if (ImGui::Button("Restart"))
	{
		app->restart();
	}
	ImGui::End();
}

