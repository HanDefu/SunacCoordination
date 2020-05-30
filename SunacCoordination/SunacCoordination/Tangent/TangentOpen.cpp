#include "StdAfx.h"

#include <afxwin.h>

// ATL includes for sqareCmd.h
#include <atlbase.h>
#include <atlcom.h>
//CComModule _Module;	//COM服务器模块，允许一个客户端访问当前模块的组件

// Acad COM and Automation interfaces
#include <acadi.h>
#include <axtempl.h>
#include <axpnt3d.h>


#include <acdocman.h>
#include <aced.h>
#include <acdb.h>
#include <adslib.h>
#include <rxmfcapi.h>
#include <dbobjptr.h>

//////////////////////////////////////////////////////////////////////////
#include <opmext.h>
#include <dbapserv.h>
#include "..\Tool\DocLock.h"


//#include "TangentCom\tch10_com19_opening.tlh"

#include "..\Tangent\TangentCom\T2013X64_tch9_com18.tlh"

#include "..\Tangent\TangentCom\T2014X64_tch9_com18.tlh"
#include "..\Tangent\TangentCom\T2014X64_tch9_com19.tlh"

#include "..\Tangent\TangentCom\T20X64_tch10_com18.tlh"
#include "..\Tangent\TangentCom\T20X64_tch10_com19.tlh"

#include "..\Tangent\TangentCom\T20V2X64_tch10_com18.tlh"
#include "..\Tangent\TangentCom\T20V2X64_tch10_com19.tlh"

#include "..\Tangent\TangentCom\T20V3X64_tch10_com18.tlh"
#include "..\Tangent\TangentCom\T20V3X64_tch10_com19.tlh"
#include "..\Tangent\TangentCom\T20V3X64_tch10_com20.tlh"

#include "..\Tangent\TangentCom\T20V4X64_tch10_com18.tlh"
#include "..\Tangent\TangentCom\T20V4X64_tch10_com19.tlh"
#include "..\Tangent\TangentCom\T20V4X64_tch10_com20.tlh"
#include "..\Tangent\TangentCom\T20V4X64_tch10_com21.tlh"

#include "..\Tangent\TangentCom\T20V5X64_tch10_com18.tlh"
#include "..\Tangent\TangentCom\T20V5X64_tch10_com19.tlh"
#include "..\Tangent\TangentCom\T20V5X64_tch10_com20.tlh"
#include "..\Tangent\TangentCom\T20V5X64_tch10_com21.tlh"
#include "..\Tangent\TangentCom\T20V5X64_tch10_com22.tlh"
#include "..\Tangent\TangentCom\T20V5X64_tch10_com23.tlh"

//////////////////////////////////////////////////////////////////////////
#include "TangentOpen.h"
#include "..\Common/ComFun_ACAD_Common.h"
#include "..\Common\ComFun_Str.h"
#include "..\Common\ComFun_Sunac.h"

//////////////////////////////////////////////////////////////////////////

AcDbObjectId AppendEntity(AcDbEntity *pEnt, const WCHAR * entry = ACDB_MODEL_SPACE)
{
	AcDbObjectId entId = AcDbObjectId::kNull;

	AcDbBlockTable *pBlockTable;
	if (acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead) != Acad::eOk)
	{
		return AcDbObjectId::kNull;
	}

	CDocLock doclock;
	AcDbBlockTableRecord *pBlockTableRecord;
	Acad::ErrorStatus es = pBlockTable->getAt(entry, pBlockTableRecord, AcDb::kForWrite);
	if (es == Acad::eOk)
	{
		es = pBlockTableRecord->appendAcDbEntity(entId, pEnt);
		pBlockTableRecord->close();
	}

	pBlockTable->close();
	pEnt->close();

	return entId;
}

std::vector<AcDbObjectId> YT_Explode(AcDbObjectId entId, const WCHAR * entry = ACDB_MODEL_SPACE)
{
	CDocLock docLock;

	std::vector<AcDbObjectId> ids;

	AcDbObjectPointer<AcDbEntity> pEnt(entId, AcDb::kForWrite);
	if (pEnt.openStatus()==Acad::eOk)
	{
		AcDbVoidPtrArray pExps;
		if (pEnt->explode(pExps) == Acad::eOk)
		{
			for (int i = 0; i < pExps.length(); i++)
			{
				AcDbEntity *pExpEnt = (AcDbEntity*)pExps[i];
				AcDbObjectId idOut = AppendEntity(pExpEnt, entry);
				if (idOut!= AcDbObjectId::kNull)
				{
					ids.push_back(idOut);
				}
			}
			pEnt->erase(true);
		}
		else
		{
			acutPrintf(_T("\n该对象不能被分解！"));
		}
	}

	return ids;
}

