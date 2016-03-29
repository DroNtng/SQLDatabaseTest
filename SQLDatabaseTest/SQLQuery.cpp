#include "stdafx.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSQLQuery::CSQLQuery( const CString& strSQLQuery )
	:m_strQuery( strSQLQuery )
{
	ASSERT( !m_strQuery.IsEmpty() );
}

CSQLQuery::~CSQLQuery()
{
}

const CString& CSQLQuery::GetString() const noexcept
{
	return m_strQuery;
}