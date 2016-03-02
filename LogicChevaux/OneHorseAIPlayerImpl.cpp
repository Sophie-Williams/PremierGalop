#include "OneHorseAIPlayerImpl.h"

tHorseTargetCase OneHorseAIPlayerImpl::ChooseMoveFrom(const tHorseTargetCaseList &listOfMoves) {
    return listOfMoves.front();
}