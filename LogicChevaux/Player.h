#ifndef  __CHEVAUX_PLAYER_H__
#define __CHEVAUX_PLAYER_H__

#include <string>
#include <vector>
#include "types.h"
#include "Horse.h"

typedef char tPLayerId;
typedef std::vector<Horse> tHorseList;

class PlayerInterface;
class Board;

class Player
{
public:
	Player(const std::string &nickname, tPLayerId playerId);

	bool AddHorses(Board * pBoard, unsigned int nbHorses);
	void setPlayerInterface(PlayerInterface *pPlayerInterface);

	tHorseTargetCase ChooseMoveFrom(const tHorseTargetCaseList &listOfMoves);
	std::string getTypeOfPlayer();

	Horse* getHorse(unsigned int nb);
	unsigned int getHorseNumber(const Horse *horse);
	unsigned int getPlayerNb();

	unsigned char getPlayerId();
	std::string& getNickname();
	void setNickname(const std::string& nickname);

	bool hasAlreadyWon();
	int getScore() const;

private:
	tHorseList m_horses;
	tPLayerId m_playerId;
	std::string m_nickname;
	PlayerInterface * m_pPlayerInterface;//Pointer is necessary, in order to prevent truncation.
};

#endif