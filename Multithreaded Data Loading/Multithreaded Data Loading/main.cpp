// main.cpp 
// created: 2018-07-21
// modified:
// author: Artyom Panfutov

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

int main()
{	
	try
	{
		setlocale(LC_ALL, "RUS");
		SetConsoleCP(866);
		// SetConsoleOutputCP(866);

		std::cout <<
			"\n\n***********************************************************************\n"
			"\n                       Fiora (alpha version)                             "
			"\n                           Data Loader                                   "
			"\n\n***********************************************************************\n";

		InputAttribute UserInfo;
		AsyncLoad MainObj(UserInfo.GetOffset());

		FALoad *CurrentThread;

		std::string
			filename = UserInfo.GetFileName(),
			inputpath,
			savepath,
			ToDisplay,
			rowterm = "\r\n",
			fieldterm;

		std::string LoadBrief;
		LoadBrief = UserInfo.GetLoadBrief();

		file DataFile(filename, inputpath, savepath);

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

		ToDisplay = " Strings to process: " + std::to_string(CountOfStrings);
		MainObj.Message(ToDisplay);

		Batch = (CountOfStrings / Threads) + (CountOfStrings % Threads);

		ToDisplay = " Creating objects for loads... Batch: " + std::to_string(Batch) + "\n";
		MainObj.Message(ToDisplay);


		// Create objects and prepare sql formula for each object.
		for (int i = 0; i < Threads; i++)
		{
			std::string ConnectionString = UserInfo.GetConnectionString();

			MainObj.FALoads.push_back(new FALoad(filename,
				UserInfo.IsDiagInfoToShow(),
				UserInfo.GetBranchID()));


			CurrentThread = MainObj.FALoads.back();


			ToDisplay =
				"\n***********************************************************************\n";
			" The object " + LoadBrief + std::string(" Number: ") + std::to_string(i) + " created! \n\n";
			MainObj.Message(ToDisplay);

			// Don't forget to rewrite this part (too many identical queries to server)
			CurrentThread->DriverConnectAndAllocHandle(ConnectionString);
			CurrentThread->GetSPID();

			ToDisplay = " Connection SPID " + std::to_string(CurrentThread->SPID);
			MainObj.Message(ToDisplay);

			CurrentThread->SetLoadBrief(LoadBrief);

			ToDisplay = " \n Getting Diasoft data types... ";
			MainObj.Message(ToDisplay);

			CurrentThread->GetDSTypesFromDB();

			ToDisplay = " \n Getting attributes of load... ";
			MainObj.Message(ToDisplay);

			CurrentThread->GetLoadInfo();
			CurrentThread->GetSetupParamsFromDB();
			CurrentThread->GetLastParamsFromDB();

			ToDisplay = " \n Preparing formula... ";
			MainObj.Message(ToDisplay);

			CurrentThread->PrepareFormula(CurrentThread->LastFormulaStr, CurrentThread->LastFormulaParams);
			CurrentThread->PrepareFormula(CurrentThread->SetupFormulaStr, CurrentThread->SetupFormulaParams);

			ToDisplay = " Preparing of the object " + LoadBrief + std::string(" Number: ") + std::to_string(i) + " is completed!";
			MainObj.Message(ToDisplay);

			ToDisplay = "\n\n***********************************************************************";
			MainObj.Message(ToDisplay);
		}

		fieldterm = std::string(reinterpret_cast<const char *> (MainObj.FALoads.front()->Delimiter));

		FirstRow = 0;
		LastRow = Batch - 1;

		// Execute the setup formula and start BCP.
		for (int i = 0; i < Threads; i++)
		{
			CurrentThread = MainObj.FALoads[i];
			CurrentThread->ExecuteFormula(CurrentThread->SetupFormulaStr);

			ToDisplay = "\n Start BCP push for the object " + LoadBrief + std::string(" Number: ") + std::to_string(i);
			MainObj.Message(ToDisplay);

			RowsDone = CurrentThread->StartBCP(
				fieldterm,
				rowterm,
				FirstRow,
				LastRow,
				DataFile.text_str);

			ToDisplay = "\n BCP load is completed!";
			MainObj.Message(ToDisplay);

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
		std::cout << "\n" << ex.what();
	}
	system("pause");

	return 0;
}