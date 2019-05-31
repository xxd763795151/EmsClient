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
	CPaintManagerUI m_pmUI;					//窗口管理器

private:
	CButtonUI* m_pCloseBtn;					//关闭按扭
	CButtonUI* m_pMaxBtn;					//最大化按扭
	CButtonUI* m_pRestoreBtn;				//恢复最大化按扭
	CButtonUI* m_pMinBtn;					//最小化按扭
	//导入题库控件
	CComboBoxUI* m_pSubjectCom;				//科目下拉框
	CButtonUI* m_pImportFileBtn;			//导入文件按钮
	CComboBoxUI* m_pQuestionTypeCom;		//题型下拉框
	CButtonUI* m_pImportQuestionStoreBtn;	//导入题库
	CTextUI* m_pFileNameText;				//文件名字
	//主菜单
	CButtonUI * m_pQuestionsStoreBtn;		//题库管理
	CButtonUI * m_pPaperManagerBtn;			//试卷管理
	CButtonUI * m_pGradeManagerBtn;			//成绩管理
	CButtonUI * m_pMessageProtectBtn;		//信息维护
	CTextUI   * m_pQuestionScanText;		//试题浏览文本控件
	//试题浏览
	CComboBoxUI* m_pScanSubjectCom;			//科目
	CComboBoxUI* m_pScanQuestionTypeCom;	//题型
	CButtonUI * m_pScanQuesBtn;					//确定
	CButtonUI * m_pPrePageBtn;				//上一页
	CButtonUI * m_pNextPageBtn;				//下一页
	//生成试卷控件
	CComboBoxUI* m_pGeneratePaperSubjectCom;//科目
	CDateTimeUI * m_pStartDate;				//开始考试日期
	CEditUI * m_pStartHour;				//开始考试小时
	CEditUI * m_pStartMinute;			//开始考试分钟
	CDateTimeUI * m_pEndDate;				//结束考试日期
	CEditUI * m_pEndHour;				//结束考试小时
	CEditUI * m_pEndMinute;				//结束考试分钟
	CButtonUI * m_pGeneratePaperBtn;		//生成试卷
	CTextUI * m_pGenerateExamText;			//考试信息
	CTextUI * m_pGeneratePaperText;			//试卷信息
	//批阅试卷
	CComboBoxUI * m_pCheckExamSubjectCom;	//科目
	CComboBoxUI * m_pCheckExamQueTypeCom;	//题型
	CButtonUI * m_pCheckExamSearchBtn;		//查询
	CButtonUI * m_pCheckExamAnswerBtn;		//参考答案
	CEditUI * m_pCheckExamScoreEdit;		//成绩
	CButtonUI * m_pCheckExamNextBtn;		//下一道题
	CTextUI * m_pCheckExamContentTxt;		//问题及答案
	CButtonUI * m_pCheckExamPopAnsBtn;		//答案弹框

	//成绩浏览
	CListUI * m_pGradeList;						//列表控件
	CComboBoxUI * m_pGradeSubjectCom;		//科目
	CEditUI * m_pGradeExamNumEdit;			//考号
	CDateTimeUI * m_GradeStartDate;			//开始日期
	CButtonUI * m_pGradeSearchBtn;			//查询

	//更新密码
	CEditUI * m_pOldPassEdit;				//老密码
	CEditUI * m_pNewPassEdit;				//新密码
	CEditUI * m_pRePassEdit;				//重复密码
	CButtonUI * m_pUpdatePassBtn;			//更新密码

	//申请考试
	CComboBoxUI * m_pApplySubjectCom;		//科目
	CEditUI * m_pApplyNumberEdit;			//学号
	CButtonUI * m_pApplySerchBtn;			//查询
	CListUI * m_pApplyList;					//申请列表
	CButtonUI * m_pApplyAllPassBtn;			//全部通过
	CButtonUI * m_pApplyAllRefuseBtn;		//全部拒绝


	std::vector<OPTIONS> m_vecOptions;

	CControlUI * m_pRoot;

private:
	TCHAR * m_szImportText = (_T("导入试题"));
	TCHAR * m_szImportName = (_T("importQuestion"));
	TCHAR * m_szQuestionScanText = (_T("试题浏览"));
	TCHAR * m_szQuestionScanName = (_T("scanQuestion"));
	TCHAR * m_szGeneratePaperText = _T("生成试卷");
	TCHAR * m_szGeneratePaperName = _T("generatePaper");
	TCHAR * m_szCorrectPaperText = _T("批阅试卷");
	TCHAR * m_szCorrectPaperName = _T("correctPaper");
	TCHAR * m_szGradeScanText = (_T("成绩浏览"));
	TCHAR * m_szGradeScanName = _T("gradeScan");
	TCHAR * m_szPassUpdateText = (_T("更新密码"));
	TCHAR * m_szPassUpdateName = _T("updatePass");
	TCHAR * m_szApplyExamText = _T("考试申请");
	TCHAR * m_szApplyExamName = _T("applyExam");

	const UINT m_nOptionNums = 6;							//option总个数
	const UINT m_nQSMNums = 2;								//题库管理option个数
	const UINT m_nPMNums = 2;								//试卷管理option个数 
	const UINT m_nGMNums = 1;								//成绩管理option个数
	const UINT m_nMPNums = 2;								//信息维护option个数

	int m_nCurQueNum;										//当前显示的试题条数
	int m_nCurPage;											//当前页数
private:
	CEmsNet * m_pClientNet;									//网络库客户端实例
	SOCKET m_sClient;										//客户端SOCKET
	CEmsClientMain m_mainLogic;								//客户逻辑操作实例


private:
	std::map<std::wstring, std::vector<std::wstring> > m_mapSubQue;//科目及题型
	CString m_strFileName;											//选中的文件路径
	std::vector<std::map<std::wstring, std::wstring> > m_vmScanQue;//浏览的试题信息
	std::vector<std::wstring> m_vsApplyId;					//申请的考试编号
	std::vector<std::map<std::wstring, std::wstring> > m_vmQuesAndAns;//试题及答案
	std::map<std::wstring, std::wstring> m_mQuesAndAns;					//试题及答案

};

