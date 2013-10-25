#ifndef __CHEVAUX_SMARTHORSEAIPLAYER__
#define __CHEVAUX_SMARTHORSEAIPLAYER__

#include "Board.h"
#include "PlayerInterface.h"

class SmartHorseAIPlayerImpl :
	public PlayerInterface
{
public:
	SmartHorseAIPlayerImpl(Board *pBoard) : PlayerInterface("IA: chevaux intelligents", pBoard) {};
	virtual tHorseTargetCase ChooseMoveFrom(const tHorseTargetCaseList &listOfMoves);
};
#endif