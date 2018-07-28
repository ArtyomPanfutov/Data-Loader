// file.cpp
// created: 2018-07-28
// author: Artyom Panfutov

#include "file.h"
#include <fstream>
#include <iostream>
/////////////////////////////////////////////////////////////////

// Constructor
/////////////////////////////////////////////////////////////////
file::file(std::string &filename, std::string &inputpath, std::string &savepath)
	: inputfile(filename), inputpath(inputpath), savepath(savepath)
{  
	outputfile = filename + ".out";
	savedfile = filename + ".sav";
}
// End of constructor
//---------------------------------------------------------------

// Destructor 
////////////////////////////////////////////////////////////////
file::~file()
{ }
// End of destructor
////////////////////////////////////////////////////////////////



// preparefile()
///////////////////////////////////////////////////////////////
void file::preparefile(unsigned long int &columns, std::string &fieldterm, std::string &rowterm, int &offset)
{
	
	//std::string term1str = (const char *)Delimiter;
	//std::string term2str = "\r\n";

	//LPCBYTE term1 = reinterpret_cast<LPCBYTE>(Delimiter);
	//LPCBYTE term2 = reinterpret_cast<LPCBYTE>("\r\n");


	std::ifstream file(inputfile);
	std::string curstr;

	std::ofstream fout(outputfile, std::ios_base::out | std::ios_base::trunc);

	std::string::size_type pos{};
	size_t count{};

	while (getline(file, curstr)) { // пока не достигнут конец файла класть очередную строку в переменную (s)
		count = 0;
		pos = 0;
		//std::cout << s << std::endl; // выводим на экран
		while ((pos = curstr.find(fieldterm, pos)) != std::string::npos)
		{
			count++;
			pos++;
		}

		if (count < columns - offset) //-3
		{

			for (auto it = count; it < columns - offset; it++) //-3
			{
				curstr += fieldterm;

			}
			//s += term2str;
			fout << curstr << rowterm;
		}

		count = 0;
		pos = 0;
		//std::cout << s << std::endl; // выводим на экран

	}

	file.close();
	fout.close();


} // End of preparefile()
//------------------------------------------------------------

