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

	/*Connection a;


	a.DriverConnectAndAllocHandle();

	a.TestConnection(1);*/
	std::string filename = "institution.txt";
	std::string inputpath = "d:";
	std::string savepath = "d:";
	file t(filename, inputpath, savepath);

	InputAttribute UserInfo;

	//UserInfo.UserNameRequest();
	//UserInfo.UserPassRequest();
	UserInfo.GetConfig();


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
	std::string rowterm("\r\n");

	
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
