#include "stdafx.h"
#include "EmsMainWnd.h"
#include "ControlEx.h"
#include "Utils.h"
#include <atlconv.h>
#include "MenuWnd.h"
#include "LoginUser.h"

CEmsMainWnd::CEmsMainWnd()
{
	m_pClientNet = CEmsNet::CreateInstance();
	m_sClient = m_pClientNet->GetClientSocket();
	m_nCurQueNum = 0;
	m_nCurPage = 0;
}


CEmsMainWnd::~CEmsMainWnd()
{
	delete m_pRoot;
}

/************************************************************************/
/* ���ش��������������                                                 */
/************************************************************************/
LPCTSTR CEmsMainWnd::GetWindowClassName() const
{
	return _T("EmsMainWnd");
}

UINT CEmsMainWnd::GetClassStyle()
{
	return CS_DBLCLKS;
}

void CEmsMainWnd::OnFinalMessage(HWND hWnd)
{
	delete this;
}
/************************************************************************/
/* ���ڳ�ʼ��ȡ��xml�еĿؼ�ID                                          */
/************************************************************************/
void CEmsMainWnd::Init()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("closebtn")));
	m_pMaxBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("maxbtn")));
	m_pRestoreBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("restorebtn")));
	m_pMinBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("minbtn")));

	m_pQuestionsStoreBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("questionsStore")));
	m_pQuestionsStoreBtn->OnNotify += MakeDelegate(this, &CEmsMainWnd::OnQuestionStoreClick);
	m_pPaperManagerBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("paperManager")));
	m_pPaperManagerBtn->OnNotify += MakeDelegate(this, &CEmsMainWnd::OnPaperManagerClick);
	m_pGradeManagerBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("gradeManager")));
	m_pGradeManagerBtn->OnNotify += MakeDelegate(this, &CEmsMainWnd::OnGradeManageClick);
	m_pMessageProtectBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("messageProtect")));
	m_pMessageProtectBtn->OnNotify += MakeDelegate(this, &CEmsMainWnd::OnMessageProtectClick);

	m_pSubjectCom = static_cast<CComboBoxUI*>(m_pmUI.FindControl(_T("subject")));
	m_pImportFileBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("importFile")));
	m_pQuestionScanText = static_cast<CTextUI*>(m_pmUI.FindControl(_T("questionScanText")));
	m_pStartDate = static_cast<CDateTimeUI*>(m_pmUI.FindControl(_T("StartDate")));
	//�ɼ����
	m_pGradeList = static_cast<CListUI*>(m_pmUI.FindControl(_T("gradeList")));
	m_pGradeExamNumEdit = static_cast<CEditUI*>(m_pmUI.FindControl(_T("userNumber")));
	m_GradeStartDate = static_cast<CDateTimeUI*>(m_pmUI.FindControl(_T("gradeScanStartTime")));
	m_pGradeSearchBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("gradeScanSearch")));

	m_pScanSubjectCom = static_cast<CComboBoxUI*>(m_pmUI.FindControl(_T("qsSubject")));
	m_pScanQuestionTypeCom = static_cast<CComboBoxUI*>(m_pmUI.FindControl(_T("qsQuestionType")));
	m_pScanQuesBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("scanQuesOK")));
	m_pPrePageBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("prePage")));
	m_pNextPageBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("nextPage")));

	m_pGeneratePaperSubjectCom = static_cast<CComboBoxUI*>(m_pmUI.FindControl(_T("generatePaperSubject")));
	m_pStartDate = static_cast<CDateTimeUI*>(m_pmUI.FindControl(_T("startDate")));
	m_pEndDate = static_cast<CDateTimeUI*>(m_pmUI.FindControl(_T("endDate")));
	m_pStartHour = static_cast<CEditUI*>(m_pmUI.FindControl(_T("startHour")));
	m_pStartMinute = static_cast<CEditUI*>(m_pmUI.FindControl(_T("startMinute")));
	m_pEndHour = static_cast<CEditUI*>(m_pmUI.FindControl(_T("endHour")));
	m_pEndMinute = static_cast<CEditUI*>(m_pmUI.FindControl(_T("endMinute")));
	m_pGeneratePaperBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("generatePaperBtn")));
	m_pGenerateExamText = static_cast<CTextUI*>(m_pmUI.FindControl(_T("showExamMsg")));
	m_pGeneratePaperText = static_cast<CTextUI*>(m_pmUI.FindControl(_T("showPaperMsg")));

	//���뿼��
	m_pApplySubjectCom = static_cast<CComboBoxUI*>(m_pmUI.FindControl(_T("applyExamSubject")));
	m_pApplyNumberEdit = static_cast<CEditUI*>(m_pmUI.FindControl(_T("applyUserNumber")));
	m_pApplySerchBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("applyExamSearch")));
	m_pApplyList = static_cast<CListUI*>(m_pmUI.FindControl(_T("applyList")));
	m_pApplyAllPassBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("applyAllPass")));
	m_pApplyAllRefuseBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("applyAllRefuse")));

	//�����Ծ�
	m_pCheckExamSubjectCom = static_cast<CComboBoxUI*>(m_pmUI.FindControl(_T("checkExamSubject")));
	m_pCheckExamQueTypeCom = static_cast<CComboBoxUI*>(m_pmUI.FindControl(_T("checkExamQueType")));
	m_pCheckExamSearchBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("checkExamSearch")));
	m_pCheckExamAnswerBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("checkExamAnswer")));
	m_pCheckExamNextBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("checkExamNext")));
	m_pCheckExamScoreEdit = static_cast<CEditUI*>(m_pmUI.FindControl(_T("checkExamScore")));
	m_pCheckExamContentTxt = static_cast<CTextUI*>(m_pmUI.FindControl(_T("quesAndAnswerText")));
	m_pCheckExamPopAnsBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("checkExamPopAnswer")));

	//��������
	m_pOldPassEdit = static_cast<CEditUI*>(m_pmUI.FindControl(_T("oldPasswd")));
	m_pNewPassEdit = static_cast<CEditUI*>(m_pmUI.FindControl(_T("newPasswd")));
	m_pRePassEdit = static_cast<CEditUI*>(m_pmUI.FindControl(_T("rePasswd")));
	m_pUpdatePassBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("updatePasswd")));

	m_pGradeSubjectCom = static_cast<CComboBoxUI*>(m_pmUI.FindControl(_T("gradeScanSubject")));
	m_pQuestionTypeCom = static_cast<CComboBoxUI*>(m_pmUI.FindControl(_T("questionType")));
	m_pImportQuestionStoreBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("imQuesToStore")));
	m_pFileNameText = static_cast<CTextUI*>(m_pmUI.FindControl(_T("fileName")));
	
	
	m_mainLogic.GetSubjectNameAndQuestionName();

	//m_pQuestionScanText->SetText(_T("<c #999999>Ҳ�����ҿ������ڶ�����������<p>�����ǵ���ɽͷ���һĨ��ϼ����ʱ�����֪���������㡣��������Ҳ�͵����ˣ����ſ�Ҫ�ڵ�����Լ����µ�̫���������鴤��һ�£���֪�������������ᣬ�������Ѿ����ƵĲݵ��ϣ�</c>����ɪɪ�������������죬��һ˿���⣬���ǲ�֪�����������������磬���ֶ��硣		Сʱ��Զ���ڵ���⣬����ͣ��������������ӡ������ص����ǣ��ƺ�Ҳ�����˽����ڵ���������������ĸ�ף��Ȳ����ö���ڵ��ں���Ҳ���������ſڲ������ѡ���֦��а������δ������һ˫����Ϊһ���˰��ϼ������պ����ա���ĸ�׵�Ǳ��ʶ�����Ҫ��һ�����⣬����һƬ������Բ�������ڵ���ա��������۵��ģ����궼Ϊ���ӵ�����ͼ����ž۵���ܰ������		���������Ҳ�֪�����������ʵ��Ϊ�˼����ǧ����ǰ�������̴����ԭ�ġ���ԭ�����ں������飬����С�������˼��������Զ�����롣����Ŀ������������ᾰ��󣬾��ľ�������Σ��֮�С����ǣ�����һҶ���ۣ�̤������Ͽ�Ĺ������Σ��߽����Լ���־Ϊ��������ʥ����á�Ȼ������������������������ʵ�ڻ��췦������Ȼ��֪��Ҫʵ���Լ������롰·��������Զ�⡱����Ȼ��Ҳ���ġ��Ὣ���¶���������Ȼ���������Ļ����£���������������������ʵ����ֻ�ܷ�������̫Ϣ�������⣬������֮��衱��̾Ϣ����ֻ�ܽ�������ǻ�Ƿ�����ʶˣ�Ϊ�������¡���ɧ���������ʡ��Ȳ���ʫƪ�󣬱���Ͷ��������ѳ־��Ҳ�������ǧ�������й�֪ʶ�������˵���Ӱ�ɡ���������������ػ���٣�ȴ���Ŷ����ϰ��ǧ�Ŵ��С������㲻���ľ���Ʒ����ҫ�������ص�Ρ��Ⱥɽ����ҫ�ų����ľ��κ��ˣ�Ҳ��ҫ���л���أ���Ϊ�����䷶��<b>������磬���ֶ��硣<b>		Сʱ��Զ���ڵ���⣬����ͣ��������������ӡ������ص����ǣ��ƺ�Ҳ�����˽����ڵ���������������ĸ�ף��Ȳ����ö���ڵ��ں���Ҳ���������ſڲ������ѡ���֦��а������δ������һ˫����Ϊһ���˰��ϼ������պ����ա���ĸ�׵�Ǳ��ʶ�����Ҫ��һ�����⣬����һƬ������Բ�������ڵ���ա��������۵��ģ����궼Ϊ���ӵ�����ͼ����ž۵���ܰ������<b>		���������Ҳ�֪�����������ʵ��Ϊ�˼����ǧ����ǰ�������̴����ԭ�ġ���ԭ�����ں������飬����С�������˼��������Զ�����롣����Ŀ������������ᾰ��󣬾��ľ�������Σ��֮�С����ǣ�����һҶ���ۣ�̤������Ͽ�Ĺ������Σ��߽����Լ���־Ϊ��������ʥ����á�Ȼ������������������������ʵ�ڻ��췦������Ȼ��֪��Ҫʵ���Լ������롰·��������Զ�⡱����Ȼ��Ҳ���ġ��Ὣ���¶���������Ȼ���������Ļ����£���������������������ʵ����ֻ�ܷ�������̫Ϣ�������⣬������֮��衱��̾Ϣ����ֻ�ܽ�������ǻ�Ƿ�����ʶˣ�Ϊ�������¡���ɧ���������ʡ��Ȳ���ʫƪ�󣬱���Ͷ��������ѳ־��Ҳ�������ǧ�������й�֪ʶ�������˵���Ӱ�ɡ���������������ػ���٣�ȴ���Ŷ����ϰ��ǧ�Ŵ��С������㲻���ľ���Ʒ����ҫ�������ص�Ρ��Ⱥɽ����ҫ�ų����ľ��κ��ˣ�Ҳ��ҫ���л���أ���Ϊ�����䷶��<p>������磬���ֶ��硣<p>		Сʱ��Զ���ڵ���⣬����ͣ��������������ӡ������ص����ǣ��ƺ�Ҳ�����˽����ڵ���������������ĸ�ף��Ȳ����ö���ڵ��ں���Ҳ���������ſڲ������ѡ���֦��а������δ������һ˫����Ϊһ���˰��ϼ������պ����ա���ĸ�׵�Ǳ��ʶ�����Ҫ��һ�����⣬����һƬ������Բ�������ڵ���ա��������۵��ģ����궼Ϊ���ӵ�����ͼ����ž۵���ܰ������<p>		���������Ҳ�֪�����������ʵ��Ϊ�˼����ǧ����ǰ�������̴����ԭ�ġ���ԭ�����ں������飬����С�������˼��������Զ�����롣����Ŀ������������ᾰ��󣬾��ľ�������Σ��֮�С����ǣ�����һҶ���ۣ�̤������Ͽ�Ĺ������Σ��߽����Լ���־Ϊ��������ʥ����á�Ȼ������������������������ʵ�ڻ��췦������Ȼ��֪��Ҫʵ���Լ������롰·��������Զ�⡱����Ȼ��Ҳ���ġ��Ὣ���¶���������Ȼ���������Ļ����£���������������������ʵ����ֻ�ܷ�������̫Ϣ�������⣬������֮��衱��̾Ϣ����ֻ�ܽ�������ǻ�Ƿ�����ʶˣ�Ϊ�������¡���ɧ���������ʡ��Ȳ���ʫƪ�󣬱���Ͷ��������ѳ־��Ҳ�������ǧ�������й�֪ʶ�������˵���Ӱ�ɡ���������������ػ���٣�ȴ���Ŷ����ϰ��ǧ�Ŵ��С������㲻���ľ���Ʒ����ҫ�������ص�Ρ��Ⱥɽ����ҫ�ų����ľ��κ��ˣ�Ҳ��ҫ���л���أ���Ϊ�����䷶��"));
	InitOption();
	
	QuestionStoreManager();
}

