#ifndef ASYNCLOAD_H
#define ASYNCLOAD_H
// AsyncLoad.h
// created: 2018-07-26
// modified:
// author: Artyom Panfutov

#include "FALoad.h"
#include "SQLException.h"
#include "constant.h"
#include <vector>
#include "file.h"
#include "connection.h"

/////////////////////////////////////////////////////////
class AsyncLoad : Connection
{
  public:
	  std::vector <FALoad *> FALoads;
	  file inputfile;

	  

	  


};

#endif // ASYNCLOAD_H