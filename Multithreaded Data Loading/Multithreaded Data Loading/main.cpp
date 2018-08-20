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
#include <vector>
#include "InputAttribute.h"

int main()
{

	setlocale(LC_ALL, "Russian");

	InputAttribute UserInfo;
	AsyncLoad MainObj;

	FALoad *CurrentThread;

	std::string
		filename = UserInfo.GetFileName(),
		inputpath,
		savepath,
		LoadBrief = UserInfo.GetLoadBrief(),
		ToDisplay,
		rowterm = "\r\n",
		fieldterm;


	file DataFile(filename, inputpath, savepath);

	const char *chLoadBrief = LoadBrief.c_str();

	std::cout << chLoadBrief;


	system("pause");

	unsigned int CountOfStrings, Threads;
	unsigned long Batch, FirstRow, LastRow, RowsDone;

	Threads = UserInfo.GetNumberOfThreads();

	DataFile.PutStrIntoVector();

	CountOfStrings = DataFile.GetCountOfStrings();
	
	ToDisplay = " Strings to process: " + std::to_string(CountOfStrings);
	MainObj.Message(ToDisplay);

	if ((CountOfStrings % Threads) != 0)
		Batch = (CountOfStrings / Threads) + .5;
	else
		Batch = CountOfStrings / Threads;

	ToDisplay = " Creating objects for loads... Batch: " + std::to_string(Batch);
	MainObj.Message(ToDisplay);


	// Create objects and prepare sql formula for each object.
	for (int i = 0; i < Threads; i++)
	{
		std::string ConnectionString = UserInfo.GetConnectionString();

		MainObj.FALoads.push_back(new FALoad(filename));

		CurrentThread = MainObj.FALoads.back();

		ToDisplay = " The object " + chLoadBrief + (const char)". Number: " + std::to_string(i) + " created!";
		MainObj.Message(ToDisplay);

		// Don't forget to rewrote this part (too many identical queries to server)
		CurrentThread->DriverConnectAndAllocHandle(ConnectionString);
		CurrentThread->SetLoadBrief(chLoadBrief);
		CurrentThread->GetDSTypesFromDB();
		CurrentThread->GetLoadInfo();
		CurrentThread->GetSetupParamsFromDB();
		CurrentThread->GetLastParamsFromDB();
		CurrentThread->GetSPID();

		ToDisplay = " Connected on SPID " + std::to_string(CurrentThread->SPID);
		MainObj.Message(ToDisplay);

		CurrentThread->PrepareFormula(CurrentThread->LastFormulaStr, CurrentThread->LastFormulaParams);
		CurrentThread->PrepareFormula(CurrentThread->SetupFormulaStr, CurrentThread->SetupFormulaParams);

		ToDisplay = " Preparing of the object " + chLoadBrief + (const char)". Number: " + std::to_string(i) + " is completed!";
		MainObj.Message(ToDisplay);
        
	}
		
    fieldterm = ((const char *)MainObj.FALoads.front()->Delimiter, strlen(reinterpret_cast<char *>(MainObj.FALoads.front()->Delimiter)));

	FirstRow = 0;
	LastRow = Batch;
	
	// Execute the setup formula and start BCP.
	for (int i = 0; i < Threads; i++)
	{
		CurrentThread = MainObj.FALoads[i];
		CurrentThread->ExecuteFormula(CurrentThread->SetupFormulaStr);

		ToDisplay = " Start BCP push for the object " + chLoadBrief + (const char)". Number: " + std::to_string(i);
		MainObj.Message(ToDisplay);
		
		RowsDone = CurrentThread->StartBCP(fieldterm, rowterm, FirstRow, LastRow, DataFile.text_str);

		ToDisplay = " BCP load is completed!";
		MainObj.Message(ToDisplay);
	}

	// Start SQL postprocessing.
	MainObj.RunLoadsInAsyncMode(MainObj.FALoads, 0);


/*	FALoad a(filename), b(filename), c(filename);

	a.DriverConnectAndAllocHandle();
	b.DriverConnectAndAllocHandle();
	c.DriverConnectAndAllocHandle();
	//a.TestConnection(1);
	//char *aptr;
	SQLCHAR a1[] = { "Клиенты" };
	
	a.SetLoadBrief(a1[0]);
	b.SetLoadBrief(a1[0]);
	c.SetLoadBrief(a1[0]);

	a.GetDSTypesFromDB();
	b.GetDSTypesFromDB();
	c.GetDSTypesFromDB();

	a.GetLoadInfo();
	b.GetLoadInfo();
	c.GetLoadInfo();

	a.GetSetupParamsFromDB();
	a.GetLastParamsFromDB();

	b.GetSetupParamsFromDB();
	b.GetLastParamsFromDB();

	c.GetSetupParamsFromDB();
	c.GetLastParamsFromDB();

	
	
	//a.ShowLoadBrief();
	

	std::string delim((const char *)a.Delimiter);


	
	std::string fieldterm((const char *)a.Delimiter, strlen(reinterpret_cast<char *>(a.Delimiter)));
	

	
	int offset = 3;



//	t.preparefile(a.TableColumns, fieldterm, rowterm, offset);
	std::vector <FALoad *> Fa;


	
	Fa.push_back(&a);

	a.GetSPID();
	b.GetSPID();
	c.GetSPID();

	t.PutStrIntoVector();

	unsigned long int afirstrow = 0, bfirstrow = 1, cfirstrow = 2;
	unsigned long int lastrow = 2;
	
	
	a.PrepareFormula(a.LastFormulaStr, a.LastFormulaParams);

	a.PrepareFormula(a.SetupFormulaStr, a.SetupFormulaParams);

	a.ExecuteFormula(a.SetupFormulaStr);


	b.PrepareFormula(b.LastFormulaStr, b.LastFormulaParams);

	b.PrepareFormula(b.SetupFormulaStr, b.SetupFormulaParams);

	b.ExecuteFormula(b.SetupFormulaStr);


	c.PrepareFormula(c.LastFormulaStr, c.LastFormulaParams);

	c.PrepareFormula(c.SetupFormulaStr, c.SetupFormulaParams);

	c.ExecuteFormula(c.SetupFormulaStr);



	a.StartBCP(fieldterm, rowterm, afirstrow, afirstrow, t.text_str);
	b.StartBCP(fieldterm, rowterm, bfirstrow, bfirstrow, t.text_str);
	c.StartBCP(fieldterm, rowterm, cfirstrow, cfirstrow, t.text_str);


	AsyncLoad async;

	async.FALoads.push_back(new FALoad(filename));
	async.FALoads.push_back(&b);
	async.FALoads.push_back(&c);
		
	async.RunLoadsInAsyncMode(async.FALoads, afirstrow);

	

	//a.ExecuteFormula(a.LastFormulaStr);

	*/


	
	system("pause");

	return 0;
}