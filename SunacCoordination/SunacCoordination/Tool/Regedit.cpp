#include "stdAfx.h"
#include "Regedit.h"

static CHAR* RegWcharToChar(const WCHAR * pchar)
{
	CHAR *m_pchar = NULL;

	int iSize = WideCharToMultiByte(CP_ACP,0,pchar,-1,NULL,0,NULL,NULL);
	//m_pchar = new CHAR[outlen];
	m_pchar = (char*)malloc((iSize+1)/**sizeof(char)*/);
	WideCharToMultiByte(CP_ACP, 0, pchar, -1, m_pchar, iSize, NULL, NULL);

	return m_pchar;
}

Regedit::Regedit(void)
{
}

Regedit::~Regedit(void)
{
}


UINT SetValue2(HKEY rootkey,LPCTSTR SubKey,LPCWSTR Name,DWORD dwValue )
{
	int irc = 0;
	HKEY hkey;
	HKEY hTempKey;  
	if (ERROR_SUCCESS == RegOpenKeyEx(MAINKEY, SubKey, 0, KEY_ALL_ACCESS, &hkey))  
	{  
		if (ERROR_SUCCESS == ::RegCreateKey(rootkey, SubKey, &hTempKey))  
		{  
			
		}  
	}  
	if (ERROR_SUCCESS != ::RegSetValueEx(hTempKey, Name, 0, REG_DWORD, (CONST BYTE*)&dwValue, sizeof(DWORD)))  
	{  
		//cout <<"写入注册表失败"<< endl;  
		irc = 1;
	}  
	::RegCloseKey(hkey); 
	return irc;
}


UINT Regedit::SetValue(HKEY rootkey,LPCTSTR SubKey,LPCTSTR Name1,double mmnumber)
{
	int i=1;//i=1为成功
	HKEY hKey; 
	if (RegOpenKeyEx(rootkey,SubKey,0,KEY_ALL_ACCESS,&hKey)!= ERROR_SUCCESS) 
	{ 
		if (ERROR_SUCCESS!=RegCreateKey(rootkey,SubKey,&hKey)) 
		{
			//JHCOM_Print(32,NULL,1); 
			i=0;
		}
	}
	char data[256] = "";
	sprintf(data,"%f",mmnumber);
	UINT cbLen=(UINT)strlen(data);
	if(RegSetValueEx(hKey,Name1,0,REG_DWORD,(const unsigned char *)data,cbLen)!=ERROR_SUCCESS) 
	{
		//JHCOM_Print(33,Name1,1); 
		i=0;
	}
	RegCloseKey(hKey); 
	return i;
}

UINT Regedit::QueryValue(HKEY rootkey,LPCTSTR SubKey,LPCTSTR Name1,double& mmnumber)
{
	int i=1;//1为成功
	HKEY hKey;
	DWORD dwtype;
	char content[256];
	DWORD cbData = 256;
	if (RegOpenKeyEx(rootkey,SubKey,0,KEY_READ,&hKey)!= ERROR_SUCCESS) 
	{ 
		//JHCOM_Print(34,NULL,1);
		i=0;
	}
	if(RegQueryValueEx(hKey,Name1, NULL, &dwtype,(unsigned char *)content,&cbData)!=ERROR_SUCCESS) 
	{
		//JHCOM_Print(35,Name1,1);i=0;
	}
	else
	{
		mmnumber=atof(content);
	}
	RegCloseKey(hKey);
	return i;
}

UINT Regedit::QueryString(HKEY rootkey,LPCTSTR SubKey,LPCTSTR Name1,CString& kword)
{
	int i=1;//1为成功
	HKEY hKey;
	DWORD dwtype;
	char content[256]="";
	DWORD cbData = 256;
	if (RegOpenKeyEx(rootkey,SubKey,0,KEY_READ,&hKey)!= ERROR_SUCCESS) 
	{ 
		//JHCOM_Print(34,NULL,1);
		i=0;
	}
	char *cname = RegWcharToChar(Name1);
	if(RegQueryValueExA(hKey,cname, NULL, &dwtype,( unsigned char *)content,&cbData)!=ERROR_SUCCESS) 
	{
		//JHCOM_Print(35,Name1,1);i=0;
	}
	else
	{
		kword= CString(content);
	}
	RegCloseKey(hKey);
	free(cname);
	return i;
}

