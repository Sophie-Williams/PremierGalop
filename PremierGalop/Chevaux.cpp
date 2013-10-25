// Chevaux.cpp : définit le point d'entrée pour l'application console.
//
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include "Chevaux.h"
//#include "Board.h"
#include "GfxBoard.h"
#include "AllHorseAIPlayerImpl.h"
#include "SmartHorseAIPlayerImpl.h"
#include "OneHorseAIPlayerImpl.h"
#include "HumanPlayerImpl.h"
#include "ConfigParam.h"

#define DEFAULT_MAP_FILE "../maps/classic4players.map"

using namespace std;

int main(int argc, char* argv[])
{
	bool result = true;
	std::string outputConfigFilePath;
	int nbHorses;
	if(!getOptions(argc, argv, outputConfigFilePath, nbHorses) || (outputConfigFilePath.size()==0))
	{//use default path
		std::cerr << "Error parsing command line or no file parameter" << std::endl;
		outputConfigFilePath = "../.PremierGalop";
	}

	ConfigParam configParam;
	//try to locate config file
	//if it can't be located, generate a new one in the installation directory (need to test if the excutable can be found, in order to know, if we are in the bin directory)
	bool bConfFromFile = configParam.loadConfigFromFile(outputConfigFilePath);
	if(nbHorses<=0)
	{//nbHorse was not retrieved from the commandline
		if(!bConfFromFile || !configParam.getNbHorses(nbHorses))
		{//if nbHorses can't be retrieved from the config file, use a default value
			nbHorses = 100;
		}
	}
	//Board board;
	GfxBoard board(nbHorses);
	std::string mapFile(DEFAULT_MAP_FILE);
	if(bConfFromFile)
	{
		configParam.getMapFile(mapFile);
	}
	//return board.ReadBoardTopologyFromFile(reinterpret_cast<char*>(argv[1]));
	if((result = board.ReadBoardTopologyFromFile(mapFile))==false)
	{
		cerr << "Can't open map file: " << mapFile << endl;
		return -1;
	}
	Player * pPlayer = board.getPlayer(0);//get first player
	tPlayerInterfaceVector vectorOfAis;
	AllHorseAIPlayerImpl allAI(&board);
	vectorOfAis.push_back(&allAI);
	SmartHorseAIPlayerImpl smartAI(&board);
	vectorOfAis.push_back(&smartAI);
	OneHorseAIPlayerImpl oneAI(&board);
	vectorOfAis.push_back(&oneAI);
	HumanPlayerImpl humanAI(&board);
	vectorOfAis.push_back(&humanAI);
	//configParam.displayAndConfigure(&board, vectorOfAis);
	if(bConfFromFile)
	{
		for(unsigned int playerId=0; playerId < board.getNumberOfPlayers(); playerId++)
		{
			int aiNumber=0;
			std::string playerName;
			if(configParam.getPlayerNameAI(playerId, aiNumber, playerName))
			{
				Player * pPlayerToUpdate = board.getPlayer(playerId);
				if(aiNumber>= 0 && static_cast<std::string::size_type>(aiNumber) < vectorOfAis.size())
				{
					pPlayerToUpdate->setPlayerInterface(vectorOfAis[aiNumber]);
				}
				else //invalid ai, so we use a default AI
				{
					cerr << "use a default AI for playerId=" << playerId << endl;
					pPlayerToUpdate->setPlayerInterface(&smartAI);
				}
				pPlayerToUpdate->setNickname(playerName);
			}
		}
	}
	else
	{
		board.getPlayer(0)->setPlayerInterface(&smartAI);//red
		board.getPlayer(1)->setPlayerInterface(&allAI);//green
		board.getPlayer(2)->setPlayerInterface(&oneAI);//blue
		board.getPlayer(3)->setPlayerInterface(&allAI);//yellow
	}
	unsigned int die = 0;
	unsigned int currentTurn = 0;
	tHorseTargetCaseList horseTargetCaseList;
	board.displayBoard(); //display all the board the first time
	tpCaseList caseListwhichNeedGraphicalUpdate;
	while(result)
	{
		result = board.playOneTurnRequest(pPlayer,horseTargetCaseList, die);
		if(result)
		{
			assert(pPlayer!=NULL);
			board.displayLeftPannel(pPlayer->getTypeOfPlayer(),
				pPlayer->getNickname(),
				die,
				board.getScores(),
				Board::GetHorseNumberInTheBox(horseTargetCaseList),
				pPlayer->getPlayerNb());

			//cout << "Le joueur <<" << pPlayer->getNickname() << ">> lance le petit cube avec des points! (il obtient " << die << ")" << endl;
			//cout << "Vous avez le(s) choix suivant(s)" << endl;
			//for(tHorseTargetCaseList::const_iterator it = horseTargetCaseList.begin(); it != horseTargetCaseList.end(); it++)
			//{
				//cout << "\t Cheval : " << it->pHorse->getHorseNumber() 
				//	 << "\t Localisation:" << it->pTargetCase->getLocalization()	<< endl ;
			//}
			tHorseTargetCase horseTargetCase = {0};
			if(horseTargetCaseList.size() == 0)
			{
				//cout << "Zut <<" << pPlayer->getNickname() <<">>, tu ne peux pas bouger de chevaux" << endl;
			}
			else if(horseTargetCaseList.size() == 1)
			{
				//cout << "Joueur: <<" << pPlayer->getNickname() << ">>. Un seul de vos chevaux peut courrir (pas la peine de choisir, je le fais pour vous!)" << endl;
				horseTargetCase = horseTargetCaseList.front();
			}
			else
			{
				bool bAllMovableHorsesAreSleepingInTheBox = true;
				for(tHorseTargetCaseList::const_iterator it = horseTargetCaseList.begin(); it != horseTargetCaseList.end(); it++)
				{
					if(!it->pHorse->isSleeping())
					{
						bAllMovableHorsesAreSleepingInTheBox = false;
					}
				}
				if(bAllMovableHorsesAreSleepingInTheBox)
				{
					//cout << "Joueur <<" << pPlayer->getNickname() << ">>. Tout vos chevaux deplacables sont aux écuries, je vais donc choisir le premier d'entre eux!" << endl;
					horseTargetCase = horseTargetCaseList.front();
				}
				else
				{
					//cout << "Le joueur <<" << pPlayer->getNickname() << ">> doit choisir le cheval qu'il veut bouger." << endl;
					horseTargetCase = pPlayer->ChooseMoveFrom(horseTargetCaseList);//call Artificial Intelligence in order to choose the move
				}
			}
			caseListwhichNeedGraphicalUpdate.clear();
			if(horseTargetCaseList.size()>=1)
			{//update graphic board when a horse is moved!
				if(horseTargetCase.pHorse!=NULL && Case::isValidCaseId(horseTargetCase.pTargetCase))
				{
					if(horseTargetCase.pHorse->isRunning())
					{
						caseListwhichNeedGraphicalUpdate.push_back(horseTargetCase.pHorse->getCase());//source case
					}
					caseListwhichNeedGraphicalUpdate.push_back(horseTargetCase.pTargetCase);//target case
				}
			}
			result = board.playOneTurnResponse(horseTargetCase.pHorse,horseTargetCase.pTargetCase,die,pPlayer);
			if(pPlayer!=NULL && pPlayer==board.getPlayer(0) && die!=board.getMaxLadderCaseValue())
			{
				std::vector<int> scores = board.getScores();
				board.storeAndReduceScores(currentTurn,800,scores);
				currentTurn++;
			}
			if(result)
			{
				result = board.displayBoard(caseListwhichNeedGraphicalUpdate);
			}
			if(pPlayer == NULL)
			{
				cout << "Le jeu est fini!" << endl;
				board.displayHistoric();
			}
		}
	}
	return result;
}

