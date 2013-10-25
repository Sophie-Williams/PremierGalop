#include <cassert>
#include "Player.h"
#include "Board.h"
#include "PlayerInterface.h"

Player::Player(const std::string &nickname, tPLayerId playerId)
: m_nickname(nickname), m_playerId(playerId), m_pPlayerInterface(NULL)
{}

bool Player::AddHorses(Board * pBoard, unsigned int nbHorses)
{
	assert(pBoard!=NULL);
	for(unsigned int i=0; i<nbHorses; i++)
	{
		m_horses.push_back(Horse(pBoard, this));
	}
	return true;
}

void Player::setPlayerInterface(PlayerInterface *pPlayerInterface)
{
	m_pPlayerInterface = pPlayerInterface;
}

tHorseTargetCase Player::ChooseMoveFrom(const tHorseTargetCaseList &listOfMoves)
{
	assert(m_pPlayerInterface!=NULL);
	return m_pPlayerInterface->ChooseMoveFrom(listOfMoves);
}

std::string Player::getTypeOfPlayer()
{
	assert(m_pPlayerInterface!=NULL);
	return m_pPlayerInterface->getTypeOfPlayer();
}

Horse* Player::getHorse(unsigned int nb)
{
	assert(nb<m_horses.size());
	return &m_horses[nb];
}

unsigned int Player::getHorseNumber(const Horse *horse)
{
	if(horse!=NULL)
	{
		for(tHorseList::iterator it = m_horses.begin(); it != m_horses.end(); it++)
		{
			if(horse==&*it)
			{
				return static_cast<unsigned int>(it-m_horses.begin());//return index of horse in the m_horses vector
			}
		}
	}
	return static_cast<unsigned int>(~0);//invalid value
}

unsigned int Player::getPlayerNb()
{
	return m_playerId - 'a';
}

unsigned char Player::getPlayerId()
{
	return m_playerId;
}

std::string& Player::getNickname()
{
	return m_nickname;
}

void Player::setNickname(const std::string& nickname)
{
	m_nickname = nickname;
}

bool Player::hasAlreadyWon()
{
	for(tHorseList::const_iterator it=m_horses.begin(); it != m_horses.end(); it++)
	{
		if(!it->isArrived())
		{
			return false;
		}
	}
	return true;
}

int Player::getScore() const
{
	int score = 0;
	for(tHorseList::const_iterator it=m_horses.begin(); it != m_horses.end(); it++)
	{
		score+=it->getScore();
	}
	return score;
}