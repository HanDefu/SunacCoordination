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
using namespace std;


bool DownloadFileTest(const std::wstring fileId);
bool UploadFileTest(std::wstring& uploadFileIdOut);
void Test();

int _tmain(int argc, _TCHAR* argv[])
{
	Test();

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

	system("pause");

	return ;
}

bool DownloadFileTest(const std::wstring fileId)
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
