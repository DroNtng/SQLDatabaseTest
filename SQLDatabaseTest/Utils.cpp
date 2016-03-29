#include "stdafx.h"
#include "Utils.h"
#include "RegistryWorker.h"
#include "WorkingDirectory.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString Utils_interface::GetAppName()
{
	static CString strAppName;
	
	if( strAppName.IsEmpty() )
		strAppName.LoadStringW( IDS_APP_NAME );

	return strAppName;
}

CString Utils_interface::LoadStringFromRes( const UINT & nID )
{
	CString str;
	const BOOL bLoad = str.LoadStringW( nID );
	ASSERT( bLoad );
	if( !bLoad )
		str = _T("");
	
	return str;
}

BOOL Utils_interface::SaveAppParameter( const CString& strParamName, const CString& strData )
{
	using namespace Utils_impl;
	return GetRegistryWorker()->SaveAppParameter( strParamName, strData );
}

BOOL Utils_interface::LoadAppParameter( const CString& strParamName, CString* const pstrData )
{
	using namespace Utils_impl;
	return GetRegistryWorker()->LoadAppParameter( strParamName, pstrData );
}

BOOL Utils_interface::SaveAppParameter( const CString& strParamName, const DWORD& dwData )
{
	using namespace Utils_impl;
	return GetRegistryWorker()->SaveAppParameter( strParamName, dwData );
}

BOOL Utils_interface::LoadAppParameter( const CString& strParamName, DWORD* const pdwData )
{
	using namespace Utils_impl;
	return GetRegistryWorker()->LoadAppParameter( strParamName, pdwData );
}

CString Utils_interface::GetWorkingDirectory()
{
	using namespace Utils_impl;
	static CWorkingDirectory workDir;
	return workDir.GetDirectory();
}

Utils_impl::CRegistryWorker* Utils_impl::GetRegistryWorker()
{
	static CRegistryWorker worker( GetAppName() );
	return &worker;
}