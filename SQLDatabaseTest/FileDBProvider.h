#pragma once
#include "DBException.h"
#include "BaseSQLQueryID.h"

class IDatabaseDataProvider
{
public:
	virtual BOOL OpenConnection( const CString& strDBPath, const CString& strDBFileName, const CString& strConnection =_T("") )
		throw ( CPathException, CDatabaseException ) = 0;
	virtual BOOL CloseConnection() noexcept = 0;
	virtual BOOL IsConnectionOpened() const noexcept = 0;
	virtual BOOL ExecuteQuery( const CString& strSQLCommand ) throw ( CQueryException ) = 0;
	virtual BOOL ExecuteQuery( const BaseSQLQueryID& ID ) throw ( CQueryException ) = 0;
	virtual ~IDatabaseDataProvider(){};
};

class IFileDBProvider : public IDatabaseDataProvider
{
public:
	virtual CString GetDBFileName() const noexcept  = 0;
	virtual CString GetDBFilePath() const noexcept  = 0;
	virtual ~IFileDBProvider(){};
};

