#include "stdafx.h"
#include "SQLiteQueryStorage.h"
#include "CompiledSQLiteQuery.h"
#include "BaseSQLQueryID.h"
#include "sqlite3.h"
#include <algorithm>
#include <functional>
#include "SQLiteException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//----------------------------------------
CSQLiteQueryStorage::CSQLiteQueryStorage()
	:m_pDBHandle( NULL )
	,m_vecBase( _IDtoIdx( BaseSQLQueryID::BSQ_COUNT ) )
{
}
//----------------------------------------
CSQLiteQueryStorage::~CSQLiteQueryStorage()
{
	UnInit();
}
//----------------------------------------
void	CSQLiteQueryStorage::Init( sqlite3* const pDBHandle ) throw ( CSQLiteQueryException, CInvalidArgumentException )
{
	if( pDBHandle == NULL )
		return throw CInvalidArgumentException();

	if( IsInit() )
		UnInit();
	
	//
	CString strQuery = _T("CREATE TABLE IF NOT EXISTS \'SHAPES\' (\'ID\' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE);");
	_SetQueryAt( BaseSQLQueryID::BSQ_INIT_SHAPES_TABLE, strQuery );
	
	strQuery = _T("CREATE TABLE IF NOT EXISTS \'POINTS\' (\'ID\' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,\
					\'X\' INTEGER NOT NULL,\
					\'Y\' INTEGER NOT NULL UNIQUE,\
					\'SHAPE_ID\' INTEGER NOT NULL REFERENCES \'SHAPES\' (\'ID\'))");
	_SetQueryAt( BaseSQLQueryID::BSQ_INIT_POINTS_TABLE, strQuery );

	strQuery = _T("INSERT INTO \'SHAPES\' VALUES(NULL)");
	_SetQueryAt( BaseSQLQueryID::BSQ_INSERT_SHAPE, strQuery );
	
	strQuery = _T("SELECT * FROM \'SHAPES\'");
	_SetQueryAt( BaseSQLQueryID::BSQ_SOME_SELECT, strQuery );
	//todo...

	_SetDBHandle( pDBHandle );
}
//----------------------------------------
BOOL	CSQLiteQueryStorage::IsInit() const noexcept
{
	return m_pDBHandle != NULL;
}
//----------------------------------------
void	CSQLiteQueryStorage::UnInit() noexcept
{
	_ClearAll();
}
//----------------------------------------
void	CSQLiteQueryStorage::AddQuery( UserSQLQueryID* pID, const CString& strSQLQuery ) throw( CSQLiteQueryException, CInvalidArgumentException )
{
	ASSERT( pID != NULL );
	if( pID == NULL ) throw CInvalidArgumentException();

	CCompiledSQLiteQuery* pNewQuery = new CCompiledSQLiteQuery( strSQLQuery );
	m_vecUser.push_back( pNewQuery );

	if( IsInit() )
	{
		pNewQuery->Compile( m_pDBHandle );
	}

	*pID = m_vecUser.size() - 1;
}
//----------------------------------------
sqlite3_stmt*	CSQLiteQueryStorage::GetQuery( const BaseSQLQueryID& ID ) throw( CSQLiteQueryException, COutOfRangeException )
{
	const size_t nIdx = _IDtoIdx( ID );
	ASSERT( nIdx >= 0 && nIdx < m_vecBase.size() );
	if( !(nIdx >= 0 && nIdx < m_vecBase.size()) )
		throw COutOfRangeException();
	
	CCompiledSQLiteQuery* pQuery = m_vecBase.at( nIdx );
	if( pQuery == NULL ) throw CSQLiteEmptyQueryException();

	if( pQuery->GetStatement() == NULL && IsInit() )
		pQuery->Compile( m_pDBHandle );

	return pQuery->GetStatement();
}
//----------------------------------------
sqlite3_stmt*	CSQLiteQueryStorage::GetQuery( const UserSQLQueryID& ID ) throw( CSQLiteQueryException, COutOfRangeException )
{
	ASSERT( ID >= 0 && ID < m_vecBase.size() );
	if( !(ID >= 0 && ID < m_vecBase.size()) )
		throw COutOfRangeException();

	CCompiledSQLiteQuery* pQuery = m_vecBase.at( ID );
	if( pQuery == NULL ) throw CSQLiteEmptyQueryException();
	
	if( pQuery->GetStatement() == NULL && IsInit() )
		pQuery->Compile( m_pDBHandle );

	return pQuery->GetStatement();
}
//----------------------------------------
BOOL	CSQLiteQueryStorage::CompileAll() throw( CSQLiteQueryException )
{
	try
	{
		_CompileQueryArray( m_vecBase );
		_CompileQueryArray( m_vecUser );
	}
	catch( const CSQLiteQueryException& )
	{
		UnInit();
		throw;
	}
	return TRUE;
}
//----------------------------------------
void	CSQLiteQueryStorage::_CompileQueryArray( CCompiledQueryArray& queryArr ) throw( CSQLiteQueryException )
{
	using namespace std;
	for_each( queryArr.begin(), queryArr.end(), bind2nd( mem_fun( &CCompiledSQLiteQuery::Compile ), m_pDBHandle ) );
}
//----------------------------------------
BOOL	CSQLiteQueryStorage::_SetQueryAt( const BaseSQLQueryID& ID, const CString& strSQLQuery ) throw ( CSQLiteQueryException, COutOfRangeException )
{
	const size_t nIdx = _IDtoIdx( ID );
	if( nIdx >= m_vecBase.size() )
		throw COutOfRangeException();

	_DeleteQuery( m_vecBase.at( nIdx ) );

	CCompiledSQLiteQuery* pNewQuery = new CCompiledSQLiteQuery( strSQLQuery );
	ASSERT( pNewQuery );
	m_vecBase.at( nIdx ) = pNewQuery;

	if( IsInit() )
	{
		pNewQuery->Compile( m_pDBHandle );
	}

	return TRUE;
}
//----------------------------------------
void	CSQLiteQueryStorage::_DeleteQuery( CCompiledSQLiteQuery*& pQuery ) noexcept
{
	if( pQuery )
	{
		delete pQuery;
		pQuery = NULL;
	}
}
//----------------------------------------
size_t CSQLiteQueryStorage::_IDtoIdx( const BaseSQLQueryID& ID ) noexcept
{
	return static_cast<size_t>( ID );
}
//----------------------------------------
BaseSQLQueryID CSQLiteQueryStorage::_IdxToID( const size_t& nIdx ) throw( CBadCastException )
{
	const BaseSQLQueryID ID = static_cast<BaseSQLQueryID>( nIdx );
	ASSERT( ID >= BaseSQLQueryID::BSQ_FIRST && ID <= BaseSQLQueryID::BSQ_LAST );
	if( !(ID >= BaseSQLQueryID::BSQ_FIRST && ID <= BaseSQLQueryID::BSQ_LAST) )
		throw CBadCastException();
	return ID;
}
//----------------------------------------
void CSQLiteQueryStorage::_SetDBHandle( sqlite3* const pDBHandle ) noexcept
{
	m_pDBHandle = pDBHandle;
}
//----------------------------------------
void	CSQLiteQueryStorage::_ClearAll() noexcept
{
	_Clear( m_vecUser );
	_Clear( m_vecBase );
	m_pDBHandle = NULL;
}
//----------------------------------------
void CSQLiteQueryStorage::_Clear( CCompiledQueryArray& arr ) noexcept
{
	for( size_t i = 0; i < arr.size(); ++i )
	{
		_DeleteQuery( arr.at( i ) );
	}
	arr.clear();
}
//----------------------------------------