#ifndef __CHEVAUX_CHEVAUX_H_
#define __CHEVAUX_CHEVAUX_H_

#include <string> 

bool getOptions(int argc, char* argv[], std::string &ouputConfigFilePath, int &ouputNbHorses);
bool generateConfigFile();
bool help();

#endif //__CHEVAUX_CHEVAUX_H_