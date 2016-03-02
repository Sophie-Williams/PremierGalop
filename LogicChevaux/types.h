#ifndef  __CHEVAUX_TYPES_H__
#define __CHEVAUX_TYPES_H__

typedef unsigned char U8;
typedef signed char S8;
typedef unsigned short U16;
typedef signed short S16;
typedef unsigned long U32;
typedef signed long S32;


//The following types are used in many header files, but they don't need the definition of classes (because of pointer usage)
#include <list>

#include "Case.h"
class Horse;

using tHorseTargetCase = struct sHorseTargetCase {
    Horse * pHorse;
    tCaseId pTargetCase;
};

const tHorseTargetCase ASK_QUIT = {nullptr, CASE_ID_UNKNOWN};

inline bool mHorseTargetCaseEquals(const tHorseTargetCase& a, const tHorseTargetCase& b) {
    return a.pHorse==b.pHorse && a.pTargetCase.id==b.pTargetCase.id;
}

using tHorseTargetCaseList = std::list<tHorseTargetCase>;

#endif