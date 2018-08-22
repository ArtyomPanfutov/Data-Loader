// FALoad.cpp
// created: 2018-07-22
// modified:
// author: Artyom Panfutov

#include "FALoad.h"
#include "SQLException.h"
#include <string>
#include "file.h"
#include <fstream>
#include <vector>

/***********************************************************************
*************** Definiton of methods. Class FALoader *******************
***********************************************************************/

// Constructor FALoader()
////////////////////////////////////////////////////////////////////////
FALoad::FALoad(std::string &filename, bool ShowDiag, unsigned long Branch) : Connection() 
{  
	

	this->ShowDiagInfo = ShowDiag;

	this->BranchID = Branch;

	this->FileName = filename; // For formula paramter(FileName)

	
}
// End of constructor FALoader()
// ---------------------------------------------------------------------


// Destructor FALoader()
////////////////////////////////////////////////////////////////////////
FALoad::~FALoad(void) 
{	


	for (std::vector< DSType *>::iterator it = DSTypes.begin(); it != DSTypes.end(); ++it)
	{
		delete *it;
	}
	DSTypes.clear();

	for (std::vector< Param *>::iterator it = SetupFormulaParams.begin(); it != SetupFormulaParams.end(); ++it)
	{
		delete *it;		
	}
	SetupFormulaParams.clear();


	for (std::vector< char *>::iterator it = Columns.begin(); it != Columns.end(); ++it)
	{
		delete *it;
	}
	Columns.clear();

	for (std::vector<int *>::iterator it = ColumnsLen.begin(); it != ColumnsLen.end(); ++it)
	{
		delete *it;
	}
	ColumnsLen.clear();

	for (std::vector< Param *>::iterator it = LastFormulaParams.begin(); it != LastFormulaParams.end(); ++it)
	{
		delete *it;		
	}
	LastFormulaParams.clear();

	
}
// End of destructor FALoader()
// ---------------------------------------------------------------------