void CEmsMainWnd::OnPrepare()
{

}
/************************************************************************/
/* ����DUILIB����Ϣ                                                     */
/************************************************************************/
void CEmsMainWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("windowinit")) OnPrepare();
	else if (msg.sType == _T("click")) {
		if (msg.pSender == m_pCloseBtn)
		{
			PostQuitMessage(0);
			return;
		}
		else if (msg.pSender == m_pMinBtn)
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return;
		}
		else if (msg.pSender == m_pMaxBtn) 
		{
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return;
		}
		else if (msg.pSender == m_pRestoreBtn)
		{
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return;
		}
		else if (msg.pSender == m_pImportFileBtn)
		{
			TCHAR szFileName[MAX_PATH] = { 0 };
			CUtils::OpenFileDialog(szFileName);
			if (_tcslen(szFileName) <= 0)
			{
				return;
			}
			TCHAR szSelFile[MAX_PATH] = { 0 };
			_tcscat(szSelFile, _T("��ѡ�ļ���"));
			_tcscat(szSelFile, szFileName);
			m_pFileNameText->SetText(szSelFile);
			m_pFileNameText->NeedParentUpdate();
			m_strFileName = szFileName;
			
		}
		else if (msg.pSender == m_pImportQuestionStoreBtn)
		{
			ImportQuesToStore();
		}else if (msg.pSender == m_pScanQuesBtn)
		{
			m_vmScanQue.clear();
			m_pQuestionScanText->SetText(_T(""));
			m_pQuestionScanText->NeedParentUpdate();
			ScanQues();
		}else if (msg.pSender == m_pNextPageBtn)
		{
			if ((unsigned)(m_nCurPage + 1)*SHOW_CCF_NUM < m_vmScanQue.size())	ScanCCFLimit((++m_nCurPage)*SHOW_CCF_NUM, SHOW_CCF_NUM);
		}else if (msg.pSender == m_pPrePageBtn)
		{
			if ((m_nCurPage - 1) >= 0)	ScanCCFLimit((--m_nCurPage)*SHOW_CCF_NUM, SHOW_CCF_NUM, false);
		}
		else if (msg.pSender == m_pGeneratePaperBtn)
		{
			GeneratePaper();
		}
		else if (msg.pSender == m_pApplySerchBtn)
		{
			PrintApplyMsg();
		}else if (msg.pSender == m_pApplyAllPassBtn)
		{
			if (m_vsApplyId.empty())
			{
				return;
			}
			m_mainLogic.HandleApplyExam(m_vsApplyId, -1);
			PrintApplyMsg();
		}else if (msg.pSender == m_pApplyAllRefuseBtn)
		{
			if (m_vsApplyId.empty())
			{
				return;
			}
			m_mainLogic.HandleApplyExam(m_vsApplyId, -1, false);
			PrintApplyMsg();
		}else if (msg.pSender == m_pCheckExamSearchBtn)
		{
			m_mQuesAndAns.clear();
			int nCurSel = m_pCheckExamSubjectCom->GetCurSel();
			if (nCurSel == -1)
			{
				MessageBox(NULL, _T("��ѡ���Ŀ"), _T("��ʾ"), MB_OK);
				return;
			}
			int nCurSelType = m_pCheckExamQueTypeCom->GetCurSel();
			if (nCurSelType == -1)
			{
				MessageBox(NULL, _T("��ѡ������"), _T("��ʾ"), MB_OK);
				return;
			}
			CDuiString strSubject = m_pCheckExamSubjectCom->GetItemAt(nCurSel)->GetText();
			CDuiString strType = m_pCheckExamQueTypeCom->GetItemAt(nCurSel)->GetText();
			m_mainLogic.GetQuestionAndUserAnswer(strSubject.GetData(), strType.GetData());
		}else if (msg.pSender == m_pCheckExamPopAnsBtn)
		{
			PopUserAnwser();
		}
		else if (msg.pSender == m_pCheckExamNextBtn)
		{
			if (m_mQuesAndAns.empty())
			{
				return;
			}
			CDuiString strScore = m_pCheckExamScoreEdit->GetText();
			if (ValidateScore(strScore) == false)
			{
				return;
			}
			GetNextAnswer(strScore);
		}else if (msg.pSender == m_pCheckExamAnswerBtn)
		{
			PopAnswer();
		}else if (msg.pSender == m_pGradeSearchBtn)
		{
			int iCurSel = m_pGradeSubjectCom->GetCurSel();
			if (iCurSel == -1)
			{
				MessageBox(NULL, _T("��ѡ���Ŀ"), _T("��ʾ"), MB_OK | MB_ICONWARNING);
				return;
			}
			CDuiString strSubject = m_pGradeSubjectCom->GetItemAt(iCurSel)->GetText();
			CDuiString strNumber = m_pGradeExamNumEdit->GetText();
			CDuiString strStartTime = m_GradeStartDate->GetText();
			m_mainLogic.GradeScan(strSubject.GetData(), strNumber.GetData(), strStartTime.GetData());
		}
		else if (msg.pSender == m_pUpdatePassBtn)
		{
			std::wstring strPass = CLoginUser::GetLoginUser()->GetPassword();
			if (m_pOldPassEdit->GetText().Compare(strPass.c_str()) != 0)
			{
				MessageBox(NULL, _T("�������������"), _T("��ʾ"), MB_OK | MB_ICONWARNING);
				return;
			}
			CDuiString strNewPass = m_pNewPassEdit->GetText();
			if (strNewPass.GetLength() > 20 || strNewPass.GetLength() < 6)
			{
				MessageBox(NULL, _T("�����볤����6��20λ֮��"), _T("��ʾ"), MB_OK | MB_ICONWARNING);
				return;
			}
			if (strNewPass.Compare(m_pRePassEdit->GetText()) != 0)
			{
				MessageBox(NULL, _T("�����������벻һ��"), _T("��ʾ"), MB_OK | MB_ICONWARNING);
				return;
			}
			std::wstring strName = CLoginUser::GetLoginUser()->GetUsername();
			m_mainLogic.UpdatePassword(strName.c_str(), strNewPass.GetData());
		}
	}
	else if (msg.sType == _T("selectchanged"))
	{
		CDuiString name = msg.pSender->GetName();
		CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_pmUI.FindControl(_T("switch")));
		if (name.Compare(m_szImportName) == 0)
			pControl->SelectItem(0);
		else if (name.Compare(m_szQuestionScanName) == 0)
			pControl->SelectItem(1);
		else if (name.Compare(m_szGeneratePaperName) == 0)
			pControl->SelectItem(2);
		else if (name.Compare(m_szCorrectPaperName) == 0)
			pControl->SelectItem(3);
		else if (name.Compare(m_szGradeScanName) == 0)
			pControl->SelectItem(4);
		else if (name.Compare(m_szPassUpdateName) == 0)
			pControl->SelectItem(5);
		else if (name.Compare(m_szApplyExamName) == 0)
			pControl->SelectItem(6);
		else if (name.Compare(_T("")) == 0)
			pControl->SelectItem(7);
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		if (msg.pSender == m_pGradeList)
		{
			return;
		}
		if (msg.pSender == m_pApplyList)
		{
			return;
		}
		ShowQuestionToWnd(msg.pSender);
		if (msg.pSender == m_pGeneratePaperSubjectCom)
		{
			int iCurSel = m_pGeneratePaperSubjectCom->GetCurSel();
			m_mainLogic.GetExamDescription(m_pGeneratePaperSubjectCom->GetItemAt(iCurSel)->GetText());
		}
		
	}
	else if (msg.sType == DUI_MSGTYPE_MENU)
	{
		if (msg.pSender != m_pApplyList || m_vsApplyId.empty())
		{
			return;
		}
		CMenuWnd* pMenu = new CMenuWnd();
		if (pMenu == NULL) { return; }
		POINT pt = { msg.ptMouse.x, msg.ptMouse.y };
		::ClientToScreen(*this, &pt);
		pMenu->Init(msg.pSender, pt);
	}
	else if (msg.sType == _T("menu_Pass"))
	{
		CListUI * pList = static_cast<CListUI*>(msg.pSender);
		int iOffset = pList->GetCurSel();
		m_mainLogic.HandleApplyExam(m_vsApplyId, iOffset);
		PrintApplyMsg();
	}
	else if (msg.sType == _T("menu_Refuse"))
	{
		CListUI * pList = static_cast<CListUI*>(msg.pSender);
		int iOffset = pList->GetCurSel();
		m_mainLogic.HandleApplyExam(m_vsApplyId, iOffset, false);
		PrintApplyMsg();
	}
}
/************************************************************************/
/* ���ڴ���������                                                     */
/************************************************************************/
LRESULT CEmsMainWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pmUI.Init(m_hWnd);
	CDialogBuilder builder;
	CDialogBuilderCallbackEx cb;
	m_pRoot = builder.Create(_T("skin.xml"), (UINT)0, &cb, &m_pmUI);
	ASSERT(m_pRoot && "Failed to parse XML");
	m_pmUI.AttachDialog(m_pRoot);
	m_pmUI.AddNotifier(this);

	Init();
	return 0;
}