HRESULT CTangentOpen::InsertWinOpenning(AcGePoint3d p_centerPt, CTOpenData p_winData, AcDbObjectId &p_tWinOpenIdOut)
{
	p_tWinOpenIdOut = AcDbObjectId::kNull;

	//CString sPath = _T("D:\\test\\Sunac_test\\T门洞.dwg"); 
	CString sPath = MD2010_GetAppPath() + L"\\Sunac2019\\Data\\Template\\T门洞.dwg";
	if (IsFileExist(sPath)==false)
	{
		acutPrintf(_T("文件不存在：")+ sPath + _T("\n"));
		return E_FAIL;
	}


#if 1
	AcDbObjectIdArray idsOut;
	Acad::ErrorStatus es = MD2010_InsertDwgFile2(sPath, p_centerPt, idsOut);
	if (es == Acad::eOk)
	{
		p_tWinOpenIdOut = idsOut[0];
	}
	else
	{
		acutPrintf(_T("插入天正门洞失败\n"));
	}

#else
	CString sBlockDefName = _T("T门洞");
	AcDbObjectId blockid = AcDbObjectId::kNull;
	int nRet = MD2010_InsertBlockFromPathName(ACDB_MODEL_SPACE, sPath, sBlockDefName, blockid, p_centerPt, 0, AcGeScale3d(1, 1, 1));
	if (blockid == AcDbObjectId::kNull)
	{
		return E_FAIL;
	}

	std::vector<AcDbObjectId> idsOut = YT_Explode(blockid, ACDB_MODEL_SPACE);
	if (idsOut.size() != 1)
	{
		return E_FAIL;
	}

	p_tWinOpenIdOut = idsOut[0];
#endif

	//////////////////////////////////////////////////////////////////////////
	return SetTangentOpenProp(p_tWinOpenIdOut, p_winData);
}


template<class T> //TCH10_COM9_T20V5X64::IComOpening
HRESULT SetTangentOpenData( AcDbObjectId p_winId, CTOpenData p_winData, REFCLSID rclsid)
{
	HRESULT hr = S_OK;

	CComPtr<T> pWin;
	CComQIPtr<IAcadBaseObject> pSquareBase;
	//CComQIPtr<IAcadBaseObject2> pSquareBase2;

	try
	{
		if (FAILED(hr = pWin.CoCreateInstance(rclsid)))//__uuidof(TCH10_COM9_T20V5X64::ComOpening)
			throw hr;

		pSquareBase = pWin;
		//pSquareBase2 = pWin;

		if (pSquareBase==NULL)
			throw E_POINTER;

		pSquareBase->SetObjectId(p_winId);

		pWin->PutWidth(p_winData.width);
		pWin->PutHeight(p_winData.height);
		pWin->PutWinSill(p_winData.bottomHeight);
		pWin->PutLabel((_bstr_t)(p_winData.sWinCode));
	}
	catch (HRESULT eHr)
	{
		acutPrintf(_T("\n Error SetTangentOpenProp_TCH10_COM9_T20V5X64."));
		return eHr;
	}

	return hr;
}