UINT Regedit::SetString(HKEY rootkey,LPCTSTR SubKey,LPCTSTR Name1,LPCTSTR kword)
{
	int i=1;//i=1为成功
	HKEY hKey; 
	if (RegOpenKeyEx(rootkey,SubKey,0,KEY_ALL_ACCESS,&hKey)!= ERROR_SUCCESS) 
	{ 
		if (ERROR_SUCCESS!=RegCreateKey(rootkey,SubKey,&hKey)) 
		{
			//JHCOM_Print(34,NULL,1); 
			i=0;
		}
	}
	CString Data;
	Data.Format(L"%s",kword);
	char* data=RegWcharToChar(Data.GetBuffer());
	UINT cbLen=(UINT)strlen(data);
	char *cname = RegWcharToChar(Name1);
	if(RegSetValueExA(hKey,cname,0,REG_SZ,(const unsigned char *)data,cbLen)!=ERROR_SUCCESS) 
	{
		//JHCOM_Print(35,Name1,1); 
		i=0;
	}
	RegCloseKey(hKey); 
	free(data);
	free(cname);
	return i;
}

/*-------------------------------------------------------------------------
 * Function Name   : GetDouble
 * Description     : Get the double value in the subkey SubKey=L"S-1-5-18\\AutoPipe" for the input name.
 * Note            : common function.
 * Input           : name - Key name to get the related double value.
 * Output          : NULL.
 * Return          : Double value for input name.
 --------------------------------------------------------------------------*/
double Regedit::GetDouble(CString name)
{
	double msb = 0;
	int rcc = 0;
	LPCTSTR SubKey=SUBKEY;

	int ar = Regedit::QueryValue(MAINKEY,SubKey,name,msb);
	if(ar == 1)
		return msb;
	else 
		return 0;
}

/*-------------------------------------------------------------------------
 * Function Name   : SetDouble
 * Description     : Set the double value in the subkey SubKey=L"S-1-5-18\\AutoPipe" for the input name.
 * Note            : common function.
 * Input           : name - Key name to set the related double value.
 *                   value - New value for the key name.
 * Output          : NULL.
 * Return          : Double value for input name.
 --------------------------------------------------------------------------*/
void Regedit::SetDouble(CString name, double value)
{
	LPCTSTR SubKey=SUBKEY;
	Regedit::SetValue(MAINKEY,SubKey,name,value);
}

/*-------------------------------------------------------------------------
 * Function Name   : GetString
 * Description     : Get the string value in the subkey SubKey=L"S-1-5-18\\AutoPipe" for the input name.
 * Note            : common function.
 * Input           : name - Key name to get the related string value.
 * Output          : NULL.
 * Return          : string value for input name.
 --------------------------------------------------------------------------*/
CString Regedit::GetString(CString name)
{
	CString msb = L"";
	LPCTSTR SubKey=SUBKEY;

	int ar = Regedit::QueryString(MAINKEY,SubKey,name,msb);

	if(ar == 1)
		return msb;
	else 
		return L"";
}

/*-------------------------------------------------------------------------
 * Function Name   : SetString
 * Description     : Set the CString value in the subkey SubKey=L"S-1-5-18\\AutoPipe" for the input name.
 * Note            : common function.
 * Input           : name - Key name to set the related CString value.
 *                   value - New value for the key name.
 * Output          : NULL.
 * Return          : CString value for input name.
 --------------------------------------------------------------------------*/
int Regedit::SetString(CString name,CString msb)
{
	int rcc = 0;
	LPCTSTR SubKey=SUBKEY;

	int ar = Regedit::SetString(MAINKEY,SubKey,name,msb);
	if(ar == 1)
		return 1;
	else 
		return 0;
}