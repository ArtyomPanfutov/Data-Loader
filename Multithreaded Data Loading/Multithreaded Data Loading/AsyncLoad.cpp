// AsyncLoad.cpp
// created: 2018-08-12
// author: Artyom Panfutov

#include "AsyncLoad.h"
#include <vector>
#include "constant.h"
#include <string>
#include "Output.h"

//////////////////////////////////////////////////////////////////////

/***********************************************************************
*************** Definiton of methods. Class AsyncLoad ******************
***********************************************************************/

std::string AsyncLogFileName = "AsyncProcessingLog.txt";
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
void AsyncLoad::RunLoadsInAsyncMode(std::vector <FALoad *> &Loads, unsigned long i)
{
	AsyncLogMessage = "Function RunLoadsInAsyncMode. Nested level: " + std::to_string(i);
	AsyncLoadLogWriter.Push(AsyncLogMessage, INFO_MESSAGE, true);

	SQLRETURN sql_retcode = 0, mrRetcode;

	SQLINTEGER cbRetVal;

	SQLHSTMT CurSTMT;

	unsigned int RetVal = 0;
	unsigned long input_i;

	size_t CountOfLoads = Loads.size();

	AsyncLogMessage = "Number of loads: " + std::to_string(CountOfLoads);
	AsyncLoadLogWriter.Push(AsyncLogMessage, INFO_MESSAGE, true);

	input_i = i;
	
	try
	{
		if (input_i < CountOfLoads)
		{
			CurSTMT = Loads[input_i]->GetHSTMT();

			AsyncLogMessage = "Processing with SPID: " + std::to_string(Loads[input_i]->SPID);
			AsyncLoadLogWriter.Push(AsyncLogMessage, INFO_MESSAGE, true);

			// Set async mode on.
			sql_retcode = SQLSetStmtAttr(
				CurSTMT,
				SQL_ATTR_ASYNC_ENABLE, 
				reinterpret_cast<SQLPOINTER>(SQL_ASYNC_ENABLE_ON), 
				0);

			if (sql_retcode != SQL_SUCCESS && sql_retcode != SQL_SUCCESS_WITH_INFO)
				throw SQLException("\n ERROR: RunLoadsInAsyncMode failed! SQLSetStmtAttr(ASYNC_ENABLE ON)\n", sql_retcode);

			AsyncLogMessage = "Asynchronous mode enabled";
			AsyncLoadLogWriter.Push(AsyncLogMessage, INFO_MESSAGE, true);

			// Execute all loads in async mode.
			while ((sql_retcode = SQLExecDirect(CurSTMT, (unsigned char*)Loads[input_i]->LastFormulaStr.c_str(), SQL_NTS)) == SQL_STILL_EXECUTING)
			{
				if (i < CountOfLoads)
				{
					i++;

					AsyncLogMessage = "Waiting for " + std::to_string(this->OffsetTime) + " ms.";
					AsyncLoadLogWriter.Push(AsyncLogMessage, INFO_MESSAGE, true);

					// Wait for offset time...
					Sleep(this->OffsetTime);

					// And go.
					RunLoadsInAsyncMode(Loads, i);
				}

			}

			// Get the returned value from the executed formula.
			do
			{
				if (sql_retcode == SQL_SUCCESS)
				{
					while (TRUE)
					{
						sql_retcode = SQLFetch(CurSTMT);

						if (sql_retcode == SQL_ERROR )
							throw SQLException("\n ERROR: RunLoadsInAsyncMode failed! SQLFetch\n", sql_retcode);

						else if (sql_retcode == SQL_SUCCESS)
						{
							// RetVal
							sql_retcode = SQLGetData(
								CurSTMT,
								1,
								SQL_C_ULONG,
								&RetVal,
								10,
								&cbRetVal);

							if (sql_retcode != SQL_SUCCESS)
								throw SQLException("\n ERROR: RunLoadsInAsyncMode failed! SQLGetData(RetVal)\n", sql_retcode);
						}
						else
							break;
					}

				}
			} while (mrRetcode = SQLMoreResults == SQL_SUCCESS);

			AsyncLogMessage = " Postprocess formula has been executed. Return value: " + std::to_string(RetVal) + ". Processing SPID: " + std::to_string(Loads[input_i]->SPID);

			sql_retcode = SQLSetStmtAttr(CurSTMT, SQL_ATTR_ASYNC_ENABLE, reinterpret_cast<SQLPOINTER>(SQL_ASYNC_ENABLE_OFF), 0);
			if (sql_retcode != SQL_SUCCESS && sql_retcode != SQL_SUCCESS_WITH_INFO)
				throw SQLException("\n ERROR: RunLoadsInAsyncMode failed! SQLSetStmtAttr(ASYNC_ENABLE)\n", sql_retcode);

			sql_retcode = SQLFreeStmt(CurSTMT, SQL_CLOSE);
			if (sql_retcode != SQL_SUCCESS && sql_retcode != SQL_SUCCESS_WITH_INFO)
				throw SQLException("\n ERROR: ExecuteFormula failed! SQLFreeStmt failed!\n", sql_retcode);
	
		}
	}
	catch (SQLException &ex)
	{
		ex.ShowMessage(Loads[input_i]->GetHSTMT());
	}

} // End of RunLoadsInAsyncMode
//--------------------------------------------------------------------



// Message
//////////////////////////////////////////////////////////////////////
void AsyncLoad::Message(std::string &text)
{
	std::cout << std::endl << text;
}