#include "AllHorseAIPlayerImpl.h"

tHorseTargetCase AllHorseAIPlayerImpl::ChooseMoveFrom(const tHorseTargetCaseList &listOfMoves) {
    for (const auto& horseTargetCase : listOfMoves) {
        //if a sleeping horse is found, we choose it!
        if (horseTargetCase.pHorse->isSleeping()) {
            return horseTargetCase;
        }
    }
    //if there is no sleeping horse, we choose the first possible move.
    //TODO in the future, it will be a good idea to check if the first possible move is the most advanced horse on the board (using score)!
    return listOfMoves.front();
}