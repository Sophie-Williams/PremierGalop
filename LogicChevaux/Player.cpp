#include <cassert>
#include "Player.h"
#include "Board.h"
#include "PlayerInterface.h"

Player::Player(const std::string &nickname, tPLayerId playerId)
: m_playerId(playerId), m_nickname(nickname) {
}

bool Player::AddHorses(Board * pBoard, unsigned int nbHorses) {
    assert(pBoard != nullptr);
    for (unsigned int i = 0; i < nbHorses; i++) {
        m_horses.push_back(Horse(pBoard, this));
    }
    return true;
}

void Player::setPlayerInterface(PlayerInterface *pPlayerInterface) {
    m_pPlayerInterface = pPlayerInterface;
}

tHorseTargetCase Player::ChooseMoveFrom(const tHorseTargetCaseList &listOfMoves) {
    tHorseTargetCase htc;
    assert(m_pPlayerInterface != nullptr);
    htc = m_pPlayerInterface->ChooseMoveFrom(listOfMoves);
    if (mHorseTargetCaseEquals(htc, ASK_QUIT))
        m_exitRequest = true;
    return htc;
}

std::string Player::getTypeOfPlayer() {
    assert(m_pPlayerInterface != nullptr);
    return m_pPlayerInterface->getTypeOfPlayer();
}

Horse* Player::getHorse(unsigned int nb) {
    assert(nb < m_horses.size());
    return &m_horses[nb];
}

unsigned int Player::getHorseNumber(const Horse *horse) const {
    if (horse != nullptr) {
        unsigned int horseNumber = 0;
        for (const auto& horseItem : m_horses) {
            if (horse == &horseItem) {
                return horseNumber; //return index of horse in the m_horses vector
            }
            horseNumber++;
        }
    }
    return static_cast<unsigned int> (~0); //invalid value
}

unsigned int Player::getPlayerNb() const {
    return m_playerId - 'a';
}

unsigned char Player::getPlayerId() const {
    return m_playerId;
}

const std::string& Player::getNickname() const {
    return m_nickname;
}

void Player::setNickname(const std::string& nickname) {
    m_nickname = nickname;
}

bool Player::hasAlreadyWon() const {
    for (const auto& horse : m_horses) {
        if (!horse.isArrived()) {
            return false;
        }
    }
    return true;
}

int Player::getScore() const {
    int score = 0;
    for (const auto& horse : m_horses) {
        score += horse.getScore();
    }
    return score;
}