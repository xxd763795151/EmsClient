#pragma once
#include "PublicDef.h"
#include <vector>
#include "EmsNet.h"
#include "EmsClientMain.h"


#define SHOW_CCF_NUM 3

class CEmsMainWnd : public CWindowWnd, public INotifyUI
{
public:
	CEmsMainWnd();
	~CEmsMainWnd();

	LPCTSTR GetWindowClassName() const;
	UINT GetClassStyle();
	void OnFinalMessage(HWND hWnd);
	void Init();
	void OnPrepare();
	void Notify(TNotifyUI& msg);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnImpotResponse(LPARAM lParam);
	LRESULT OnScanQuestion(WPARAM wParam, LPARAM lParam);
	LRESULT OnAddExamRet(LPARAM lParam);
	LRESULT OnGetExamDescription(LPARAM lParam);
	LRESULT OnGetApplyMsg(LPARAM lParam);
	LRESULT OnGetUserAnswer(LPARAM lParam);
	LRESULT OnGetAnswer(LPARAM lParam);
	LRESULT OnGradeShow(LPARAM lParam);
private:
	void InitOption();
	void QuestionStoreManager();
	void SetOptionNotVisible(UINT nNums);
	void SetOptionImage(BOOL bClick);
	void ShowSubject(char * szData);
	void ShowSubjectToWnd();
	void ShowQuestionToWnd(CControlUI* pSender);
	void InsertQuestion(std::vector<std::wstring> vecQues,CComboBoxUI * pComUI);
	bool ImportQuesToStore();
	bool ScanQues();
	void ScanCCFQuestion(char *szData);
	void ScanCCFLimit(int iBegin, int iOffset, bool bNext = true);
	void GeneratePaper();
	bool ValidateExamTime(SYSTEMTIME sysStartTime, SYSTEMTIME sysEndTime, int iStartHour, int iEndHour, int iStartMin, int iEndMin);
	void PrintApplyMsg();
	void PopUserAnwser();
	void PopAnswer();
	bool ValidateScore(CDuiString strScore);
	void GetNextAnswer(CDuiString strScore);
private:
	bool OnQuestionStoreClick(LPVOID lParam);
	bool OnPaperManagerClick(LPVOID lParam);
	bool OnGradeManageClick(LPVOID lParam);
	bool OnMessageProtectClick(LPVOID lParam);
	bool OnImportQuesToStoreClick(LPVOID lParam);
	CDuiString GetApplyExamId();

public:
	CPaintManagerUI m_pmUI;					//���ڹ�����

private:
	CButtonUI* m_pCloseBtn;					//�رհ�Ť
	CButtonUI* m_pMaxBtn;					//��󻯰�Ť
	CButtonUI* m_pRestoreBtn;				//�ָ���󻯰�Ť
	CButtonUI* m_pMinBtn;					//��С����Ť
	//�������ؼ�
	CComboBoxUI* m_pSubjectCom;				//��Ŀ������
	CButtonUI* m_pImportFileBtn;			//�����ļ���ť
	CComboBoxUI* m_pQuestionTypeCom;		//����������
	CButtonUI* m_pImportQuestionStoreBtn;	//�������
	CTextUI* m_pFileNameText;				//�ļ�����
	//���˵�
	CButtonUI * m_pQuestionsStoreBtn;		//������
	CButtonUI * m_pPaperManagerBtn;			//�Ծ����
	CButtonUI * m_pGradeManagerBtn;			//�ɼ�����
	CButtonUI * m_pMessageProtectBtn;		//��Ϣά��
	CTextUI   * m_pQuestionScanText;		//��������ı��ؼ�
	//�������
	CComboBoxUI* m_pScanSubjectCom;			//��Ŀ
	CComboBoxUI* m_pScanQuestionTypeCom;	//����
	CButtonUI * m_pScanQuesBtn;					//ȷ��
	CButtonUI * m_pPrePageBtn;				//��һҳ
	CButtonUI * m_pNextPageBtn;				//��һҳ
	//�����Ծ�ؼ�
	CComboBoxUI* m_pGeneratePaperSubjectCom;//��Ŀ
	CDateTimeUI * m_pStartDate;				//��ʼ��������
	CEditUI * m_pStartHour;				//��ʼ����Сʱ
	CEditUI * m_pStartMinute;			//��ʼ���Է���
	CDateTimeUI * m_pEndDate;				//������������
	CEditUI * m_pEndHour;				//��������Сʱ
	CEditUI * m_pEndMinute;				//�������Է���
	CButtonUI * m_pGeneratePaperBtn;		//�����Ծ�
	CTextUI * m_pGenerateExamText;			//������Ϣ
	CTextUI * m_pGeneratePaperText;			//�Ծ���Ϣ
	//�����Ծ�
	CComboBoxUI * m_pCheckExamSubjectCom;	//��Ŀ
	CComboBoxUI * m_pCheckExamQueTypeCom;	//����
	CButtonUI * m_pCheckExamSearchBtn;		//��ѯ
	CButtonUI * m_pCheckExamAnswerBtn;		//�ο���
	CEditUI * m_pCheckExamScoreEdit;		//�ɼ�
	CButtonUI * m_pCheckExamNextBtn;		//��һ����
	CTextUI * m_pCheckExamContentTxt;		//���⼰��
	CButtonUI * m_pCheckExamPopAnsBtn;		//�𰸵���

