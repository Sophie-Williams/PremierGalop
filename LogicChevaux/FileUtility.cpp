#include <iostream>
#include "FileUtility.h"

bool tryToLoadFromParentDirectory(std::ifstream& inputFile, const std::string& file) {
    if (inputFile.fail()) {
        std::cerr << "Fail to open the path: " << file.c_str() << std::endl;
        std::string::size_type index = file.find(STR_PARENT_DIRECTORY);
        if (index == 0) //it means that ../ has been found at the first position of the string!
        {//Remove one dot ("../" becomes "./");
            std::cerr << "Index found for " << STR_PARENT_DIRECTORY << " : " << index << std::endl;
            inputFile.clear(); //must clear failbit, before trying again to open a file
            std::string parentDirectory = file.substr(STR_PARENT_DIRECTORY.length());
            inputFile.open(parentDirectory); //open file with the string without the first char
            if (inputFile.fail()) {
                std::cerr << "Fail to open the alternative path: " << parentDirectory << std::endl;
                return false;
            }
        } else//can't find ../ at the beginning of the string
        {
            return false;
        }
    }
    return true;
}