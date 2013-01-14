#ifndef __CHEVAUX_SMARTHORSEAIPLAYER__
#define __CHEVAUX_SMARTHORSEAIPLAYER__

#include "Board.h"
#include "PlayerInterface.h"

class SmartHorseAIPlayerImpl :
	public PlayerInterface
{
public:
	SmartHorseAIPlayerImpl() : PlayerInterface("IA: chevaux intelligents") {};
	virtual tHorseTargetCase ChooseMoveFrom(const tHorseTargetCaseList &listOfMoves);
};
#endif