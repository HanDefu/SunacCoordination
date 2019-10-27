#pragma once

#include <vector>
#include <gecomp3d.h>
#include <brshell.h>
#include <brcplx.h >

//String functions
/*-------------------------------------------------------------------------
* Function Name   : JHCom_FileExist
* Description     : This function help to check whether the file exist or not.
* Note            : NULL.
* Input           : FileName :  File to check.
* Output          : NULL.
* Return          : true : Exist, false : Not Exist.
--------------------------------------------------------------------------*/
bool JHCom_FileExist(CString FileName);

/*-------------------------------------------------------------------------
* Function Name   : JHCom_RemoveStrSpace
* Description     : This function remove the head, tail space and file end sign in one input str.
* Note            : NULL.
* Input           : str :  string to remove.
* Output          : NULL.
* Return          : Error number.
--------------------------------------------------------------------------*/
int  JHCom_RemoveStrSpace(char * str);

/*-------------------------------------------------------------------------
* Function Name   : JHCom_RemoveSpaceHead
* Description     : This function remove the head space.
* Note            : NULL.
* Input           : str :  string to remove head space.
* Output          : NULL.
* Return          : Error number.
--------------------------------------------------------------------------*/
int  JHCom_RemoveSpaceHead(char * instr);

/*-------------------------------------------------------------------------
* Function Name   : JHCom_RemoveSpaceTail
* Description     : This function remove the tail space.
* Note            : NULL.
* Input           : str :  string to remove tail space.
* Output          : NULL.
* Return          : Error number.
--------------------------------------------------------------------------*/
int  JHCom_RemoveSpaceTail(char * str);

/*-------------------------------------------------------------------------
* Function Name   : JHCom_RemoveTailFileEndSign
* Description     : This function remove the last file end sign.
* Note            : NULL.
* Input           : str :  string to remove last file end sign.
* Output          : NULL.
* Return          : Error number.
--------------------------------------------------------------------------*/
int  JHCom_RemoveTailFileEndSign(char * str);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetNextString
* Description     : This function get a next string for a given string.
* Note            : such as x0-->x1, abc->abc1, 01->02.
* Input           : pstr : String to get next string.
* Output          : NULL.
* Return          : Next String.
--------------------------------------------------------------------------*/
CString           JHCOM_GetNextString(CString pstr);

/*-------------------------------------------------------------------------
* Function Name   : JHCom_SplitToStrs
* Description     : This function split one string to several strings by a key str.
* Note            : NULL.
* Input           : instr -  input string to split.
*                   key  - key for split.
* Output          : strs - output strings.
*                   numStr - strings number.
* Return          : Error number.
--------------------------------------------------------------------------*/
int  JHCom_SplitToStrs(char *instr, char* key, char **strs, int &numStr);

/*-------------------------------------------------------------------------
* Function Name   : JHCom_GetFileName
* Description     : This function get a file name from a path name.
*                   we get "case.txt" from "c:\\case.txt"
* Note            : NULL.
* Input           : fullname -  path name.
* Output          : filename - file name with extention.
* Return          : NULL.
--------------------------------------------------------------------------*/
void JHCom_GetFileName(const WCHAR *fullname, WCHAR *filename);

/*-------------------------------------------------------------------------
* Function Name   : JHCom_GetFileName
* Description     : This function get a file name from a name with extention.
*                   we get "case" from "case.txt"
* Note            : NULL.
* Input           : filename -  file name.
* Output          : filename2 - file name without extention.
* Return          : NULL.
--------------------------------------------------------------------------*/
void JHCom_GetFileName2(const WCHAR *filename, WCHAR *filename2);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetStringFromCString
* Description     : This function get a std::string from a CString.
* Note            : NULL.
* Input           : cstr :  CString to get string.
* Output          : NULL.
* Return          : std::string.
--------------------------------------------------------------------------*/
std::string JHCOM_GetStringFromCString(CString cstr);

std::vector<CString> YT_SplitCString(CString strSource, const TCHAR ch);

CString FilePathToFileName(const CString &p_sFilePath);
