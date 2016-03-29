#pragma once
#include "FileDBProvider.h"
#include "SQLiteQueryStorage.h"

struct sqlite3;
class CSQLiteDBProvider: public IFileDBProvider
{
public:
	CSQLiteDBProvider();
	virtual ~CSQLiteDBProvider();

	//IDatabaseDataProvider implementation
	BOOL	OpenConnection( const CString& strDBPath, const CString& strDBFileName, const CString& strConnection )
		throw ( CPathException, CDatabaseException );
	BOOL	CloseConnection() noexcept;
	BOOL	IsConnectionOpened() const noexcept;
	BOOL	ExecuteQuery( const CString& strSQLCommand ) throw ( CQueryException );
	BOOL	ExecuteQuery( const BaseSQLQueryID& ID ) throw ( CQueryException );

	//IFileDBProvider implementation
	CString GetDBFileName() const noexcept;
	CString GetDBFilePath() const noexcept;

private:

	CSQLiteDBProvider( const CSQLiteDBProvider& other );
	CSQLiteDBProvider& operator = ( const CSQLiteDBProvider& other );

	void			_SetDBHandle( sqlite3* const pDBHandle ) noexcept;
	sqlite3*		_GetDBHandle() const noexcept;

	static CStringA _WideToANSI( const CStringW& str ) noexcept;
	static CString  _ANSIToWide( const CStringA& str ) noexcept;

	BOOL			_CheckDir( const CString& strPath ) noexcept;
	BOOL			_InitStorage() noexcept;
private:
	sqlite3*		m_pDBHandle;

	CString			m_strDBFileName;
	CString			m_strDBFilePath;

	CSQLiteQueryStorage m_queryStorage;
};

