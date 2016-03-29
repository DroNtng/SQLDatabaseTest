#pragma once
#include "DBException.h"


class CSQLiteQueryException: public CQueryException
{
public:
	CSQLiteQueryException( const CString& strDesc );
	virtual ~CSQLiteQueryException();
};
//-------------------------------
class CSQLiteEmptyQueryException: public CSQLiteQueryException
{
public:
	CSQLiteEmptyQueryException();
	virtual ~CSQLiteEmptyQueryException();
};
//-------------------------------
class CSQLiteQueryCompilationException: public CSQLiteQueryException
{
public:
	explicit CSQLiteQueryCompilationException( const CString& strQuery );
	virtual ~CSQLiteQueryCompilationException();
};
//-------------------------------