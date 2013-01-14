#ifndef __CHEVAUX_ONEHORSEAIPLAYER__
#define __CHEVAUX_ONEHORSEAIPLAYER__

#include "PlayerInterface.h"

class OneHorseAIPlayerImpl :
	public PlayerInterface
{
public:
	OneHorseAIPlayerImpl() : PlayerInterface("IA: un cheval") {};
	virtual tHorseTargetCase ChooseMoveFrom(const tHorseTargetCaseList &listOfMoves);
};

#endif
