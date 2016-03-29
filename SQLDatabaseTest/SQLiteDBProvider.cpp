#include "stdafx.h"
#include "sqlite3.h"
#include "SQLiteDBProvider.h"
#include "SQLiteException.h"
#include "Utils.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSQLiteDBProvider::CSQLiteDBProvider()
	:m_pDBHandle( NULL )
{
}

CSQLiteDBProvider::~CSQLiteDBProvider()
{
	VERIFY( CloseConnection() );
}

BOOL CSQLiteDBProvider::OpenConnection( const CString& strDBPath, const CString& strDBName, const CString& strConnection )
	throw ( CPathException, CDatabaseException )
{
	UNREFERENCED_PARAMETER( strConnection );
	using namespace Utils;

	if( strDBPath.IsEmpty() || strDBName.IsEmpty() ) throw CPathException( LoadStringFromRes( IDS_NULL_DB_PATH ) );
	if( !_CheckDir( strDBPath ) ) throw CPathException( LoadStringFromRes( IDS_CANT_PREPARE_DIR ) + strDBPath );

	VERIFY( CloseConnection() );

	const TCHAR chSlash = _T('\\');
	CString strPath( strDBPath );
	if( strPath.GetAt( strPath.GetLength() - 1 ) != chSlash )
		strPath += chSlash;

	CString strFullPath( strPath + strDBName );
	
	const int nOpenDBRes = sqlite3_open16( strFullPath, &m_pDBHandle );
	if( m_pDBHandle == NULL ) throw CDBConnectionException( LoadStringFromRes( IDS_SQLITE_OUT_OF_MEMORY ) );

	if( nOpenDBRes != SQLITE_OK )
	{
		VERIFY( CloseConnection() );
		throw CDatabaseException( LoadStringFromRes( IDS_CANT_OPEN_DB_CONNECT ) + strFullPath );
	}

	m_strDBFileName = strDBName;
	m_strDBFilePath = strPath;

	if( !_InitStorage() )
	{
		VERIFY( CloseConnection() );
		throw CDBConnectionException( LoadStringFromRes( IDS_COULDNT_INIT_DB ) );
	}
	return TRUE;
}

BOOL CSQLiteDBProvider::CloseConnection() noexcept
{
	if( IsConnectionOpened() )
	{
		m_queryStorage.UnInit();

		const int nCloseResult = sqlite3_close( _GetDBHandle() );
		ASSERT( nCloseResult == SQLITE_OK );
		_SetDBHandle( NULL );
		return nCloseResult == SQLITE_OK;
	}
	return TRUE;
}

BOOL CSQLiteDBProvider::ExecuteQuery( const CString& strSQLCommand ) throw ( CQueryException )
{
	if( !IsConnectionOpened() )
		return FALSE;

	CStringA strCommand = _WideToANSI( strSQLCommand );

	//todo: set callback func
	char* chErrorMSG = 0;
	const int SQL_EXEC_RES = sqlite3_exec( _GetDBHandle(), strCommand, NULL, NULL, &chErrorMSG );

	if( SQL_EXEC_RES != SQLITE_OK )
	{
		CString strError( chErrorMSG );
		sqlite3_free( chErrorMSG );
		TRACE( _T("\nSQL ERROR %d\nMESSAGE: %s"), SQL_EXEC_RES, strError );
		throw CSQLiteQueryException( Utils::LoadStringFromRes( IDS_QUERY_EXEC_FAIL ) + strSQLCommand );
	}
	return SQL_EXEC_RES == SQLITE_OK;
}

BOOL CSQLiteDBProvider::ExecuteQuery( const BaseSQLQueryID& ID ) throw ( CQueryException )
{
	if( !( IsConnectionOpened() && m_queryStorage.IsInit() ) )
		return FALSE;

	//[sqlite3_step()]
	int nStepResult = sqlite3_step( m_queryStorage.GetQuery( ID ) );

	if( nStepResult != SQLITE_DONE || nStepResult == SQLITE_ROW )
	{
		CString strMessage = _ANSIToWide( sqlite3_errmsg( m_pDBHandle ) );
		throw CSQLiteQueryException( strMessage );
	}
	while( nStepResult != SQLITE_DONE || nStepResult == SQLITE_ROW )
	{
		nStepResult = sqlite3_step( m_queryStorage.GetQuery( ID ) );
	}
	return TRUE;
}

BOOL CSQLiteDBProvider::IsConnectionOpened() const noexcept
{
	return _GetDBHandle() != NULL;
}

CString CSQLiteDBProvider::GetDBFileName() const noexcept
{
	return m_strDBFileName;
}

CString CSQLiteDBProvider::GetDBFilePath() const noexcept
{
	return m_strDBFilePath;
}

BOOL CSQLiteDBProvider::_InitStorage() noexcept
{
	BOOL bInit = FALSE;
	try
	{
		m_queryStorage.Init( m_pDBHandle );
		VERIFY( ExecuteQuery( BaseSQLQueryID::BSQ_INIT_SHAPES_TABLE ) );
		VERIFY( ExecuteQuery( BaseSQLQueryID::BSQ_INIT_POINTS_TABLE ) );

		VERIFY( ExecuteQuery( BaseSQLQueryID::BSQ_SOME_SELECT ) );

		bInit = TRUE;
	}
	catch( const CSQLiteQueryException& e )
	{
		AfxMessageBox( e.GetMessage() );
		return bInit;
	}
	catch( const CInvalidArgumentException& )
	{
		return bInit;
	}
	return bInit;
}

void CSQLiteDBProvider::_SetDBHandle( sqlite3* const pDBHandle ) noexcept
{
	m_pDBHandle = pDBHandle;
}

sqlite3* CSQLiteDBProvider::_GetDBHandle() const noexcept
{
	return m_pDBHandle;
}

CStringA CSQLiteDBProvider::_WideToANSI( const CStringW& str ) noexcept
{
	CStringA strA = CW2A( str );
	return strA;
}

CString CSQLiteDBProvider::_ANSIToWide( const CStringA& str ) noexcept
{
	CString strW = CA2W( str );
	return strW;
}

BOOL CSQLiteDBProvider::_CheckDir( const CString& strPath ) noexcept
{
	const DWORD dwAttributes = GetFileAttributes( strPath );

	if( dwAttributes == INVALID_FILE_ATTRIBUTES )
		return FALSE;
	if( !( dwAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
		return FALSE;

	return TRUE;
}