// GetLoadInfo()
////////////////////////////////////////////////////////////////////////
void FALoad::GetLoadInfo()
{
	SQLCHAR SQLGetLoadInfo[] = "select ImportFileID,\
		                               ltrim(rtrim(Delimiter)),\
                                       PathInput,\
                                       PathOutput,\
                                       ParamList,\
                                       OEMToANSI,\
                                       SetupFormula,\
                                       LastFormula\
                                  from tImportFile with (nolock)\
                                 where Brief like ? and ImportType = 5";
	

	SQLINTEGER
		cbImportFileID,
		cbBrief = SQL_NTS,
		cbDelimiter,
		cbPathInput,
		cbPathOutput,
		cbParamList,
		cbOEMToANSI,
		cbSetupFormula,
		cbLastFormula;

	
	char *BriefParam = (char *)this->LoadBrief.c_str();

	try
	{
		retcode = SQLBindParameter( hstmt, 
			                        1,
			                        SQL_PARAM_INPUT,
			                        SQL_C_CHAR, 
			                        SQL_VARCHAR,
			                        10,                   // Nailed on the DSBRIEFNAME length
			                        0,
			                        BriefParam,
			                        sizeof(BriefParam),
			                        &cbBrief );

		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
			throw SQLException("GetLoadInfo failed! SQLBindParameter(Brief)", retcode);
		}
		
		retcode = SQLExecDirect( hstmt, 
			                     SQLGetLoadInfo, 
			                     SQL_NTS );
		
		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
			throw SQLException("GetLoadInfo failed! SQLExecDirect(SQLGetLoadInfo)", retcode);
		}
		
		if (retcode == SQL_SUCCESS)
		{
			

				
			while (TRUE)
			{
				retcode = SQLFetch(hstmt);
				if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
				{
					throw SQLException("GetLoadInfo failed! SQLFetch", retcode);
				}

				else if (retcode == SQL_SUCCESS)
				{
					// ImportFileID
					retcode = SQLGetData( hstmt, 
						                  1,
						                  SQL_C_DOUBLE,
						                  &ImportFileID,
						                  10,
						                  &cbImportFileID );

					if (retcode != SQL_SUCCESS)
						throw SQLException("GetLoadInfo failed! SQLGetData(ImportFileID)", retcode);

					// Delimiter										
					retcode = SQLGetData( hstmt,
										  2,
										  SQL_C_CHAR,
										  &Delimiter,
										  sizeof(Delimiter),
										  &cbDelimiter );

					if (retcode != SQL_SUCCESS)
						throw SQLException("GetLoadInfo failed! SQLGetData(Delimiter)", retcode);

					// PathInput									
					retcode = SQLGetData( hstmt,
						                  3,
						                  SQL_C_CHAR,
						                  &PathInput,
						                  sizeof(PathInput),
						                  &cbPathInput );

					if (retcode != SQL_SUCCESS)
						throw SQLException("GetLoadInfo failed! SQLGetData(PathInput)", retcode);

					// PathOutput								
					retcode = SQLGetData( hstmt,
						                  4,
						                  SQL_C_CHAR,
						                  &PathOutput,
						                  sizeof(PathOutput),
						                  &cbPathOutput );

					if (retcode != SQL_SUCCESS)
						throw SQLException("GetLoadInfo failed! SQLGetData(PathOutput)", retcode);

					// ParamList							
					retcode = SQLGetData( hstmt,
						                  5,
						                  SQL_C_CHAR,
						                  &ParamList,
						                  sizeof(ParamList),
						                  &cbParamList );

					if (retcode != SQL_SUCCESS)
						throw SQLException("GetLoadInfo failed! SQLGetData(ParamList)", retcode);

					// OEMToANSI
					retcode = SQLGetData( hstmt,
						                  6,
						                  SQL_C_DOUBLE,
						                  &OEMToANSI,
						                  10,
						                  &cbOEMToANSI);

					if (retcode != SQL_SUCCESS)
						throw SQLException("GetLoadInfo failed! SQLGetData(OEMToANSI)", retcode);

					// SetupFormula
					retcode = SQLGetData( hstmt,
					                      7,
						                  SQL_C_DOUBLE,
						                  &SetupFormula,
						                  10,
						                  &cbSetupFormula );

					if (retcode != SQL_SUCCESS)
						throw SQLException("GetLoadInfo failed! SQLGetData(SetupFormula)", retcode);

					// LastFormula
					retcode = SQLGetData( hstmt,
						                  8,
						                  SQL_C_DOUBLE,
						                  &LastFormula,
						                  10,
						                  &cbLastFormula );

					if (retcode != SQL_SUCCESS)
						throw SQLException("GetLoadInfo failed! SQLGetData(LastFormula)", retcode);

					if (ShowDiagInfo)
					{
						std::cout.setf(std::ios::fixed);
						std::cout << "\n ImportFileID " << ImportFileID;
						std::cout << "\n Delimiter " << Delimiter;
						std::cout << "\n PathInput " << PathInput;
						std::cout << "\n PathOutput " << PathOutput;
						std::cout << "\n ParamList " << ParamList;
						std::cout << "\n OEMToANSI " << OEMToANSI;
						std::cout << "\n SetupFormula " << SetupFormula;
						std::cout << "\n LastFormula" << LastFormula;
					}
				}
				else
				{
				//	std::cout << "\nSQLGetLoadID: No Info!\n";
					break;
				}
			}

		}
		else
			throw SQLException("GetLoadInfo failed!", retcode);

		retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
			throw SQLException("GetLoadInfo failed! SQLFreeStmt failed!", retcode);

		this->GetFormulaText(hstmt, SetupFormula, SetupFormulaStr);
		this->GetFormulaText(hstmt, LastFormula, LastFormulaStr);
		
		
		GetTableColumnsFromDB();
		
		if (ShowDiagInfo)
		{
			std::cout << "\n SetupFormulaStr:\n" << SetupFormulaStr;
			std::cout << "\n SetupFormulaStr:\n" << LastFormulaStr;

			std::cout << "\n TableColumns: " << TableColumns;

			for (int i = 0; i < ColumnsLen.size(); i++)
			{
				std::cout << "\n ColumnLen " << i << " " << *ColumnsLen[i];
			}
		}
		
	}
	catch (SQLException &ex)
	{
		ex.ShowMessage(hstmt);
	}
	catch (std::exception &ex)
	{
		std::cout << ex.what();
	}
	
}// End of GetLoadInfo()
//---------------------------------------------------------------------


// SetLoadBrief()
///////////////////////////////////////////////////////////////////////
void FALoad::SetLoadBrief(std::string &Brief)
{
	this->LoadBrief = Brief;
} // End of SetLoadBrief()
//---------------------------------------------------------------------


// ShowLoadBrief()
///////////////////////////////////////////////////////////////////////
void FALoad::ShowLoadBrief()
{
	//setlocale(LC_ALL, "Russian");
	  std::cout << "\n Сокращение загрузки: " << this->LoadBrief;
} // End of ShowLoadBrief()
//--------------------------------------------------------------------


