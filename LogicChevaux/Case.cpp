#include <cassert>
#include "Case.h"
#include "Horse.h"
#include "Player.h"
#include "Board.h"

Case::Case(Board * pBoard, unsigned int x, unsigned int y,tCaseId id, char caseValue)
: m_pNextCase(CASE_ID_UNKNOWN), m_pNextLadderCase(CASE_ID_UNKNOWN), m_pHorse(nullptr),
        m_ladderValue(0), m_caseValue(caseValue), m_id(id), m_pBoard(pBoard),
        m_logicalLocalization(x,y)
{
}

Case::Case(Board * pBoard, const LogicalLocalization &localization, tCaseId id, char caseValue)
: m_pNextCase(CASE_ID_UNKNOWN), m_pNextLadderCase(CASE_ID_UNKNOWN), m_pHorse(nullptr),
        m_ladderValue(0), m_caseValue(caseValue), m_id(id), m_pBoard(pBoard),
        m_logicalLocalization(localization)
{
}


bool Case::getPossibleMoves(Horse* horse, tMapDieNumberCase &outputMoves, unsigned int level) const
{
	assert(horse!=nullptr);
	Horse* pHorseOnNextCase;
	if(horse->isSleeping() && isAStartCaseForHorse(horse))
	{
		assert(level==0);
		if(m_pHorse==nullptr)
		{//case is free (no horse on the case)
			//return m_pNextCase->nbPossibleMove() + 1;
			outputMoves[getLadderCaseValue()] = this->getCaseId();
			return true;
		}
		else //there is a horse on the case
		{
			if(horse->haveSamePlayerOwner(m_pHorse))
			{//return 0;//don't have the right to eliminate the horse of the same player
				return true;
			}
			else
			{//return 1;//have the right to eliminate the horse of the opponent!
				outputMoves[getLadderCaseValue()] = this->getCaseId();
				return true;
			}
		}
	}
	else if(isANormalCase() && !isALadderForPlayer(horse->getPlayer()))
	{
		assert(Case::isValidCaseId(m_pNextCase));
		if(Case::isValidCaseId(m_pNextCase))
		{
			pHorseOnNextCase = m_pBoard->getCase(m_pNextCase).getHorse();
			if(pHorseOnNextCase==nullptr)
			{//case is free (no horse on the case)
				//return m_pNextCase->nbPossibleMove() + 1;
				outputMoves[level + 1] = m_pNextCase;
				return m_pBoard->getCase(m_pNextCase).getPossibleMoves(horse,outputMoves, level + 1);
			}
			else //there is a horse on the case
			{
				if(horse->haveSamePlayerOwner(pHorseOnNextCase))
				{
					//return 0;//don't have the right to eliminate the horse of the same player
					return true;
				}
				else
				{
					//return 1;//have the right to eliminate the horse of the opponent!
					outputMoves[level + 1] = m_pNextCase;
					return true;
				}
			}
		}
		else
		{//should not occurs, because there is always a successor to a normal case
			return false;
		}
	}
	else //if it is a ladder case or a base normal case of ladder
	{
		pHorseOnNextCase = m_pBoard->getCase(m_pNextLadderCase).getHorse();
		if(pHorseOnNextCase==nullptr)
		{//case is free (no horse on the case)
			if(Case::isValidCaseId(m_pNextLadderCase))
			{
				//return m_pNextLadderCase->getLadderCaseValue();
				if(outputMoves.size()==0)//ladder move is always unic, no other moves are possible!
				{
					outputMoves[m_pBoard->getCase(m_pNextLadderCase).getLadderCaseValue()] = m_pNextLadderCase;
				}
				return true;
			}
			else
			{
				//return 0;//no more cases, after the finish case!
				return true;
			}
		}
		else //there is a horse on the case
		{
			if(pHorseOnNextCase->haveSamePlayerOwner(horse))
			{
				//return 0;//don't have the right to eliminate the horse of the same player
				return true;
			}
			else
			{//in the standard game, it is not possible to have opponent horses on the ladder
				//return 1;//have the right to eliminate the horse of the opponent!
				if(outputMoves.size()==0)//ladder move is always unic, no other moves are possible!
				{
					outputMoves[m_pBoard->getCase(m_pNextLadderCase).getLadderCaseValue()] = m_pNextLadderCase;
				}
				return true;
			}
		}
	}
}