LRESULT CEmsMainWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CEmsMainWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostQuitMessage(0L);

	bHandled = FALSE;
	return 0;
}

LRESULT CEmsMainWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (::IsIconic(*this)) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CEmsMainWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CEmsMainWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

/************************************************************************/
/* �Ի洰��û�б��������ú�������һ�����������д����ƶ�                                                                     */
/************************************************************************/
LRESULT CEmsMainWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	RECT rcCaption = m_pmUI.GetCaptionRect();
	if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
		CControlUI* pControl = static_cast<CControlUI*>(m_pmUI.FindControl(pt));
		if (pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 &&
			_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
			_tcscmp(pControl->GetClass(), _T("TextUI")) != 0)
			return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CEmsMainWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_pmUI.GetRoundCorner();
	if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;

	//int iWidth = GetSystemMetrics(SM_CXSCREEN);
	//int iHeight = GetSystemMetrics(SM_CYSCREEN);
	RECT rc;
	GetClientRect(this->m_hWnd, &rc);
	m_pQuestionScanText->SetFixedWidth(rc.right - rc.left-20);
	//m_pQuestionScanText->SetMaxHeight(iHeight);

	return 0;
}

LRESULT CEmsMainWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

	//���ƴ��ڴ�С
	LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
	lpMMI->ptMaxPosition.x = rcWork.left;
	lpMMI->ptMaxPosition.y = rcWork.top;
	lpMMI->ptMaxSize.x = rcWork.right;
	lpMMI->ptMaxSize.y = rcWork.bottom;

	bHandled = FALSE;
	return 0;
}

