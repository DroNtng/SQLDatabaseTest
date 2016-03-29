#pragma once
#include "SQLQuery.h"
#include "SQLiteException.h"

struct sqlite3;
struct sqlite3_stmt;

class CCompiledSQLiteQuery : public CSQLQuery
{
public:
	
	friend class CSQLiteQueryStorage;

protected:
	CCompiledSQLiteQuery( const CString& strSQLQuery );
	virtual ~CCompiledSQLiteQuery();

	void				Compile( sqlite3* const pDBHandle ) throw ( CSQLiteQueryException, CInvalidArgumentException );
	sqlite3_stmt*		GetStatement() noexcept;

private:
	CCompiledSQLiteQuery( const CCompiledSQLiteQuery& other );
	CCompiledSQLiteQuery& operator = ( const CCompiledSQLiteQuery& other );

protected:
	BOOL			_Finalize() noexcept;

protected:
	sqlite3_stmt*	m_pCompiledStatement;
};