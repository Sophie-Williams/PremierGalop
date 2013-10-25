#ifndef __CHEVAUX_ALLHORSEAIPLAYER__
#define __CHEVAUX_ALLHORSEAIPLAYER__

#include "Board.h"
#include "PlayerInterface.h"

class AllHorseAIPlayerImpl :
	public PlayerInterface
{
public:
	AllHorseAIPlayerImpl(Board *pBoard) : PlayerInterface("IA: max chevaux", pBoard) {};
	virtual tHorseTargetCase ChooseMoveFrom(const tHorseTargetCaseList &listOfMoves);
};
#endif