// GetDSTypesFromDB()
//////////////////////////////////////////////////////////////////////
void FALoad::GetDSTypesFromDB()
{
  SQLCHAR SQLGetDSTypes[] = " \
	                        select s.name,\
                                    s.length,\
                                    s.prec\
	                          from systypes s with (nolock)\
                             inner join tDSType d with (nolock)\
                                     on s.name = d.typename";
	  
  SQLINTEGER cbName,
	         cbLength,
	         cbPrec;
 
  try 
  {
    retcode = SQLExecDirect( hstmt,
	                         SQLGetDSTypes,
	                         SQL_NTS );

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
    {
	  throw SQLException("GetDSTypesFromDB faled! SQLExecDirect(SQLGetDSTypes)", retcode);
    }

    if (retcode == SQL_SUCCESS)
    {
	    while (TRUE)
	    {
		  retcode = SQLFetch(hstmt);
		  if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
		  {
			  throw SQLException("GetDSTypesFromDB failed! SQLFetch", retcode);
		  }

		  else if (retcode == SQL_SUCCESS)
		  {
			  DSTypes.push_back(new DSType);

			  // Name
			  retcode = SQLGetData( hstmt,
				                    1,
				                    SQL_C_CHAR,
				                    DSTypes.back()->Name,
				                    sizeof(DSTypes.back()->Name),
				                    &cbName );

			  if (retcode != SQL_SUCCESS)
				  throw SQLException("GetDSTypesFromDB failed! SQLGetData(Name)", retcode);

			  // Length
			  retcode = SQLGetData( hstmt,
				                    2,
				                    SQL_C_ULONG,
				                    &DSTypes.back()->Length,
				                    sizeof(&DSTypes.back()->Length),
				                    &cbLength );

			  if (retcode != SQL_SUCCESS)
				  throw SQLException("GetDSTypesFromDB failed! SQLGetData(Length)", retcode);

			  // Precision
			  retcode = SQLGetData( hstmt,
				                    3,
				                    SQL_C_ULONG,
				                    &DSTypes.back()->Precision,
				                    sizeof(&DSTypes.back()->Precision),
				                    &cbPrec );

			  if (retcode != SQL_SUCCESS)
				  throw SQLException("GetDSTypesFromDB failed! SQLGetData(Precision)", retcode);

			  /*std::cout.setf(std::ios::fixed);
			  std::cout << "\n Name " << DSTypes.back()->Name;
			  std::cout << "\n Length " << DSTypes.back()->Length;
			  std::cout << "\n Precision " << DSTypes.back()->Precision;*/
		  }
		  else
		  {
			  break;
		  }
	    }

    }
    else
	  throw SQLException("GetLoadInfo failed!", retcode);


    retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	  throw SQLException("GetDSTypeFromDB failed! SQLFreeStmt failed!", retcode);
  }
  catch (SQLException &ex)
  {
	ex.ShowMessage(hstmt);
  }


  // Write into variables
  try
  {
	  for (auto i = 0; i < DSTypes.size(); i++)
	  {

		  if (strcmp(DSTypes[i]->Name, "DSBRIEFNAME") == 0)
		  {
			  DSBRIEFNAME_Length = DSTypes[i]->Length;
		  }
		  else if (strcmp(DSTypes[i]->Name, "DSCOMMENT") == 0)
		  {
			  DSCOMMENT_Length = DSTypes[i]->Length;
		  }


	  }

	  if (DSBRIEFNAME_Length < 0)
		  throw std::exception("\nType DSBRIEFNAME has incorrect length!\n");

	  if (DSCOMMENT_Length < 0)
		  throw std::exception("\nType DSCOMMENT has incorrect length!\n");
  }
  catch (std::exception &ex)
  {
	  std::cout << ex.what();
  }

} // End of GetDSTypesFromDB()
//---------------------------------------------------------------------------------


// GetType()
///////////////////////////////////////////////////////////////////////////////////
char * DSType::GetType()
{
	return Name;
} // End of GetType()
//--------------------------------------------------------------------------------


