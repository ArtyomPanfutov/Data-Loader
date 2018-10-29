// Output.cpp
// created: 2018-09-20
//

#include "Output.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include "constant.h"

////////////////////////////////////////////////////////////////////////

/***********************************************************************
*************** Definiton of methods. Class Log *********************
***********************************************************************/


// Log
////////////////////////////////////////////////////////////////////////
Log::Log(std::string &FileName)
{
	this->OutputFileName = FileName;

	//OutputFile = std::ofstream(OutputFileName);
	OutputFile.open(OutputFileName, std::ios::app);
	if (!OutputFile.is_open())
		throw std::exception("Log file is not open!");
} // End of Log
//----------------------------------------------------------------------




// ~Log
////////////////////////////////////////////////////////////////////////
Log::~Log()
{
	CloseFile();
} // End of ~Log
//---------------------------------------------------------------------


// CloseFile
////////////////////////////////////////////////////////////////////////
void Log::CloseFile()
{
	OutputFile.close();
} // End of CloseFile
//----------------------------------------------------------------------


// DisplayOnConsole
////////////////////////////////////////////////////////////////////////
void Log::DisplayOnConsole(std::string &Message)
{
	std::cout << std::endl << Message;
} // End of DisplayOnConsole
//----------------------------------------------------------------------


// ClearFile 
////////////////////////////////////////////////////////////////////////
void Log::ClearFile()
{
	if (!OutputFile.is_open())
	{
		OutputFile.open(OutputFileName, std::ofstream::out | std::ofstream::trunc);
	}
	else
	{
		OutputFile.close();
		OutputFile.open(OutputFileName, std::ofstream::out | std::ofstream::trunc);
	}

	OutputFile.close();
	OutputFile.open(OutputFileName, std::ios::app);

} // End of ClearFile 
//----------------------------------------------------------------------



// WriteIntoFile
////////////////////////////////////////////////////////////////////////
void Log::WriteIntoFile(std::string &Message)
{
	OutputFile << std::endl << Message;
	OutputFile.flush();
} // End of WriteIntoFile
//----------------------------------------------------------------------


// Push
////////////////////////////////////////////////////////////////////////
void Log::Push(std::string &Message, std::string Type, bool IntoFile)
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	std::string MessageToPush;

	tstruct = *localtime(&now);

	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	if ((Type != ERROR_MESSAGE) && (Type != INFO_MESSAGE) && (Type != WARNING_MESSAGE))
	{
		throw std::exception("Unknown message type!");
	}
	
	MessageToPush = " [" + Type + "]" + " [" + buf + "] " + Message;

	DisplayOnConsole(MessageToPush);

	if (IntoFile)
	{
		WriteIntoFile(MessageToPush);
	}
} // End of Push
//----------------------------------------------------------------------