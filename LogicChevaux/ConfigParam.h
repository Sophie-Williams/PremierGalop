#ifndef __CHEVAUX__CONFIGPARAM__
#define __CHEVAUX__CONFIGPARAM__

#include <vector>
#include <map>

#define CONFIG_TOPOLOGY		"Topology"
#define CONFIG_AI			"AI_"
#define CONFIG_NICKNAME		"Nickname_"
#define CONFIG_NBHORSES		"NbHorses"

class Board;
class PlayerInterface;

typedef std::vector<PlayerInterface*> tPlayerInterfaceVector;
typedef std::map<std::string,std::string> tMapStringString;

class ConfigParam
{
public:
	ConfigParam();
	virtual ~ConfigParam();

	//ask options from user and fill the dictionnary with the answers
	void displayAndConfigure(Board *board, const tPlayerInterfaceVector &playerInterfaceVector);

	//Read options from file and save in the dictionary
	bool loadConfigFromFile(const std::string &fileToOpen);

	//saving dictionary into file
	bool saveConfigToFile(const std::string &outputFile);

	//Reading methods from dictionary
	bool getStrValue(const std::string &keyStr, std::string &outputStr);
	bool getIntValue(const std::string &keyStr, int &outputInt);
	bool getMapFile(std::string &outputStr);
	bool getPlayerNameAI(int playerId, int &outputAiNumber, std::string &outputPlayerName);
	bool getNbHorses(int &outputNbHorses);

private:
	tMapStringString m_dico;
};

#endif