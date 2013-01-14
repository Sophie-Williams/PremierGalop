#ifndef __CHEVAUX_ALLHORSEAIPLAYER__
#define __CHEVAUX_ALLHORSEAIPLAYER__

#include "Board.h"
#include "PlayerInterface.h"

class AllHorseAIPlayerImpl :
	public PlayerInterface
{
public:
	AllHorseAIPlayerImpl() : PlayerInterface("IA: max chevaux") {};
	virtual tHorseTargetCase ChooseMoveFrom(const tHorseTargetCaseList &listOfMoves);
};
#endif