// GetFormulaText()
///////////////////////////////////////////////////////////////////////////////////
void FALoad::GetFormulaText(SQLHSTMT &hstmt, double &FormulaID, std::string &QueryStr)
{
	SQLCHAR SQLGetQueryStr[] = "select QueryStr\
                                  from tQueryLine with (nolock index = XPKtQueryLine)\
 		                         where Type = 1\
		                           and ContextID = ?";

	SQLINTEGER
		cbFormulaID = SQL_NTS,
		cbQueryStr;

	SQLCHAR SingleStr[256];


	try
	{
		retcode = SQLBindParameter(
			hstmt,
			1,
			SQL_PARAM_INPUT, 
			SQL_C_DOUBLE,
			SQL_DOUBLE,
			10,
			0,
			&FormulaID,
			sizeof(FormulaID),
			&cbFormulaID);

		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
			throw SQLException("GetFormulaText failed! SQLBindParameter(FormulaID)", retcode);
		}

		retcode = SQLExecDirect(
			hstmt,
			SQLGetQueryStr,
			SQL_NTS);

		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
			throw SQLException("GetFormulaText failed! SQLExecDirect(SQLGetQueryStr)", retcode);
		}

		if (retcode == SQL_SUCCESS)
		{
			while (TRUE)
			{
				retcode = SQLFetch(hstmt);
				if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
				{
					throw SQLException("GetFormulaText failed! SQLFetch", retcode);
				}

				else if (retcode == SQL_SUCCESS)
				{
					// QueryStr										
					retcode = SQLGetData(
						hstmt,
						1,
						SQL_C_CHAR,
						&SingleStr,
						sizeof(SingleStr),
						&cbQueryStr);

					if (retcode != SQL_SUCCESS)
						throw SQLException("GetFormulaText failed! SQLGetData(QueryStr)", retcode);

					if (strcmp((const char *)SingleStr, "SQL:") != 0) // Ingore first row "SQL:" from formula's text
					{
						QueryStr += "\n";
						QueryStr += (const char*)SingleStr;
					}

				}
				else
					break;
			}
		}

		retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
			throw SQLException("GetFormulaText failed! SQLFreeStmt failed!", retcode);
	}
	catch (SQLException &ex)
	{
		ex.ShowMessage(hstmt);
	}
} // End of GetFormulaText()
//---------------------------------------------------------------------------------


// GetSetupParamsFromDB
///////////////////////////////////////////////////////////////////////////////////
void FALoad::GetSetupParamsFromDB()
{
	SQLCHAR SQLGetSetupFormulaParams[] = "select Number,\
                                            CalcField,\
                                            CalcProperty\
                                       from tParam with (nolock index = XPKtParam)\
 		                              where FormulaID = ?";

	SQLINTEGER
		cbFormulaID = SQL_NTS,
		cbNumber,
		cbCalcField,
		cbCalcProperty;

	try
	{
		retcode = SQLBindParameter(
			hstmt,
			1,
			SQL_PARAM_INPUT,
			SQL_C_DOUBLE,
			SQL_DOUBLE,
			15,
			0,
			&SetupFormula,
			sizeof(SetupFormula),
			&cbFormulaID);

		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
			throw SQLException("GetSetupParamsFromDB failed! SQLBindParameter(FormulaID)", retcode);
		}

		retcode = SQLExecDirect(
			hstmt,
			SQLGetSetupFormulaParams,
			SQL_NTS);

		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
			throw SQLException("GetSetupParamsFromDB failed! SQLExecDirect(SQLGetFormulaParams)", retcode);
		}

		if (retcode == SQL_SUCCESS)
		{
			while (TRUE)
			{
				retcode = SQLFetch(hstmt);
				if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
				{
					throw SQLException("GetSetupParamsFromDB failed! SQLFetch", retcode);
				}

				else if (retcode == SQL_SUCCESS)
				{
					SetupFormulaParams.push_back(new Param);

					// Number									
					retcode = SQLGetData(
						hstmt,
						1,
						SQL_C_DOUBLE,
						&SetupFormulaParams.back()->Number,
						sizeof(SetupFormulaParams.back()->Number),
						&cbNumber);

					if (retcode != SQL_SUCCESS)
						throw SQLException("GetSetupParamsFromDB failed! SQLGetData(Number)", retcode);

					// CalcField								
					retcode = SQLGetData(
						hstmt,
						2,
						SQL_C_CHAR,
						&SetupFormulaParams.back()->CalcField,
						sizeof(SetupFormulaParams.back()->CalcField),
						&cbCalcField);

					if (retcode != SQL_SUCCESS)
						throw SQLException("GetSetupParamsFromDB failed! SQLGetData(CalcField)", retcode);

					// CalcProperty
					retcode = SQLGetData(
						hstmt,
						3,
						SQL_C_ULONG,
						&SetupFormulaParams.back()->CalcProperty,
						sizeof(SetupFormulaParams.back()->CalcProperty),
						&cbCalcProperty);

					if (retcode != SQL_SUCCESS)
						throw SQLException("GetSetupParamsFromDB failed! SQLGetData(CalcProperty)", retcode);
			
				}
				else
					break;
			}
		}

		retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
			throw SQLException("GetSetupParamsFromDB failed! SQLFreeStmt failed!", retcode);
	}
	catch (SQLException &ex)
	{
		ex.ShowMessage(hstmt);
	}

} // End of GetSetupParamsFromDB


  // GetLastParamsFromDB
  ///////////////////////////////////////////////////////////////////////////////////
