#include "stdafx.h"
#include "RegistryWorker.h"
#include "winreg.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Utils_impl;

CRegistryWorker::CRegistryWorker( const CString& strAppName )
	:m_hCurUser( NULL )
	,m_hSoftware( NULL )
	,m_hApp( NULL )
{
	VERIFY( _SetAppName( strAppName ) );
}
//----------------------------------------------------
CRegistryWorker::~CRegistryWorker()
{
	_CloseRegKeys();
}
//----------------------------------------------------
inline CString CRegistryWorker::GetAppName() const
{
	return m_strAppName;
}
//----------------------------------------------------
BOOL CRegistryWorker::SaveAppParameter( const CString& strParamName, const CString& strData )
{
	if( strParamName.IsEmpty() || strData.IsEmpty() )
		return TRUE;

	if( !_PrepareSaveLoad() )
		return FALSE;

	if( ERROR_SUCCESS !=
		RegSetKeyValue( _GetAppKey(), NULL, strParamName, REG_SZ, strData, sizeof( strData[0] ) * (strData.GetLength() + 1) ) )
		return FALSE;
	return TRUE;
}
//----------------------------------------------------
BOOL CRegistryWorker::LoadAppParameter( const CString& strParamName, CString* const pstrData )
{
	if( pstrData == NULL || strParamName.IsEmpty() )
		return FALSE;
	ASSERT_POINTER( pstrData, CString );

	if( !_PrepareSaveLoad() )
		return FALSE;

	TCHAR buff [ MAX_PATH ];
	ZeroMemory( buff, sizeof( buff[0] ) * _countof( buff ) );

	DWORD dwDataType = REG_SZ;
	DWORD dwBufSize = sizeof( buff[0] ) * _countof( buff );
	if( ERROR_SUCCESS != RegGetValue( _GetAppKey(), NULL, strParamName, RRF_RT_REG_SZ, &dwDataType, buff, &dwBufSize ) )
	{
		*pstrData = _T("");
		return FALSE;
	}
	*pstrData = CString( buff );
	return TRUE;
}
//----------------------------------------------------
BOOL CRegistryWorker::SaveAppParameter( const CString& strParamName, const DWORD& dwData )
{
	if( strParamName.IsEmpty() )
		return FALSE;

	if( !_PrepareSaveLoad() )
		return FALSE;

	if( ERROR_SUCCESS !=
		RegSetKeyValue( _GetAppKey(), NULL, strParamName, REG_DWORD, &dwData, sizeof( DWORD ) ) )
		return FALSE;
	return TRUE;
}
//----------------------------------------------------
BOOL CRegistryWorker::LoadAppParameter( const CString& strParamName, DWORD* const pdwData )
{
	if( pdwData == NULL || strParamName.IsEmpty() )
		return FALSE;
	ASSERT_POINTER( pdwData, DWORD );

	if( !_PrepareSaveLoad() )
		return FALSE;

	DWORD dwDataType = REG_DWORD;
	DWORD dwBufSize = sizeof( DWORD );
	if( ERROR_SUCCESS != RegGetValue( _GetAppKey(), NULL, strParamName, RRF_RT_DWORD, &dwDataType, pdwData, &dwBufSize ) )
		return FALSE;
	return TRUE;
}
//----------------------------------------------------
BOOL CRegistryWorker::_CreateOpenRegKeys()
{
	if( !_OpenCurUserKey() )
		return FALSE;

	if( !_CreateOpenSoftwareKey( _GetCurUserKey() ) )
		return FALSE;

	if( !_CreateOpenAppKey( _GetSoftwareKey() ) )
		return FALSE;

	return TRUE;
}
//----------------------------------------------------
BOOL CRegistryWorker::_CloseRegKeys()
{
	BOOL bCloseResult = TRUE;
	if( _GetAppKey() && !_CloseAppKey() )
	{
		ASSERT( FALSE );
		bCloseResult = FALSE;
	}
	
	if( _GetSoftwareKey() && !_CloseSoftwareKey() )
	{
		ASSERT( FALSE );
		bCloseResult = FALSE;
	}
	
	if( _GetAppKey() && !_CloseAppKey() )
	{
		ASSERT( FALSE );
		bCloseResult = FALSE;
	}
	return bCloseResult;
}
//----------------------------------------------------
BOOL CRegistryWorker::_OpenCurUserKey()
{
	if( _GetCurUserKey() )
		_CloseRegKeys();

	if( ERROR_SUCCESS != RegOpenCurrentUser( KEY_WRITE, &m_hCurUser ) )
		return FALSE;
	
	return TRUE;
}
//----------------------------------------------------
BOOL CRegistryWorker::_CloseCurUserKey()
{
	return _CloseKey( &m_hCurUser );
}
//----------------------------------------------------
BOOL CRegistryWorker::_CreateOpenSoftwareKey(const HKEY& hParent)
{
	if( _GetSoftwareKey() )
	{
		VERIFY( _CloseAppKey() );
		VERIFY( _CloseSoftwareKey() );
	}

	if( ERROR_SUCCESS != RegCreateKey( hParent, _T("Software"), &m_hSoftware ) )
		return FALSE;
	
	return TRUE;
}
//----------------------------------------------------
BOOL CRegistryWorker::_CloseSoftwareKey()
{
	return _CloseKey( &m_hSoftware );
}
//----------------------------------------------------
BOOL CRegistryWorker::_CreateOpenAppKey(const HKEY& hParent)
{
	if( _GetAppKey() )
		VERIFY( _CloseAppKey() );

	if( ERROR_SUCCESS != RegCreateKey( hParent, GetAppName(), &m_hApp ) )
		return FALSE;

	return TRUE;
}
//----------------------------------------------------
BOOL CRegistryWorker::_CloseAppKey()
{
	return _CloseKey( &m_hApp );
}
//----------------------------------------------------
BOOL CRegistryWorker::_CloseKey( PHKEY const pKey )
{
	ASSERT( pKey != NULL );
	if( *pKey != NULL )
	{
		const BOOL bClose = ( ERROR_SUCCESS == RegCloseKey( *pKey ) );
		if( bClose )
			*pKey = NULL;
		return bClose;
	}
	return TRUE;
}
//----------------------------------------------------
inline HKEY CRegistryWorker::_GetCurUserKey() const
{
	return m_hCurUser;
}
//----------------------------------------------------
inline HKEY CRegistryWorker::_GetSoftwareKey() const
{
	return m_hSoftware;
}
//----------------------------------------------------
inline HKEY CRegistryWorker::_GetAppKey() const
{
	return m_hApp;
}
//----------------------------------------------------
BOOL CRegistryWorker::_SetAppName( const CString& strAppName )
{
	if( strAppName.IsEmpty() )
	{
		m_strAppName.LoadStringW( IDS_UNKNOWN_APP );
		return FALSE;
	}
	m_strAppName = strAppName;
	return TRUE;
}
//----------------------------------------------------
inline BOOL CRegistryWorker::_IsReadySaveLoad() const
{
	return _GetAppKey() && _GetSoftwareKey() && _GetCurUserKey();
}
//----------------------------------------------------
BOOL CRegistryWorker::_PrepareSaveLoad()
{
	if( !_IsReadySaveLoad() )
	{
		VERIFY( _CreateOpenRegKeys() );
	}
	return _IsReadySaveLoad();
}
//----------------------------------------------------
CRegistryWorker::CRegistryWorker( const CRegistryWorker& other )
{
	UNREFERENCED_PARAMETER( other );
}
//----------------------------------------------------
CRegistryWorker& CRegistryWorker::operator = ( const CRegistryWorker& other )
{
	UNREFERENCED_PARAMETER( other );
	return *this;
}