	//�ɼ����
	CListUI * m_pGradeList;						//�б�ؼ�
	CComboBoxUI * m_pGradeSubjectCom;		//��Ŀ
	CEditUI * m_pGradeExamNumEdit;			//����
	CDateTimeUI * m_GradeStartDate;			//��ʼ����
	CButtonUI * m_pGradeSearchBtn;			//��ѯ

	//��������
	CEditUI * m_pOldPassEdit;				//������
	CEditUI * m_pNewPassEdit;				//������
	CEditUI * m_pRePassEdit;				//�ظ�����
	CButtonUI * m_pUpdatePassBtn;			//��������

	//���뿼��
	CComboBoxUI * m_pApplySubjectCom;		//��Ŀ
	CEditUI * m_pApplyNumberEdit;			//ѧ��
	CButtonUI * m_pApplySerchBtn;			//��ѯ
	CListUI * m_pApplyList;					//�����б�
	CButtonUI * m_pApplyAllPassBtn;			//ȫ��ͨ��
	CButtonUI * m_pApplyAllRefuseBtn;		//ȫ���ܾ�


	std::vector<OPTIONS> m_vecOptions;

	CControlUI * m_pRoot;

private:
	TCHAR * m_szImportText = (_T("��������"));
	TCHAR * m_szImportName = (_T("importQuestion"));
	TCHAR * m_szQuestionScanText = (_T("�������"));
	TCHAR * m_szQuestionScanName = (_T("scanQuestion"));
	TCHAR * m_szGeneratePaperText = _T("�����Ծ�");
	TCHAR * m_szGeneratePaperName = _T("generatePaper");
	TCHAR * m_szCorrectPaperText = _T("�����Ծ�");
	TCHAR * m_szCorrectPaperName = _T("correctPaper");
	TCHAR * m_szGradeScanText = (_T("�ɼ����"));
	TCHAR * m_szGradeScanName = _T("gradeScan");
	TCHAR * m_szPassUpdateText = (_T("��������"));
	TCHAR * m_szPassUpdateName = _T("updatePass");
	TCHAR * m_szApplyExamText = _T("��������");
	TCHAR * m_szApplyExamName = _T("applyExam");

	const UINT m_nOptionNums = 6;							//option�ܸ���
	const UINT m_nQSMNums = 2;								//������option����
	const UINT m_nPMNums = 2;								//�Ծ����option���� 
	const UINT m_nGMNums = 1;								//�ɼ�����option����
	const UINT m_nMPNums = 2;								//��Ϣά��option����

	int m_nCurQueNum;										//��ǰ��ʾ����������
	int m_nCurPage;											//��ǰҳ��
private:
	CEmsNet * m_pClientNet;									//�����ͻ���ʵ��
	SOCKET m_sClient;										//�ͻ���SOCKET
	CEmsClientMain m_mainLogic;								//�ͻ��߼�����ʵ��


private:
	std::map<std::wstring, std::vector<std::wstring> > m_mapSubQue;//��Ŀ������
	CString m_strFileName;											//ѡ�е��ļ�·��
	std::vector<std::map<std::wstring, std::wstring> > m_vmScanQue;//�����������Ϣ
	std::vector<std::wstring> m_vsApplyId;					//����Ŀ��Ա��
	std::vector<std::map<std::wstring, std::wstring> > m_vmQuesAndAns;//���⼰��
	std::map<std::wstring, std::wstring> m_mQuesAndAns;					//���⼰��

};

