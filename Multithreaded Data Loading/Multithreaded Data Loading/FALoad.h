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
  public:
	std::vector <DSType *> DSTypes; // Для хранения типов данных из БД Диасофт

	SQLINTEGER
		DSBRIEFNAME_Length,
		DSCOMMENT_Length;

	double
		ImportFileID, // Идентификатор загрузки
		OEMToANSI,    // Флаг OEM\ANSI
		ContextID,    // Идентификатор формулы с параметрами
		SetupFormula, // Идентификатор формулы предобработки
		LastFormula;  // Идентификатор формулы постаброботки

	SQLCHAR  
		*LoadBrief,       // Сокращение загрузки
		 Delimiter[11],   // Символ разделитель
	     PathInput[256],  // Путь к файлу загрузки 
		 PathOutput[256], // Путь для сохранения файла
		 ParamList[256];  // Таблица для загрузки BCP
	
	
    FALoad();
	void SetLoadBrief(SQLCHAR &); // Записать сокращение загрузки
	void ShowLoadBrief();         // Вывести на экран сокращение загрузки
    void GetLoadInfo();           // Начитка информации о загрузке из БД
    void GetDSTypesFromDB();      // Начитать типы данных Диасофт
};


#endif // FALoad_H
