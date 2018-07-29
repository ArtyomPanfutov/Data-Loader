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
FALoad::FALoad() : Connection()
{  }
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
                                 where Brief = ? and ImportType = 5";
	

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

	try
	{
		retcode = SQLBindParameter( hstmt, 
			                        1,
			                        SQL_PARAM_INPUT,
			                        SQL_C_CHAR, 
			                        SQL_VARCHAR,
			                        10,
			                        0,
			                        LoadBrief,
			                        sizeof(LoadBrief),
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
		std::cout << "\n SetupFormulaStr:\n" << SetupFormulaStr;
		std::cout << "\n SetupFormulaStr:\n" << LastFormulaStr;

		GetTableColumnsFromDB();
		std::cout << "\n TableColumns: " << TableColumns;
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
void FALoad::SetLoadBrief(SQLCHAR &Brief)
{
	LoadBrief = &Brief;
} // End of SetLoadBrief()
//---------------------------------------------------------------------


// ShowLoadBrief()
///////////////////////////////////////////////////////////////////////
void FALoad::ShowLoadBrief()
{
	  std::cout << "\n Сокращение загрузки: " << LoadBrief;
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
			10,
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
			10,
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


	SQLINTEGER
		cbParamList = SQL_NTS,
		cbTableColumns;

	try
	{
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
		
	}
	catch (SQLException &ex)
	{
		ex.ShowMessage(hstmt);
	}

} // End of GetTableColumnsFromDB
  //---------------------------------------------------------------------------------



// StartBCP
/////////////////////////////////////////////////////////////////////////////////////
void FALoad::StartBCP(std::string &filename, std::string &fieldterm_str, std::string &rowterm_str)
{
	LPCBYTE
		termfield = reinterpret_cast<LPCBYTE>(fieldterm_str.c_str()),
	    termrow = reinterpret_cast<LPCBYTE>(rowterm_str.c_str());

	SDWORD cRows;


	try
	{
		// Initialize the bulk copy.  
		retcode = bcp_init(hdbc, reinterpret_cast<LPCSTR>(ParamList), filename.c_str(), "ERRORBCP.txt", DB_IN);
		if (retcode != SUCCEED)
			throw SQLException("StartBCP failed! bcp_init", retcode);


		// Set keep nulls
		retcode = bcp_control(hdbc, BCPKEEPNULLS, (void*)TRUE);
		if (retcode == FAIL)
			throw SQLException("StartBCP failed! bcp_control(bcpkeepnulls)", retcode);


		// Set code page
		retcode = bcp_control(hdbc, BCPFILECP, (void*)BCPFILECP_RAW);
		if (retcode == FAIL)
			throw SQLException("StartBCP failed! bcp_control(bcpfilecp)", retcode);

		retcode = bcp_columns(hdbc, TableColumns);
		if (retcode == FAIL)
			throw SQLException("StartBCP failed! bcp_columns", retcode);
			
		
		for (unsigned long int i = 1; i <= TableColumns; i++)
		{
			
			if ((i != TableColumns) && (i != 1) && (i != 2))
				bcp_colfmt(hdbc, i, NULL, NULL, NULL, termfield, 1, i);
			else if (i == 1)
				bcp_colfmt(hdbc, i, SQLVARCHAR, NULL, SQL_VARLEN_DATA, termfield, 1, i);
			else if (i == 2)
				bcp_colfmt(hdbc, i, SQLVARCHAR, NULL, SQL_VARLEN_DATA, termfield, 1, i);
			else
				bcp_colfmt(hdbc, i, SQLVARCHAR, NULL, SQL_VARLEN_DATA, termrow, 2, i);

		}


		// Execute the bulk copy.  
		retcode = bcp_exec(hdbc, &cRows);
		if ((retcode != SUCCEED))
			throw SQLException("StartBCP failed! bcp_exec", retcode);

		std::cout << "\n rows copied " << cRows;
	}
	catch (SQLException &ex)
	{
		ex.ShowMessage(hstmt);
	}

} // End of StartBCP
//-----------------------------------------------------------------------------------
