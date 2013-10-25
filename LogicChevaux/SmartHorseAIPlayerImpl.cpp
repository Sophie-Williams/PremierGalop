#include "SmartHorseAIPlayerImpl.h"

tHorseTargetCase SmartHorseAIPlayerImpl::ChooseMoveFrom(const tHorseTargetCaseList &listOfMoves)
{
	int iNormalCase=0;

	for(tHorseTargetCaseList::const_iterator it = listOfMoves.begin(); it != listOfMoves.end(); it++)
	{
		//urgence are always played in all cases. We must avoid the return to the sleeping box!
		if(m_pBoard->getCase(it->pTargetCase).isABaseForLadder() || it->pHorse->isRunningOnfirstCaseofLadderCase())
		{
			return *it;
		}
		if(it->pHorse->isRunningOnNormalCase())
		{
			iNormalCase++;
			if(iNormalCase>=2)
			{//Don't add a horse on the Normal Cases if there is already two horses on Normal Case!
				break;	
			}
		}
	}
	const tHorseTargetCase * pHorseTargetCase = NULL;
	int score = 0;
	for(tHorseTargetCaseList::const_iterator it = listOfMoves.begin(); it != listOfMoves.end(); it++)
	{
		int localScore = it->pHorse->getScore();
		if( localScore > score)
		{
			pHorseTargetCase = &*it;
			score = localScore;
		}
		if(iNormalCase<=2 && it->pHorse->isSleeping())
		{
			return *it;
		}
	}
	//if there is no sleeping horse, we choose the first possible move.
	return *pHorseTargetCase;
}