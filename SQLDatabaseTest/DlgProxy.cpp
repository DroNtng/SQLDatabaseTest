
// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "SQLDatabaseTest.h"
#include "DlgProxy.h"
#include "SQLDatabaseTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSQLDatabaseTestDlgAutoProxy

IMPLEMENT_DYNCREATE(CSQLDatabaseTestDlgAutoProxy, CCmdTarget)

CSQLDatabaseTestDlgAutoProxy::CSQLDatabaseTestDlgAutoProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CSQLDatabaseTestDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CSQLDatabaseTestDlg)))
		{
			m_pDialog = reinterpret_cast<CSQLDatabaseTestDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CSQLDatabaseTestDlgAutoProxy::~CSQLDatabaseTestDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CSQLDatabaseTestDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CSQLDatabaseTestDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CSQLDatabaseTestDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// Note: we add support for IID_ISQLDatabaseTest to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {B4464F34-3A49-4307-9259-20740788B214}
static const IID IID_ISQLDatabaseTest =
{ 0xB4464F34, 0x3A49, 0x4307, { 0x92, 0x59, 0x20, 0x74, 0x7, 0x88, 0xB2, 0x14 } };

BEGIN_INTERFACE_MAP(CSQLDatabaseTestDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CSQLDatabaseTestDlgAutoProxy, IID_ISQLDatabaseTest, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {3CB48DAF-C443-4ED3-B317-B6A5AC8A5361}
IMPLEMENT_OLECREATE2(CSQLDatabaseTestDlgAutoProxy, "SQLDatabaseTest.Application", 0x3cb48daf, 0xc443, 0x4ed3, 0xb3, 0x17, 0xb6, 0xa5, 0xac, 0x8a, 0x53, 0x61)


// CSQLDatabaseTestDlgAutoProxy message handlers
