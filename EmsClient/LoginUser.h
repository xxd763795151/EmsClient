#pragma once
class CLoginUser
{
public:
	~CLoginUser();
	static CLoginUser * GetLoginUser();
	
	static void DestroyLoginUser();
	
	void SetUserName(std::wstring strUsername)
	{
		m_strUsername = strUsername;
	}
	void SetPassword(std::wstring strPassword)
	{
		m_strPassword = strPassword;
	}
	inline std::wstring GetUsername()
	{
		return m_strUsername;
	}
	inline std::wstring GetPassword()
	{
		return m_strPassword;
	}

private:
	CLoginUser();

	static CLoginUser * m_pThis;
	std::wstring m_strUsername;
	std::wstring m_strPassword;
};

