// bin2C.cpp : définit le point d'entrée pour l'application console.
//

#include <iostream>
#include <iomanip>
#include <fstream>


int main(int argc, char* argv[])
{
	if(argc>=2)
	{
		std::ifstream inputStream(argv[1],std::ios_base::binary);
		std::string inputFileStr(argv[1]);
		std::string outputFileStr(inputFileStr);
		outputFileStr += ".h";
		std::ofstream outputStream(outputFileStr.c_str());
		std::string varNameStr;
		for(std::string::iterator it=inputFileStr.begin();it!=inputFileStr.end();it++)
		{
			if(*it=='\\' || *it == '/')
			{//it means that we where parsing a directory, not a file name, so we reset the var name.
				varNameStr.clear();
			}
			else if(*it=='.' || *it==' ' || *it=='-')//replace all invalid characters for a variable by an underscore
			{
				varNameStr.push_back('_');
			}
			else
			{
				varNameStr.push_back(*it);
			}
		}
		if(inputStream.is_open() && outputStream.is_open())
		{
			outputStream << "const char " << varNameStr.c_str() << "[] = {" << std::hex << std::endl;
			char buffer[1024];
			while(inputStream.good() && !inputStream.eof())
			{
				inputStream.read(buffer,sizeof(buffer));
				int nbReadBytes = inputStream.gcount();
				for(int i=0; i < nbReadBytes; i++)
				{
					outputStream << "\t'\\x" << std::setw(2) << std::setfill('0') << (static_cast<unsigned int>(buffer[i]) & 0xFF) << "'," << std::endl;
				}
			}
			outputStream << "};" << std::endl;
			return 0;
		}
		else
			return 2;
	}
	return 1;
}

