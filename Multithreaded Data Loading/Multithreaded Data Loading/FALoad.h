#ifndef FALoad_H
#define FALoad_H
// FALoad.h
// created: 2018-07-22
// modified:
// author: Artyom Panfutov

#include "connection.h"
#include "constant.h"
#include <vector>

// ���� ������ �� �� �������
struct DSType 
{
	char Name[256];
	unsigned int Length;
	unsigned int Precision;

	char * GetType(); // ������� ������������ ���� ������
};


/////////////////////////////////////////////////////////////
class FALoad : public Connection
{
  public:
	std::vector <DSType *> DSTypes; // ��� �������� ����� ������ �� �� �������

	SQLINTEGER
		DSBRIEFNAME_Length,
		DSCOMMENT_Length;

	double
		ImportFileID, // ������������� ��������
		OEMToANSI,    // ���� OEM\ANSI
		ContextID,    // ������������� ������� � �����������
		SetupFormula, // ������������� ������� �������������
		LastFormula;  // ������������� ������� �������������

	SQLCHAR  
		*LoadBrief,       // ���������� ��������
		 Delimiter[11],   // ������ �����������
	     PathInput[256],  // ���� � ����� �������� 
		 PathOutput[256], // ���� ��� ���������� �����
		 ParamList[256];  // ������� ��� �������� BCP
	
	
    FALoad();
	void SetLoadBrief(SQLCHAR &); // �������� ���������� ��������
	void ShowLoadBrief();         // ������� �� ����� ���������� ��������
    void GetLoadInfo();           // ������� ���������� � �������� �� ��
    void GetDSTypesFromDB();      // �������� ���� ������ �������
};


#endif // FALoad_H
