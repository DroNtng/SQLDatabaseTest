
// DlgProxy.h: header file
//

#pragma once

class CSQLDatabaseTestDlg;


// CSQLDatabaseTestDlgAutoProxy command target

class CSQLDatabaseTestDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CSQLDatabaseTestDlgAutoProxy)

	CSQLDatabaseTestDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CSQLDatabaseTestDlg* m_pDialog;

// Operations
public:

// Overrides
	public:
	virtual void OnFinalRelease();

// Implementation
protected:
	virtual ~CSQLDatabaseTestDlgAutoProxy();

	// Generated message map functions

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSQLDatabaseTestDlgAutoProxy)

	// Generated OLE dispatch map functions

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

