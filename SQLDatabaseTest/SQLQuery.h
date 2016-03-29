#pragma once


class CSQLQuery
{
public:
	CSQLQuery( const CString& strSQLQuery );
	virtual ~CSQLQuery();

	const CString& GetString() const noexcept;

protected:
	CString			m_strQuery;
};

