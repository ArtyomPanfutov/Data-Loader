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
                                       ContextID\
                                  from tImportFile with (nolock)\
                                 where Brief = ? and ImportType = 5";
	
	double ImportFileID;
	SQLINTEGER cbImportFileID,
		       cbBrief = SQL_NTS,
		       cbDelimiter,
		       DSBRIEFNAME_Length;

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
			// Get the length of DS types
			for (auto i = 0; i < DSTypes.size(); i++)
			{
				//std::cout << DSTypes[i]->Name << std::endl;
				if (DSTypes[i]->Name == "DSBRIEFNAME")
				{
					std::cout << "\nlength " << DSBRIEFNAME_Length;
					DSBRIEFNAME_Length = DSTypes[i]->Length;
				}
			}
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
										  DSBRIEFNAME_Length + 1,
										  &cbDelimiter);

					if (retcode != SQL_SUCCESS)
						throw SQLException("GetLoadInfo failed! SQLGetData(Delimiter)", retcode);


					std::cout.setf(std::ios::fixed);
					std::cout << "\n ImportFileID " << ImportFileID;
					std::cout << "\n Delimiter " << Delimiter;
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
} // End of GetDSTypesFromDB()
//---------------------------------------------------------------------------------


// GetType()
///////////////////////////////////////////////////////////////////////////////////
char * DSType::GetType()
{
	return Name;
} // End of GetType()
//--------------------------------------------------------------------------------