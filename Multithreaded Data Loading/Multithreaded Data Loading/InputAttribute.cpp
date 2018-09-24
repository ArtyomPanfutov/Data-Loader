// InputAttribute.cpp
// created: 2018-08-18
// author: Artyom Panfutov

#include "InputAttribute.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>
#include "Output.h"
#include "constant.h"


////////////////////////////////////////////////////////////////////////

/***********************************************************************
*************** Definiton of methods. Class InputAttribute *************
***********************************************************************/

std::string LogFileName = "LoadLog.txt";
Log InputLogWriter(LogFileName);

// Constructor of InputAttribute
////////////////////////////////////////////////////////////////////////
InputAttribute::InputAttribute() 
{
  // Get info from config file.
	this->GetConfig();
	
  // Get info from user.
    this->ServerNameRequest(this->server.from_file);

	this->DBNameRequest(this->database.from_file);

	this->DriverRequest(this->driver.from_file);

	this->UserNameRequest();

	this->UserPassRequest();

	this->BuildConnectionString();

	this->LoadBriefRequest();

	this->FileNameRequest();

	this->ThreadsRequest();	

	this->BranchIDRequest();

  // Clear the console.
	system("cls");

  // And display input data.
	this->DisplayAllAttributes();


} // End of InputAttribute constructor 
//----------------------------------------------------------------------

// Destructor of InputAttribute
////////////////////////////////////////////////////////////////////////
InputAttribute::~InputAttribute()
{ }
//----------------------------------------------------------------------

// UserNameRequest
////////////////////////////////////////////////////////////////////////
void InputAttribute::UserNameRequest()
{
	std::cout << "\n";
	std::cout << "\n Username: ";
	std::cin >> this->user_name;

} // End of UserNameRequest
//----------------------------------------------------------------------


// UserPassRequest
////////////////////////////////////////////////////////////////////////
void InputAttribute::UserPassRequest()
{
	// Set console mode to hide input characters.
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode = 0;
	GetConsoleMode(hStdin, &mode);
	SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));

	std::cout << "\n Password(characters hided): ";
	
	std::cin >> this->user_pass;
	std::cout << std::endl;

	// Cleanup.
	SetConsoleMode(hStdin, mode);

} // End of UserPassRequest
//----------------------------------------------------------------------


// ServerNameRequest
////////////////////////////////////////////////////////////////////////
void InputAttribute::ServerNameRequest(bool &from_file)
{
	if (!from_file)
	{
		std::cout << "\n Servername: ";
		std::cin >> this->server.srv_name;
		std::cout << std::endl;
	}


} // End of ServerNameRequest
//---------------------------------------------------------------------



// DBNameRequest
///////////////////////////////////////////////////////////////////////
void InputAttribute::DBNameRequest(bool &from_file)
{
	if (!from_file)
	{
		std::cout << "\n Database name: ";
		std::cin >> this->database.database_name;
		std::cout << std::endl;
	}
	
	
} // End of DBNameRequest
//--------------------------------------------------------------------



// DriverRequest
///////////////////////////////////////////////////////////////////////
void InputAttribute::DriverRequest(bool &from_file)
{
	if (!from_file)
	{
		std::cout << "\n Enter database: ";
		std::cin >> this->database.database_name;
		std::cout << std::endl;
	}


} // End of DriverRequest
  //--------------------------------------------------------------------



