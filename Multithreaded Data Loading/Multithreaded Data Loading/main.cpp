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
#include <sql.h>
#include <sqlext.h>
#include <odbcss.h>

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
	
	//a.ShowLoadBrief();

    	

	system("pause");

	return 0;
}