LRESULT CEmsMainWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// ��ʱ�����յ�WM_NCDESTROY���յ�wParamΪSC_CLOSE��WM_SYSCOMMAND
	if (wParam == SC_CLOSE) {
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);//�����Ƿ����
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(*this) != bZoomed) {
		if (!bZoomed) {
			CControlUI* pControl = static_cast<CControlUI*>(m_pmUI.FindControl(_T("maxbtn")));
			if (pControl) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_pmUI.FindControl(_T("restorebtn")));
			if (pControl) pControl->SetVisible(true);
		}
		else {
			CControlUI* pControl = static_cast<CControlUI*>(m_pmUI.FindControl(_T("maxbtn")));
			if (pControl) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_pmUI.FindControl(_T("restorebtn")));
			if (pControl) pControl->SetVisible(false);
		}
	}
	return lRes;
}
/************************************************************************/
/* ����windows��Ϣ                                                      */
/************************************************************************/
LRESULT CEmsMainWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	case WM_SHOW_SUBJECT_QUESTION:ShowSubject((char*)lParam); break;
	case WM_IMPORT_RESPONSE: lRes = OnImpotResponse(lParam);	 break;
	case WM_SCAN_QUESTION:	lRes = OnScanQuestion(wParam, lParam);	break;
	case WM_ADD_EXAM_RET:	lRes = OnAddExamRet(lParam);	break;
	case WM_EXAM_DESCRIPTION:	lRes = OnGetExamDescription(lParam);	break;
	case WM_APPLY_MSG:		lRes = OnGetApplyMsg(lParam);	break;
	case WM_USER_ANSWER_MSG:		lRes = OnGetUserAnswer(lParam);	break;
	case WM_ORIGIN_ANSWER_MSG:		lRes = OnGetAnswer(lParam);	break;
	case WM_GRADE_SHOW_MSG:		lRes = OnGradeShow(lParam);	break;
	default:
		bHandled = FALSE;
	}
	if (bHandled) return lRes;
	if (m_pmUI.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

/************************************************************************/
/* ��ʼ����ťѡ��                                                       */
/************************************************************************/
void CEmsMainWnd::InitOption()
{
	for (size_t i = 0; i < m_nOptionNums; ++i)
	{
		TCHAR szOption[0xff];
		ZeroMemory(szOption, 0xff);
		wsprintf(szOption, _T("option%d"),i);
		CButtonUI* pBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(szOption));
		pBtn->SetVisible(false);
		OPTIONS sOption = { 0 };
		sOption.szName = szOption;
		sOption.bVisible = false;
		sOption.strOptionId = szOption;
		m_vecOptions.push_back(sOption);
	}
	
}
/************************************************************************/
/* ��ʾ�������Ӧ��option                                             */
/************************************************************************/
void CEmsMainWnd::QuestionStoreManager()
{
	CButtonUI* pBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("option0")));
	m_vecOptions[0].szName = m_szImportName;
	m_vecOptions[0].szText = m_szImportText;
	m_vecOptions[0].bVisible = true;

	pBtn->SetName(m_vecOptions[0].szName);
	pBtn->SetText(m_vecOptions[0].szText);
	pBtn->SetVisible(m_vecOptions[0].bVisible);

	pBtn->SetAttribute(_T("selected"), _T("true"));
	m_pFileNameText->SetText(_T(""));
	m_pFileNameText->NeedParentUpdate();
	m_strFileName.Empty();

	pBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("option1")));
	m_vecOptions[1].szName = m_szQuestionScanName;
	m_vecOptions[1].szText = m_szQuestionScanText;
	m_vecOptions[1].bVisible = true;
	
	pBtn->SetName(m_vecOptions[1].szName);
	pBtn->SetText(m_vecOptions[1].szText);
	pBtn->SetVisible(m_vecOptions[1].bVisible);
	TNotifyUI msg;
	msg.sType = _T("selectchanged");
	msg.pSender = new CButtonUI;
	msg.pSender->SetName(m_szImportName);
	m_pmUI.SendNotify(msg);
	delete msg.pSender;
	SetOptionNotVisible(m_nQSMNums);
	
}
/************************************************************************/
/* ��ʾoption                                                           */
/************************************************************************/
void CEmsMainWnd::SetOptionNotVisible(UINT nNums)
{
	for (; nNums < m_nOptionNums; ++nNums)
	{
		if (m_vecOptions[nNums].bVisible)
		{
			m_vecOptions[nNums].bVisible = false;
			TCHAR szOption[0xff] = { 0 };
			wsprintf(szOption, _T("option%d"), nNums);
			CButtonUI * pBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(szOption));
			pBtn->SetVisible(false);
		}
		else
		{
			break;
		}
	}
}
/************************************************************************/
/* ���������Ť��Ϣ������                                         */
/************************************************************************/
bool CEmsMainWnd::OnQuestionStoreClick(LPVOID lParam)
{
	QuestionStoreManager();
	return true;
}
/************************************************************************/
/* ����Ծ����ť��Ϣ������                                         */
/************************************************************************/
bool CEmsMainWnd::OnPaperManagerClick(LPVOID lParam)
{
	CButtonUI* pBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("option0")));
	m_vecOptions[0].szName = m_szGeneratePaperName;
	m_vecOptions[0].szText = m_szGeneratePaperText;
	m_vecOptions[0].bVisible = true;

	pBtn->SetName(m_vecOptions[0].szName);
	pBtn->SetText(m_vecOptions[0].szText);
	pBtn->SetVisible(m_vecOptions[0].bVisible);

	pBtn->SetAttribute(_T("selected"), _T("true"));

	pBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("option1")));
	m_vecOptions[1].szName = m_szCorrectPaperName;
	m_vecOptions[1].szText = m_szCorrectPaperText;
	m_vecOptions[1].bVisible = true;

	pBtn->SetName(m_vecOptions[1].szName);
	pBtn->SetText(m_vecOptions[1].szText);
	pBtn->SetVisible(m_vecOptions[1].bVisible);
	
	SetOptionNotVisible(m_nPMNums);
	TNotifyUI msg;
	msg.sType = DUI_MSGTYPE_SELECTCHANGED;
	msg.pSender = new CButtonUI;
	msg.pSender->SetName(m_szGeneratePaperName);
	m_pmUI.SendNotify(msg);
	delete msg.pSender;
	return true;
}
/************************************************************************/
/* ����ɼ�����ť��Ϣ������                                         */
/************************************************************************/
bool CEmsMainWnd::OnGradeManageClick(LPVOID lParam)
{
	CButtonUI* pBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("option0")));
	m_vecOptions[0].szName = m_szGradeScanName;
	m_vecOptions[0].szText = m_szGradeScanText;
	m_vecOptions[0].bVisible = true;

	pBtn->SetName(m_vecOptions[0].szName);
	pBtn->SetText(m_vecOptions[0].szText);
	pBtn->SetVisible(m_vecOptions[0].bVisible);

	pBtn->SetAttribute(_T("selected"), _T("true"));

	SetOptionNotVisible(m_nGMNums);
	TNotifyUI msg;
	msg.sType = _T("selectchanged");
	msg.pSender = new CButtonUI;
	msg.pSender->SetName(m_szGradeScanName);
	m_pmUI.SendNotify(msg);
	delete msg.pSender;
	return true;
}
/************************************************************************/
/* ��Ϣά����ť��Ϣ������                                             */
/************************************************************************/
bool CEmsMainWnd::OnMessageProtectClick(LPVOID lParam)
{
	CButtonUI* pBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("option0")));
	m_vecOptions[0].szName = m_szPassUpdateName;
	m_vecOptions[0].szText = m_szPassUpdateText;
	m_vecOptions[0].bVisible = true;

	pBtn->SetName(m_vecOptions[0].szName);
	pBtn->SetText(m_vecOptions[0].szText);
	pBtn->SetVisible(m_vecOptions[0].bVisible);

	pBtn->SetAttribute(_T("selected"), _T("true"));

	pBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("option1")));
	m_vecOptions[1].szName = m_szApplyExamName;
	m_vecOptions[1].szText = m_szApplyExamText;
	m_vecOptions[1].bVisible = true;

	pBtn->SetName(m_vecOptions[1].szName);
	pBtn->SetText(m_vecOptions[1].szText);
	pBtn->SetVisible(m_vecOptions[1].bVisible);

	SetOptionNotVisible(m_nMPNums);
	TNotifyUI msg;
	msg.sType = _T("selectchanged");
	msg.pSender = new CButtonUI;
	msg.pSender->SetName(m_szPassUpdateName);
	m_pmUI.SendNotify(msg);
	delete msg.pSender;
	
	return true;
}
/************************************************************************/
/* �����л����˵�ʱoption����ʾͼƬ,�ǵ�������˵�(true)����option      */
/************************************************************************/
void CEmsMainWnd::SetOptionImage(BOOL bClick)
{
	if (bClick)
	{
		CButtonUI* pBtnTmp = static_cast<CButtonUI*>(m_pmUI.FindControl(m_vecOptions[0].strOptionId));
		pBtnTmp->SetBkImage(_T("tabbar_pushed.png"));
		for (size_t i = 1; i < m_vecOptions.size(); ++i)
		{
			pBtnTmp = static_cast<CButtonUI*>(m_pmUI.FindControl(m_vecOptions[i].strOptionId));
			pBtnTmp->SetBkImage(_T("tabbar_normal.png"));
		}
	}
	else
	{
		for (size_t i = 0; i < m_vecOptions.size(); ++i)
		{
			CButtonUI* pBtnTmp = static_cast<CButtonUI*>(m_pmUI.FindControl(m_vecOptions[0].strOptionId));
			pBtnTmp = static_cast<CButtonUI*>(m_pmUI.FindControl(m_vecOptions[i].strOptionId));
			pBtnTmp->SetNormalImage(_T("tabbar_normal.png"));
			pBtnTmp->SetPushedImage(_T("tabbar_pushed.png"));
		}
	}
}
/************************************************************************/
/* ��ʾ��Ŀ                                                             */
/************************************************************************/
void CEmsMainWnd::ShowSubject(char * szData)
{
	USES_CONVERSION;
	TCHAR * szwData = A2W(szData);
	std::wstring strData = szwData;
	size_t kPos = 0, vPos = 0;
	int iRet = 0;
	std::wstring strKeyFind = L"DB_Name:";
	std::wstring strValueFind = L"DB_Value:";
	iRet = strData.find(strKeyFind, kPos);
	int iFlag = 0;
	std::map<std::wstring, std::vector<std::wstring> >::iterator it;
	while (true)
	{
		kPos = iRet + strKeyFind.size();
		vPos = strData.find(strValueFind, iRet);
		std::wstring strKey = strData.substr(kPos, vPos - kPos);
		iRet = strData.find(strKeyFind, vPos);
		if (iRet == -1)
		{
			int iValueBegin = vPos + strValueFind.size();
			std::wstring strValue = strData.substr(iValueBegin, strData.size() - iValueBegin);
			it->second.push_back(strValue);
			break;
		}
		int iValueBegin = vPos + strValueFind.size();
		std::wstring strValue = strData.substr(iValueBegin, iRet - iValueBegin);
		iFlag++;
		if (iFlag % 2 != 0)
		{
			//ȡ��Ŀ
			std::vector<std::wstring> vsQue;
			m_mapSubQue.insert(make_pair(strValue, vsQue));
			it = m_mapSubQue.find(strValue);
		}
		else
		{
			//ȡ����
			if (it != m_mapSubQue.end())
			{
				it->second.push_back(strValue);
			}

		}
	}
	ShowSubjectToWnd();
}
/************************************************************************/
/* ����Ŀ��Ϣ��ʾ������                                                 */
/************************************************************************/
void CEmsMainWnd::ShowSubjectToWnd()
{
	for (std::map<std::wstring, std::vector<std::wstring> >::iterator it = m_mapSubQue.begin(); it != m_mapSubQue.end(); ++it)
	{
		CListLabelElementUI * pLabel = new CListLabelElementUI;
		pLabel->SetText(it->first.c_str());
		m_pSubjectCom->Add(pLabel);
		pLabel = new CListLabelElementUI;
		pLabel->SetText(it->first.c_str());
		m_pScanSubjectCom->Add(pLabel);
		pLabel = new CListLabelElementUI;
		pLabel->SetText(it->first.c_str());
		m_pGeneratePaperSubjectCom->Add(pLabel);
		pLabel = new CListLabelElementUI;
		pLabel->SetText(it->first.c_str());
		m_pGradeSubjectCom->Add(pLabel);
		pLabel = new CListLabelElementUI;
		pLabel->SetText(it->first.c_str());
		m_pApplySubjectCom->Add(pLabel);
		pLabel = new CListLabelElementUI;
		pLabel->SetText(it->first.c_str());
		m_pCheckExamSubjectCom->Add(pLabel);
	}
}
/************************************************************************/
/* ��ʾ����                                                             */
/************************************************************************/
void CEmsMainWnd::ShowQuestionToWnd(CControlUI* pSender)
{
	CComboBoxUI * pCurCom = static_cast<CComboBoxUI*>(pSender);
	int iCurSel = pCurCom->GetCurSel();
	CControlUI * pLabel = pCurCom->GetItemAt(iCurSel);
	CDuiString strText = pLabel->GetText();
	std::wstring sText = strText;
	std::map<std::wstring, std::vector<std::wstring> >::iterator itMap = m_mapSubQue.find(sText);
	if (pCurCom == m_pSubjectCom)
	{
		InsertQuestion(itMap->second, m_pQuestionTypeCom);
	}
	else if (pCurCom == m_pScanSubjectCom)
	{
		InsertQuestion(itMap->second, m_pScanQuestionTypeCom);
	}else if (pCurCom == m_pCheckExamSubjectCom)
	{
		InsertQuestion(itMap->second, m_pCheckExamQueTypeCom);
	}
	
}
/************************************************************************/
/* �������������������                                                 */
/************************************************************************/
void CEmsMainWnd::InsertQuestion(std::vector<std::wstring> vecQues, CComboBoxUI * pComUI)
{
	/*int iCount = pComUI->GetCount();
	for (int i = 0; i < iCount; ++i)
	{
		CControlUI * pLabel = pComUI->GetItemAt(i);
		delete pLabel;
	}*/
	pComUI->RemoveAll();
	for (std::vector<std::wstring>::iterator it = vecQues.begin(); it != vecQues.end(); ++it)
	{
		CListLabelElementUI * pLabel = new CListLabelElementUI;
		pLabel->SetText((*it).c_str());
		pComUI->Add(pLabel);
	}
}
/************************************************************************/
/* ���������ⰴť��Ϣ������                                         */
/************************************************************************/
bool CEmsMainWnd::OnImportQuesToStoreClick(LPVOID lParam)
{
	bool bRet = false;
	do
	{
		MessageBox(NULL, _T("1111111111111"), _T(""), MB_OK);
		bRet = true;
	} while (false);
	return bRet;
}
/************************************************************************/
/* �������                                                             */
/************************************************************************/
bool CEmsMainWnd::ImportQuesToStore()
{
	bool bRet = false;
	do
	{
		int iSel = m_pSubjectCom->GetCurSel();
		if (-1 == iSel)
		{
			MessageBox(NULL, _T("��ѡ���Ŀ"), _T("����"), MB_OK|MB_ICONWARNING);
			break;
		}
		CDuiString strSubjectName = m_pSubjectCom->GetItemAt(iSel)->GetText();
		iSel = m_pQuestionTypeCom->GetCurSel();
		if (-1 == iSel)
		{
			MessageBox(NULL, _T("��ѡ������"), _T("����"), MB_OK | MB_ICONWARNING);
			break;
		}
		CDuiString strQuestinName = m_pQuestionTypeCom->GetItemAt(iSel)->GetText();
		CDuiString strFileName = m_pFileNameText->GetText();
		if (strFileName.IsEmpty())
		{
			MessageBox(NULL, _T("��ѡ��Ҫ����������ļ�"), _T("����"), MB_OK | MB_ICONWARNING);
			break;
		}
		m_mainLogic.ImportQuestion(m_strFileName.GetBuffer(0), _T("Sheet1"), strSubjectName.GetData(), strQuestinName.GetData());
		bRet = true;
	} while (false);
	return bRet;
}
/************************************************************************/
/* ������������Ӧ����                                                 */
/************************************************************************/
LRESULT CEmsMainWnd::OnImpotResponse(LPARAM lParam)
{
	MessageBoxA(NULL, (char*)lParam, "��ʾ", MB_OK | MB_ICONINFORMATION);
	m_strFileName.Empty();
	m_pFileNameText->SetText(_T(""));
	m_pFileNameText->NeedParentUpdate();
	return 0;
}
/************************************************************************/
/* �������                                                             */
/************************************************************************/
bool CEmsMainWnd::ScanQues()
{
	bool bRet = false;
	do
	{
		int iSel = m_pScanSubjectCom->GetCurSel();
		if (-1 == iSel)
		{
			MessageBox(NULL, _T("��ѡ���Ŀ"), _T("����"), MB_OK | MB_ICONWARNING);
			break;
		}
		CDuiString strSubjectName = m_pScanSubjectCom->GetItemAt(iSel)->GetText();
		iSel = m_pScanQuestionTypeCom->GetCurSel();
		if (-1 == iSel)
		{
			MessageBox(NULL, _T("��ѡ������"), _T("����"), MB_OK | MB_ICONWARNING);
			break;
		}
		CDuiString strQuestinName = m_pScanQuestionTypeCom->GetItemAt(iSel)->GetText();
		m_mainLogic.ScanQuestion(strSubjectName.GetData(), strQuestinName.GetData());
		bRet = true;
	} while (false);
	return bRet;
}
/************************************************************************/
/* ����չʾ                                                             */
/************************************************************************/
LRESULT CEmsMainWnd::OnScanQuestion(WPARAM wParam, LPARAM lParam)
{
	RESULT_TYPE enType = (RESULT_TYPE)wParam;
	m_nCurQueNum = 0;
	m_nCurPage = 0;
	switch (enType)
	{
	case CCF_PROGRAMME_RET:	ScanCCFQuestion((char*)lParam);	break;
	default:
		break;
	}
	return 0;
}
/************************************************************************/
/* ���CCF����                                                          */
/************************************************************************/
void CEmsMainWnd::ScanCCFQuestion(char *szData)
{
	USES_CONVERSION;
	TCHAR * szwData = A2W(szData);
	std::wstring strData = szwData;
	m_vmScanQue.clear();
	m_mainLogic.String2VecMap(strData, m_vmScanQue);
	ScanCCFLimit(0, SHOW_CCF_NUM);
}
/************************************************************************/
/* ���ߺ�������ʾָ��������CCF����                                                */
/************************************************************************/
void CEmsMainWnd::ScanCCFLimit(int iBegin, int iOffset, bool bNext)
{
	std::map<std::wstring, std::wstring>::iterator itMap;
	std::wstring strQue;
	if (m_vmScanQue.empty() || iBegin < 0 || (unsigned)iBegin >= m_vmScanQue.size())
	{
		return;
	}
	for (size_t i = iBegin, j = 0; i < m_vmScanQue.size() && j < (unsigned)iOffset; ++i,++j)
	{
		itMap = m_vmScanQue[i].find(_T("������"));
		strQue = strQue + _T("<b>") + itMap->first + _T("</b>      ");
		strQue = strQue + itMap->second + _T("<p>");
		itMap = m_vmScanQue[i].find(_T("��������"));
		strQue = strQue + _T("<b>") + itMap->first + _T("</b>      ");
		strQue = strQue + itMap->second + _T("<p>");
		itMap = m_vmScanQue[i].find(_T("ʱ������"));
		strQue = strQue + _T("<b>") + itMap->first + _T("</b>      ");
		strQue = strQue + itMap->second + _T("<p>");
		itMap = m_vmScanQue[i].find(_T("�ڴ�����"));
		strQue = strQue + _T("<b>") + itMap->first + _T("</b>      ");
		strQue = strQue + itMap->second + _T("<p>");
		itMap = m_vmScanQue[i].find(_T("��������"));
		strQue = strQue + _T("<b>") + itMap->first + _T("</b>      ");
		strQue = strQue + itMap->second + _T("<p>\r\n\r\n");
		if (bNext)
		{
			++m_nCurQueNum;
		}
	}
	if ((unsigned)m_nCurQueNum >= m_vmScanQue.size())   m_nCurQueNum = m_vmScanQue.size() - 1;
	m_pQuestionScanText->SetText(strQue.c_str());
	m_pQuestionScanText->NeedParentUpdate();
}
/************************************************************************/
/* ���ɿ�����Ϣ���Ծ���Ϣ                                               */
/************************************************************************/
void CEmsMainWnd::GeneratePaper()
{
	int iSel = m_pGeneratePaperSubjectCom->GetCurSel();
	if (iSel == -1)
	{
		MessageBox(NULL, _T("��ѡ���Ŀ������"), _T("����"), MB_OK | MB_ICONWARNING);
		return;
	}
	CDuiString strSubjectName = m_pScanSubjectCom->GetItemAt(iSel)->GetText();
	SYSTEMTIME sysStartTime = m_pStartDate->GetTime();
	CDuiString strStartTime =  m_pStartDate->GetText();
	SYSTEMTIME sysEndTime = m_pEndDate->GetTime();
	CDuiString strEndTime = m_pEndDate->GetText();
	CDuiString strStartHour = m_pStartHour->GetText();
	CDuiString strStartMin = m_pStartMinute->GetText();
	CDuiString strEndHour = m_pEndHour->GetText();
	CDuiString strEndMin = m_pEndMinute->GetText();
	int iStartHour = _wtoi(strStartHour.GetData());
	int iStartMin = _wtoi(strStartMin.GetData());
	int iEndHour = _wtoi(strEndHour.GetData());
	int iEndMin = _wtoi(strEndMin.GetData());
	if (!ValidateExamTime(sysStartTime, sysEndTime, iStartHour, iEndHour, iStartMin, iEndMin))
	{
		return;
	}
	strStartTime = strStartTime + _T(" ") + strStartHour + _T(":") + strStartMin + _T(":00");
	strEndTime = strEndTime + _T(" ") + strEndHour + _T(":") + strEndMin + _T(":00");
	m_mainLogic.AddExamMsg(strSubjectName.GetData(), strStartTime.GetData(), strEndTime.GetData());
	CDuiString strExamText = _T("<b>�����ɿ�����Ϣ��\r\n��Ŀ��\r\n</b>");
	strExamText += strSubjectName;
	strExamText += _T("\r\n<b>��ʼʱ�䣺\r\n</b>");
	strExamText += strStartTime;
	strExamText += _T("\r\n<b>����ʱ�䣺\r\n</b>");
	strExamText += strEndTime;
	m_pGenerateExamText->SetText(strExamText);

}
/************************************************************************/
/* ���ߺ�����У�鿼������                                                         */
/************************************************************************/
bool CEmsMainWnd::ValidateExamTime(SYSTEMTIME sysStartTime, SYSTEMTIME sysEndTime, int iStartHour, int iEndHour, int iStartMin, int iEndMin)
{
	bool bRet = false;
	do 
	{
		if (iStartHour < 0 || iStartHour > 23
			|| iStartMin < 0 || iStartMin > 59
			|| iEndHour < 0 || iEndHour > 23
			|| iEndHour < 0 || iEndMin > 59)
		{
			MessageBox(NULL, _T("ʱ�ֲ�����Ҫ�󣡣���"), _T("����"), MB_OK | MB_ICONWARNING);
			break;
		}
		if (sysEndTime.wYear < sysStartTime.wYear || sysEndTime.wMonth < sysStartTime.wMonth
			|| sysEndTime.wDay < sysStartTime.wDay)
		{
			MessageBox(NULL, _T("���Խ������ڲ���С�ڿ�ʼ����"), _T("����"), MB_OK | MB_ICONWARNING);
			break;
		}
		if (sysEndTime.wDay != sysStartTime.wDay)
		{
			MessageBox(NULL, _T("����ʱ�䲻�ܸ�ҹ"), _T("����"), MB_OK | MB_ICONWARNING);
			break;
		}
		if (iEndHour < iStartHour)
		{
			MessageBox(NULL, _T("���Խ������ڲ���С�ڿ�ʼ����"), _T("����"), MB_OK | MB_ICONWARNING);
			break;
		}
		if (iEndHour == iStartHour && iEndMin < iStartMin+10)
		{
			MessageBox(NULL, _T("���Խ�����������Ҫ�ȿ�ʼ������10����"), _T("����"), MB_OK | MB_ICONWARNING);
			break;
		}
		bRet = true;
	} while (false);
	return bRet;
}
/************************************************************************/
/* ���ӿ�����Ϣ���ؽ����Ӧ��Ϣ������                                 */
/************************************************************************/
LRESULT CEmsMainWnd::OnAddExamRet(LPARAM lParam)
{
	char * szRet = (char*)lParam;
	if (szRet[0] == '0')//ͬһ��Ŀ����ʱ���ͻ
	{
		m_pGenerateExamText->SetText(_T("<b>δ�ɹ����ɿ�����Ϣ\r\nʧ��ԭ��:</b>ʱ���ͻ"));
		MessageBoxA(NULL, "�ÿ�Ŀ����ʱ���ͻ", "֪ͨ", MB_OK | MB_ICONWARNING);
	}
	else if (szRet[0] == '1')//����ʧ��
	{
		m_pGenerateExamText->SetText(_T("<b>δ�ɹ����ɿ�����Ϣ\r\nʧ��ԭ��:</b>��������ʧ��"));
		MessageBoxA(NULL, "���ӿ�����Ϣʧ��", "֪ͨ", MB_OK | MB_ICONWARNING);
	}
	else if (szRet[0] == '9')//���ӳɹ�
	{
		MessageBoxA(NULL, "���ӿ�����Ϣ�ɹ�", "֪ͨ", MB_OK | MB_ICONINFORMATION);
	}

	return 0;
}
/************************************************************************/
/* չʾ����������Ϣ                                                     */
/************************************************************************/
LRESULT CEmsMainWnd::OnGetExamDescription(LPARAM lParam)
{
	USES_CONVERSION;
	TCHAR * szwData = A2W((char*)lParam);
	std::wstring strData = szwData;
	strData = strData.substr(strData.find(RES_VW) + _tcslen(RES_VW));
	m_pGeneratePaperText->SetText(strData.c_str());
	return 0;
}

