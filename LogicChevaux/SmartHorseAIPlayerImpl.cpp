#include "SmartHorseAIPlayerImpl.h"

tHorseTargetCase SmartHorseAIPlayerImpl::ChooseMoveFrom(const tHorseTargetCaseList &listOfMoves) {
    int iNormalCase = 0;

    for (const auto & horseTargetCase : listOfMoves) {
        //urgence are always played in all cases. We must avoid the return to the sleeping box!
        if (m_pBoard->getCase(horseTargetCase.pTargetCase).isABaseForLadder() || horseTargetCase.pHorse->isRunningOnfirstCaseofLadderCase()) {
            return horseTargetCase;
        }
        if (horseTargetCase.pHorse->isRunningOnNormalCase()) {
            iNormalCase++;
            if (iNormalCase >= 2) {//Don't add a horse on the Normal Cases if there is already two horses on Normal Case!
                break;
            }
        }
    }
    const tHorseTargetCase * pHorseTargetCase = nullptr;
    int score = 0;
    for (const auto & horseTargetCase : listOfMoves) {
        int localScore = horseTargetCase.pHorse->getScore();
        if (localScore > score) {
            pHorseTargetCase = &horseTargetCase;
            score = localScore;
        }
        if (iNormalCase <= 2 && horseTargetCase.pHorse->isSleeping()) {
            return horseTargetCase;
        }
    }
    //if there is no sleeping horse, we choose the first possible move.
    return *pHorseTargetCase;
}