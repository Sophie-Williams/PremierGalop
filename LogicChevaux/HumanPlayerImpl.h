#ifndef __CHEVAUX_HUMANPLAYERINTERFACE__
#define __CHEVAUX_HUMANPLAYERINTERFACE__
#include "PlayerInterface.h"

class HumanPlayerImpl :
	public PlayerInterface
{
public:
	HumanPlayerImpl(Board * pBoard) :  PlayerInterface("Humain", pBoard) {};
	virtual tHorseTargetCase ChooseMoveFrom(const tHorseTargetCaseList &listOfMoves);
};

#endif