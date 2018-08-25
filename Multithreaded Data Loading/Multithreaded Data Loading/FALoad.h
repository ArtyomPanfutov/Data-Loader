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

// Типы данных из БД Диасофт
struct DSType 
{
	char Name[256];
	unsigned int Length;
	unsigned int Precision;

	char * GetType(); // Вернуть наименование типа данных
};

// Параметры формул
struct Param
{
	double Number;             // Номер параметра
    char CalcField[30];        // Имя параметра
	unsigned int CalcProperty; // 0 - числовой 1 - строковый
	std::string Value;         // Значение параметра
	double GetInfo(std::string &, unsigned int &); // Возвращает номер параметра + информацию по ссылкам
};



/////////////////////////////////////////////////////////////
class FALoad : public Connection
{
  public:
	std::vector <DSType *> DSTypes;           // Для хранения типов данных из БД Диасофт
	std::vector <Param *> SetupFormulaParams; // Параметры формулы предобработки
	std::vector <Param *> LastFormulaParams;  // Параметры формулы постобработки
	std::vector <char *> Columns;             // Данные для каждого столбца таблицы записывать сюда
	std::vector <int *> ColumnsLen;           // Длина столбцов 
	std::vector <bool> IsNumeric;             // Тип данных столбца числовой или не числовой

	bool ShowDiagInfo;

	SQLINTEGER
		DSBRIEFNAME_Length,
		DSCOMMENT_Length;

	unsigned long
		TableColumns, // Количество столбцов в таблице из ParamList
		FirstRow,     // Номер первой строки из диапазона, которая будет обработана(включительно)
		LastRow,      // Номер последенй строки из диапазона, которая будет обработана(включительно)
		BranchID;     // Идентификатор филиала

	double
		ImportFileID, // Идентификатор загрузки
		OEMToANSI,    // Флаг OEM\ANSI
		ContextID,    // Идентификатор формулы с параметрами
		SetupFormula, // Идентификатор формулы предобработки
		LastFormula;  // Идентификатор формулы постаброботки

	 SQLCHAR
		 Delimiter[11],   // Символ разделитель
		 PathInput[256],  // Путь к файлу загрузки 
		 PathOutput[256], // Путь для сохранения файла
		 ParamList[256];  // Таблица для загрузки BCP
		//SetupFormulaStr[4000]; // Текст формулы предобработки
	
	 std::string
		 SetupFormulaStr, // Текст формулы предобработки
		 LastFormulaStr,  // Текст формулы постобработки
		 FileName,        // Имя загружаемое файла
		 LoadBrief;     // Сокращение загрузки

	 SQLRETURN sql_retcode;
	
    FALoad(std::string &, bool, unsigned long);
	~FALoad();
	void SetLoadBrief(std::string &); // Записать сокращение загрузки
	void ShowLoadBrief();         // Вывести на экран сокращение загрузки
    void GetLoadInfo();           // Начитка информации о загрузке из БД
	void GetFormulaText(SQLHSTMT &, double &, std::string &); // Вытащить текст формулы из БД
    void GetDSTypesFromDB();      // Начитать типы данных Диасофт
	void GetSetupParamsFromDB();  // Начитать параметры формулы предобработки
	void GetLastParamsFromDB();   // Начитать параметры формулы постобработки
	void GetTableColumnsFromDB(); // Получить количество столбцов таблицы из ParamList + информация о длине каждого столбца в Columns
	void PrepareFormula(std::string &, std::vector<Param *> &); // Обработка текста формулы (подстановка параметров)
	void ExecuteFormula(std::string &); // Запуск запроса с текстом формулы
	void SetProcessingRange(unsigned long &, unsigned long &); // Установить границы диапазона обрабатываемых строк 
	unsigned long StartBCP(std::string &, std::string &, unsigned long int&, unsigned long int&, std::vector<std::string> &); // Загрузить данные в таблица по BCP из файла
};


#endif // FALoad_H
