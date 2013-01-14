#ifndef __CHEVAUX_PLAYERINTERFACE__
#define __CHEVAUX_PLAYERINTERFACE__

#include <string>
#include "Board.h"

class PlayerInterface
{
protected:
	PlayerInterface(const std::string &sAiType) : m_sAiType(sAiType) {};
public:
	virtual tHorseTargetCase ChooseMoveFrom(const tHorseTargetCaseList &listOfMoves) = 0;
	virtual std::string& getTypeOfPlayer() { return m_sAiType;};
private:
	std::string m_sAiType;
};

#endif