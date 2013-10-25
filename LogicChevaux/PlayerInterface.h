#ifndef __CHEVAUX_PLAYERINTERFACE__
#define __CHEVAUX_PLAYERINTERFACE__

#include <string>
#include "Board.h"

class PlayerInterface
{
protected:
	PlayerInterface(const std::string &sAiType, Board *pBoard) : m_sAiType(sAiType), m_pBoard(pBoard) {};
public:
	virtual tHorseTargetCase ChooseMoveFrom(const tHorseTargetCaseList &listOfMoves) = 0;
	virtual std::string& getTypeOfPlayer() { return m_sAiType;};
protected:
	Board * m_pBoard;
private:
	std::string m_sAiType;
};

#endif