HRESULT CTangentOpen::SetTangentOpenProp(AcDbObjectId p_winId, CTOpenData p_winData)
{
	HRESULT hr = S_OK;

	if (p_winId==AcDbObjectId::kNull)
	{
		return E_FAIL;
	}

	AcDbEntity *pent;
	Acad::ErrorStatus es = acdbOpenAcDbEntity(pent, p_winId, AcDb::kForRead);
	if (es!=Acad::eOk)
	{
		return E_FAIL;
	}

	CLSID entClsid = {0};
	es = pent->getClassID(&entClsid);	//可根据entClsid来决定使用何种版本的天正插件初始化，返回的是ComOpening接口

	pent->close();

	//if ( IsEqualCLSID(entClsid, __uuidof(TCH10_COM9_T20V5X64::ComOpening)) )
	//{
	//	return SetTangentOpenData<TCH10_COM9_T20V5X64::IComOpening>(p_winId, p_winData, __uuidof(TCH10_COM9_T20V5X64::ComOpening));
	//}
	if (IsEqualCLSID(entClsid, __uuidof(T2013X64_tch9_com18::ComOpening)))
	{
		return SetTangentOpenData<T2013X64_tch9_com18::IComOpening>(p_winId, p_winData, __uuidof(T2013X64_tch9_com18::ComOpening));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T2014X64_tch9_com18::ComOpening)))
	{
		return SetTangentOpenData<T2014X64_tch9_com18::IComOpening>(p_winId, p_winData, __uuidof(T2014X64_tch9_com18::ComOpening));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T2014X64_tch9_com19::ComOpening)))
	{
		return SetTangentOpenData<T2014X64_tch9_com19::IComOpening>(p_winId, p_winData, __uuidof(T2014X64_tch9_com19::ComOpening));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20X64_tch10_com18::ComOpening)))
	{
		return SetTangentOpenData<T20X64_tch10_com18::IComOpening>(p_winId, p_winData, __uuidof(T20X64_tch10_com18::ComOpening));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20X64_tch10_com19::ComOpening)))
	{
		return SetTangentOpenData<T20X64_tch10_com19::IComOpening>(p_winId, p_winData, __uuidof(T20X64_tch10_com19::ComOpening));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V2X64_tch10_com18::ComOpening)))
	{
		return SetTangentOpenData<T20V2X64_tch10_com18::IComOpening>(p_winId, p_winData, __uuidof(T20V2X64_tch10_com18::ComOpening));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V2X64_tch10_com19::ComOpening)))
	{
		return SetTangentOpenData<T20V2X64_tch10_com19::IComOpening>(p_winId, p_winData, __uuidof(T20V2X64_tch10_com19::ComOpening));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V3X64_tch10_com18::ComOpening)))
	{
		return SetTangentOpenData<T20V3X64_tch10_com18::IComOpening>(p_winId, p_winData, __uuidof(T20V3X64_tch10_com18::ComOpening));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V3X64_tch10_com19::ComOpening)))
	{
		return SetTangentOpenData<T20V3X64_tch10_com19::IComOpening>(p_winId, p_winData, __uuidof(T20V3X64_tch10_com19::ComOpening));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V3X64_tch10_com20::ComOpening)))
	{
		return SetTangentOpenData<T20V3X64_tch10_com20::IComOpening>(p_winId, p_winData, __uuidof(T20V3X64_tch10_com20::ComOpening));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V4X64_tch10_com18::ComOpening)))
	{
		return SetTangentOpenData<T20V4X64_tch10_com18::IComOpening>(p_winId, p_winData, __uuidof(T20V4X64_tch10_com18::ComOpening));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V4X64_tch10_com19::ComOpening)))
	{
		return SetTangentOpenData<T20V4X64_tch10_com19::IComOpening>(p_winId, p_winData, __uuidof(T20V4X64_tch10_com19::ComOpening));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V4X64_tch10_com20::ComOpening)))
	{
		return SetTangentOpenData<T20V4X64_tch10_com20::IComOpening>(p_winId, p_winData, __uuidof(T20V4X64_tch10_com20::ComOpening));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V4X64_tch10_com21::ComOpening)))
	{
		return SetTangentOpenData<T20V4X64_tch10_com21::IComOpening>(p_winId, p_winData, __uuidof(T20V4X64_tch10_com21::ComOpening));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V5X64_tch10_com18::ComOpening)))
	{
		return SetTangentOpenData<T20V5X64_tch10_com18::IComOpening>(p_winId, p_winData, __uuidof(T20V5X64_tch10_com18::ComOpening));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V5X64_tch10_com19::ComOpening)))
	{
		return SetTangentOpenData<T20V5X64_tch10_com19::IComOpening>(p_winId, p_winData, __uuidof(T20V5X64_tch10_com19::ComOpening));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V5X64_tch10_com20::ComOpening)))
	{
		return SetTangentOpenData<T20V5X64_tch10_com20::IComOpening>(p_winId, p_winData, __uuidof(T20V5X64_tch10_com20::ComOpening));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V5X64_tch10_com21::ComOpening)))
	{
		return SetTangentOpenData<T20V5X64_tch10_com21::IComOpening>(p_winId, p_winData, __uuidof(T20V5X64_tch10_com21::ComOpening));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V5X64_tch10_com22::ComOpening)))
	{
		return SetTangentOpenData<T20V5X64_tch10_com22::IComOpening>(p_winId, p_winData, __uuidof(T20V5X64_tch10_com22::ComOpening));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V5X64_tch10_com23::ComOpening)))
	{
		return SetTangentOpenData<T20V5X64_tch10_com23::IComOpening>(p_winId, p_winData, __uuidof(T20V5X64_tch10_com23::ComOpening));
	}
	else
	{
		hr = E_FAIL;
	}

	return hr;
}


