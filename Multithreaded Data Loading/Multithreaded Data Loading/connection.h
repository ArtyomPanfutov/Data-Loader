#ifndef CONNECTION_H
#define CONNECTION_H
// connection.h
// created: 2018-07-21
// modified: 
// author: Artyom Panfutov

#include <windows.h>
#include "constant.h"
#include <sql.h>
#include <sqlext.h>
#include <odbcss.h>

/////////////////////////////////////////////////////////////////////////
class Connection
{
  private:
    // Data for connection string
    SQLCHAR 
		UID[SQL_USERNAME_MAX_LENGTH],
	    PWD[SQL_PASSWORD_MAX_LENGTH],
        Server[SQL_INSTANCE_MAX_LENGTH],
	    Database[SQL_DB_MAX_LENGTH],
	    Driver[SQL_DRIVER_MAX_LENGTH];

  protected:
	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt;

	SQLRETURN retcode;

	SQLCHAR OutConnStr[255];

	SQLSMALLINT OutConnStrLen;

	HWND desktopHandle = GetDesktopWindow();   // desktop's window handle  

  public:
	  Connection();
	  ~Connection();
	  int DriverConnectAndAllocHandle();
	  int TestConnection(const short int&);
	  void CleanUp();  
	  void ShowErrCode(SQLRETURN&);
	  //void SQLGetDiagMsg();
	  
};
////////////////////////////////////////////////////////////////////////



#endif // CONNECTION_H
