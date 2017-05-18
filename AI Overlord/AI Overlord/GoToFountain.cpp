#include "GoToFountain.h"
#include "Agent.h"


GoToFountain::GoToFountain()
{
}


GoToFountain::~GoToFountain()
{
}

float GoToFountain::evaluate(Agent * agent, float dt)
{
	float score = 0.5f - (0.5f *  (1.f - agent->getHealthPercentage() + 0.01f)) + 0.5f - (0.5f *  (1.f - agent->getManaPercentage() + 0.01f));

	return score;
}

void GoToFountain::enter(Agent * agent, float dt)
{

}

void GoToFountain::exit(Agent * agent, float dt)
{
	agent->setCurrentAction(nullptr);
	m_currentPath.clear();
}

void GoToFountain::updateAction(Agent * agent, float dt)
{
}
