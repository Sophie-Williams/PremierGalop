#include <cassert>
#include "Horse.h"
#include "Player.h"

Horse::Horse(Player *player) : m_pPlayer(player), m_pCase(NULL), m_bIsArrived(false), m_score(0)
{
}

Player * Horse::getPlayer() const
{
	return m_pPlayer;
}

Case * Horse::getCase() const
{
	return m_pCase;
}

bool Horse::haveSamePlayerOwner(const Horse *horse) const
{
	if(horse==NULL)
		return false;
	else
		return horse->getPlayer() == this->getPlayer();
}

void Horse::setCase(Case* pCaseToSet)
{
	m_pCase = pCaseToSet;
}

bool Horse::isSleeping() const
{
	return m_pCase == NULL //not on a case
		&& !m_bIsArrived; // and not arrived
}

bool Horse::isArrived() const
{
	assert( (m_pCase==NULL && m_bIsArrived) || !m_bIsArrived);//check consistency
	return m_bIsArrived;
}

bool Horse::isRunning() const
{
	assert( (m_pCase==NULL && m_bIsArrived) || !m_bIsArrived);//check consistency
	return m_pCase != NULL
		&& !m_bIsArrived;
}

bool Horse::isRunningOnNormalCase() const
{
	return isRunning() && !m_pCase->isALadderCase();
}

bool Horse::isRunningOnLadderCase() const
{
	return isRunning() && !isRunningOnNormalCase();
}

bool Horse::isRunningOnfirstCaseofLadderCase() const
{
	return isRunningOnLadderCase() && m_pCase->getLadderCaseValue()==1;
}

void Horse::returnToSleepingBox()
{
	m_pCase=NULL;
	resetScore();
}
void Horse::setArrivedState()
{
	m_pCase=NULL;
	m_bIsArrived=true;
}

//horse is considered as equal only if they have the same pointer!
bool Horse::operator==(const Horse &horse) const
{
	return this == &horse;
}

unsigned int Horse::getHorseNumber() const
{
	return m_pPlayer->getHorseNumber(this);
}

int Horse::getScore() const
{
	return m_score;
}

void Horse::resetScore()
{
	m_score=0;
}

void Horse::incrementScore(int increment)
{
	m_score+=increment;
}