#include "stdafx.h"
#include "SQLiteException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSQLiteQueryException::CSQLiteQueryException( const CString& strDesc )
	:CQueryException( _T("SQLite. ") )
{
	m_strMessage += strDesc;
}

CSQLiteQueryException::~CSQLiteQueryException()
{}
//---------------------------------------------------
CSQLiteEmptyQueryException::CSQLiteEmptyQueryException()
	:CSQLiteQueryException(_T(""))
{
	m_strMessage += _T("<Empty query compile> ");
};

CSQLiteEmptyQueryException::~CSQLiteEmptyQueryException()
{}
//-------------------------------
CSQLiteQueryCompilationException::CSQLiteQueryCompilationException( const CString& strQuery )
	:CSQLiteQueryException(_T(""))
{
	m_strMessage += _T("Unable to compile query: ") + strQuery;
}

CSQLiteQueryCompilationException::~CSQLiteQueryCompilationException()
{}
//-------------------------------