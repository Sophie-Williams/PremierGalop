#include <iostream>
#include <sstream>
#include <fstream>
#include "ConfigParam.h"
#include "Board.h"
#include "PlayerInterface.h"
#include "FileUtility.h"
#include "StringUtility.h"

#define MANUAL_CONFIG "ManualConfig"
#define TOPOLOGY_FILE "Topology"
#define AI_TYPE "AI_%d"
#define PLAYER_NICKNAME "Nickname_%d"

ConfigParam::ConfigParam()
{
}

ConfigParam::~ConfigParam()
{
}

void ConfigParam::displayAndConfigure(Board *pBoard, const tPlayerInterfaceVector &playerInterfaceVector)
{
	if(pBoard!=NULL)
	{
		std::cout << "Game configuration:" << std::endl;
		std::cout << "You can choose the following intelligences :" << std::endl;
		unsigned int i = 0;
		for(tPlayerInterfaceVector::const_iterator it=playerInterfaceVector.begin();it!=playerInterfaceVector.end();it++,i++)
		{
			std::cout << i << " : " << (*it)->getTypeOfPlayer() << std::endl;
		}
		std::string name;
		for(tPlayerList::iterator it=pBoard->getPlayersList().begin();it!=pBoard->getPlayersList().end();it++)
		{
			//Step 1 choose AI
			do
			{
				std::cout << "choose Ai number for player " << it - pBoard->getPlayersList().begin() << " : ";
				std::cin >> i;
				if (std::cin.fail())
				{
					// not a numeric character, probably
					// clear the failure and pull off the non-numeric character
					std::cin.clear();
					char c;
					std::cin >> c;
				}
			}
			while(i<0 || i>= playerInterfaceVector.size());
			it->setPlayerInterface(playerInterfaceVector[i]);
			//Step 2 Choose Name:
			std::cout << "Choose Name for player " << it - pBoard->getPlayersList().begin() << " : ";
			name.clear();
			std::cin >> name;
			it->setNickname(name);
		}
	}
}

bool ConfigParam::loadConfigFromFile(const std::string &fileToOpen)
{
	//read file line by line
	std::ifstream configFile(fileToOpen.c_str());
	if(tryToLoadFromParentDirectory(configFile, fileToOpen))
	{
		while(configFile.good() && !configFile.eof())
		{
			std::string line;
			std::getline(configFile,line);
			//analyse line and register the variable in the dictionary:
			std::string::size_type startPosition = line.find_first_not_of(" \t");
			if(startPosition<line.size()
				&& line[startPosition]!='#')//it is not a comment!
			{
				std::string::size_type equalPosition = line.find_first_of('=',startPosition);
				if(equalPosition < line.size())
				{
					std::string trimmedVariableName = line.substr(startPosition,equalPosition-startPosition);
					std::string trimmedVariableValue = line.substr(equalPosition+1,line.size()-(equalPosition+1));
					//Remove white spaces at the begin and the end of the string
					trimmedVariableName = trim(trimmedVariableName);
					trimmedVariableValue = trim(trimmedVariableValue);
					m_dico[trimmedVariableName]=trimmedVariableValue; //variable = value
				}
			}
		}
		return true;
	}
	//close the file
	return false;
}

bool ConfigParam::saveConfigToFile(const std::string &outputFile)
{
	//write file line by line
	std::ofstream configFile(outputFile.c_str());
	if(!configFile.fail() && configFile.good())
	{
		for(tMapStringString::iterator it=m_dico.begin();it!=m_dico.end();it++)
		{
			configFile << it->first << "=" << it->second;
		}
		return true;
	}
	//close the file
	return false;
}

bool ConfigParam::getStrValue(const std::string &keyStr, std::string &outputStr)
{
	tMapStringString::iterator it =  m_dico.find(keyStr);
	if(it!=m_dico.end())
	{
		outputStr = it->second;
		return true;
	}
	return false;
}

bool ConfigParam::getIntValue(const std::string &keyStr, int &outputInt)
{
	std::string outputStr;
	if(getStrValue(keyStr,outputStr))
	{
		std::stringstream ouputStrStream;
		ouputStrStream << outputStr;
		int i = 0;
		ouputStrStream >> i;
		if (!ouputStrStream.fail())
		{
			outputInt = i;
			return true;
		}
	}
	return false;
}

bool ConfigParam::getMapFile(std::string &outputStr)
{
	return getStrValue(CONFIG_TOPOLOGY,outputStr);
}

bool ConfigParam::getPlayerNameAI(int playerId, int &outputAiNumber, std::string &outputPlayerName)
{
	std::string aiLabel;
	std::stringstream ouputStrStream;
	ouputStrStream << CONFIG_AI << playerId;
	ouputStrStream >> aiLabel;
	ouputStrStream.clear();
	std::string playerNameLabel;
	ouputStrStream << CONFIG_NICKNAME << playerId;
	ouputStrStream >> playerNameLabel;
	if(!getIntValue(aiLabel,outputAiNumber))
	{
		outputAiNumber = 0;//use a default value
	}
	return getStrValue(playerNameLabel,outputPlayerName);
}

bool ConfigParam::getNbHorses(int &outputNbHorses)
{
	if(!getIntValue(CONFIG_NBHORSES,outputNbHorses))
	{
		outputNbHorses = -1;//use a default value
		return false;
	}
	return true;
}