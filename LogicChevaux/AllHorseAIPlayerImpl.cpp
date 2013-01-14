#include "AllHorseAIPlayerImpl.h"

tHorseTargetCase AllHorseAIPlayerImpl::ChooseMoveFrom(const tHorseTargetCaseList &listOfMoves)
{
	for(tHorseTargetCaseList::const_iterator it = listOfMoves.begin(); it != listOfMoves.end(); it++)
	{
		//if a sleeping horse is found, we choose it!
		if(it->pHorse->isSleeping())
		{
			return *it;
		}
	}
	//if there is no sleeping horse, we choose the first possible move.
	//TODO in the future, it will be a good idea to check if the first possible move is the most advanced horse on the board (using score)!
	return listOfMoves.front();
}