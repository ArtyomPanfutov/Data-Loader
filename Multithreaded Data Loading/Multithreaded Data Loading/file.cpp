// file.cpp
// created: 2018-07-28
// author: Artyom Panfutov

#include "file.h"
#include "FALoad.h"
#include <vector>
#include <fstream>
#include <string>
#include <list>
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
void file::preparefile( std::vector< FALoad *> Loadvec, std::string &fieldterm, std::string &rowterm)
{

	//std::string term1str = (const char *)Delimiter;
	//std::string term2str = "\r\n";

	//LPCBYTE term1 = reinterpret_cast<LPCBYTE>(Delimiter);
	//LPCBYTE term2 = reinterpret_cast<LPCBYTE>("\r\n");

	unsigned long
		batch = 0,
		strings_in_file = 0,
		threads = 0;

	std::ifstream file(this->inputfile);
	std::string cur_str;
	

	std::ofstream copy_out(outputfile, std::ios_base::out | std::ios_base::trunc);
	std::ofstream fout(outputfile, std::ios_base::out | std::ios_base::trunc);

	std::string::size_type pos{};
	size_t count{};

	std::vector<std::string> from_file;
	while (getline(file, cur_str))
	{
		//file >> cur_str;
		
		from_file.push_back(cur_str);
		copy_out << cur_str;
	}

	copy_out.close();

	strings_in_file = from_file.size();
	threads = Loadvec.size();

	batch = strings_in_file / threads;

	int columns = Loadvec.front()->TableColumns,
		line_num = 1,
		cur_thread = 0;

	int SPID = Loadvec[cur_thread]->SPID;
	
	for (int i = 0; i < from_file.size(); i++)
	{
		if (line_num > batch) 
		{
			line_num = 1;
			cur_thread++;
			SPID = Loadvec[cur_thread]->SPID;
		}
	    
		count = 0;
		pos = 0;

		cur_str = from_file[i];

		while ((pos = cur_str.find(fieldterm, pos)) != std::string::npos)
		{
			count++;
			pos++;
		}

		if (count < columns - 3) //-3
		{
			for (auto it = count; it < columns - 3; it++) //-3
			{
				cur_str += fieldterm;
			}
		    
			// Line num column
			
			
			
			
			// SPID column
		//	cur_str.insert(0, reinterpret_cast<char *>(SPID));
			//cur_str.insert(1, "`");
			
			
		}
		cur_str.insert(0, std::to_string(SPID) + fieldterm + std::to_string(line_num) + fieldterm);
		fout << cur_str << rowterm;
		line_num++;
	}


	file.close();
	fout.close();


} // End of preparefile()
//------------------------------------------------------------