bool getOptions(int argc, char* argv[], std::string &ouputConfigFilePath, int &ouputNbHorses)
{
	ouputNbHorses = -1;
	string tempStr;
	for(int i=1; i<argc; i++)//first argument is the name of the program, so it is skipped by the analysis
	{
		tempStr = argv[i];
		if(tempStr == "-c" || tempStr == "--config")
		{
			if(i + 1 < argc)
			{
				ouputConfigFilePath = argv[++i];
			}
			else
			{
				std::cerr << "-c or --config flag must be followed by a configuration file path" << std::endl;
				exit(200);
			}
		}
		else if(tempStr == "-g" || tempStr == "--generate")
		{
			exit(generateConfigFile()?0:300);
		}
		else if(tempStr == "-n" || tempStr == "--nbhorse")
		{
			if(i + 1 < argc)
			{
				if(!sscanf(argv[++i],"%d",&ouputNbHorses))
				{
					ouputNbHorses = -1;
					std::cerr << "Invalid format for the number of horses." << std::endl;
					exit(400);
				}
			}
			else
			{
				std::cerr << "-n or -nbhorse flag must be followed by a number of horses" << std::endl;
				exit(401);
			}
		}
		else
		{
			help();
			exit(500);
		}
	}
	return true;
}

bool generateConfigFile()
{
	//TODO implement generateConfigFile
	return true;
}

bool help()
{
	std::cerr << "program can take the following arguments:" << std::endl;
	std::cerr << "-h or --help for help" << std::endl;
	std::cerr << "-c or --config for indicating where to find the config file" << std::endl;
	std::cerr << "-g or --generate for generating a model of config file in the current directory" << std::endl;
	std::cerr << "-n or --nbHorse for indicationg how many horses will be used for each player"
		" during the game (this parameter replaces the number of horses in the config file parameter)" << std::endl;
	return true;
}