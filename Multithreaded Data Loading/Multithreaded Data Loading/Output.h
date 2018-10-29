#define _CRT_SECURE_NO_WARNINGS
#ifndef OUTPUT_H
#define OUTPUT_H
// Output.h
// created: 2018-09-20
//

#include <string> 
#include <fstream>

class Log
{
  private:
	  std::string OutputFileName;
	  std::ofstream OutputFile;
  public:	
	  Log(std::string &FileName);
	  Log(std::string &FileName, bool ClearFile);
	  ~Log();

	  void DisplayOnConsole(std::string &Message); 
	  void WriteIntoFile(std::string &Message);
	  void Push(std::string &Message, std::string Type, bool IntoFile);
	  void ClearFile();
	  inline void CloseFile();
};

#endif // OUTPUT_H