void FALoad::GetLastParamsFromDB()
{
	SQLCHAR SQLGetLastFormulaParams[] = "select Number,\
                                            CalcField,\
                                            CalcProperty\
                                       from tParam with (nolock index = XPKtParam)\
 		                              where FormulaID = ?";

	SQLINTEGER
		cbFormulaID = SQL_NTS,
		cbNumber,
		cbCalcField,
		cbCalcProperty;

	try
	{
		retcode = SQLBindParameter(
			hstmt,
			1,
			SQL_PARAM_INPUT,
			SQL_C_DOUBLE,
			SQL_DOUBLE,
			15,
			0,
			&LastFormula,
			sizeof(LastFormula),
			&cbFormulaID);

		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
			throw SQLException("GetLastParamsFromDB failed! SQLBindParameter(FormulaID)", retcode);
		}

		retcode = SQLExecDirect(
			hstmt,
			SQLGetLastFormulaParams,
			SQL_NTS);

		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
			throw SQLException("GetLastParamsFromDB failed! SQLExecDirect(SQLGetFormulaParams)", retcode);
		}

		if (retcode == SQL_SUCCESS)
		{
			while (TRUE)
			{
				retcode = SQLFetch(hstmt);
				if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
				{
					throw SQLException("GetLastParamsFromDB failed! SQLFetch", retcode);
				}

				else if (retcode == SQL_SUCCESS)
				{
					LastFormulaParams.push_back(new Param);

					// Number									
					retcode = SQLGetData(
						hstmt,
						1,
						SQL_C_DOUBLE,
						&LastFormulaParams.back()->Number,
						sizeof(LastFormulaParams.back()->Number),
						&cbNumber);

					if (retcode != SQL_SUCCESS)
						throw SQLException("GetLastParamsFromDB failed! SQLGetData(Number)", retcode);

					// CalcField								
					retcode = SQLGetData(
						hstmt,
						2,
						SQL_C_CHAR,
						&LastFormulaParams.back()->CalcField,
						sizeof(LastFormulaParams.back()->CalcField),
						&cbCalcField);

					if (retcode != SQL_SUCCESS)
						throw SQLException("GetLastParamsFromDB failed! SQLGetData(CalcField)", retcode);

					// CalcProperty
					retcode = SQLGetData(
						hstmt,
						3,
						SQL_C_ULONG,
						&LastFormulaParams.back()->CalcProperty,
						sizeof(LastFormulaParams.back()->CalcProperty),
						&cbCalcProperty);

					if (retcode != SQL_SUCCESS)
						throw SQLException("GetLastParamsFromDB failed! SQLGetData(CalcProperty)", retcode);

				}
				else
					break;
			}
		}

		retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
			throw SQLException("GetLastParamsFromDB failed! SQLFreeStmt failed!", retcode);
	}
	catch (SQLException &ex)
	{
		ex.ShowMessage(hstmt);
	}

} // End of GetLastParamsFromDB
//--------------------------------------------------------------------------------


// GetInfo
///////////////////////////////////////////////////////////////////////////////////
void GetInfo(std::string &CalcField, unsigned int &CalcProperty)
{
	


} // End of GetInfo
//---------------------------------------------------------------------------------


