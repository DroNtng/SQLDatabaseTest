#pragma once

class CMessageException
{
public:
	explicit CMessageException( const CString& strMessage );
	virtual ~CMessageException();
	CString GetMessage() const;
protected:
	CString m_strMessage;
};
//------------------------------------
class CPathException : public CMessageException
{
public:
	explicit CPathException( const CString& strMessage );
	virtual ~CPathException();
};
//------------------------------------
class CDatabaseException : public CMessageException
{
public:
	explicit CDatabaseException( const CString& strMessage );
	virtual ~CDatabaseException();
};
//------------------------------------
class CDBConnectionException : public CDatabaseException
{
public:
	CDBConnectionException( const CString& strMessage );
	virtual ~CDBConnectionException();
};
//------------------------------------
class CQueryException : public CMessageException
{
public:
	CQueryException( const CString& strMessage );
	virtual ~CQueryException();
};
//------------------------------------
class CInvalidArgumentException
{};
//------------------------------------
class COutOfRangeException
{};
//------------------------------------
class CBadCastException
{};