/************************************************************************/
/* չʾ����������Ϣ                                                     */
/************************************************************************/
LRESULT CEmsMainWnd::OnGetApplyMsg(LPARAM lParam)
{
	USES_CONVERSION;
	std::vector<map<std::wstring, std::wstring> > vecMap;
	m_pApplyList->RemoveAll();
	TCHAR * szwData = A2W((char*)lParam);
	m_mainLogic.String2VecMap(szwData, vecMap);
	map<std::wstring, std::wstring>::iterator itMap;
	int iTag = 0;
	m_vsApplyId.clear();
	for each (map<std::wstring, std::wstring> row in vecMap)
	{
		CListTextElementUI* pListElement = new CListTextElementUI;
		pListElement->SetTag(iTag);
		m_pApplyList->Add(pListElement);
		if ((itMap = row.find(_T("���Ա��")))!= row.end())
		{
			pListElement->SetText(0, itMap->second.c_str());
			m_vsApplyId.push_back(itMap->second);
		}
		if ((itMap = row.find(_T("ѧ��"))) != row.end())
		{
			pListElement->SetText(1, itMap->second.c_str());
		}
		if ((itMap = row.find(_T("����"))) != row.end())
		{
			pListElement->SetText(2, itMap->second.c_str());
		}
		if ((itMap = row.find(_T("���ڰ༶"))) != row.end())
		{
			pListElement->SetText(3, itMap->second.c_str());
		}
		if ((itMap = row.find(_T("���Կ�Ŀ"))) != row.end())
		{
			pListElement->SetText(4, itMap->second.c_str());
		}
		if ((itMap = row.find(_T("��ʼʱ��"))) != row.end())
		{
			pListElement->SetText(5, itMap->second.c_str());
		}
		if ((itMap = row.find(_T("����ʱ��"))) != row.end())
		{
			pListElement->SetText(6, itMap->second.c_str());
		}
		++iTag;
	}
	return 0;
}

