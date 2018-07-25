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
  private:
	SQLINTEGER SetupFormula, // ������������� ������� �������������
	           LastFormula,  // ������������� ������� �������������
		       ContextID;    // ������������� ������� ��� ����������

	const short int ImportType = 5; // ��� ��������: BCP

  public:
    SQLCHAR *LoadBrief,                     // ���������� ��������
		     Delimiter; // ������ �����������

	std::vector <DSType *> DSTypes; // ��� �������� ����� ������ �� �� �������
	



    FALoad();
	void SetLoadBrief(SQLCHAR &);
	void ShowLoadBrief();
    void GetLoadInfo();
	void GetDSTypesFromDB();
};


#endif // FALoad_H
