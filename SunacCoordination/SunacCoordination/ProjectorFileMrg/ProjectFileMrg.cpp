#include "StdAfx.h"
#include "ProjectFileMrg.h"

CProjectFileMrg* CProjectFileMrg::Instance()
{
	static CProjectFileMrg instance;
	return &instance;
}
CProjectFileMrg::CProjectFileMrg()
{
}

CProjectFileMrg::~CProjectFileMrg()
{
	Clear();
}

void CProjectFileMrg::Clear()
{
	for (UINT i = 0; i < m_projects.size(); i++)
	{
		delete m_projects[i];
	}

	m_projects.clear();
}

CProjectData* CProjectFileMrg::GetProject(CString p_sPrjName)
{
	for (UINT i = 0; i < m_projects.size(); i++)
	{
		if (m_projects[i]->GetPrjInfo().m_sName.CompareNoCase(p_sPrjName)==0)
		{
			return m_projects[i];
		}
	}
	return NULL;
}

void CProjectFileMrg::InitTestData()
{
	CProjectData* project1 = new CProjectData();
	CProjectDir* folderA = project1->m_rootDir.AddFolder(_T("A区资料"));
	CProjectDir* folderB = project1->m_rootDir.AddFolder(_T("B区资料"));

	CProjectDir* folderA_1 = folderA->AddFolder(_T("施工图"));
	CProjectDir* folderA_2 = folderA->AddFolder(_T("设计图"));

	CProjectDir* folderB_1 = folderB->AddFolder(_T("施工图"));
	CProjectDir* folderB_2 = folderB->AddFolder(_T("设计图"));

	CProjectFile TestFile1,TestFile2;
	TestFile1.m_id = 2;
	TestFile1.m_sName = L"D区1栋设计图";
	TestFile1.m_sCreator = L"LeavE";
	TestFile1.m_sCreateTime = L"2019/10/23 10:16:14";
	TestFile1.m_sFileUrl = L"";
	TestFile1.m_sFileSize = L"5.3MB";
	TestFile1.m_sSaveName = L"95s6d2a286as33sey3";
	TestFile1.m_sUpdator = L"LeavE";
	TestFile1.m_sUpdateTime = L"2019/10/23 10:18:33";

	TestFile2.m_id = 3;
	TestFile2.m_sName = L"D区2栋设计图";
	TestFile2.m_sCreator = L"LeavE";
	TestFile2.m_sCreateTime = L"2019/10/23 10:20:16";
	TestFile2.m_sFileUrl = L"";
	TestFile2.m_sFileSize = L"3.0MB";
	TestFile2.m_sSaveName = L"46sd4d9h6s3b85m9f63w3g";
	TestFile2.m_sUpdator = L"LeavE";
	TestFile2.m_sUpdateTime = L"2019/10/23 10:20:00";

	project1->m_prjInfo.m_sArea = _T("北京区域");
	project1->m_prjInfo.m_sName = _T("北京枫丹壹号");
	project1->m_prjInfo.m_sCityCompany = _T("北京融科房地产有限公司");

	folderA_1->AddFile(TestFile1);
	folderA_1->AddFile(TestFile2);


	m_projects.push_back(project1);
	//////////////////////////////////////////////////////////////////////////
}

vector<CString> CProjectFileMrg::GetAllAreas()const
{
	vector<CString> allArea;

	for (UINT i = 0; i < m_projects.size(); i++)
	{
		bool bFind = false;
		for (UINT n = 0; n < allArea.size(); n++)
		{
			bFind = allArea[n].CompareNoCase(m_projects[i]->GetPrjInfo().m_sArea) == 0;
			if (bFind)
			{
				break;
			}
		}

		if (bFind==false)
		{
			allArea.push_back(m_projects[i]->GetPrjInfo().m_sArea);
		}
	}

	return allArea;
}

vector<CProjectData*> CProjectFileMrg::GetProjectsByArea(CString p_sArea)
{
	vector<CProjectData*> ProjectsByArea;
	for (UINT i = 0; i < m_projects.size(); i++)
	{
		if (m_projects[i]->GetPrjInfo().m_sArea.CompareNoCase(p_sArea) == 0)
		{
			ProjectsByArea.push_back(m_projects[i]);
		}
	}

	return ProjectsByArea;
}
