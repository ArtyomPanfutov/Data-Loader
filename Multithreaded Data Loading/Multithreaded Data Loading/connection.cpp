// connection.cpp
// created: 2018-07-21
// modified:
// author: Artyom Panfutov

#include "connection.h"
#include "SQLException.h"
#include "constant.h"
#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <odbcss.h>



/***********************************************************************
************* Definiton of methods. Class Connection *******************
***********************************************************************/


// Connection constructor
///////////////////////////////////////////////////////////////////////
Connection::Connection()
{
	try
	{
		// Allocate enviroment handle 
		retcode = SQLAllocHandle( SQL_HANDLE_ENV, 
			                      SQL_NULL_HANDLE,
			                      &henv );

		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		  throw SQLException("Connection failed! SQLAllocHandle(henv)", retcode);


		// Set the ODBC version environment attribute  
		retcode = SQLSetEnvAttr( henv,
			                     SQL_ATTR_ODBC_VERSION,
			                     (SQLPOINTER*)SQL_OV_ODBC3,
			                     0 );

		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
   		  throw SQLException("Connection failed! SQLSetEnvAttr(henv)", retcode);

		// Allocate connection handle  
		retcode = SQLAllocHandle( SQL_HANDLE_DBC,
			                      henv,
			                      &hdbc );

		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
          throw SQLException("Connection failed! SQLAllocHandle(henv)", retcode);

	}
	catch (SQLException &ex)
	{
	  std::cout << "Exception!\n";
      ex.ShowMessage(henv);
	}

} // End of constructor
//---------------------------------------------------------------------


// Connection destructor
///////////////////////////////////////////////////////////////////////
Connection::~Connection()
{
	CleanUp();
} // End of destructor
//--------------------------------------------------------------------


// Method for cleaning
///////////////////////////////////////////////////////////////////////

void Connection::CleanUp()
{
	// Free handles and disconnect.

	if (hstmt)
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	
	if (hdbc)
	{
		SQLDisconnect(hdbc);
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	}

	if (henv)
		SQLFreeHandle(SQL_HANDLE_ENV, henv);


} // End of CleanUp()
//-------------------------------------------------------------------


// DriverConnectAndAllocHandle() 
//////////////////////////////////////////////////////////////////////

int Connection::DriverConnectAndAllocHandle(std::string &ConnectionString)
{
	SQLCHAR *ConnStr = (SQLCHAR *)ConnectionString.c_str();

	try
	{ 
	  retcode = SQLSetConnectAttr(hdbc, SQL_COPT_SS_BCP, (void *)SQL_BCP_ON, SQL_IS_INTEGER);
	  if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
			throw SQLException("DriverConnectAndAllocHandle() failed! SQLSetConnectAttr", retcode);

	  // Set login timeout to 5 seconds  
	  SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

      retcode = SQLDriverConnect( hdbc,
                                  desktopHandle,
                                  ConnStr,
                                  ConnectionString.length(),
                                  OutConnStr,
                                  255,
                                  &OutConnStrLen,
                                  SQL_DRIVER_COMPLETE );

	  if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
  		throw SQLException("Connection failed! SQLDriverConnect", retcode);


	  // Allocate statement handle  
	  retcode = SQLAllocHandle( SQL_HANDLE_STMT, 
		                        hdbc,
		                        &hstmt );

	  if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	  	throw SQLException("Connection failed! SQLAllocHandle(hdbc)", retcode);


	}
	catch (SQLException &ex)
	{
	  CleanUp();
      ex.ShowMessage(hstmt);
    }
    
	return retcode;

} // End of DriverConnectAndAllocHandle()
//--------------------------------------------------------------------


// ShowErrCode() 
//////////////////////////////////////////////////////////////////////
void Connection::ShowErrCode(SQLRETURN& retval)
{
	std::cout << "\nRetcode: " << retval << std::endl;

} // End of ShowErrCode()
//--------------------------------------------------------------------