template<class T> 
HRESULT GetTWallData(AcDbObjectId p_id, CTWallData& p_wallData, REFCLSID rclsid)
{
	HRESULT hr = S_OK;

	CComPtr<T> pWall;
	CComQIPtr<IAcadBaseObject> pSquareBase;
	//CComQIPtr<IAcadBaseObject2> pSquareBase2;

	try
	{
		if (FAILED(hr = pWall.CoCreateInstance(rclsid)))//__uuidof(TCH10_COM9_T20V5X64::ComWall)
			throw hr;

		pSquareBase = pWall;

		if (pSquareBase == NULL)
			throw E_POINTER;

		p_wallData.thick = pWall->GetTotalWidth();
		p_wallData.leftT = pWall->GetLeftWidth();
		p_wallData.rightT = pWall->GetRightWidth();
	}
	catch (HRESULT eHr)
	{
		acutPrintf(_T("\n Error SetTangentOpenProp_TCH10_COM9_T20V5X64."));
		return eHr;
	}

	return hr;
}

HRESULT CTangentOpen::GetTangentWallData(AcDbObjectId p_id, CTWallData& p_wallDataOut)
{
	HRESULT hr = S_OK;
	if (p_id == AcDbObjectId::kNull)
		return E_FAIL;

	AcDbObjectPointer<AcDbEntity> pEnt(p_id, AcDb::kForRead);
	if (pEnt.openStatus() != Acad::eOk)
	{
		return E_FAIL;
	}

	pEnt->getGeomExtents(p_wallDataOut.extents);

	CLSID entClsid = { 0 };
	Acad::ErrorStatus es = pEnt->getClassID(&entClsid);	//可根据entClsid来决定使用何种版本的天正插件初始化，返回的是ComOpening接口
	if (es != Acad::eOk)
		return E_FAIL;

	if (IsEqualCLSID(entClsid, __uuidof(T2013X64_tch9_com18::ComWall)))
	{
		return GetTWallData<T2013X64_tch9_com18::IComWall>(p_id, p_wallDataOut, __uuidof(T2013X64_tch9_com18::ComWall));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T2014X64_tch9_com18::ComWall)))
	{
		return GetTWallData<T2014X64_tch9_com18::IComWall>(p_id, p_wallDataOut, __uuidof(T2014X64_tch9_com18::ComWall));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T2014X64_tch9_com19::ComWall)))
	{
		return GetTWallData<T2014X64_tch9_com19::IComWall>(p_id, p_wallDataOut, __uuidof(T2014X64_tch9_com19::ComWall));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20X64_tch10_com18::ComWall)))
	{
		return GetTWallData<T20X64_tch10_com18::IComWall>(p_id, p_wallDataOut, __uuidof(T20X64_tch10_com18::ComWall));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20X64_tch10_com19::ComWall)))
	{
		return GetTWallData<T20X64_tch10_com19::IComWall>(p_id, p_wallDataOut, __uuidof(T20X64_tch10_com19::ComWall));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V2X64_tch10_com18::ComWall)))
	{
		return GetTWallData<T20V2X64_tch10_com18::IComWall>(p_id, p_wallDataOut, __uuidof(T20V2X64_tch10_com18::ComWall));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V2X64_tch10_com19::ComWall)))
	{
		return GetTWallData<T20V2X64_tch10_com19::IComWall>(p_id, p_wallDataOut, __uuidof(T20V2X64_tch10_com19::ComWall));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V3X64_tch10_com18::ComWall)))
	{
		return GetTWallData<T20V3X64_tch10_com18::IComWall>(p_id, p_wallDataOut, __uuidof(T20V3X64_tch10_com18::ComWall));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V3X64_tch10_com19::ComWall)))
	{
		return GetTWallData<T20V3X64_tch10_com19::IComWall>(p_id, p_wallDataOut, __uuidof(T20V3X64_tch10_com19::ComWall));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V3X64_tch10_com20::ComWall)))
	{
		return GetTWallData<T20V3X64_tch10_com20::IComWall>(p_id, p_wallDataOut, __uuidof(T20V3X64_tch10_com20::ComWall));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V4X64_tch10_com18::ComWall)))
	{
		return GetTWallData<T20V4X64_tch10_com18::IComWall>(p_id, p_wallDataOut, __uuidof(T20V4X64_tch10_com18::ComWall));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V4X64_tch10_com19::ComWall)))
	{
		return GetTWallData<T20V4X64_tch10_com19::IComWall>(p_id, p_wallDataOut, __uuidof(T20V4X64_tch10_com19::ComWall));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V4X64_tch10_com20::ComWall)))
	{
		return GetTWallData<T20V4X64_tch10_com20::IComWall>(p_id, p_wallDataOut, __uuidof(T20V4X64_tch10_com20::ComWall));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V4X64_tch10_com21::ComWall)))
	{
		return GetTWallData<T20V4X64_tch10_com21::IComWall>(p_id, p_wallDataOut, __uuidof(T20V4X64_tch10_com21::ComWall));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V5X64_tch10_com18::ComWall)))
	{
		return GetTWallData<T20V5X64_tch10_com18::IComWall>(p_id, p_wallDataOut, __uuidof(T20V5X64_tch10_com18::ComWall));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V5X64_tch10_com19::ComWall)))
	{
		return GetTWallData<T20V5X64_tch10_com19::IComWall>(p_id, p_wallDataOut, __uuidof(T20V5X64_tch10_com19::ComWall));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V5X64_tch10_com20::ComWall)))
	{
		return GetTWallData<T20V5X64_tch10_com20::IComWall>(p_id, p_wallDataOut, __uuidof(T20V5X64_tch10_com20::ComWall));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V5X64_tch10_com21::ComWall)))
	{
		return GetTWallData<T20V5X64_tch10_com21::IComWall>(p_id, p_wallDataOut, __uuidof(T20V5X64_tch10_com21::ComWall));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V5X64_tch10_com22::ComWall)))
	{
		return GetTWallData<T20V5X64_tch10_com22::IComWall>(p_id, p_wallDataOut, __uuidof(T20V5X64_tch10_com22::ComWall));
	}
	else if (IsEqualCLSID(entClsid, __uuidof(T20V5X64_tch10_com23::ComWall)))
	{
		return GetTWallData<T20V5X64_tch10_com23::IComWall>(p_id, p_wallDataOut, __uuidof(T20V5X64_tch10_com23::ComWall));
	}
	else
	{
		hr = E_FAIL;
	}


	return E_FAIL;
}


