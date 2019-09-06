#include "stdAfx.h"
#include "Regedit.h"
#include "../Common/ComFun_Convert.h"

Regedit::Regedit(void)
{
}

Regedit::~Regedit(void)
{
}
UINT Regedit::SetValue(HKEY rootkey,LPCTSTR SubKey,wchar_t *Name1,double mmnumber)
{
	int i=1;//i=1为成功
	HKEY hKey; 
	if (RegOpenKeyEx(rootkey,SubKey,0,KEY_ALL_ACCESS,&hKey)!= ERROR_SUCCESS) 
	{ 
		if (ERROR_SUCCESS!=RegCreateKey(rootkey,SubKey,&hKey)) 
		{
			i=0;
		}
	}
	char data[256] = "";
	sprintf(data,"%f",mmnumber);
	UINT cbLen=(UINT)strlen(data);
	if(RegSetValueEx(hKey,Name1,0,REG_SZ,(const unsigned char *)data,cbLen)!=ERROR_SUCCESS) 
	{
		i=0;
	}
	RegCloseKey(hKey); 
	return i;
}

UINT Regedit::QueryValue(HKEY rootkey,LPCTSTR SubKey,wchar_t *Name1,double& mmnumber)
{
	int i=1;//1为成功
	HKEY hKey;
	DWORD dwtype;
	char content[256];
	DWORD cbData = 256;
	if (RegOpenKeyEx(rootkey,SubKey,0,KEY_ALL_ACCESS,&hKey)!= ERROR_SUCCESS) 
	{ 
		i=0;
	}
	if(RegQueryValueEx(hKey,Name1, NULL, &dwtype,(unsigned char *)content,&cbData)!=ERROR_SUCCESS) 
	{
		i=0;
	}
	else
	{
		mmnumber=atof(content);
	}
	RegCloseKey(hKey);
	return i;
}


UINT Regedit::SetValue(HKEY rootkey,LPCTSTR SubKey,wchar_t *Name1,int mmnumber)
{
	int i=1;//i=1为成功
	HKEY hKey; 
	if (RegOpenKeyEx(rootkey,SubKey,0,KEY_ALL_ACCESS,&hKey)!= ERROR_SUCCESS) 
	{ 
		if (ERROR_SUCCESS!=RegCreateKey(rootkey,SubKey,&hKey)) 
		{
			i=0;
		}
	}
	char data[256] = "";
	sprintf(data,"%d",mmnumber);
	UINT cbLen=(UINT)strlen(data);
	if(RegSetValueEx(hKey,Name1,0,REG_SZ,(const unsigned char *)data,cbLen)!=ERROR_SUCCESS) 
	{
		i=0;
	}
	RegCloseKey(hKey); 
	return i;
}

UINT Regedit::QueryValue(HKEY rootkey,LPCTSTR SubKey,wchar_t *Name1,int& mmnumber)
{
	int i=1;//1为成功
	HKEY hKey;
	DWORD dwtype;
	char content[256];
	DWORD cbData = 256;
	if (RegOpenKeyEx(rootkey,SubKey,0,KEY_ALL_ACCESS,&hKey)!= ERROR_SUCCESS) 
	{ 
		i=0;
	}
	if(RegQueryValueEx(hKey,Name1, NULL, &dwtype,(unsigned char *)content,&cbData)!=ERROR_SUCCESS) 
	{
		i=0;
	}
	else
	{
		mmnumber=atoi(content);
	}
	RegCloseKey(hKey);
	return i;
}

UINT Regedit::QueryString(HKEY rootkey,LPCTSTR SubKey,wchar_t *Name1,CString& kword)
{
	int i=1;//1为成功
	HKEY hKey;
	DWORD dwtype;
	char content[256]="";
	DWORD cbData = 256;
	if (RegOpenKeyEx(rootkey,SubKey,0,KEY_ALL_ACCESS,&hKey)!= ERROR_SUCCESS) 
	{ 
		i=0;
	}
	char *cname = JHCOM_WcharToChar(Name1);
	if(RegQueryValueExA(hKey,cname, NULL, &dwtype,( unsigned char *)content,&cbData)!=ERROR_SUCCESS) 
	{
		i=0;
	}
	else
	{
		kword= CString(content);
	}
	RegCloseKey(hKey);
	free(cname);
	return i;
}

UINT Regedit::SetString(HKEY rootkey,LPCTSTR SubKey,wchar_t *Name1,wchar_t* kword)
{
	int i=1;//i=1为成功
	HKEY hKey; 
	if (RegOpenKeyEx(rootkey,SubKey,0,KEY_ALL_ACCESS,&hKey)!= ERROR_SUCCESS) 
	{ 
		if (ERROR_SUCCESS!=RegCreateKey(rootkey,SubKey,&hKey)) 
		{
			i=0;
		}
	}
	CString Data;
	Data.Format(L"%s",kword);
	char* data=JHCOM_WcharToChar(Data.GetBuffer());
	UINT cbLen=(UINT)strlen(data);
	char *cname = JHCOM_WcharToChar(Name1);
	if(RegSetValueExA(hKey,cname,0,REG_SZ,(const unsigned char *)data,cbLen)!=ERROR_SUCCESS) 
	{
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
	LPCTSTR SubKey=L"SUNAC-TYCAD-2019\\RCTYCAD";

	int ar = Regedit::QueryValue(HKEY_USERS,SubKey,name.GetBuffer(),msb);
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
	LPCTSTR SubKey=L"SUNAC-TYCAD-2019\\RCTYCAD";
	Regedit::SetValue(HKEY_USERS,SubKey,name.GetBuffer(),value);
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
	CString msb = "";
	int rcc = 0;
	LPCTSTR SubKey=L"SUNAC-TYCAD-2019\\RCTYCAD";

	int ar = Regedit::QueryString(HKEY_USERS,SubKey,name.GetBuffer(),msb);
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
	LPCTSTR SubKey=L"SUNAC-TYCAD-2019\\RCTYCAD";

	int ar = Regedit::SetString(HKEY_USERS,SubKey,name.GetBuffer(),msb.GetBuffer());
	if(ar == 1)
		return 1;
	else 
		return 0;
}

/*-------------------------------------------------------------------------
 * Function Name   : GetDouble
 * Description     : Get the double value in the subkey SubKey=L"S-1-5-18\\AutoPipe" for the input name.
 * Note            : common function.
 * Input           : name - Key name to get the related double value.
 * Output          : NULL.
 * Return          : Double value for input name.
 --------------------------------------------------------------------------*/
int Regedit::GetInt(CString name)
{
	int msb = 0;
	int rcc = 0;
	LPCTSTR SubKey=L"SUNAC-TYCAD-2019\\RCTYCAD";

	int ar = Regedit::QueryValue(HKEY_USERS,SubKey,name.GetBuffer(),msb);
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
void Regedit::SetInt(CString name, int value)
{
	LPCTSTR SubKey=L"SUNAC-TYCAD-2019\\RCTYCAD";
	Regedit::SetValue(HKEY_USERS,SubKey,name.GetBuffer(),value);
}