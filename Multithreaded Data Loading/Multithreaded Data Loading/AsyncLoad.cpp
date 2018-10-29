//
// AsyncLoad.cpp
// created: 2018-08-12
// author: Artyom Panfutov
//

#include "AsyncLoad.h"
#include <vector>
#include "constant.h"
#include <string>
#include "Output.h"

//////////////////////////////////////////////////////////////////////

/***********************************************************************
*************** Definiton of methods. Class AsyncLoad ******************
***********************************************************************/

std::string AsyncLogFileName = "LoadLog.txt";
std::string AsyncLogMessage;
Log AsyncLoadLogWriter(AsyncLogFileName);

// AsyncLoad constructor
////////////////////////////////////////////////////////////////////////
AsyncLoad::AsyncLoad(unsigned int Time) : OffsetTime(Time)
{ } // End of constructor
//---------------------------------------------------------------------



// AsyncLoad destructor
AsyncLoad::~AsyncLoad()
{
	FALoads.clear();
} // End of destructor
//---------------------------------------------------------------------



// RunLoadsInAsyncMode
///////////////////////////////////////////////////////////////////////
void AsyncLoad::RunLoadsInAsyncMode(std::vector <FALoad *> &Loads, unsigned long Level)
{
	SQLRETURN SqlReturnCode = 0, mrRetcode;

	SQLINTEGER cbRetVal;

	SQLHSTMT CurSTMT;

	unsigned int RetVal = 0;
	unsigned long InputLevel;

	size_t CountOfLoads = Loads.size();
	InputLevel = Level;
	try
	{
		if (InputLevel < CountOfLoads) 
		{
			AsyncLogMessage = "Function RunLoadsInAsyncMode. Nested level: " + std::to_string(Level);
			AsyncLoadLogWriter.Push(AsyncLogMessage, INFO_MESSAGE, true);

			AsyncLogMessage = "Number of loads: " + std::to_string(CountOfLoads);
			AsyncLoadLogWriter.Push(AsyncLogMessage, INFO_MESSAGE, true);

			CurSTMT = Loads[InputLevel]->GetHSTMT();

			AsyncLogMessage = "Processing with SPID: " + std::to_string(Loads[InputLevel]->SPID);
			AsyncLoadLogWriter.Push(AsyncLogMessage, INFO_MESSAGE, true);

			// Set async mode on.
			SqlReturnCode = SQLSetStmtAttr(
				CurSTMT,
				SQL_ATTR_ASYNC_ENABLE, 
				reinterpret_cast<SQLPOINTER>(SQL_ASYNC_ENABLE_ON), 
				0);

			if (SqlReturnCode != SQL_SUCCESS && SqlReturnCode != SQL_SUCCESS_WITH_INFO)
				throw SQLException("\n ERROR: RunLoadsInAsyncMode failed! SQLSetStmtAttr(ASYNC_ENABLE ON)\n", SqlReturnCode);

			AsyncLogMessage = "Asynchronous mode enabled";
			AsyncLoadLogWriter.Push(AsyncLogMessage, INFO_MESSAGE, true);

			SQLRETURN returnval;

            
			Level++;
			if (Level < CountOfLoads)
			{
				AsyncLogMessage = "Waiting for " + std::to_string(this->OffsetTime) + " ms.";
				AsyncLoadLogWriter.Push(AsyncLogMessage, INFO_MESSAGE, true);

				// Start next load
				RunLoadsInAsyncMode(Loads, Level);
			}
	
			// Wait for finish
			while (true)
			{
				SqlReturnCode = this->ExecuteSQL(CurSTMT, (unsigned char*)Loads[InputLevel]->LastFormulaStr.c_str(), SQL_NTS, Loads[InputLevel]);

				if (SqlReturnCode != SQL_STILL_EXECUTING)
				{
					Loads[InputLevel]->IsExecuted = true;
					break;
				}
			}


			// Get the returned value from the executed formula.
			do
			{
				if (SqlReturnCode == SQL_SUCCESS)
				{
					while (TRUE)
					{
						SqlReturnCode = SQLFetch(CurSTMT);

						if (SqlReturnCode == SQL_ERROR )
							throw SQLException("\n ERROR: RunLoadsInAsyncMode failed! SQLFetch\n", SqlReturnCode);

						else if (SqlReturnCode == SQL_SUCCESS)
						{
							// RetVal
							SqlReturnCode = SQLGetData(
								CurSTMT,
								1,
								SQL_C_ULONG,
								&RetVal,
								10,
								&cbRetVal);

							if (SqlReturnCode != SQL_SUCCESS)
								throw SQLException("\n ERROR: RunLoadsInAsyncMode failed! SQLGetData(RetVal)\n", SqlReturnCode);
						}
						else
							break;
					}

				}
			} while (mrRetcode = SQLMoreResults == SQL_SUCCESS);

			AsyncLogMessage = "Postprocess formula has been executed. Return value: " + std::to_string(RetVal) + ". Processing SPID: " + std::to_string(Loads[InputLevel]->SPID);
			AsyncLoadLogWriter.Push(AsyncLogMessage, INFO_MESSAGE, true);

			SqlReturnCode = SQLSetStmtAttr(CurSTMT, SQL_ATTR_ASYNC_ENABLE, reinterpret_cast<SQLPOINTER>(SQL_ASYNC_ENABLE_OFF), 0);
			if (SqlReturnCode != SQL_SUCCESS && SqlReturnCode != SQL_SUCCESS_WITH_INFO)
				throw SQLException("\n ERROR: SqlReturnCode failed! SQLSetStmtAttr(ASYNC_ENABLE)\n", SqlReturnCode);

			SqlReturnCode = SQLFreeStmt(CurSTMT, SQL_CLOSE);
			if (SqlReturnCode != SQL_SUCCESS && SqlReturnCode != SQL_SUCCESS_WITH_INFO)
				throw SQLException("\n ERROR: ExecuteFormula failed! SQLFreeStmt failed!\n", SqlReturnCode);
	
		}
	}
	catch (SQLException &ex)
	{
		ex.ShowMessage(Loads[InputLevel]->GetHSTMT());
	}


} // End of RunLoadsInAsyncMode
//--------------------------------------------------------------------

// ExecuteSQL
///////////////////////////////////////////////////////////////////////
SQLRETURN AsyncLoad::ExecuteSQL(SQLHSTMT CurSTMT, SQLCHAR* Statement, SQLINTEGER TextLength, FALoad *Load)
{
	//
	// Wrapper for SQLExecDirect. 
	// Returns 0 or value from SQLExecDirect
	//

	SQLRETURN RetVal;

	if (!Load->IsExecuted)
		RetVal = SQLExecDirect(CurSTMT, (unsigned char*)Load->LastFormulaStr.c_str(), SQL_NTS);
	else
		RetVal = 0;
	return RetVal;
} // End of ExecuteSQL
//--------------------------------------------------------------------


// Message
//////////////////////////////////////////////////////////////////////
void AsyncLoad::Message(std::string &text)
{
	std::cout << std::endl << text;
}