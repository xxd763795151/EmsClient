#pragma once

class CUtils
{
public:
	CUtils();
	~CUtils();

public:
	void static OpenFileDialog(TCHAR * szFileName);
	void static SaveFileDialog();
	void static ChoiceFileDialog();
	std::wstring static ReplaceAll(std::wstring & str, std::wstring old, std::wstring replace);
};

