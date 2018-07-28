#ifndef FILE_H
#define FILE_H
// file.h
// created: 2018-07-28
// author: Artyom Panfutov

#include "Connection.h"
#include <string>

class file
{
public:
	std::string inputfile;
	std::string outputfile;
	std::string savedfile;
	std::string inputpath;
	std::string savepath;
	file(std::string &, std::string &, std::string &);
	~file();

	void preparefile(unsigned long int &, std::string &, std::string &, int &);
};


#endif