#pragma once
#include <vector>
#include "SQLiteException.h"

struct sqlite3;
struct sqlite3_stmt;
class CCompiledSQLiteQuery;
enum BaseSQLQueryID;

class CSQLiteQueryStorage
{
public:

	CSQLiteQueryStorage();
	virtual ~CSQLiteQueryStorage();

	typedef size_t UserSQLQueryID;

	//init the query storage with the specified DBHandle. Query IDs gets from SQLiteQueryID enum
	//and IDs, returned from AddQuery() function.
	void	Init( sqlite3* const pDBHandle ) throw ( CSQLiteQueryException, CInvalidArgumentException );
	BOOL	IsInit() const noexcept;
	BOOL	CompileAll() throw( CSQLiteQueryException );
	void	UnInit() noexcept;

	//adds user-defined query into storage. Set *pID to call the query. If calls before Init method, added queries will be compiled
	//with Init() function. Otherwise, AddQuery() try to compile the query, if fail throws CSQLiteQueryException
	void	AddQuery( UserSQLQueryID* pID, const CString& strSQLQuery ) throw( CSQLiteQueryException, CInvalidArgumentException );

	//returns compiled byte-code of the query with specified ID from SQLiteQueryID enum
	//if there is no such compiled query in storage, throws CSQLiteQueryException
	sqlite3_stmt*	GetQuery( const BaseSQLQueryID& ID ) throw( CSQLiteQueryException, COutOfRangeException );

	//returns compiled byte-code of the query with specified ID, returned from AddQuery() function.
	//if there is no such compiled query in storage, throws CQueryCompilationException
	sqlite3_stmt*	GetQuery( const UserSQLQueryID& ID ) throw( CSQLiteQueryException, COutOfRangeException );

protected:
	//replaces default query(from SQLiteQueryID enum) with specified ID with query from strSQLQuery.
	//If calls before Init method, added query will be compiled
	//with Init() function. Otherwise, _SetQueryAt() try to compile the query, if fail throws CSQLiteQueryException
	virtual BOOL _SetQueryAt( const BaseSQLQueryID& ID, const CString& strSQLQuery ) throw ( CSQLiteQueryException, COutOfRangeException );
	
protected:
	void	_SetDBHandle( sqlite3* const pDBHandle ) noexcept;

	typedef std::vector<CCompiledSQLiteQuery*>  CCompiledQueryArray;
	void	_CompileQueryArray( CCompiledQueryArray& queryArr ) throw( CSQLiteQueryException );
	
	void	_ClearAll() noexcept;
	void	_Clear( CCompiledQueryArray& arr ) noexcept;
	void	_DeleteQuery( CCompiledSQLiteQuery*& pQuery ) noexcept;
	// converts query ID to storage-index
	static size_t			_IDtoIdx( const BaseSQLQueryID& ID ) noexcept;
	//converts query index to ID
	static BaseSQLQueryID	_IdxToID( const size_t& nIdx ) throw( CBadCastException );

protected:

	sqlite3*	m_pDBHandle;
	
	CCompiledQueryArray m_vecBase;
	CCompiledQueryArray m_vecUser;
};