// GetTableColumnsFromDB()
///////////////////////////////////////////////////////////////////////////////////
void FALoad::GetTableColumnsFromDB()
{
	SQLCHAR SQLGetColumns[] = "select count(*)\
                                 from syscolumns with (nolock)\
                                where id = object_id(?)";

	SQLCHAR SQLGetLengthColumn[] = "\
		select c.prec\
          from syscolumns c with (nolock)\
         where c.id = object_id(?)\
         order by c.colorder asc";

	SQLINTEGER
		cbParamList = SQL_NTS,
		cbTableColumns,
		cbColumnLength;

	try
	{
		// 1. Get count of columns
		retcode = SQLBindParameter(
			hstmt,
			1,
			SQL_PARAM_INPUT,
			SQL_C_CHAR,
			SQL_VARCHAR,
			256,
			0,
			ParamList,
			sizeof(ParamList),
			&cbParamList);

		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
			throw SQLException("GetTableColumnsFromDB failed! SQLBindParameter(ParamList)", retcode);
	

		retcode = SQLExecDirect(
			hstmt,
			SQLGetColumns,
			SQL_NTS);

		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
			throw SQLException("GetTableColumnsFromDB failed! SQLExecDirect(SQLGetColumns)", retcode);
		

		if (retcode == SQL_SUCCESS)
		{
			while (TRUE)
			{
				retcode = SQLFetch(hstmt);
				if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
				{
					throw SQLException("GetTableColumnsFromDB failed! SQLFetch", retcode);
				}

				else if (retcode == SQL_SUCCESS)
				{
					// Count									
					retcode = SQLGetData(
						hstmt,
						1,
						SQL_C_ULONG,
						&TableColumns,
						0,
						&cbTableColumns);

					if (retcode != SQL_SUCCESS)
						throw SQLException("GetTableColumnsFromDB failed! SQLGetData(Number)", retcode);
				}
				else
					break;
			}
		}

		retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
			throw SQLException("GetTableColumnsFromDB failed! SQLFreeStmt failed!", retcode);


		// 2. Get length for each column
		retcode = SQLBindParameter(
			hstmt,
			1,
			SQL_PARAM_INPUT,
			SQL_C_CHAR,
			SQL_VARCHAR,
			256,
			0,
			ParamList,
			sizeof(ParamList),
			&cbParamList);

		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
			throw SQLException("GetTableColumnsFromDB failed! SQLBindParameter(ParamList)", retcode);


		retcode = SQLExecDirect(
			hstmt,
			SQLGetLengthColumn,
			SQL_NTS);

		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
			throw SQLException("GetTableColumnsFromDB failed! SQLExecDirect(SQLGetLengthColumn)", retcode);

		if (retcode == SQL_SUCCESS)
		{    
			int i = 0;
			Columns.erase(Columns.cbegin(), Columns.cend());
			while (TRUE)
			{
				ColumnsLen.push_back(new int);

				retcode = SQLFetch(hstmt);
				if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
				{
					throw SQLException("GetTableColumnsFromDB failed! SQLFetch", retcode);
				}

				else if (retcode == SQL_SUCCESS)
				{
					// Length
					retcode = SQLGetData(
						hstmt,
						1,
						SQL_C_ULONG,
						ColumnsLen[i],
						0,
						&cbTableColumns);

					if (retcode != SQL_SUCCESS)
						throw SQLException("GetTableColumnsFromDB failed! SQLGetData(Length)", retcode);

					i++;
				}
				else
					break;
			}
		}

		retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
			throw SQLException("GetTableColumnsFromDB failed! SQLFreeStmt failed!", retcode);
		
	}
	catch (SQLException &ex)
	{
		ex.ShowMessage(hstmt);
	}

} // End of GetTableColumnsFromDB
  //---------------------------------------------------------------------------------



// StartBCP
/////////////////////////////////////////////////////////////////////////////////////
unsigned long FALoad::StartBCP(std::string &fieldterm_str, std::string &rowterm_str, unsigned long int &firstrow, unsigned long int &lastrow, std::vector<std::string> &file_str)
{
	int cbCol = 0;

//	std::ifstream file(filename);
	std::string cur_str;

	int it = 0,
		cur_pos = 0,
		last_pos = 0,
		str_num = 1;

	char *chColumn;  
	char terminator = '\0';

	DBINT cRowsDone = 0;

	RETCODE SendRet;

	// Create objects for each column.
	for (int i = 0; i < TableColumns; i++)
       Columns.push_back(new char[*ColumnsLen[i]]);

	// Set SPID Column. Temp table always has the first column "SPID".
	sprintf_s(Columns[0], *ColumnsLen[0], "%d", SPID);
	
	try
	{
		// Initialize the bulk copy.  
		retcode = bcp_init(hdbc, (const char*)ParamList, NULL, "bcp_log.txt", DB_IN);
		if (retcode != SUCCEED)
			throw SQLException("StartBCP failed! bcp_init", retcode);


		// Set code page.
		if (this->OEMToANSI == 1)
			retcode = bcp_control(hdbc, BCPFILECP, (void *)1251);
		else 
			retcode = bcp_control(hdbc, BCPFILECP, (void *)BCPFILECP_OEMCP);

		if (retcode == FAIL)
			throw SQLException("StartBCP failed! bcp_control(CP)", retcode);


		// Bind variables to table columns.
		for (int i = 0; i < TableColumns; i++)
		{
			retcode = bcp_bind(
				hdbc,
				(BYTE *)Columns[i],
				NULL,
				SQL_VARLEN_DATA,
				(UCHAR *)&terminator,
				1,
				SQLCHARACTER,
				i + 1);

			if ((retcode != SUCCEED))
				throw SQLException("StartBCP failed! bcp_bind", retcode);
		}


		for (int i = firstrow; i <= lastrow; i++)
		{
			cur_str = file_str[i];

			// Set num column.
			sprintf_s(Columns[1], *ColumnsLen[1], "%d", str_num);

			if (ShowDiagInfo)
			{
				std::cout << "\n Col " << 0 << " " << Columns[0];
				std::cout << "\n Col " << 1 << " " << Columns[1];
			}

			it = 2, cur_pos = 0, last_pos = 0;

			if (it < TableColumns - 1)
			{
				while ((cur_pos = cur_str.find(fieldterm_str, cur_pos)) != std::string::npos)
				{
					if (it > Columns.size() - 1)
						break;
					else
					{
						strcpy_s(Columns[it], *ColumnsLen[it], (cur_str.substr(last_pos, cur_pos - last_pos)).c_str());

						if (ShowDiagInfo)
						{
							std::cout << "\n Col " << it << " " << Columns[it];
						}

						last_pos = cur_pos + 1;
						cur_pos++;
						it++;
					}
				}
			}
			if (it == TableColumns - 1)
			{
				cur_pos = cur_str.find(rowterm_str, cur_pos);
				strcpy_s(Columns[it], *ColumnsLen[it], (cur_str.substr(last_pos, cur_pos - last_pos)).c_str());

				if (ShowDiagInfo)
				{
					std::cout << "\n Col " << it << " " << Columns[it];
				}

				last_pos = cur_pos - 1;
				it++;
			}

			// For those case when not all columns exists in the input file.
			if (it < TableColumns - 1)
			{
				while (it <= TableColumns - 1)
				{
					strcpy_s(Columns[it], *ColumnsLen[it], " ");

					if (ShowDiagInfo)
					{
						std::cout << "\n Col " << it << " " << Columns[it];
					}

					it++;
				}

			}

			// And send this row to the table.
			SendRet = bcp_sendrow(hdbc);
			if (SendRet != SUCCEED)
			{
				std::cout << "\n bcp_sendrow failed! retcode: " << SendRet;
			}

			str_num++;
		}

		// Signal the end of the bulk copy operation.  
		cRowsDone = bcp_done(hdbc);
		if ((cRowsDone == -1)) 
		{
			std::cout << "\n bcp_done(hdbc) Failed\n\n";
			
		}

		std::cout << "\n\n rows copied: " << cRowsDone << std::endl;

		//file.close();
	}
	catch (SQLException &ex)
	{
		ex.ShowMessage(hstmt);
	}

	return cRowsDone;
} // End of StartBCP
//-----------------------------------------------------------------------------------


