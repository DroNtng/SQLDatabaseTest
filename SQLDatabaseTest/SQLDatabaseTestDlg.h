
// SQLDatabaseTestDlg.h : header file
//

#pragma once

class CSQLDatabaseTestDlgAutoProxy;


// CSQLDatabaseTestDlg dialog
class IFileDBProvider;
class CSQLDatabaseTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSQLDatabaseTestDlg);
	friend class CSQLDatabaseTestDlgAutoProxy;

// Construction
public:
	CSQLDatabaseTestDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CSQLDatabaseTestDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SQLDATABASETEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	CSQLDatabaseTestDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	IFileDBProvider* m_pDBProvider;

	BOOL CanExit();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
