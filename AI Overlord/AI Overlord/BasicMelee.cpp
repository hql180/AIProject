#include "BasicMelee.h"
#include "Agent.h"
#include "Gizmos.h"
#include <iostream>

BasicMelee::BasicMelee() 
{
	name = "Basic Melee";
	m_cost = 0;
	m_attackRange = 1.f;
	m_damageMultiplier = 1.5f;
	m_CD = 0.8f;
	m_CDTimer = 0;
	m_castTime = 0.2f;
	m_castTimer = 0;
	m_isCasting = false;
}

BasicMelee::~BasicMelee()
{
}

float BasicMelee::evaluate(Agent* agent, float dt)
{
	//if (checkCoolDown(agent) == 0)
	//	return 0;
	
	float score = checkDamage(agent) / agent->getETarget()->getCurrentHealth();

	if (score > 1)
		score = 1.f;

	score += agent->getHealthPercentage();

	score = score + (agent->getHealthPercentage() / agent->getETarget()->getHealthPercentage() < 1) ? agent->getHealthPercentage() / agent->getETarget()->getHealthPercentage() : 1.f;

	score = score - (glm::length(agent->getPostion() - agent->getETarget()->getPostion()) / agent->getVisionRange()) * 0.5f;
	
	return score;
}

void BasicMelee::enter(Agent* agent, float dt)
{
	
}

void BasicMelee::exit(Agent* agent, float dt)
{
	m_isCasting = false;
	m_castTimer = 0;
	agent->setCurrentAction(nullptr);
	m_currentPath.clear();
}

void BasicMelee::updateAction(Agent* agent, float dt)
{
	float distance = glm::length(agent->getPostion() - agent->getTarget()->getPostion());
	if (distance > m_attackRange && !m_isCasting)
	{
		seekTarget(agent, dt);
	}
	else if (distance <= m_attackRange && m_CDTimer <= 0 || m_isCasting && m_CDTimer <= 0)
	{
		m_isCasting = true;
		m_castTimer += (dt * ( 1.f + agent->getStats().agility * 0.01f));
		aie::Gizmos::addLine(agent->getPostion(), agent->getTarget()->getPostion(), agent->getColour());

		if (m_castTimer >= m_castTime)
		{	

			finishAttack(agent);
			applyDamage(agent->getTarget(), agent->getAttackDamage() * m_damageMultiplier);			
			exit(agent, dt);
		}
	}
}

void BasicMelee::updateTimer(float dt)
{
	Attack::updateTimer(dt);
}