// GetConfig
//////////////////////////////////////////////////////////////////////
void InputAttribute::GetConfig()
{
	std::cout << "\n Getting configuration info...";

	std::string cur_str, temp_str;

	std::ifstream config_file;

	config_file.open("config.ini", std::ifstream::in);

	if (!config_file.is_open())
		throw std::exception("\n ERROR: Cannot open the configuration file!\n");

	std::vector <std::string> tags;

	size_t cur_pos = 0;

	tags.push_back(SERVERNAME_TAG);
	tags.push_back(DATABASE_TAG);
	tags.push_back(DRIVER_TAG);
	tags.push_back(SHOWDIAGINFO_TAG);
	tags.push_back(OFFSET_TAG);

	while (getline(config_file, cur_str))
	{
		cur_pos = 0;

		if (cur_pos = cur_str.find(";", cur_pos) == std::string::npos )  // Ignore comments.	
		{			
			int i = 0, tag_length;

			while (i < tags.size())
			{
				cur_pos = 0;
				tag_length = tags[i].length();
				while ((cur_pos = cur_str.find(tags[i], cur_pos)) != std::string::npos)
				{
					if (tags[i] == SERVERNAME_TAG)
					{
						this->server.srv_name = cur_str.substr(tag_length + 1, (cur_str.length() - tag_length) + 1);

						if (this->server.srv_name != std::string(""))
						{
							this->server.from_file = true;
						}
						else						
							this->server.from_file = false;									
						
						break;
					}

					if (tags[i] == DATABASE_TAG)
					{
						this->database.database_name = cur_str.substr(tag_length + 1, (cur_str.length() - tag_length) + 1);


						if (this->database.database_name != std::string(""))
						{
							this->database.from_file = true;
						}
						else
							this->database.from_file = false;

						
						break;
					}

					if (tags[i] == DRIVER_TAG)
					{
						this->driver.driver_name = cur_str.substr(tag_length + 1, (cur_str.length() - tag_length) + 1);


						if (this->driver.driver_name != std::string(""))
						{
							this->driver.from_file = true;
						}
						else
							this->driver.from_file = false;


						break;
					}

					if (tags[i] == SHOWDIAGINFO_TAG)
					{
						std::string tag_value;

						tag_value = cur_str.substr(tag_length + 1, (cur_str.length() - tag_length) + 1);

						if (strcmp(tag_value.c_str(), "1") == 0)
							this->ShowDiagInfo = true;
						else
							this->ShowDiagInfo = false;

						break;
					}

					if (tags[i] == OFFSET_TAG)
					{
						std::string tag_value;

						tag_value = cur_str.substr(tag_length + 1, (cur_str.length() - tag_length) + 1);

						this->Offset = std::stoi(tag_value);

						break;
					}
					
					cur_pos++;					
				}				
				i++;
			}

		}
	}

	config_file.close();
		
} // End of GetConfig
//--------------------------------------------------------------------


// GetUser
//////////////////////////////////////////////////////////////////////
std::string InputAttribute::GetUser()
{
	return this->user_name;
} // End of GetUser 
//--------------------------------------------------------------------


// GetUserPass
//////////////////////////////////////////////////////////////////////
std::string InputAttribute::GetUserPass()
{
	return this->user_pass;
} // End of GetUserPass


// GetDB
//////////////////////////////////////////////////////////////////////
std::string InputAttribute::GetDB()
{
	return this->database.database_name;
} // End of GetDB
//----------------------------------------------------------------------



// GetServer
//////////////////////////////////////////////////////////////////////
std::string InputAttribute::GetServer()
{
	return this->server.srv_name;
} // End of GetServer
//----------------------------------------------------------------------



// GetDriver
//////////////////////////////////////////////////////////////////////
std::string InputAttribute::GetDriver()
{
	return this->driver.driver_name;
} // End of GetDriver
//----------------------------------------------------------------------


// BuildConnectionString
////////////////////////////////////////////////////////////////////////
void InputAttribute::BuildConnectionString()
{

	std::cout << "\n Building the connection string. \n";

	this->connection_string =
		"Driver={" + this->GetDriver()
		+ "}; Server=" + this->GetServer()
		+ ";Database=" + this->GetDB()
		+ ";Uid=" + this->GetUser()
		+ ";Pwd=" + this->GetUserPass()
		+ ";Asynchronous Processing=True;";

} // End of BuildConnectionString 
//----------------------------------------------------------------------



// GetConnectionString
////////////////////////////////////////////////////////////////////////
std::string InputAttribute::GetConnectionString()
{
	return this->connection_string;
    
} // End of GetConnectionString 
//----------------------------------------------------------------------



