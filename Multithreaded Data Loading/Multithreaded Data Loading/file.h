#ifndef FILE_H
#define FILE_H
//
// File.h
// created: 2018-07-28
// author: Artyom Panfutov
//

#include "Connection.h"
#include <string>
#include "FALoad.h"
#include <vector>
class File
{
public:
	std::string InputFileName;
	std::string OutputFileName;
	std::string SavedFileName;
	std::string InputPath;
	std::string OutputPath;
	std::vector<std::string> LinesFromFile;
	File(std::string &, std::string &, std::string &);
	~File();

	void preparefile(std::vector <FALoad *>, std::string &, std::string &);

	void PutStrIntoVector();
	unsigned int GetCountOfStrings();
};


#endif