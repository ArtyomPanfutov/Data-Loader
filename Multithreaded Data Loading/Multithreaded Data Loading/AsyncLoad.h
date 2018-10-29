#ifndef ASYNCLOAD_H
#define ASYNCLOAD_H
// AsyncLoad.h
// created: 2018-07-26
// modified:
// author: Artyom Panfutov
//

#include "FALoad.h"
#include "SQLException.h"
#include "constant.h"
#include <vector>
#include <string>
#include "file.h"
#include "connection.h"

/////////////////////////////////////////////////////////
class AsyncLoad 
{
  public:
	  AsyncLoad(unsigned int);
	  ~AsyncLoad();
	  std::vector <FALoad *> FALoads;
	  unsigned long ProcessingRange;
	  unsigned int OffsetTime;

	  void RunLoadsInAsyncMode(std::vector <FALoad *> &, unsigned long);
	  void Message(std::string &);
	  SQLRETURN ExecuteSQL(SQLHSTMT CurSTMT, SQLCHAR* Statement, SQLINTEGER TextLength, FALoad *Load);

};

#endif // ASYNCLOAD_H