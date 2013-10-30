#include "HumanPlayerImpl.h"

tHorseTargetCase HumanPlayerImpl::ChooseMoveFrom(const tHorseTargetCaseList &listOfMoves)
{
	tHorseTargetCase horseTargetCase = {0};
	bool bOK = false;
	do
	{
		eUserEventType userEvent = None;
		int horseNb = -1;
		int playerNb = -1;
		if(m_pBoard->GetChoiceFromEvents(userEvent, horseNb, playerNb) && userEvent!=CloseEvent)
		{
			horseTargetCase.pHorse=NULL;
			horseTargetCase.pTargetCase=CASE_ID_UNKNOWN;
			for(tHorseTargetCaseList::const_iterator it = listOfMoves.begin(); !bOK && it != listOfMoves.end(); it++)
			{
				/*if(it->pHorse->isSleeping())
				{
					bOK = true;
					horseTargetCase = *it;
				}*/

				switch(userEvent)
				{
				case ClickedCase:
					if(playerNb != static_cast<int>(it->pHorse->getPlayer()->getPlayerNb()))
					{//It is not a horse of the current player!
						break;//exit the switch (in fact it will be also possible to exit the for, because iteration is always on the same player)
					}
					//NO BREAK , we use the same treatment than HorseKey
				case HorseKey:
					if(static_cast<int>(it->pHorse->getHorseNumber()) == horseNb)
					{
						bOK = true;
						horseTargetCase = *it;
					}
					break;
				case ClickedPanel:
					//find the first Horse in the box
					if(it->pHorse->isSleeping())
					{
						bOK = true;
						horseTargetCase = *it;
					}
				}
			}
			/*if(horseTargetCase.pHorse == NULL)
			{
				bOK = true;
				horseTargetCase = listOfMoves.front();
			}*/
		}
		else
		{
			if(userEvent==CloseEvent)
			{
				horseTargetCase = ASK_QUIT;
				bOK = true;
			}
		}
	}
	while(!bOK);
	return horseTargetCase;
}