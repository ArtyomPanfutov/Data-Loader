#ifndef INPUT_ATTRIBUTE_H_
#define INPUT_ATTRIBUTE_H_
// InputAttribute.h
// created: 2018-08-18
// author: Artyom Panfutov

#include "constant.h"
#include <string>


struct server_name
{
	bool from_file;
	std::string srv_name;
};

struct db_name
{
	bool from_file;
	std::string database_name;
};

struct driver
{
	bool from_file;
	std::string driver_name;
};

//////////////////////////////////////////////////////////
class InputAttribute
{
  private:
	  server_name server;
	  db_name database;
	  driver driver;

	  bool ShowDiagInfo;

	  std::string
		  user_pass,
		  user_name,
		  config,   // Data from "config.ini"
		  connection_string,
		  filename;

	  std::string load_brief;
    
	  std::string GetUser();
	  std::string GetUserPass();
	  std::string GetServer();
	  std::string GetDB();
	  std::string GetDriver();
	  
	  unsigned int NumberOfThreads, Offset;
	  unsigned long BranchID;

  public:
	  InputAttribute();
	  ~InputAttribute();
	  void UserNameRequest();      
	  void UserPassRequest();
	  void ServerNameRequest(bool &); // from file: true - read from file; false - from console
	  void DBNameRequest(bool &);     // from file: true - read from file; false - from console
	  void GetConfig();               // read configuration file
	  void DriverRequest(bool &);     // from file: true - read from file; false - from console	  	  	  
	  void LoadBriefRequest();
	  void FileNameRequest();
	  void BranchIDRequest();
	  void ThreadsRequest();
	  void BuildConnectionString();   
	  void DisplayAllAttributes();    // Displaying all attributes on the console
	  bool IsDiagInfoToShow();

	  std::string GetLoadBrief();
	  std::string GetConnectionString();
	  std::string GetFileName();

	  unsigned int GetNumberOfThreads();
	  unsigned int GetOffset();
	  unsigned long GetBranchID();
};   

#endif // INPUT_ATTRIBUTE_H_