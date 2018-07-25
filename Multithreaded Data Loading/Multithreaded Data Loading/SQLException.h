#ifndef SQLEXCEPTION_H
#define SQLEXCEPTION_H
// SQLException.h
// created: 2018-07-22
// modified:
// author: Artyom Panfutov


#include <iostream>
#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <odbcss.h>

///////////////////////////////////////////////////////////////
class SQLException : public std::exception 
{
  private:
    SQLRETURN RetVal;
	SQLCHAR SQLState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER NativeError;
	SQLSMALLINT MsgLen;
  public:
    SQLException(const char*, SQLRETURN);
    void ShowMessage(SQLHSTMT);
};


#endif // SQLEXCEPTION_H
