#ifndef __CHEVAUX_HUMANPLAYERINTERFACE__
#define __CHEVAUX_HUMANPLAYERINTERFACE__
#include "PlayerInterface.h"

class HumanPlayerImpl :
	public PlayerInterface
{
public:
	HumanPlayerImpl(Board * pBoard) :  PlayerInterface("Humain") , m_pBoard(pBoard) {};
	virtual tHorseTargetCase ChooseMoveFrom(const tHorseTargetCaseList &listOfMoves);
protected:
	Board * m_pBoard; //in fact there is a big chance that the board is a GfxBoard!
};

#endif