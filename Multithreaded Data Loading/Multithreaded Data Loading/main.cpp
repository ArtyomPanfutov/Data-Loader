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

int main()
{

	setlocale(LC_ALL, "Russian");

	/*Connection a;


	a.DriverConnectAndAllocHandle();

	a.TestConnection(1);*/

	FALoad a;

	a.DriverConnectAndAllocHandle();

	//a.TestConnection(1);
	//char *aptr;
	SQLCHAR a1[] = { "Клиенты" };
	
	a.SetLoadBrief(a1[0]);

	a.GetDSTypesFromDB();

	a.GetLoadInfo();

	a.GetSetupParamsFromDB();
	a.GetLastParamsFromDB();
	
	a.GetSPID();

	//a.ShowLoadBrief();
	std::string filename = "institution.txt";
	std::string inputpath = "d:";
	std::string savepath = "d:";
	file t(filename, inputpath, savepath);

	std::string delim((const char *)a.Delimiter);


	
	std::string fieldterm((const char *)a.Delimiter, strlen(reinterpret_cast<char *>(a.Delimiter)));
	std::string rowterm("\r\n");

	
	int offset = 3;

//	t.preparefile(a.TableColumns, fieldterm, rowterm, offset);
	std::vector <FALoad *> Fa;



	Fa.push_back(&a);

	t.preparefile(Fa, fieldterm, rowterm);

	a.StartBCP(t.outputfile, fieldterm, rowterm);

	a.GetSPID();
	system("pause");

	return 0;
}
