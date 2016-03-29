#include "stdafx.h"
#include "WorkingDirectory.h"
#include "Utils.h"
#include "winreg.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Utils_impl;

CWorkingDirectory::CWorkingDirectory():
	 m_bInit( FALSE )
	,m_strWorkDir( _T("") )
{
	VERIFY( _Init() );
}
//----------------------------------------------------
CWorkingDirectory::~CWorkingDirectory()
{
}
//----------------------------------------------------
CString CWorkingDirectory::GetDirectory() const
{
	ASSERT( _IsInit() );
	return m_strWorkDir;
}
//----------------------------------------------------
CString CWorkingDirectory::_WorkDirParamName()
{
	return _T("WorkDir");
}
//----------------------------------------------------
BOOL CWorkingDirectory::_Init()
{
	CString strWorkDir;
	GetRegistryWorker()->LoadAppParameter( _WorkDirParamName(), &strWorkDir );
	
	if( !_SetDirectory( strWorkDir ) )
	{
		while( !_SelectWorkDirFileDialog() )
			;
		VERIFY( GetRegistryWorker()->SaveAppParameter( _WorkDirParamName(), GetDirectory() ) );
	}
	
	return TRUE;
}
//----------------------------------------------------
BOOL CWorkingDirectory::_SelectWorkDirFileDialog()
{
	// sets the root of tree folder dialog to execute directory
	//PIDLIST_ABSOLUTE pExeFolderID = NULL;
	//SFGAOF stSFGAOFIn = 0;
	//SFGAOF stSFGAOFOut = 0;
	//CString strExecDir;
	//_GetExecutionDir( &strExecDir );
	//if( !strExecDir.IsEmpty() )
		//SHParseDisplayName( strExecDir, NULL, &pExeFolderID, stSFGAOFIn, &stSFGAOFOut );
	// even if SHParseDisplayName fails, pExeFolderID is set to NULL - the default browse path for SHBrowseForFolder
	
	TCHAR folderName[ MAX_PATH ];
	
	BROWSEINFO bi;
	bi.hwndOwner = 0;
	bi.pidlRoot = NULL/*pExeFolderID*/;
	bi.pszDisplayName = folderName;
	CString strTitle = LoadStringFromRes( IDS_SELECT_WORK_DIR_DLG_TITLE );
	bi.lpszTitle = strTitle;
	bi.ulFlags = 0;
	bi.lpfn = 0;

	LPITEMIDLIST pidl = SHBrowseForFolder( &bi );
	if ( pidl == NULL )
		return FALSE;
	if ( !SHGetPathFromIDList( pidl, folderName ) )
		return FALSE;
	if( !_SetDirectory( folderName ) )
		return FALSE;
	return TRUE;
}
//----------------------------------------------------
BOOL CWorkingDirectory::_IsInit() const
{
	return m_bInit;
}
//----------------------------------------------------
BOOL CWorkingDirectory::_ValidateDir( const CString& strDir ) const
{
	if( strDir.IsEmpty() )
		return FALSE;

	if( !PathFileExists( strDir ) )
		return FALSE;

	if( !_CanAccessFolder( strDir, GENERIC_READ | GENERIC_WRITE ) )
		return FALSE;

	return TRUE;
}
//----------------------------------------------------
BOOL CWorkingDirectory::_CanAccessFolder( const CString& folderName, DWORD genericAccessRights ) const
{
	BOOL bRet = FALSE;
	DWORD length = 0;

	if (!::GetFileSecurity( folderName, OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION, NULL, NULL, &length ) && 
		ERROR_INSUFFICIENT_BUFFER == ::GetLastError() )
	{
		PSECURITY_DESCRIPTOR security = static_cast< PSECURITY_DESCRIPTOR >( ::malloc( length ) );
		if (security && ::GetFileSecurity( folderName, OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION
			| DACL_SECURITY_INFORMATION, security, length, &length ))
		{
			HANDLE hToken = NULL;
			if (::OpenProcessToken( ::GetCurrentProcess(), TOKEN_IMPERSONATE | TOKEN_QUERY | 
				TOKEN_DUPLICATE | STANDARD_RIGHTS_READ, &hToken ))
			{
				HANDLE hImpersonatedToken = NULL;
				if (::DuplicateToken( hToken, SecurityImpersonation, &hImpersonatedToken ))
				{
					GENERIC_MAPPING mapping = { 0xFFFFFFFF };
					PRIVILEGE_SET privileges = { 0 };
					DWORD grantedAccess = 0, privilegesLength = sizeof( privileges );
					BOOL result = FALSE;

					mapping.GenericRead = FILE_GENERIC_READ;
					mapping.GenericWrite = FILE_GENERIC_WRITE;
					mapping.GenericExecute = FILE_GENERIC_EXECUTE;
					mapping.GenericAll = FILE_ALL_ACCESS;

					::MapGenericMask( &genericAccessRights, &mapping );
					if (::AccessCheck( security, hImpersonatedToken, genericAccessRights, 
						&mapping, &privileges, &privilegesLength, &grantedAccess, &result ))
					{
						bRet = (result == TRUE);
					}
					::CloseHandle( hImpersonatedToken );
				}
				::CloseHandle( hToken );
			}
			::free( security );
		}
	}

	return bRet;
}
//----------------------------------------------------
BOOL CWorkingDirectory::_SetDirectory( const CString& strDir )
{
	if( _ValidateDir( strDir ) )
	{
		m_strWorkDir = strDir;
		m_bInit = TRUE;
		return TRUE;
	}
	return FALSE;
}
//----------------------------------------------------
CWorkingDirectory::CWorkingDirectory( const CWorkingDirectory& other )
{
	UNREFERENCED_PARAMETER( other );
}
//----------------------------------------------------
CWorkingDirectory& CWorkingDirectory::operator = ( const CWorkingDirectory& other )
{
	UNREFERENCED_PARAMETER( other );
	return *this;
}
//----------------------------------------------------
BOOL CWorkingDirectory::_GetExecutionDir( CString* const pExeDir )
{
	ASSERT_POINTER( pExeDir, CString );
	if( pExeDir == NULL )
		return FALSE;

	*pExeDir = _T("");

	CString strExeDir;
	strExeDir.GetBufferSetLength( MAX_PATH + 1 );
	const DWORD dwModNameRes = GetModuleFileName( NULL, strExeDir.GetBuffer(), strExeDir.GetLength() );

	ASSERT( dwModNameRes != 0 );
	if( dwModNameRes == 0 )
		return FALSE;
	
	if( PathRemoveFileSpec( strExeDir.GetBuffer() ) )
	{
		*pExeDir = strExeDir;
		return TRUE;
	}
	return FALSE;
}