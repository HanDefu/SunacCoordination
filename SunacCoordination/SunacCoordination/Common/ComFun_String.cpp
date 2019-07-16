#include "stdafx.h"
#include <fstream>
#include <sstream>
#include <dbxutil.h >
#include <brletrav.h>
#include <brbvtrav.h>
#include <geell3d.h>
#include <genurb3d.h>
#include <geextsf.h>
#include <gexbndsf.h>
#include <gecylndr.h>
#include <gecsint.h>
#include <dbxline.h>
#include <float.h>
#include <gecurv3d.h>
#include <geblok3d.h >
#include <brbstrav.h>
#include <brbctrav.h>
#include <brsftrav.h>
#include <algorithm>
#include <gecomp2d.h>
#include "ComFun_Def.h"
using namespace std;

//文件操作
bool JHCom_FileExist(CString FileName)
{
     CFileFind fFind;
     return fFind.FindFile(FileName) == TRUE?true:false;
}

int JHCom_RemoveStrSpace(char * str)
{
	JHCom_RemoveSpaceHead(str);
	JHCom_RemoveSpaceTail(str);
	JHCom_RemoveTailFileEndSign(str);
	return 0;
}

int  JHCom_RemoveSpaceHead(char * instr)
{
	if (NULL == instr)
		return 1;

	if(strlen(instr) > 256)
		return 1;
	char *copyInStr = (char *)malloc(strlen(instr) + 1);
	strcpy(copyInStr,instr);

	int i = 0;
	while(copyInStr[i] == ' ' || copyInStr[i] == ' ' || copyInStr[i] == 10)
		++i;
	strcpy(instr, &(copyInStr[i]));
	free(copyInStr);
	return 0;
}

int  JHCom_RemoveSpaceTail(char * str)
{
	if (NULL == str)
		return 1;
	if(strlen(str) > 256)
		return 1;
	int i = (int)strlen(str);
	i--;
	while(i >= 0 && (str[i] == ' ' || str[i] == ' ' || str[i] == 10 ))
	{
		str[i] = '\0';
		i--;
	}

	return 0;
}

int  JHCom_RemoveTailFileEndSign(char * str)
{
	if (NULL == str)
		return 1;
	int len = (int)strlen(str);
	if (len == 0) return 0;
	if (str[len -1] == 10)
	{
		str[len - 1] = '\0';
	}
	return 0;
}

int JHCom_SplitToStrs(char *instr, char* key, char **strs, int &numStr)
{
	if (strlen(instr) == 0) return 0;
	numStr = 0;
	char *copyInStr = (char *)malloc(strlen(instr) + 1);
	strcpy(copyInStr,instr);
	char *p = strtok(copyInStr,key);//第一次调用,传要处理的串参数地址s 
	while(p != NULL)
	{
		strcpy(strs[numStr++],p);
		p = strtok(NULL,key);
	}
	free(copyInStr);
	return 0;
}

//此函数用于获得文件名
void JHCom_GetFileName(const WCHAR *fullname, WCHAR *filename)
{
	WCHAR name[256];
	wcscpy_s(name,fullname);
	WCHAR *p = wcstok(name,L"\\");
	WCHAR *pre = NULL;
	while(p != NULL)
	{
		pre = p;
		p = wcstok(NULL, L"\\");
	}
	wcscpy(filename,pre);
	return;
}

//此函数用于去掉扩展名
void JHCom_GetFileName2(const WCHAR *filename, WCHAR *filename2)
{
	WCHAR name[256];
	wcscpy_s(name,filename);
	WCHAR *p = wcstok(name,L".");
	wcscpy(filename2,p);
	return;
}

std::string JHCOM_GetStringFromCString(CString cstr)
{
	CStringA stra(cstr.GetBuffer(0)); 
	string strs (stra.GetBuffer(0)); 
	stra.ReleaseBuffer();
	return strs;
}


/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetNextString
* Description     : This function get a next string for a given string.
* Note            : such as x0-->x1, abc->abc1, 01->02.
* Input           : pstr : String to get next string.
* Output          : NULL.
* Return          : Next String.
--------------------------------------------------------------------------*/
CString JHCOM_GetNextString(CString pstr)
{
	int len = pstr.GetLength();
	wchar_t *pbuf = pstr.GetBuffer();

	int i = 0;
	int i2 = INT_MAX;
	for (i = len-1; i>=0; i--)
	{
		if (pbuf[i] >= '0' && pbuf[i] <= '9')
		{
			
			if (pbuf[i] !='0')
			{
				i2 = i;
			}
		}
		else
			break;
	}

	if(i2< 0)
		i2 =0;
    
	CString str;
	if(i2 == INT_MAX)
	{
        str.Format(L"%s%d",pstr,1);
	}
	else
	{
		wchar_t buf2[128] = L"",buf3[128] = L"";
		wcsncpy(buf2,&(pbuf[i2]),len - i2 );
		wcsncpy(buf3,pbuf,i2 );
		int vv = _wtoi(buf2);

		str.Format(L"%s%d",buf3,vv+1);
	}
	return str;
}


//-------From PIP Com-------//
