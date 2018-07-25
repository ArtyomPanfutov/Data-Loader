// FALoad.cpp
// created: 2018-07-22
// modified:
// author: Artyom Panfutov

#include "FALoad.h"
#include "SQLException.h"
#include <string>

/***********************************************************************
************* Definiton of functions. Class FALoader *******************
***********************************************************************/

// Constructor FALoader()
////////////////////////////////////////////////////////////////////////
FALoad::FALoad() : Connection()
{  }
// End of constructor FALoader()
// ---------------------------------------------------------------------

// GetLoadInfo()
////////////////////////////////////////////////////////////////////////
void FALoad::GetLoadInfo()
{
	SQLCHAR SQLGetLoadInfo[] = "select ImportFileID,\
		                               Delimiter,\
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
			                        SQL_PARAM_INPUT,\
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


} // End of GetDSTypesFromDB()
//---------------------------------------------------------------------------------


// GetType()
///////////////////////////////////////////////////////////////////////////////////
char * DSType::GetType()
{
	return Name;
} // End of GetType()
//--------------------------------------------------------------------------------