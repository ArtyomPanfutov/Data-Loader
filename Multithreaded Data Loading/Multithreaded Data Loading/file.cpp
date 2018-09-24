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
File::File(std::string &filename, std::string &InputPath, std::string &OutputPath)
	: InputFileName(filename), InputPath(InputPath), OutputPath(OutputPath)
{  
	OutputFileName = filename + ".out";
	SavedFileName = filename + ".sav";
}
// End of constructor
//---------------------------------------------------------------

// Destructor 
////////////////////////////////////////////////////////////////
File::~File()
{
	LinesFromFile.clear();
}
// End of destructor
////////////////////////////////////////////////////////////////



// preparefile()
///////////////////////////////////////////////////////////////
void File::preparefile( std::vector< FALoad *> Loadvec, std::string &fieldterm, std::string &rowterm)
{
	unsigned long
		batch = 0,
		strings_in_file = 0,
		threads = 0;

	std::ifstream File(this->InputFileName);
	std::string cur_str;
	

	std::ofstream copy_out(OutputFileName, std::ios_base::out | std::ios_base::trunc);
	std::ofstream fout(OutputFileName, std::ios_base::out | std::ios_base::trunc);

	std::string::size_type pos{};
	size_t count{};

	std::vector<std::string> from_file;
	while (getline(File, cur_str))
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
		    
		}
		cur_str.insert(0, std::to_string(SPID) + fieldterm + std::to_string(line_num) + fieldterm);
		fout << cur_str << rowterm;
		line_num++;
	}


	File.close();
	fout.close();


} // End of preparefile()
//------------------------------------------------------------

// PutStrIntoVector
//////////////////////////////////////////////////////////////
void File::PutStrIntoVector()
{
  std::ifstream File;

  File.open(this->InputFileName, std::ifstream::in);

  if (!File.is_open())
	  throw std::exception("\n ERROR: Cannot open input file! \n"); 
 

  std::string cur_str;

  LinesFromFile.erase(LinesFromFile.cbegin(), LinesFromFile.cend());

  while (getline(File, cur_str))
  {
	  LinesFromFile.push_back(cur_str);
  }

  File.close();
} // End of PutStrIntoVector
//------------------------------------------------------------


// GetCountOfStrings()
///////////////////////////////////////////////////////////////
unsigned int File::GetCountOfStrings()
{
	return this->LinesFromFile.size();
} // End of GetCountOfStrings
//-------------------------------------------------------------