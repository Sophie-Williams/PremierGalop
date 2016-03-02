#include "HumanPlayerImpl.h"

tHorseTargetCase HumanPlayerImpl::ChooseMoveFrom(const tHorseTargetCaseList &listOfMoves) {
    tHorseTargetCase horseTargetCase = {nullptr, CASE_ID_UNKNOWN};
    bool bOK = false;
    do {
        eUserEventType userEvent = None;
        int horseNb = -1;
        int playerNb = -1;
        if (m_pBoard->GetChoiceFromEvents(userEvent, horseNb, playerNb) && userEvent != CloseEvent) {
            horseTargetCase.pHorse = nullptr;
            horseTargetCase.pTargetCase = CASE_ID_UNKNOWN;
            for (const auto & horseTargetCaseItem : listOfMoves) {
                switch (userEvent) {
                    case ClickedCase:
                        if (playerNb != static_cast<int> (horseTargetCaseItem.pHorse->getPlayer()->getPlayerNb())) {//It is not a horse of the current player!
                            break; //exit the switch (in fact it will be also possible to exit the for, because iteration is always on the same player)
                        }
                        //NO BREAK , we use the same treatment than HorseKey
                    case HorseKey:
                        if (static_cast<int> (horseTargetCaseItem.pHorse->getHorseNumber()) == horseNb) {
                            bOK = true;
                            horseTargetCase = horseTargetCaseItem;
                        }
                        break;
                    case ClickedPanel:
                        //find the first Horse in the box
                        if (horseTargetCaseItem.pHorse->isSleeping()) {
                            bOK = true;
                            horseTargetCase = horseTargetCaseItem;
                        }
                        break;
                    case None:
                    case CloseEvent:
                        break;
                }
            }
        } else {
            if (userEvent == CloseEvent) {
                horseTargetCase = ASK_QUIT;
                bOK = true;
            }
        }
    } while (!bOK);
    return horseTargetCase;
}