// PrepareFormula()
/////////////////////////////////////////////////////////////////////////////////////
void FALoad::PrepareFormula(std::string &Formula, std::vector<Param *> &Params)
{
	try
	{
		for (int i = 0; i < Params.size(); i++)
		{
			if (strcmp(Params[i]->CalcField, "DealProtocolID") == 0)
			{
				SQLCHAR SQLGetNewImportProtocolID[] =
					" set nocount on " 
					" declare @RetVal int, @ID DSIDENTIFIER, @CurDate DSOPERDAY "
					" select @CurDate = CONVERT (date, GETDATE())  "
					" exec @RetVal = ImportProtocol_Insert @ID out, ?, @CurDate, @CurDate " 
					" select @RetVal, @ID ";

				SQLINTEGER
					cbRetVal = 0,
					cbImportProtocolID = 0,
					cbImportFileID = 0,
					RetVal = 0;

				double ImportProtocolID, ImportID;

				ImportID = this->ImportFileID;

				// Bind ImportFileID
				retcode = SQLBindParameter(
					hstmt,
					1,
					SQL_PARAM_INPUT,
					SQL_C_DOUBLE,
					SQL_DOUBLE,
					15,
					0,
					&ImportID,
					sizeof(ImportID),
					&cbImportFileID);

				if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
				{
					throw SQLException("GetLoadInfo failed! SQLBindParameter(Brief)", retcode);
				}

				retcode = SQLExecDirect(
					hstmt,
					SQLGetNewImportProtocolID,
					SQL_NTS);

				if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO && retcode != SQL_NEED_DATA)
					throw SQLException("PrepareFormula failed! SQLExecDirect(SQLGetNewImportProtocolID)", retcode);
			

				if (retcode == SQL_SUCCESS)
				{
					while (TRUE)
					{
						retcode = SQLFetch(hstmt);
						if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
						{
							throw SQLException("PrepareFormula failed! SQLFetch", retcode);
						}

						else if (retcode == SQL_SUCCESS)
						{
							// RetVal								
							retcode = SQLGetData(
								hstmt,
								1,
								SQL_C_ULONG,
								&RetVal,
								0,
								&cbRetVal);

							if (retcode != SQL_SUCCESS)
								throw SQLException("PrepareFormula failed! SQLGetData(RetVal)", retcode);							

							// ImportProtocolID							
							retcode = SQLGetData(
								hstmt,
								2,
								SQL_C_DOUBLE,
								&ImportProtocolID,
								0,
								&cbImportProtocolID);

							if (retcode != SQL_SUCCESS)
								throw SQLException("PrepareFormula failed! SQLGetData(ImportProtocolID)", retcode);
						}
						else
							break;
					}
				}

				SQLRETURN closeCursRet = SQLCloseCursor(hstmt);
				if (closeCursRet != SQL_SUCCESS)
					throw SQLException("PrepareFormumla failed! SQLCloseCursor", retcode);

				retcode = SQLFreeStmt(hstmt, SQL_CLOSE);

				if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
					throw SQLException("PrepareFormula failed! SQLFreeStmt failed!", retcode);

				// Check @RetVal 
				if (RetVal == 0)
				{	
					Params[i]->Value = std::to_string(ImportProtocolID);
					int pos = 0;
					pos = Params[i]->Value.find(",", pos);
					Params[i]->Value.erase(pos, Params[i]->Value.length() - pos);
				}
				else
					throw SQLException("ImportProtocol_Insert returned retval!", retcode);

			
			}
			else if (strcmp(Params[i]->CalcField, "Branch") == 0)
			{

				if (Params[i]->CalcProperty == 0)
				  Params[i]->Value = std::to_string(this->BranchID);
				else 
					Params[i]->Value = std::string("'") + std::to_string(BranchID) + std::string("'");
			}
			else if (strcmp(Params[i]->CalcField, "FileName") == 0)
			{
				Params[i]->Value = std::string("'") + FileName + std::string("'");
			}
			else
			{ 
				std::string Value;
				std::cout << "\n Enter " << Params[i]->CalcField << ": ";
				std::cin >> Value;

				if (Params[i]->CalcProperty == 0)
					Params[i]->Value = Value;
				else
					Params[i]->Value = std::string("'") + Value + std::string("'");
			}

			// Change formula's text
			std::string finding_str;

			finding_str = std::string("%") + std::to_string(static_cast<int>(Params[i]->Number)) + std::string("!");

			if (ShowDiagInfo)
			  std::cout << "\n finding str " << finding_str;

			int cur_pos = 0;

			while ((cur_pos = Formula.find(finding_str, cur_pos)) != std::string::npos)
			{
				Formula.replace(cur_pos, finding_str.length(), Params[i]->Value);
				cur_pos++;
			}

			cur_pos = 0;

            // Set nocount setting, if not setted.
			std::string nocount_on_str = "set nocount on ";
			if (cur_pos = Formula.find(nocount_on_str, cur_pos) == 0)
				Formula.insert(0, nocount_on_str);

		}
		if (ShowDiagInfo)
		  std::cout << "\nPrepared formula's text: \n" << Formula;

	}
	catch (SQLException &ex)
	{
		ex.ShowMessage(hstmt);
	}
} // End of PrepareFormula
//----------------------------------------------------------------------------------


