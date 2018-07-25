#ifndef FALoad_H
#define FALoad_H
// FALoad.h
// created: 2018-07-22
// modified:
// author: Artyom Panfutov

#include "connection.h"
#include "constant.h"
#include <vector>

// Типы данных из БД Диасофт
struct DSType 
{
	char Name[256];
	unsigned int Length;
	unsigned int Precision;

	char * GetType(); // Вернуть наименование типа данных
};


/////////////////////////////////////////////////////////////
class FALoad : public Connection
{
  private:
	SQLINTEGER SetupFormula, // Идентификатор формулы предобработки
	           LastFormula,  // Идентификатор формулы постобработки
		       ContextID;    // Идентификатор формулы для параметров

	const short int ImportType = 5; // Тип загрузки: BCP

  public:
    SQLCHAR *LoadBrief,                     // Сокращение загрузки
		     Delimiter; // Символ разделитель

	std::vector <DSType *> DSTypes; // Для хранения типов данных из БД Диасофт
	



    FALoad();
	void SetLoadBrief(SQLCHAR &);
	void ShowLoadBrief();
    void GetLoadInfo();
	void GetDSTypesFromDB();
};


#endif // FALoad_H