bool Case::moveFrom(tCaseId pCaseSource, Horse* pHorse)
{
	assert((!Case::isValidCaseId(pCaseSource) && pHorse!=nullptr && pHorse->isSleeping())
		|| Case::isValidCaseId(pCaseSource));
	assert(pHorse!=nullptr || !pHorse->haveSamePlayerOwner(m_pHorse));//check if the player is an opponent!
	if((Case::isValidCaseId(pCaseSource) && m_pBoard->getCase(pCaseSource).getHorse() != nullptr)
		|| pHorse != nullptr)
	{
		Horse* pHorseToMove;
		if(pHorse != nullptr)
		{//Horse is currently not on the board (so not on a case), but in its rest box.
			pHorseToMove = pHorse;
		}
		else
		{//Horse is on the board
			pHorseToMove = m_pBoard->getCase(pCaseSource).getHorse();
		}
		if(m_pHorse!=nullptr)//TODO
		{
			m_pHorse->returnToSleepingBox();
		}
		if(isAFinishCase())
		{
			pHorseToMove->setArrivedState();
		}
		else //it is not a finished case
		{
			setHorse(pHorseToMove);
		}
		if(Case::isValidCaseId(pCaseSource))
		{
			m_pBoard->getCase(pCaseSource).setHorse(nullptr);//remove horse from the previous case
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool Case::appendBaseLadderForPlayer(Player* player)
{
	std::pair< std::set<Player *>::iterator, bool > pr = m_PlayerNumberBaseLadder.insert(player);
	return pr.second;
}

bool Case::isALadderForPlayer(Player* pPlayer) const
{
	return m_PlayerNumberBaseLadder.find(pPlayer) != m_PlayerNumberBaseLadder.end();
}

bool Case::isAStartCaseForPlayer(Player* pPlayer) const
{
	if(pPlayer!=nullptr)
	{
		return pPlayer->getPlayerId() == m_caseValue;
	}
	return false;
}

bool Case::isAStartCaseForHorse(Horse* pHorse) const
{
	if(pHorse!=nullptr)
	{
		return isAStartCaseForPlayer(pHorse->getPlayer());
	}
	return false;
}

bool Case::isALadderForHorse(Horse* pHorse) const
{
	if(pHorse!=nullptr)
	{
		return isALadderForPlayer(pHorse->getPlayer());
	}
	return false;
}

bool Case::copyLadderSet(const tCaseId sourceCase)
{
	if(Case::isValidCaseId(sourceCase))
	{
		//m_PlayerNumberBaseLadder=sourceCase->m_PlayerNumberBaseLadder;
		m_PlayerNumberBaseLadder.insert(m_pBoard->getCase(sourceCase).m_PlayerNumberBaseLadder.begin(),
										m_pBoard->getCase(sourceCase).m_PlayerNumberBaseLadder.end());
		return true;
	}
	return false;
}

bool Case::setNextCase(tCaseId caseToReference)
{
	m_pNextCase = caseToReference;
	return Case::isValidCaseId(m_pNextCase);
}

bool Case::setNextLadderCase(tCaseId caseToReference)
{
	m_pNextLadderCase = caseToReference;
	return Case::isValidCaseId(m_pNextLadderCase);
}

bool Case::isALadderCase() const {return isALadderCase(m_caseValue);}
bool Case::isANormalCase() const{return isANormalCase(m_caseValue);}
bool Case::isAPureNormalCase() const {return isAPureNormalCase(m_caseValue);}
bool Case::isAStartCase() const {return isAStartCase(m_caseValue);}
bool Case::isABaseForLadder() const {return isABaseForLadder(m_caseValue);}
bool Case::isAFinishCase() const {return isAFinishCase(m_caseValue);}
bool Case::isAPureLadderCase() const{return isAPureLadderCase(m_caseValue);}

bool Case::isALadderCase(char charId)
{
	return (isAPureLadderCase(charId)
		|| isAFinishCase(charId));
}

bool Case::isANormalCase(char charId)
{
	return isAPureNormalCase(charId)
		|| isAStartCase(charId)
		|| isABaseForLadder(charId);
}

bool Case::isAPureNormalCase(char charId)
{
	return (charId == '#');
}

bool Case::isAStartCase(char charId)
{
	return (charId >= 'a' && charId <= 'z');
}

bool Case::isABaseForLadder(char charId)
{
	return (charId >= 'A' && charId <= 'Z');
}

bool Case::isAPureLadderCase(char charId)
{
	return (charId == '=');
}

bool Case::isAFinishCase(char charId)
{
	return (charId == '$');
}

bool Case::isACase(char charId)
{
	return isANormalCase(charId)
		|| isALadderCase(charId);
}

const LogicalLocalization& Case::getLocalization() const
{
	return m_logicalLocalization;
}

Horse * Case::getHorse() const
{
	return m_pHorse;
}

void Case::setHorse(Horse* horse)
{
	m_pHorse = horse;
	if(horse!=nullptr)
	{
		horse->setCase(this->getCaseId());
	}
}

unsigned int Case::getLadderCaseValue() const
{
	return m_ladderValue;
}

void Case::setLadderCaseValue(unsigned int ladderValue)
{
	assert(isALadderCase() || isAStartCase());
	m_ladderValue = ladderValue;
}

bool Case::isValidCaseId(tCaseId caseId)
{
	return caseId.id>=0;
}