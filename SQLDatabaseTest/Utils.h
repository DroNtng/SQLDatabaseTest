#pragma once

namespace Utils_interface
{
	//returns Application name
	CString GetAppName();
	//returns CString object, filled with specified nID - resource
	CString LoadStringFromRes( const UINT & nID );

	BOOL SaveAppParameter( const CString& strParamName, const CString& strData );
	BOOL LoadAppParameter( const CString& strParamName, CString* const pstrData );

	BOOL SaveAppParameter( const CString& strParamName, const DWORD& dwData );
	BOOL LoadAppParameter( const CString& strParamName, DWORD* const pdwData );

	CString GetWorkingDirectory();
};

namespace Utils
{
	using namespace Utils_interface;
};

namespace Utils_impl
{
	using namespace Utils_interface;

	class CRegistryWorker;
	CRegistryWorker* GetRegistryWorker();
};