// webtest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <iosfwd>
#include <xstring>
#include <atlconv.h>  
#include "..\SunacCadWeb\ArgumentSettingServiceSoap.nsmap"
#include "..\SunacCadWeb\soapArgumentSettingServiceSoapProxy.h"

#include "..\cpp_base64\src\Base64Decoder.h"

using namespace std;


bool DownloadFileTest(const std::wstring fileId);
bool DownloadFileTest(const int fileId);
bool UploadFileTest(std::wstring& uploadFileIdOut);
void Test();
void Test2();

//wstring=>string
std::string WString2String(const std::wstring& ws)
{
	std::string strLocale = setlocale(LC_ALL, "");
	const wchar_t* wchSrc = ws.c_str();
	size_t nDestSize = wcstombs(NULL, wchSrc, 0) + 1;
	char *chDest = new char[nDestSize];
	memset(chDest, 0, nDestSize);
	wcstombs(chDest, wchSrc, nDestSize);
	std::string strResult = chDest;
	delete[]chDest;
	setlocale(LC_ALL, strLocale.c_str());
	return strResult;
}
// string => wstring
std::wstring String2WString(const std::string& s)
{
	std::string strLocale = setlocale(LC_ALL, "");
	const char* chSrc = s.c_str();
	size_t nDestSize = mbstowcs(NULL, chSrc, 0) + 1;
	wchar_t* wchDest = new wchar_t[nDestSize];
	wmemset(wchDest, 0, nDestSize);
	mbstowcs(wchDest, chSrc, nDestSize);
	std::wstring wstrResult = wchDest;
	delete[]wchDest;
	setlocale(LC_ALL, strLocale.c_str());
	return wstrResult;
}

int _tmain(int argc, _TCHAR* argv[])
{
	Test();

	//Test2();

	DownloadFileTest(43);

	system("pause");
	return 0;
}


void Test()
{
	std::wstring attName = _T("area");
	ArgumentSettingServiceSoapProxy cadWeb;

	_ns1__StandardDesignAttribute desingAtt;
	desingAtt.AtrributeName = &attName;

	_ns1__StandardDesignAttributeResponse attResult;

	int nRet = cadWeb.StandardDesignAttribute(&desingAtt, attResult);

	UINT  len = (attResult.StandardDesignAttributeResult)->length();


	MessageBox(NULL, attResult.StandardDesignAttributeResult->c_str(), _T("返回结果"), 0);

	//wprintf(attResult.StandardDesignAttributeResult->c_str());
	//wprintf(_T("\r\n"));
	//wcout << attResult.StandardDesignAttributeResult << endl;


	return ;
}

void Test2()
{
	std::wstring sOpenType;
	std::wstring sOpenNum;
	std::wstring sGongnengqu;

	_ns1__GetWindows ns;
	ns.width = 1800;
	ns.height = 1800;
	ns.openType = &sOpenType;
	ns.openNum = &sOpenNum;
	ns.gongNengQu = &sGongnengqu;

	_ns1__GetWindowsResponse nsResponse;

	ArgumentSettingServiceSoapProxy cadWeb;
	int nRet = cadWeb.GetWindows(&ns, nsResponse);

	if (nsResponse.GetWindowsResult!=NULL)
	{
		MessageBox(NULL, nsResponse.GetWindowsResult->c_str(), _T("返回结果"), 0);
	}

}

bool DownloadFileTest(const int fileId)
{
	_ns1__CadFileDownload nsCadFile;
	nsCadFile.Id = fileId;
	_ns1__CadFileDownloadResponse cadFileResponse;

	ArgumentSettingServiceSoapProxy cadWeb;
	int nRet = cadWeb.CadFileDownload(&nsCadFile, cadFileResponse);

	wstring* swReturn = cadFileResponse.CadFileDownloadResult;
	if (swReturn == NULL)
	{
		return false;
	}

	std::string sReturn = WString2String(*swReturn);

	Base64Decoder decoder;

	wstring filenameOut = L"test.dwg";
	ofstream ofs(filenameOut, ofstream::out | ofstream::binary);
	if (ofs)
	{
		UINT numberOfBytes = sReturn.length() + 1;
		char* decodedBuffer = new char[numberOfBytes * 2];
		memset(decodedBuffer, 0, numberOfBytes * 2);

		int numberOfBytesDecoded = decoder.decode(sReturn.c_str(), numberOfBytes, decodedBuffer);
		ofs.write(decodedBuffer, numberOfBytesDecoded);
	}
	else
	{
		wcout << L"Cannot open file: " << filenameOut << endl;
	}

	ofs.close();

	return true;

}
bool DownloadFileTest2(const std::wstring fileId)
{
	//WSPDMStandardFacadeSoapBindingProxy pdmProxy;

	//const std::wstring _secretKey = L"RDsAACpZyBgGrxtO";		//可配置
	//const std::wstring _creater = L"55555";///(_param_1._loginReturn->sessionId);
	////const std::wstring _acttid = L"RDsAACpXNzZEhYqb"; //图文档附件ID
	//const std::wstring _acttid = fileId; //图文档附件ID
	//const std::wstring _sourceSys = L"SHTY";		//可配置
	//xsd__base64Binary _downloadFileReturn;
	//int nRet = pdmProxy.downloadFile(_secretKey, _creater, _acttid, _sourceSys, _downloadFileReturn);
	//if (SOAP_OK == nRet && _downloadFileReturn.__ptr && _downloadFileReturn.__size>0)
	//{
	//	//保存到文件
	//	ofstream examplefile("test.dwg", ios::out  | ios::binary);
	//	if (examplefile.is_open())
	//	{
	//		examplefile.write((const char *)(_downloadFileReturn.__ptr), _downloadFileReturn.__size);
	//		examplefile.close();
	//	}

	//	//CFile fileOut;
	//	//fileOut.Open(_T("test.dwg"), CFile::modeCreate);

	//	//fileOut.Write(_downloadFileReturn.__ptr, _downloadFileReturn.__size);

	//	//fileOut.Close();

	//	cout << "下载成功" << endl;
	//	return true;
	//}
	//else
	//{
	//	cout << "下载失败" << endl;
	//	return false;
	//}

	return  false;
}
