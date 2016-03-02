#ifndef  __CHEVAUX_PLAYER_H__
#define __CHEVAUX_PLAYER_H__

#include <string>
#include <vector>
#include "types.h"
#include "Horse.h"

using tPLayerId = char;
using tHorseList = std::vector<Horse>;

class PlayerInterface;
class Board;

class Player {
public:
    Player(const std::string &nickname, tPLayerId playerId);

    bool AddHorses(Board * pBoard, unsigned int nbHorses);
    void setPlayerInterface(PlayerInterface *pPlayerInterface);

    tHorseTargetCase ChooseMoveFrom(const tHorseTargetCaseList &listOfMoves);
    std::string getTypeOfPlayer();

    Horse* getHorse(unsigned int nb);
    unsigned int getHorseNumber(const Horse *horse) const;
    unsigned int getPlayerNb() const;

    unsigned char getPlayerId() const;
    const std::string& getNickname() const;
    void setNickname(const std::string& nickname);

    bool hasAlreadyWon() const;
    int getScore() const;

    inline bool isExitRequested() {
        return m_exitRequest;
    };

private:
    tHorseList m_horses;
    tPLayerId m_playerId;
    std::string m_nickname;
    PlayerInterface * m_pPlayerInterface = nullptr; //Pointer is necessary, in order to prevent truncation.
    bool m_exitRequest = false;
};

#endif