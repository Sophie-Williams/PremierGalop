#include <cassert>
#include "Horse.h"
#include "Player.h"
#include "Board.h"

Horse::Horse(Board * pBoard, Player *player) : m_pPlayer(player), m_pCase(CASE_ID_UNKNOWN), m_bIsArrived(false), m_score(0), m_pBoard(pBoard)
{
}

Player * Horse::getPlayer() const
{
	return m_pPlayer;
}

tCaseId Horse::getCase() const
{
	return m_pCase;
}

bool Horse::haveSamePlayerOwner(const Horse *horse) const
{
	if(horse==nullptr)
		return false;
	else
		return horse->getPlayer() == this->getPlayer();
}

void Horse::setCase(tCaseId pCaseToSet)
{
	m_pCase = pCaseToSet;
}

bool Horse::isSleeping() const
{
	return !Case::isValidCaseId(m_pCase) //not on a case
		&& !m_bIsArrived; // and not arrived
}

bool Horse::isArrived() const
{
	assert( (!Case::isValidCaseId(m_pCase) && m_bIsArrived) || !m_bIsArrived);//check consistency
	return m_bIsArrived;
}

bool Horse::isRunning() const
{
	assert( (!Case::isValidCaseId(m_pCase) && m_bIsArrived) || !m_bIsArrived);//check consistency
	return m_pCase.id>=0
		&& !m_bIsArrived;
}

bool Horse::isRunningOnNormalCase() const
{
	return isRunning() && !m_pBoard->getCase(m_pCase).isALadderCase();
}

bool Horse::isRunningOnLadderCase() const
{
	return isRunning() && !isRunningOnNormalCase();
}

bool Horse::isRunningOnfirstCaseofLadderCase() const
{
	return isRunningOnLadderCase() && m_pBoard->getCase(m_pCase).getLadderCaseValue()==1;
}

void Horse::returnToSleepingBox()
{
	m_pCase=CASE_ID_UNKNOWN;
	resetScore();
}
void Horse::setArrivedState()
{
	m_pCase=CASE_ID_UNKNOWN;
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