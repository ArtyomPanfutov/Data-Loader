#ifndef FALoad_H
#define FALoad_H
// FALoad.h
// created: 2018-07-22
// modified:
// author: Artyom Panfutov

#include "connection.h"
#include "constant.h"
#include <vector>
#include <string>

// ���� ������ �� �� �������
struct DSType 
{
	char Name[256];
	unsigned int Length;
	unsigned int Precision;

	char * GetType(); // ������� ������������ ���� ������
};

// ��������� ������
struct Param
{
	double Number;             // ����� ���������
    char CalcField[30];        // ��� ���������
	unsigned int CalcProperty; // 0 - �������� 1 - ���������
	std::string Value;         // �������� ���������
	double GetInfo(std::string &, unsigned int &); // ���������� ����� ��������� + ���������� �� �������
};



/////////////////////////////////////////////////////////////
class FALoad : public Connection
{
  public:
	std::vector <DSType *> DSTypes;           // ��� �������� ����� ������ �� �� �������
	std::vector <Param *> SetupFormulaParams; // ��������� ������� �������������
	std::vector <Param *> LastFormulaParams;  // ��������� ������� �������������
	std::vector <char *> Columns;             // ������ ��� ������� ������� ������� ���������� ����
	std::vector <int *> ColumnsLen;           // ����� �������� 
	std::vector <bool> IsNumeric;             // ��� ������ ������� �������� ��� �� ��������

	bool ShowDiagInfo;

	SQLINTEGER
		DSBRIEFNAME_Length,
		DSCOMMENT_Length;

	unsigned long
		TableColumns, // ���������� �������� � ������� �� ParamList
		FirstRow,     // ����� ������ ������ �� ���������, ������� ����� ����������(������������)
		LastRow,      // ����� ��������� ������ �� ���������, ������� ����� ����������(������������)
		BranchID;     // ������������� �������

	double
		ImportFileID, // ������������� ��������
		OEMToANSI,    // ���� OEM\ANSI
		ContextID,    // ������������� ������� � �����������
		SetupFormula, // ������������� ������� �������������
		LastFormula;  // ������������� ������� �������������

	 SQLCHAR
		 Delimiter[11],   // ������ �����������
		 PathInput[256],  // ���� � ����� �������� 
		 PathOutput[256], // ���� ��� ���������� �����
		 ParamList[256];  // ������� ��� �������� BCP
		//SetupFormulaStr[4000]; // ����� ������� �������������
	
	 std::string
		 SetupFormulaStr, // ����� ������� �������������
		 LastFormulaStr,  // ����� ������� �������������
		 FileName,        // ��� ����������� �����
		 LoadBrief;     // ���������� ��������

	 SQLRETURN sql_retcode;
	
    FALoad(std::string &, bool, unsigned long);
	~FALoad();
	void SetLoadBrief(std::string &); // �������� ���������� ��������
	void ShowLoadBrief();         // ������� �� ����� ���������� ��������
    void GetLoadInfo();           // ������� ���������� � �������� �� ��
	void GetFormulaText(SQLHSTMT &, double &, std::string &); // �������� ����� ������� �� ��
    void GetDSTypesFromDB();      // �������� ���� ������ �������
	void GetSetupParamsFromDB();  // �������� ��������� ������� �������������
	void GetLastParamsFromDB();   // �������� ��������� ������� �������������
	void GetTableColumnsFromDB(); // �������� ���������� �������� ������� �� ParamList + ���������� � ����� ������� ������� � Columns
	void PrepareFormula(std::string &, std::vector<Param *> &); // ��������� ������ ������� (����������� ����������)
	void ExecuteFormula(std::string &); // ������ ������� � ������� �������
	void SetProcessingRange(unsigned long &, unsigned long &); // ���������� ������� ��������� �������������� ����� 
	unsigned long StartBCP(std::string &, std::string &, unsigned long int&, unsigned long int&, std::vector<std::string> &); // ��������� ������ � ������� �� BCP �� �����
};


#endif // FALoad_H
