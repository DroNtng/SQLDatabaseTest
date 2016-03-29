#pragma once

namespace Utils_impl
{

	class CRegistryWorker
	{
		// CRegistryWorker class represents a helper class to work with Windows registry.
		// Main methods for saving load application parameters in registry are [Save/Load]AppParameter
		// This class save App params in regisrty by path "HKCU\\Software\<YourAppName>"
		// <YourAppName> - is the constructor parameter(must not be empty string)
	public:
		CRegistryWorker( const CString& strAppName );
		~CRegistryWorker();
	private:
		CRegistryWorker( const CRegistryWorker& other );
		CRegistryWorker& operator = ( const CRegistryWorker& other );

	public:
		CString GetAppName() const;

		// writes to registry param named strParamName with strData value
		BOOL SaveAppParameter( const CString& strParamName, const CString& strData );
		// reads from registry param named strParamName to pstrData value
		BOOL LoadAppParameter( const CString& strParamName, CString* const pstrData );
	
		// writes to registry param named strParamName with dwData value
		BOOL SaveAppParameter( const CString& strParamName, const DWORD& dwData );
		// reads from registry param named strParamName to pdwData value
		BOOL LoadAppParameter( const CString& strParamName, DWORD* const pdwData );
	
	private:
		BOOL _SetAppName( const CString& strAppName );

		BOOL _CreateOpenRegKeys();
		BOOL _CloseRegKeys();

		BOOL _OpenCurUserKey();
		BOOL _CloseCurUserKey();

		BOOL _CreateOpenSoftwareKey( const HKEY& hParent );
		BOOL _CloseSoftwareKey();

		BOOL _CreateOpenAppKey( const HKEY& hParent );
		BOOL _CloseAppKey();

		HKEY _GetCurUserKey() const;
		HKEY _GetSoftwareKey() const;
		HKEY _GetAppKey() const;

		BOOL _CloseKey( PHKEY const pKey );

		// return TRUE if all registry keys are opened, otherwise return FALSE
		BOOL _IsReadySaveLoad() const;
		// if needed opens all regKeys for save/load operations
		BOOL _PrepareSaveLoad();

	private:
		CString m_strAppName;
		HKEY	m_hCurUser;
		HKEY	m_hSoftware;
		HKEY	m_hApp;
	};

};

