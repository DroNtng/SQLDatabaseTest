#pragma once

#include "windows.h"
#include "sqlite3.h"
#include "RegistryWorker.h"

namespace Utils_impl
{

	class CWorkingDirectory
	{
	public:
		CWorkingDirectory();
		~CWorkingDirectory();

		CString GetDirectory() const;

	private:
		CWorkingDirectory( const CWorkingDirectory& other );
		CWorkingDirectory& operator = ( const CWorkingDirectory& other );
	
	private:
		//
		BOOL _Init();
		//return TRUE if m_strWorkDir contains a valid directory
		BOOL _IsInit() const;
		BOOL _SetDirectory( const CString& strDir );
		//cheks dir for existing and write-access rights
		BOOL _ValidateDir( const CString& strDir ) const;
		BOOL _CanAccessFolder( const CString& folderName, DWORD genericAccessRights ) const;
		//call CFileDialog to select working directory
		BOOL _SelectWorkDirFileDialog();

		static BOOL _GetExecutionDir( CString* const pExeDir );
		static CString _WorkDirParamName();
	
	private:
		CString m_strWorkDir;
		BOOL	m_bInit;
	};

};
