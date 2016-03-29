#include "stdafx.h"
#include "CompiledSQLiteQuery.h"
#include "sqlite3.h"
#include "SQLiteException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCompiledSQLiteQuery::CCompiledSQLiteQuery( const CString& strSQLQuery )
	:CSQLQuery( strSQLQuery )
	,m_pCompiledStatement( NULL )
{
}

CCompiledSQLiteQuery::~CCompiledSQLiteQuery()
{
	VERIFY( _Finalize() );
}

void CCompiledSQLiteQuery::Compile( sqlite3* const pDBHandle ) throw ( CSQLiteQueryException, CInvalidArgumentException )
{
	if( pDBHandle == NULL ) throw CInvalidArgumentException();
	if( GetString().IsEmpty() ) throw CSQLiteEmptyQueryException();
	
	VERIFY( _Finalize() );

	const TCHAR* pTail = NULL;
	const int nCompileResult = sqlite3_prepare16_v2( pDBHandle, GetString(),
		GetString().GetLength() * sizeof( GetString().GetAt( 0 ) ), &m_pCompiledStatement, (const void**) &pTail );

	if( pTail && pTail[0] != _T('\0') )
	{
		TRACE( _T("\nUNCOMPILED QUERY PART:\n%s"), pTail );
		_ASSERTE( FALSE && _T("not entire statement have been compiled(see Output for uncompiled part). Please split statements by one in a query;") );
	}

	ASSERT( nCompileResult == SQLITE_OK );
	if( nCompileResult != SQLITE_OK )
	{
		_Finalize();
		throw CSQLiteQueryCompilationException( GetString() );
	}
}

sqlite3_stmt* CCompiledSQLiteQuery::GetStatement() noexcept
{
	return m_pCompiledStatement;
}

BOOL CCompiledSQLiteQuery::_Finalize() noexcept
{
	if( m_pCompiledStatement )
	{
		const int nFinResult = sqlite3_finalize( m_pCompiledStatement );
		m_pCompiledStatement = NULL;
		ASSERT( nFinResult == SQLITE_OK );
		return nFinResult == SQLITE_OK;
	}
	return TRUE;
}

CCompiledSQLiteQuery::CCompiledSQLiteQuery( const CCompiledSQLiteQuery& other )
	:CSQLQuery( other.GetString() )
	,m_pCompiledStatement( NULL )
{
}

CCompiledSQLiteQuery& CCompiledSQLiteQuery::operator = ( const CCompiledSQLiteQuery& other )
{
	UNREFERENCED_PARAMETER( other );
	return *this;
}