/************************************************************************/
/* �������뿼�ԵĿ��Ա��                                               */
/************************************************************************/
DuiLib::CDuiString CEmsMainWnd::GetApplyExamId()
{
	int nCount = m_pApplyList->GetCount();
	for (int i = 0; i < nCount; ++i)
	{
		CDuiString str = m_pApplyList->GetItemAt(i)->GetText();
	}

	return L"";
}
/************************************************************************/
/* չʾ������Ϣ                                                         */
/************************************************************************/
void CEmsMainWnd::PrintApplyMsg()
{
	CDuiString strSubject = _T("");
	CDuiString strNumber = m_pApplyNumberEdit->GetText();
	int iCurSel = m_pApplySubjectCom->GetCurSel();
	if (iCurSel != -1)
	{
		strSubject = m_pApplySubjectCom->GetItemAt(iCurSel)->GetText();
	}
	m_mainLogic.GetApplyMsg(strSubject.GetData(), strNumber.GetData());
}

/************************************************************************/
/* ��ʾ�û��Ĵ���Ϣ��Ϣ����                                           */
/************************************************************************/
LRESULT CEmsMainWnd::OnGetUserAnswer(LPARAM lParam)
{
	USES_CONVERSION;
	std::vector<std::map<std::wstring, std::wstring> > vecMap;
	TCHAR * szwData = A2W((char*)lParam);
	m_mainLogic.String2VecMap(szwData, vecMap);
	m_mQuesAndAns.clear();
	std::wstring strContent = _T("");
	if (!vecMap.empty())
	{
		m_mQuesAndAns = vecMap.at(0);
		std::map<std::wstring, std::wstring>::iterator it;
		it = m_mQuesAndAns.find(_T("score"));
		strContent.append(_T("<b>������</b>")).append(it->second).append(_T("\r\n<b>��������</b>\r\n"));
		it = m_mQuesAndAns.find(_T("question"));
		strContent.append(it->second).append(_T("\r\n<b>�û���</b>\r\n"));
		it = m_mQuesAndAns.find(_T("answer"));
		std::wstring strAnswer = it->second;
		strContent.append(CUtils::ReplaceAll(strAnswer,_T("\\r\\n"),_T("\r\n")));
	}
	if (strContent == _T(""))
	{
		strContent.append(_T("û����..."));
	}
	m_pCheckExamContentTxt->SetText(strContent.c_str());
	m_pCheckExamContentTxt->NeedParentUpdate();
	return 0;
}

