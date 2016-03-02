#ifndef __CHEVAUX_ONEHORSEAIPLAYER__
#define __CHEVAUX_ONEHORSEAIPLAYER__

#include "PlayerInterface.h"

class OneHorseAIPlayerImpl :
public PlayerInterface {
public:

    OneHorseAIPlayerImpl(Board *pBoard) : PlayerInterface("IA: un cheval", pBoard) {
    };
    virtual tHorseTargetCase ChooseMoveFrom(const tHorseTargetCaseList &listOfMoves);
};

#endif
