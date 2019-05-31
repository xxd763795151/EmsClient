#pragma once

class ImportQuestionUI : public CContainerUI
{
public:
	ImportQuestionUI()
	{
		CDialogBuilder builder;
		CContainerUI* pComputerExamine = static_cast<CContainerUI*>(builder.Create(_T("importQuestion.xml"), (UINT)0));
		if( pComputerExamine ) {
			this->Add(pComputerExamine);
		}
		else {
			this->RemoveAll();
			return;
		}
	}
	~ImportQuestionUI()
	{
		delete this;
	}
};

class CDialogBuilderCallbackEx : public IDialogBuilderCallback
{
public:	
	CControlUI* CreateControl(LPCTSTR pstrClass) 
	{
		if( _tcscmp(pstrClass, _T("ImportQuestion")) == 0 ) return new ImportQuestionUI;
		return NULL;
	}
};