//  CLSIDFromProgID()、CLSIDFromProgIDEx()	由 ProgID 得到 CLSID。没什么好说的，你自己都可以写，查注册表贝
//	ProgIDFromCLSID()	由 CLSID 得到 ProgID，调用者使用完成后要释放 ProgID 的内存(注5)
//	CoCreateGuid()	随机生成一个 GUID
//	IsEqualGUID()、IsEqualCLSID()、IsEqualIID()	比较2个ID是否相等
//	StringFromCLSID()、StringFromGUID2()、StringFromIID()	由 CLSID,IID 得到注册表中CLSID样式的字符串，注意释放内存

EXTERN_C const CLSID IID_IOPMPropertyExpander;
//extern const IID IID_IOPMPropertyExpander;

//MIDL_DEFINE_GUID(CLSID, IID_IOPMPropertyExpander_TY,0x5d535710, 0xd49a, 0x11d1, 0xb6, 0x16, 0x0, 0x60, 0xb0, 0x87, 0xe2, 0x35);

//DEFINE_GUID(IID_IOPMPropertyExpander_TY, 
//	0x5d535710, 0xd49a, 0x11d1, 0xb6, 0x16, 0x0, 0x60, 0xb0, 0x87, 0xe2, 0x35);

//void GuidTest()
//{
//	//CLSID clsid;  
//	//IID iid;  
//	//HRESULT hr = CLSIDFromString(OLESTR("{92e83a37-399a-47b1-bf6c-0ab052650cfe}"),&clsid); 
//
//}