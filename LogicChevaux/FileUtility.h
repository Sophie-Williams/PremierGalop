#ifndef CHEVAUX_FILE_UTILITY_H
#define CHEVAUX_FILE_UTILITY_H

#include <fstream>
#include <string>

const std::string STR_PARENT_DIRECTORY = "../";

bool tryToLoadFromParentDirectory(std::ifstream& inputFile, const std::string& file);

#endif // CHEVAUX_FILE_UTILITY_H