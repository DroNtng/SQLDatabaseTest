#include "stdafx.h"
#include "DBException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMessageException::CMessageException( const CString& strMessage )
	:m_strMessage( strMessage )
{}

CMessageException::~CMessageException()
{}

CString CMessageException::GetMessage() const
{
	return m_strMessage;
}
//---------------------------------------
CPathException::CPathException( const CString& strMessage )
	: CMessageException( _T("Path exception. ") )
{
	m_strMessage += strMessage;
}

CPathException::~CPathException()
{}
//---------------------------------------
CDatabaseException::CDatabaseException( const CString& strMessage )
	: CMessageException( _T("Database exception. ") )
{
	m_strMessage += strMessage;
}

CDatabaseException::~CDatabaseException()
{}
//---------------------------------------
CDBConnectionException::CDBConnectionException( const CString& strMessage )
	:CDatabaseException( _T("Connection error. ") )
{
	m_strMessage += strMessage;
}

CDBConnectionException::~CDBConnectionException()
{}
//---------------------------------------
CQueryException::CQueryException( const CString& strMessage )
	: CMessageException( _T("Query exception. ") )
{
	m_strMessage += strMessage;
}

CQueryException::~CQueryException()
{}