/************************************************************************/
/* �����û��Ĵ�                                                       */
/************************************************************************/
void CEmsMainWnd::PopUserAnwser()
{
	if (!m_mQuesAndAns.empty())
	{
		std::wstring strAnswer = m_mQuesAndAns.find(_T("answer"))->second;
		MessageBox(NULL,CUtils::ReplaceAll(strAnswer,_T("\\r\\n"),_T("\n")).c_str() ,_T("�û���"), MB_OK);
	}
}

/************************************************************************/
/* �����ο���                                                         */
/************************************************************************/
void CEmsMainWnd::PopAnswer()
{
	if (m_mQuesAndAns.empty())
	{
		return;
	}
	std::wstring questionId = m_mQuesAndAns.find(_T("questionId"))->second;
	m_mainLogic.GetAnswerById(questionId);
}

/************************************************************************/
/* ��ʾ�ο�����Ϣ����                                                 */
/************************************************************************/
LRESULT CEmsMainWnd::OnGetAnswer(LPARAM lParam)
{
	USES_CONVERSION;
	std::vector<std::map<std::wstring, std::wstring> > vecMap;
	TCHAR * szwData = A2W((char*)lParam);
	m_mainLogic.String2VecMap(szwData, vecMap);
	if (!vecMap.empty())
	{
		std::wstring strAnswer = vecMap.at(0).find(_T("answer"))->second;
		MessageBox(NULL, strAnswer.c_str(), _T("�ο���"), MB_OK);
	}
	
	return 0;
}