// ExecuteFormula
////////////////////////////////////////////////////////////////////////////////////
void FALoad::ExecuteFormula(std::string &Formula)
{
	SQLCHAR *SQLFormula = (unsigned char *)Formula.c_str();	

	SQLRETURN mrRetcode;

	SQLINTEGER cbRetVal;

	unsigned int RetVal = 0; // For returning value from the formula

	try
	{
		retcode = SQLExecDirect(
			hstmt,
			SQLFormula,
			SQL_NTS);

		if (retcode != SQL_SUCCESS && retcode != SQL_NO_DATA)
			throw SQLException("ExecuteFormula failed! SQLExecDirect(SQLFormula)", retcode);	
	
		do 
		{  
			if (retcode == SQL_SUCCESS)
			{	
				while (TRUE)
				{
					retcode = SQLFetch(hstmt);

					if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
						throw SQLException("ExecuteFormula failed! SQLFetch", retcode);
					else if (retcode == SQL_SUCCESS)
					{
						// RetVal
						retcode = SQLGetData(
							hstmt,
							1,
							SQL_C_ULONG,
							&RetVal,
							10,
							&cbRetVal);

						if (retcode != SQL_SUCCESS)
							throw SQLException("ExecuteFormula failed! SQLGetData(RetVal)", retcode);
					}
					else
						break;
				}

			}
		} while (mrRetcode = SQLMoreResults == SQL_SUCCESS);

		//std::cout << "\n RetVal " << RetVal;

		retcode = SQLFreeStmt(hstmt, SQL_CLOSE);
		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		  throw SQLException("ExecuteFormula failed! SQLFreeStmt failed!", retcode);



	}
	catch (SQLException &ex)
	{
		ex.ShowMessage(hstmt);
	}	
	
} // End of ExecuteFormula
//----------------------------------------------------------------------------------


// SetProcessingRange
/////////////////////////////////////////////////////////////////////////////////////
void FALoad::SetProcessingRange(unsigned long &first, unsigned long&last)
{
	this->FirstRow = first;
	this->LastRow = last;

} // End of SetProcessingRange
//-----------------------------------------------------------------------------------