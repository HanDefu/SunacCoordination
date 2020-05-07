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

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_WcharToChar
* Description     : This function convert wchar_t string to char string.
* Note            : Returned value should be freed by free();.
* Input           : pchar : wchar_t string to convert.
*                   
* Output          : NULL.
* Return          : char string pointer.
--------------------------------------------------------------------------*/
CHAR* JHCOM_WcharToChar(const WCHAR * pchar)
{
	CHAR *m_pchar = NULL;

	int iSize = WideCharToMultiByte(CP_ACP,0,pchar,-1,NULL,0,NULL,NULL);
	//m_pchar = new CHAR[outlen];
	m_pchar = (char*)malloc((iSize+1)/**sizeof(char)*/);
	WideCharToMultiByte(CP_ACP, 0, pchar, -1, m_pchar, iSize, NULL, NULL);

	return m_pchar;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_CharToWchar
* Description     : This function convert char string to wchar_t string.
* Note            : Returned value should be freed by delete;.
* Input           : pchar : char string to convert.
*                   
* Output          : NULL.
* Return          : wchar_t string pointer.
--------------------------------------------------------------------------*/
WCHAR* JHCOM_CharToWchar(const char * pchar)
{
	WCHAR *m_pWchar = NULL;

	int outlen = MultiByteToWideChar(CP_ACP,0,pchar,-1,NULL,0);
	m_pWchar = new WCHAR[outlen];
	MultiByteToWideChar(CP_ACP,0,pchar,-1,m_pWchar,outlen);

	return m_pWchar;
}


/*-------------------------------------------------------------------------
* Function Name   : JHCOM_AdsToAcGe
* Description     : This function convert ads_point to AcGePoint3d.
* Note            : NULL.
* Input           : pts : ads_point to convert.
*                   pt : AcGePoint3d to converted to.                   
* Output          : NULL.
* Return          : NULL.
--------------------------------------------------------------------------*/
void JHCOM_AdsToAcGe(ads_point& pts,AcGePoint3d &pt)
{
	pt.x = pts[X];
	pt.y = pts[Y];
	pt.z = pts[Z];

}
/*-------------------------------------------------------------------------
* Function Name   : JHCOM_AdsToAcGe
* Description     : This function convert AcGePoint3d to ads_point.
* Note            : NULL.
* Input           : pt : AcGePoint3d to convert.
*                   pts : ads_point to converted to.                   
* Output          : NULL.
* Return          : NULL.
--------------------------------------------------------------------------*/
void JHCOM_AcGeToads(AcGePoint3d &pt,ads_point& pts)
{
	pts[X] = pt.x;
	pts[Y] = pt.y;
	pts[Z] = pt.z;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_DoubleToStr
* Description     : This function convert double to CString with 3 length of decimals.
* Note            : NULL.
* Input           : value : double value to convert.       
* Output          : NULL.
* Return          : CString value of converted.
--------------------------------------------------------------------------*/
CString JHCOM_DoubleToStr(double value)
{
	CString str;
	str.Format(L"%.3f", value);
	return str;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_IntToStr
* Description     : This function convert int to CString.
* Note            : NULL.
* Input           : value : int value to convert.       
* Output          : NULL.
* Return          : CString value of converted.
--------------------------------------------------------------------------*/
CString JHCOM_IntToStr(int value)
{
	CString str;
	str.Format(L"%d", value);
	return str;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_WcsToUcs
* Description     : This function convert one AcGePoint3d point from wcs to ucs.
* Note            : NULL.
* Input           : pt : AcGePoint3d to convert.       
* Output          : NULL.
* Return          : AcGePoint3d value of converted.
--------------------------------------------------------------------------*/
AcGePoint3d JHCOM_WcsToUcs(AcGePoint3d &pt)
{
	struct resbuf wcs, ucs;
	wcs.restype = RTSHORT;
	wcs.resval.rint = 0;
	ucs.restype = RTSHORT;
	ucs.resval.rint = 1;

	ads_point adspt,adspto;
	JHCOM_AcGeToads(pt,adspt);
	acedTrans(adspt,&wcs,&ucs,0,adspto);
	AcGePoint3d pto;
	JHCOM_AdsToAcGe(adspto,pto);

	return pto;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_WcsToUcs
* Description     : This function convert one AcGeVector3d point from wcs to ucs.
* Note            : NULL.
* Input           : pt : AcGeVector3d to convert.       
* Output          : NULL.
* Return          : AcGeVector3d value of converted.
--------------------------------------------------------------------------*/
AcGeVector3d JHCOM_WcsToUcs(AcGeVector3d &vec)
{
	struct resbuf wcs, ucs;
	wcs.restype = RTSHORT;
	wcs.resval.rint = 0;
	ucs.restype = RTSHORT;
	ucs.resval.rint = 1;

	ads_point adspt,adspto;
	adspt[X] = vec.x;
	adspt[Y] = vec.y;
	adspt[Z] = vec.z;
	acedTrans(adspt,&wcs,&ucs,2,adspto);

	AcGeVector3d veco;
	veco.x = adspto[X];
	veco.y = adspto[Y];
	veco.z = adspto[Z];

	return veco;
}



/*-------------------------------------------------------------------------
* Function Name   : JHCOM_UcsToWcs
* Description     : This function convert one AcGePoint3d point from ucs to wcs.
* Note            : NULL.
* Input           : pt : AcGePoint3d to convert.       
* Output          : NULL.
* Return          : AcGePoint3d value of converted.
--------------------------------------------------------------------------*/
AcGePoint3d JHCOM_UcsToWcs(AcGePoint3d &pt)
{
	struct resbuf wcs, ucs;
	wcs.restype = RTSHORT;
	wcs.resval.rint = 0;
	ucs.restype = RTSHORT;
	ucs.resval.rint = 1;

	ads_point adspt,adspto;
	JHCOM_AcGeToads(pt,adspt);
	acedTrans(adspt,&ucs,&wcs,0,adspto);
	AcGePoint3d pto;
	JHCOM_AdsToAcGe(adspto,pto);

	return pto;
}
AcGePoint2d JHCOM_UcsToWcs(AcGePoint2d &pt)
{
	struct resbuf wcs, ucs;
	wcs.restype = RTSHORT;
	wcs.resval.rint = 0;
	ucs.restype = RTSHORT;
	ucs.resval.rint = 1;

	ads_point adspt,adspto;
	AcGePoint3d p3d(pt.x,pt.y,0);
	JHCOM_AcGeToads(p3d,adspt);
	acedTrans(adspt,&ucs,&wcs,0,adspto);
	AcGePoint3d pto;
	JHCOM_AdsToAcGe(adspto,pto);
	AcGePoint2d p2d(pto.x,pto.y);

	return p2d;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_UcsToWcs
* Description     : This function convert one AcGeVector3d point from ucs to wcs.
* Note            : NULL.
* Input           : pt : AcGeVector3d to convert.       
* Output          : NULL.
* Return          : AcGeVector3d value of converted.
--------------------------------------------------------------------------*/
AcGeVector3d JHCOM_UcsToWcs(AcGeVector3d &vec)
{
	struct resbuf wcs, ucs;
	wcs.restype = RTSHORT;
	wcs.resval.rint = 0;
	ucs.restype = RTSHORT;
	ucs.resval.rint = 1;

	ads_point adspt,adspto;
	adspt[X] = vec.x;
	adspt[Y] = vec.y;
	adspt[Z] = vec.z;
	acedTrans(adspt,&ucs,&wcs,2,adspto);

	AcGeVector3d veco;
	veco.x = adspto[X];
	veco.y = adspto[Y];
	veco.z = adspto[Z];

	return veco;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_UcsToEcs
* Description     : This function convert one AcGePoint3d point from ucs to ecs.
* Note            : NULL.
* Input           : pt : AcGePoint3d to convert.       
* Output          : NULL.
* Return          : AcGePoint3d value of converted.
--------------------------------------------------------------------------*/
AcGePoint3d JHCOM_UcsToEcs(AcGePoint3d pt, AcGeVector3d normal)
{
	ads_point adspt,adspto,nor;
	JHCOM_AcGeToads(pt,adspt);
	nor[0] = normal.x;
	nor[1] = normal.y;
	nor[2] = normal.z;

	bool isok = acdbUcs2Ecs(adspt,adspto,nor,false);

	//acedTrans(adspt,&ucs,&wcs,0,adspto);
	AcGePoint3d pto;
	JHCOM_AdsToAcGe(adspto,pto);

	return pto;
}


int JHCOM_GetAcDbHandle(AcDbObjectId Id, AcDbHandle &handle)
{
	handle.setNull();
	AcDbObject *pobj = NULL;
	if(acdbOpenObject(pobj, Id, AcDb::kForRead)==Acad::eOk)
	{
		pobj->getAcDbHandle(handle);
		pobj->close();
		return 0;
	}
	return 69;
}

int JHCOM_GetAcDbHandles(vAcDbObjectId vIds, vAcDbHandle &vHandles)
{
	vHandles.clear();
	AcDbObject *pobj = NULL;
	for (int i = 0; i < vIds.size(); i++)
	{
		if (acdbOpenObject(pobj, vIds[i], AcDb::kForRead) == Acad::eOk)
		{
			AcDbHandle handle;
			pobj->getAcDbHandle(handle);
			pobj->close();
			vHandles.push_back(handle);
		}
	}
	return 0;
}

//转换AcDbPolyline to GeCurve2d
Acad::ErrorStatus JHCOM_convertPolylineToGeCurve(AcDbPolyline* pPoly, AcGeCurve2d*& pGeCurve)
{
	AcGeVector3d normal;
	double elevation;
	int nSegs;
	AcGeLineSeg2d *pLine;
	AcGeLineSeg2d line;
	AcGeCircArc2d *pArc;
	AcGeCircArc2d arc;
	AcGeVoidPointerArray geCurves;

	normal = pPoly->normal();
	elevation = pPoly->elevation();
	nSegs = pPoly->numVerts() - 1;

	for(int i = 0; i < nSegs; i++)
	{

		if(pPoly->segType(i) == AcDbPolyline::kLine)
		{
			pPoly->getLineSegAt(i, line);
			pLine = new AcGeLineSeg2d(line);
			geCurves.append(pLine); 
		}
		else if(pPoly->segType(i) == AcDbPolyline::kArc)
		{
			pPoly->getArcSegAt(i, arc);
			pArc = new AcGeCircArc2d(arc);
			geCurves.append( pArc );
		}
	}// for

	if(geCurves.length() == 1)
	{
		pGeCurve = (AcGeCurve2d*)(geCurves[0]);
	}
	else
	{
		pGeCurve = new AcGeCompositeCurve2d(geCurves);
	}
	return Acad::eOk;
}

//转换AcDbPolyline to GeCurve3d
/*
如果你的2dPolyline是简单，或者FIT的，可以通过AcDbPolyline::convertFrom(...)把2DPOLY转换成LWPOLYLINE,然后用上面的函数转换几何实体，然后求“样本点”。
http://bbs.xdcad.net/thread-14459-1-1.html
*/
Acad::ErrorStatus JHCOM_convertPolylineToGeCurve(AcDbPolyline* pPoly, AcGeCurve3d*& pGeCurve)
{
	AcGeVector3d normal;
	double elevation;
	int nSegs;
	AcGeLineSeg3d *pLine;
	AcGeLineSeg3d line;
	AcGeCircArc3d *pArc;
	AcGeCircArc3d arc;
	AcGeVoidPointerArray geCurves;

	normal = pPoly->normal();
	elevation = pPoly->elevation();
	nSegs = pPoly->numVerts() - 1;

	for(int i = 0; i < nSegs; i++)
	{

		if(pPoly->segType(i) == AcDbPolyline::kLine)
		{
			pPoly->getLineSegAt(i, line);
			pLine = new AcGeLineSeg3d(line);
			geCurves.append(pLine); 
		}
		else if(pPoly->segType(i) == AcDbPolyline::kArc)
		{
			pPoly->getArcSegAt(i, arc);
			pArc = new AcGeCircArc3d(arc);
			geCurves.append( pArc );
		}
	}// for

	if(geCurves.length() == 1)
	{
		pGeCurve = (AcGeCurve3d*)(geCurves[0]);
	}
	else
	{
		pGeCurve = new AcGeCompositeCurve3d(geCurves);
	}
	return Acad::eOk;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_ConvertDbCurveToGeCurve
* Description     : This function convert AcDbCurve to AcGeCurve3d.
* Note            : This function getted from object arx autodesk form.
* Input           : pDbCurve : AcDbCurve to get AcGeCurve3d.
* Output          : pGeCurve : AcGeCurve3d for AcDbCurve.
* Return          : Error status.
--------------------------------------------------------------------------*/
Acad::ErrorStatus JHCOM_ConvertDbCurveToGeCurve(AcDbCurve *pDbCurve,AcGeCurve3d *&pGeCurve)
{
	pGeCurve=NULL;
	if (pDbCurve->isKindOf(AcDbLine::desc()))
	{
		AcDbLine *pL=(AcDbLine *)pDbCurve;
		AcGeLineSeg3d *pGL=new AcGeLineSeg3d;
		pGL->set(pL->startPoint(),pL->endPoint());
		pGeCurve=(AcGeCurve3d *)pGL;
	}
	else if (pDbCurve->isKindOf(AcDbArc::desc()))
	{
		AcDbArc *pArc=(AcDbArc *)pDbCurve;
		double ans,ane;
		ans=pArc->startAngle();
		ane=pArc->endAngle();
		AcGeCircArc3d *pGArc=new AcGeCircArc3d(pArc->center(), pArc->normal(),pArc->radius());
		pGArc->setAngles(ans,ane);
		pGeCurve=(AcGeCurve3d *)pGArc;
	}
	else if (pDbCurve->isKindOf(AcDbCircle::desc()))
	{
		AcDbCircle *pCir=(AcDbCircle *)pDbCurve;
		AcGeCircArc3d * pGCir=new AcGeCircArc3d(pCir->center(), pCir->normal(),pCir->radius());
		pGeCurve=(AcGeCurve3d *)pGCir;
	}
	else if (pDbCurve->isKindOf(AcDbEllipse::desc()))
	{
		AcDbEllipse *pEli=(AcDbEllipse *)pDbCurve;
		AcGePoint3d pt1,center=pEli->center();
		AcGeEllipArc3d *pGEli=new AcGeEllipArc3d;
		pGEli->setCenter(center);
		pGEli->setAxes(pEli->majorAxis(),pEli->minorAxis());
		pEli->getClosestPointTo(center,pt1,Adesk::kTrue);
		pGEli->setMajorRadius(pt1.distanceTo(center)/pEli->radiusRatio());
		pGEli->setMinorRadius(pt1.distanceTo(center));
		double endang=pEli->endAngle(),startang=pEli->startAngle();
		if (startang>endang){
			endang+=2*PI;
		}
		pGEli->setAngles(endang,startang);
		pGeCurve=(AcGeCurve3d *)pGEli;
	}
	else if (pDbCurve->isKindOf(AcDbSpline::desc()))
	{
		AcDbSpline *pSL=(AcDbSpline *)pDbCurve;
		if (!pSL)
			return Acad::eNotImplemented;
		if (pSL->isNull()==Adesk::kTrue)
			return Acad::eNotImplemented;
		int degree;
		Adesk::Boolean rational;
		Adesk::Boolean closed;
		Adesk::Boolean periodic;
		AcGePoint3dArray controlPoints;
		AcGeDoubleArray knots;
		AcGeDoubleArray weights;
		double controlPtTol;
		double knotTol;
		AcGeTol tol;
		Acad::ErrorStatus es;
		es=pSL->getNurbsData(degree,rational,closed,periodic,controlPoints,knots,weights,
			controlPtTol,knotTol);
		if (es!=Acad::eOk)
			return Acad::eNotImplemented;
		if (rational==Adesk::kTrue)
		{
			AcGeNurbCurve3d *pNurb=new AcGeNurbCurve3d(degree,knots,controlPoints,weights,periodic);
			if (closed==Adesk::kTrue)
				pNurb->makeClosed();
			if (pSL->hasFitData()==Adesk::kTrue)
			{
				AcGePoint3dArray fitPoints;
				double fitTolerance;
				Adesk::Boolean tangentsExist;
				AcGeVector3d startTangent;
				AcGeVector3d endTangent;
				pSL->getFitData(fitPoints,degree,fitTolerance,tangentsExist,startTangent,endTangent);
				tol.setEqualPoint(fitTolerance);
				if (tangentsExist==Adesk::kTrue)
					pNurb->setFitData(fitPoints,startTangent,endTangent,tol);
				else
					pNurb->setFitData(degree,fitPoints,tol);
			}
			pGeCurve=(AcGeCurve3d *)pNurb;
		}
		else
		{
			AcGeNurbCurve3d *pNurb=new AcGeNurbCurve3d(degree,knots,controlPoints,periodic);
			if (closed==Adesk::kTrue)
				pNurb->makeClosed();
			if (pSL->hasFitData()==Adesk::kTrue)
			{
				AcGePoint3dArray fitPoints;
				double fitTolerance;
				Adesk::Boolean tangentsExist;
				AcGeVector3d startTangent;
				AcGeVector3d endTangent;
				pSL->getFitData(fitPoints,degree,fitTolerance,tangentsExist,startTangent,endTangent);
				tol.setEqualPoint(fitTolerance);
				if (tangentsExist==Adesk::kTrue)
					pNurb->setFitData(fitPoints,startTangent,endTangent,tol);
				else
					pNurb->setFitData(degree,fitPoints,tol);
			}
			pGeCurve=(AcGeCurve3d *)pNurb;
		}
	}
	else if ((pDbCurve->isKindOf(AcDb2dPolyline::desc()))||
		(pDbCurve->isKindOf(AcDbPolyline::desc())))
	{
		int type=0;
		AcDbPolyline *pPoly;
		if (pDbCurve->isKindOf(AcDb2dPolyline::desc()))
		{
			AcDb2dPolyline *p2L=(AcDb2dPolyline *)pDbCurve;
			AcDbEntity *penty = pDbCurve;
			//XdDbUtils::Poly2dToLWPoly(p2L,pPoly);
			pPoly = new AcDbPolyline();
			pPoly->convertFrom(penty);
			type=1;
		}
		else
			pPoly=(AcDbPolyline *)pDbCurve;
		
		JHCOM_convertPolylineToGeCurve(pPoly,pGeCurve);
		if (type)
			delete pPoly;
	}
	return (pGeCurve)?Acad::eOk:Acad::eNotImplemented;
}

AcDbObjectId JHCOM_GetConvertGeCurveToAcDbCurve(AcGeCurve3d &geCurve3d)

{
#if (defined ARX_2008) || (defined ARX_2009)||(defined ARX_2010) || (defined ARX_2011) || (defined ARX_2012) || (defined ARX_2013)
	return 0;
#else
	AcDbObjectId oid = AcDbObjectId::kNull;
	Acad::ErrorStatus es = Acad::eOk;
	AcDbCurve *pDbCurve = NULL;
	es = acdbConvertGelibCurveToAcDbCurve(geCurve3d, pDbCurve);
	if (es == Acad::eOk)
	{
		oid = JHCOM_PostToModelSpace(pDbCurve);
		//postToDb(pDbCurve, oid);
	}
	return oid;
#endif
}

size_t ConvertStringToUTF8( LPCTSTR strIn, char *& strOutUTF8MB )
{
	size_t len=_tcslen(strIn);

#ifdef UNICODE
	int iRequiredSize=WideCharToMultiByte(CP_UTF8, 0, strIn, -1, 0, 0, 0, 0);

	strOutUTF8MB=new char[iRequiredSize];
	strOutUTF8MB[0]=0;

	WideCharToMultiByte(CP_UTF8, 0, strIn, -1, strOutUTF8MB, iRequiredSize, 0, 0);
#else
	WCHAR * wChar=new WCHAR[len+1];
	wChar[0]=0;
	MultiByteToWideChar(CP_ACP, 0, strIn, (int)len+1, wChar, (int)len+1);
	int iRequiredSize=WideCharToMultiByte(CP_UTF8, 0, wChar, (int)len+1, 0, 0, 0, 0);
	strOutUTF8MB=new char[iRequiredSize];
	strOutUTF8MB[0]=0;
	WideCharToMultiByte(CP_UTF8, 0, wChar, (int)len+1, strOutUTF8MB, iRequiredSize, 0, 0);
	delete [] wChar;
#endif

	return iRequiredSize;
}

CString UTF8ToGBK(const std::string& strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	unsigned short * wszGBK = new unsigned short[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUTF8.c_str(), -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP,0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL);
	//strUTF8 = szGBK;
	CString strTemp(szGBK);
	delete[]szGBK;
	delete[]wszGBK;
	return strTemp;
}

CString GBKToUTF8(const std::string& strGBK)
{
	CString strOutUTF8 = "";
	WCHAR * str1;
	int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
	str1 = new WCHAR[n];
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
	char * str2 = new char[n];
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
	strOutUTF8 = str2;
	delete[]str1;
	str1 = NULL;
	delete[]str2;
	str2 = NULL;
	return strOutUTF8;
}

CString FileSizeToString(long long p_size)
{
	long long kSize = p_size >> 10; //转为k
	long long mSize = p_size >> 20; //转为M
	long long gSize = mSize >> 10; //转为G

	CString str;
	if (gSize>0)
	{
		str.Format(_T("%dM"), (int)mSize);
	}
	else if (mSize>0)
	{
		str.Format(_T("%.3fM"), ((double)kSize)/1000);
	}
	else
	{
		str.Format(_T("%dK"), (int)kSize);
	}

	return str;
}


CString BytesToHexString(const byte* input, size_t length) 
{
	CString ret;
	const wchar_t cHex[] = L"0123456789ABCDEF";
	for (size_t i = 0; i < length; i++)
	{
		byte temp = input[i];
		ret += cHex[temp / 16];
		ret += cHex[temp % 16];
	}
	return ret;
}

size_t HexStringToBytes(CString input, byte* pOutput)
{
	for (int i = 0; i < input.GetLength(); i++)
	{
		wchar_t x;
		if (input[i] >= 'A')
			x = input[i] - 'A' + 10;
		else
			x = input[i] - '0';
		pOutput[i / 2] = (pOutput[i / 2] << 4 & 0xFF) + x;
	}
	return input.GetLength() / 2;
}

//将double 转ACHAR*
//ACHAR* DoubleToACHAR(long double len)
//{
//	//double转string
//	stringstream ss;
//	ss << len;
//	string _str_Val = ss.str();
//	//string转const char*
//	const char* ac = _str_Val.c_str();
//	//const char*转ACHAR*
//	_bstr_t AStr = ac;
//	LPWSTR AstrW = LPTSTR(AStr);
//	ACHAR* entryName = (ACHAR *)AstrW;
//	return entryName;
//}

ACHAR* CStringToACHAR(CString cStr)
{
	string _str_Val = CW2A(cStr);
	//string转const char*
	const char* ac = _str_Val.c_str();
	//const char*转ACHAR*
	_bstr_t AStr = ac;
	LPWSTR AstrW = LPTSTR(AStr);
	ACHAR* entryName = (ACHAR *)AstrW;
	return entryName;
}