/************************************************************************/
/* У������ĳɼ��Ƿ�Ϸ�                                               */
/************************************************************************/
bool CEmsMainWnd::ValidateScore(CDuiString strScore)
{
	bool bRet = false;
	do 
	{
		if (strScore.IsEmpty() || strScore.Compare(_T("")) == 0)
		{
			MessageBox(NULL, _T("������ɼ�"), _T("��ʾ"), MB_OK | MB_ICONWARNING);
			break;
		}
		std::wstring strOriginScore;
		try
		{
			strOriginScore = m_mQuesAndAns.find(_T("score"))->second;
		}
		catch (CException* e)
		{
			e->AssertValid();
			MessageBox(NULL, _T("��ȡԭʼ�ɼ��쳣"), _T("��ʾ"), MB_OK | MB_ICONWARNING);
			break;
		}
		try
		{
			float fOriginScore = std::stof(strOriginScore);
			for (int i = 0; i < strScore.GetLength(); ++i)
			{
				if (strScore[i] > (int)'9' || strScore[i] < (int)'0')
				{
					MessageBox(NULL, _T("���зǷ��ַ�"), _T("��ʾ"), MB_OK | MB_ICONWARNING);
					break;
				}
			}
			float fScore = (float)_wtof(strScore.GetData());
			if (fScore > fOriginScore || fScore <  0)
			{
				MessageBox(NULL, _T("�ɼ���С��Χ���Ϸ�"), _T("��ʾ"), MB_OK | MB_ICONWARNING);
				break;
			}
		}
		catch (CException* e)
		{
			e->AssertValid();
			MessageBox(NULL, _T("�ɼ���ʽ����"), _T("��ʾ"), MB_OK | MB_ICONWARNING);
			break;
		}
		bRet = true;
	} while (false);
	return bRet;
}

/************************************************************************/
/* ������һ����Ĵ�                                                   */
/************************************************************************/
void CEmsMainWnd::GetNextAnswer(CDuiString strScore)
{
	std::wstring strApplyId = m_mQuesAndAns.find(_T("applyId"))->second;
	std::wstring strQuestionId = m_mQuesAndAns.find(_T("questionId"))->second;
	int nCurSel = m_pCheckExamSubjectCom->GetCurSel();
	if (nCurSel == -1)
	{
		MessageBox(NULL, _T("��ѡ���Ŀ"), _T("��ʾ"), MB_OK);
		return;
	}
	int nCurSelType = m_pCheckExamQueTypeCom->GetCurSel();
	if (nCurSelType == -1)
	{
		MessageBox(NULL, _T("��ѡ������"), _T("��ʾ"), MB_OK);
		return;
	}
	CDuiString strSubject = m_pCheckExamSubjectCom->GetItemAt(nCurSel)->GetText();
	CDuiString strType = m_pCheckExamQueTypeCom->GetItemAt(nCurSel)->GetText();
	m_mainLogic.GetCheckNextAnswer(strApplyId, strQuestionId, strScore.GetData(), strSubject.GetData(), strType.GetData());
	
}

/************************************************************************/
/* ��ʾ�ɼ�                                                             */
/************************************************************************/
LRESULT CEmsMainWnd::OnGradeShow(LPARAM lParam)
{
	m_pGradeList->RemoveAll();
	USES_CONVERSION;
	char * szData = (char*)lParam;
	TCHAR * szwData = A2W(szData);
	std::wstring strData = szwData;
	std::vector<std::map<std::wstring, std::wstring> > vmGrade;
	m_mainLogic.String2VecMap(szwData, vmGrade);
	std::map<std::wstring, std::wstring>::iterator it;
	CListTextElementUI* pListElement = nullptr;
	int nNum = 0;
	for each (std::map<std::wstring, std::wstring> var in vmGrade)
	{
		pListElement = new CListTextElementUI;
		pListElement->SetTag(nNum++);
		m_pGradeList->Add(pListElement);
		it = var.find(_T("number"));
		if (it != var.end())
		{
			pListElement->SetText(0, it->second.c_str());
		}
		it = var.find(_T("username"));
		if (it != var.end())
		{
			pListElement->SetText(1, it->second.c_str());
		}
		it = var.find(_T("subjectName"));
		if (it != var.end())
		{
			pListElement->SetText(2, it->second.c_str());
		}
		it = var.find(_T("startTime"));
		if (it != var.end())
		{
			pListElement->SetText(3, it->second.c_str());
		}
		it = var.find(_T("endTime"));
		if (it != var.end())
		{
			pListElement->SetText(4, it->second.c_str());
		}
		it = var.find(_T("score"));
		if (it != var.end())
		{
			pListElement->SetText(5, it->second.c_str());
		}
	}

	return 0;
}