// LoadBriefRequest
///////////////////////////////////////////////////////////////////////
void InputAttribute::LoadBriefRequest()
{
	// Nailed to a brief of the load... Don't forget to rewrite
	int chosen_loadnum;
	std::cout << "\n Choose the load:";
	std::cout << "\n 1. Клиенты \n 2. Счета \n 3. Счета(массовая) \n 4. Документы(ВВС) \n 5. Документы(Кассовые) \n 6. Документы(Межбанковские) \n 7. Межвалютные \n" ;
	std::cout << " Enter: ";
	std::cin >> chosen_loadnum;

	switch (chosen_loadnum)
	{
	  case 1 :
		  this->load_brief = "Клиенты";		  
	      break;
	  case 2:
		  this->load_brief = "Счета";
		  break;
	  case 3:
		  this->load_brief = "СчетМасс";
		  break;
	  case 4:
		  this->load_brief = "ДокВВС";
		  break;
	  case 5:
		  this->load_brief = "ДокКас";
		  break;
	  case 6:
		  this->load_brief = "ДокМежБанк";
		  break;
	  case 7:
		  this->load_brief = "ДокМежВал";
		  break;
	  default:
		  std::cout << "\n Incorrect input! \n";
		  break;
	}

	//std::cin >> this->load_brief;

} // End of LoadBriefRequest
//--------------------------------------------------------------------



// GetLoadBrief
///////////////////////////////////////////////////////////////////////
std::string InputAttribute::GetLoadBrief()
{
	return this->load_brief;
} // End of GetLoadBrief
//----------------------------------------------------------------------



// ThreadsRequest()
////////////////////////////////////////////////////////////////////////
void InputAttribute::ThreadsRequest()
{
	std::cout << "\n The nubmer of threads: ";
	std::cin >> this->NumberOfThreads;

} // End of ThreadsRequest
//----------------------------------------------------------------------



// GetNumberOfThreads
////////////////////////////////////////////////////////////////////////
unsigned int InputAttribute::GetNumberOfThreads()
{
	return this->NumberOfThreads;
} // End of GetNumberOfThreads
//----------------------------------------------------------------------



// FileNameRequest
////////////////////////////////////////////////////////////////////////
void InputAttribute::FileNameRequest()
{
	std::cout << "\n Name of the file to load(with extenstion): ";
	std::cin >> this->filename;

} // End of FileNameRequest
//----------------------------------------------------------------------



// GetFileName
////////////////////////////////////////////////////////////////////////
std::string InputAttribute::GetFileName()
{
	return this->filename;
} // End of GetFileName
//----------------------------------------------------------------------



// DisplayAllAttributes
////////////////////////////////////////////////////////////////////////
void InputAttribute::DisplayAllAttributes()
{

	std::string ToDisplay;
	ToDisplay = "\n\n"
	            " Input data: "
	            "\n 1. Server   - " + this->GetServer() +
	            "\n 2. Database - " + this->GetDB() +
	            "\n 3. User     - " + this->GetUser() +
	            "\n 4. Driver   - " + this->GetDriver() +
	            "\n 5. File     - " + this->GetFileName() +
	            "\n 6. Threads  - " + std::to_string(this->GetNumberOfThreads()) +
	            "\n 7. Brief    - " + this->GetLoadBrief() +
	            "\n 8. BranchID - " + std::to_string(this->GetBranchID()) +
	            "\n\n";
	std::cout << ToDisplay;
	InputLogWriter.WriteIntoFile(ToDisplay);
	
 } // End if DisplayAllAttributes
//----------------------------------------------------------------------


// IsDiagInfoToShow
/////////////////////////////////////////////////////////////////////////
bool InputAttribute::IsDiagInfoToShow()
{
	return this->ShowDiagInfo;
} // End of IsDiagInfoToShow
//-----------------------------------------------------------------------



// BranchIDRequest
/////////////////////////////////////////////////////////////////////////
void InputAttribute::BranchIDRequest()
{
	std::cout << "\n BranchID: ";
	std::cin >> this->BranchID;
} // End of BranchIDRequst
//-----------------------------------------------------------------------



// GetBranchID
/////////////////////////////////////////////////////////////////////////
unsigned long InputAttribute::GetBranchID()
{
	return this->BranchID;
} // End of GetBranchID
//-----------------------------------------------------------------------



// GetOffset
/////////////////////////////////////////////////////////////////////////
unsigned int InputAttribute::GetOffset()
{
	return this->Offset;
} // End of GetOffset
//---------------------------------------------------------------------