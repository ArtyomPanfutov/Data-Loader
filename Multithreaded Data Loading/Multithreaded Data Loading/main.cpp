//
// main.cpp 
// created: 2018-07-21
// modified: 2018-09-24
// author: Artyom Panfutov
//

#include <iostream>
#include <cstdlib>
#include "connection.h"
#include "SQLException.h"
#include "constant.h"
#include "FALoad.h"
#include "file.h"
#include "AsyncLoad.h"
#include <sql.h>
#include <sqlext.h>
#include <odbcss.h>
#include <locale>
#include <vector>
#include "InputAttribute.h"
#include <Windows.h>
#include "Output.h"


int main()
{
	setlocale(LC_ALL, "RUS");
	SetConsoleCP(866);

	std::string LogFileName = "LoadLog.txt";

	Log LogWriter(LogFileName);
	LogWriter.ClearFile();

	std::cout <<
		"\n\n *******************************************************************************\n"
		"\n                              Asynchronous Data Loader                                   "
		"\n\n *******************************************************************************\n";

	try
	{		
		InputAttribute UserInfo;
		AsyncLoad MainObj(UserInfo.GetOffset());

		FALoad *CurrentThread, *FirstThread;
		  
		std::string
			FileName = UserInfo.GetFileName(),
			InputPath,
			OutputPath,
			ToDisplay,
			FieldTerminator,
		    RowTerminator = "\r\n",
			LoadBrief;

		LoadBrief = UserInfo.GetLoadBrief();

		File DataFile(FileName, InputPath, OutputPath);

		unsigned int
			CountOfStrings,
			Threads;
		unsigned long
			Batch,
			FirstRow,
			LastRow,
			RowsDone;

		////////////////////////////////////////////////////////////////////////

		Threads = UserInfo.GetNumberOfThreads();

		DataFile.PutStrIntoVector();

		CountOfStrings = DataFile.GetCountOfStrings();

		ToDisplay = "Lines to process: " + std::to_string(CountOfStrings);
		LogWriter.Push(ToDisplay, INFO_MESSAGE, true);

		Batch = (CountOfStrings / Threads) + (CountOfStrings % Threads);

		ToDisplay = "Creating objects for loads... Batch: " + std::to_string(Batch) + "\n";
		LogWriter.Push(ToDisplay, INFO_MESSAGE, true);

		// Create objects and prepare sql formula for each object.
		for (int i = 0; i < Threads; i++)
		{
			std::string ConnectionString = UserInfo.GetConnectionString();

			MainObj.FALoads.push_back(new FALoad(FileName,
				UserInfo.IsDiagInfoToShow(),
				UserInfo.GetBranchID()));


			CurrentThread = MainObj.FALoads.back();
			FirstThread = MainObj.FALoads.front();

			ToDisplay =
				"\n *******************************************************************************\n";
			" The object " + LoadBrief + std::string(" Number: ") + std::to_string(i) + " created! \n\n";
			LogWriter.Push(ToDisplay, INFO_MESSAGE, true);

			// TODO: rewrite this part (too many identical queries to server)
			CurrentThread->DriverConnectAndAllocHandle(ConnectionString);
			CurrentThread->GetSPID();

			ToDisplay = "Connection SPID: " + std::to_string(CurrentThread->SPID);
			LogWriter.Push(ToDisplay, INFO_MESSAGE, true);

			CurrentThread->SetLoadBrief(LoadBrief);

			ToDisplay = "Getting Diasoft data types... ";
			LogWriter.Push(ToDisplay, INFO_MESSAGE, true);

			CurrentThread->GetDSTypesFromDB();

			ToDisplay = "Getting attributes of load... ";
			LogWriter.Push(ToDisplay, INFO_MESSAGE, true);

			CurrentThread->GetLoadInfo();
			CurrentThread->GetSetupParamsFromDB();
			CurrentThread->GetLastParamsFromDB();

			ToDisplay = "Preparing formula... ";
			LogWriter.Push(ToDisplay, INFO_MESSAGE, true);

			// Uses params from the front thread, because they are identical to all threads 
			CurrentThread->PrepareFormula(CurrentThread->LastFormulaStr, FirstThread->LastFormulaParams);
			CurrentThread->PrepareFormula(CurrentThread->SetupFormulaStr, FirstThread->SetupFormulaParams);

			ToDisplay = "Preparing of the object " + LoadBrief + std::string(" Number: ") + std::to_string(i) + " is completed!";
			LogWriter.Push(ToDisplay, INFO_MESSAGE, true);
		}

		FieldTerminator = std::string(reinterpret_cast<const char *> (MainObj.FALoads.front()->Delimiter));

		FirstRow = 0;
		LastRow = Batch - 1;

		ToDisplay = " *******************************************************************************";
		MainObj.Message(ToDisplay);

		ToDisplay = " *******************************************************************************";
		MainObj.Message(ToDisplay);

		ToDisplay = " *******************************************************************************";
		MainObj.Message(ToDisplay);

		// Execute the setup formula and start BCP.
		for (int i = 0; i < Threads; i++)
		{
			CurrentThread = MainObj.FALoads[i];
			CurrentThread->ExecuteFormula(CurrentThread->SetupFormulaStr);

			ToDisplay = "Start BCP push for object " + LoadBrief + std::string(" Number: ") + std::to_string(i) + "\n Lines [" + std::to_string(FirstRow) + ".." + std::to_string(LastRow) + "]";
			LogWriter.Push(ToDisplay, INFO_MESSAGE, true);

			RowsDone = CurrentThread->StartBCP(
				FieldTerminator,
				RowTerminator,
				FirstRow,
				LastRow,
				DataFile.LinesFromFile);

			ToDisplay = "BCP load for object " + LoadBrief + std::string(" Number: ") + std::to_string(i) + " is completed! \n"
				        "\n *******************************************************************************\n";

			LogWriter.Push(ToDisplay, INFO_MESSAGE, true);

			FirstRow = LastRow + 1;
			LastRow += Batch;

			if (FirstRow >= CountOfStrings)
				FirstRow = CountOfStrings - 1;

			if (LastRow >= CountOfStrings)
				LastRow = CountOfStrings - 1;
		}

		// Start SQL postprocessing.
		MainObj.RunLoadsInAsyncMode(MainObj.FALoads, 0);
	}
	catch (std::exception &ex)
	{
		std::string ErrorMessage(ex.what());
		LogWriter.Push(ErrorMessage, ERROR_MESSAGE, true);
	}
	system("pause");

	return 0;
}