// TestConnection()
//////////////////////////////////////////////////////////////////////
int Connection::TestConnection(const short int& display)
{
	SQLCHAR ShowDiagInfoQuery[] = "set rowcount 1\
                                   select @@spid,\
                                          CURRENT_USER,\
                                          @@SERVERNAME,\
                                          DB_NAME()\
                                          set rowcount 0";
   int CurrentSPID;

   SQLINTEGER cbCurrentSPID, 
		      cbCurrentUser, 
		      cbCurrentServer,
		      cbCurrentDB;

   SQLCHAR CurrentUser[SQL_USERNAME_MAX_LENGTH],
     	   CurrentServer[SQL_INSTANCE_MAX_LENGTH],
		   CurrentDB[SQL_DB_MAX_LENGTH];

	try
	{

		retcode = SQLExecDirect(hstmt,
			                    ShowDiagInfoQuery,
			                    SQL_NTS);

		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
			throw SQLException("Test connection failed! SQLExecDirect(ShowDiagInfoQuery)", retcode);
		}

    	if (retcode == SQL_SUCCESS)
	    {
		while (TRUE)
		{
			retcode = SQLFetch(hstmt);
			if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
			{
				throw SQLException("Test connection failed! SQLFetch", retcode);
			}

			else if (retcode == SQL_SUCCESS) 
			{
				SQLGetData(hstmt, 1, SQL_C_ULONG, &CurrentSPID, 0, &cbCurrentSPID);
				SQLGetData(hstmt, 2, SQL_C_CHAR, &CurrentUser, SQL_USERNAME_MAX_LENGTH, &cbCurrentUser);
				SQLGetData(hstmt, 3, SQL_C_CHAR, &CurrentServer, SQL_INSTANCE_MAX_LENGTH, &cbCurrentServer);
				SQLGetData(hstmt, 4, SQL_C_CHAR, &CurrentDB, SQL_DB_MAX_LENGTH, &cbCurrentDB);

				std::cout << std::endl << std::endl;
				std::cout << "**************************************************";
				std::cout << std::endl;
				std::cout << "************ Connection successfull! *************";
				std::cout << std::endl;
				std::cout << "**************************************************";
				std::cout << std::endl << std::endl;

				if (display > 0)
				{
				  std::cout << "Username: " << CurrentUser << "\nSPID: " << CurrentSPID << std::endl;
				  std::cout << "Server: " << CurrentServer << "\nDatabase: " << CurrentDB << std::endl << std::endl << std::endl;
				}
			}
			else
				break;

		}

	}
	else
			throw SQLException("Test connection failed!", retcode);

	retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
			throw SQLException("Test connection. SQLFreeStmt failed!", retcode);

}
	catch (SQLException &ex)
	{
      ex.ShowMessage(hstmt);	
	}

	

	return retcode;
} // End of TestConnection()
//--------------------------------------------------------------------

// GetSPID
//////////////////////////////////////////////////////////////////////
void Connection::GetSPID()
{
	SQLCHAR 
		SQLGetSPID[] = "select @@spid";

	SQLINTEGER
		cbSPID;

	try
	{
		retcode = SQLExecDirect(
			hstmt,
			SQLGetSPID,
			SQL_NTS);

		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
			throw SQLException("GetSPID failed! SQLExecDirect(SQLGetSPID)", retcode);


		if (retcode == SQL_SUCCESS)
		{
			while (TRUE)
			{
				retcode = SQLFetch(hstmt);
				if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
				{
					throw SQLException("GetSPID failed! SQLFetch", retcode);
				}

				else if (retcode == SQL_SUCCESS)
				{
					// spid									
					retcode = SQLGetData(
						hstmt,
						1,
						SQL_C_ULONG,
						&SPID,
						0,
						&cbSPID);

					if (retcode != SQL_SUCCESS)
						throw SQLException("GetSPID failed! SQLGetData(spid)", retcode);
				}
				else
					break;
			}
		}

		retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
			throw SQLException("GetSPID failed! SQLFreeStmt failed!", retcode);

	}
	catch (SQLException &ex)
	{
		ex.ShowMessage(hstmt);
	}
} // End of GetSPID
//-------------------------------------------------------------------



// GetHSTMT
SQLHSTMT Connection::GetHSTMT()
{
	return this->hstmt;
} // End of GetHSTMT
//-------------------------------------------------------------------