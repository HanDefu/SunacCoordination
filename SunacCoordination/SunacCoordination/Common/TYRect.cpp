#include "StdAfx.h"
#include "TYRect.h"
#include <float.h>
#include <acdocman.h>
#include <dbxutil.h>
#include "ComFun_Math.h"
#include "ComFun_ACAD_Common.h"

TYRect::TYRect()
{
    m_lb = AcGePoint3d(DBL_MAX,DBL_MAX,0);
	m_rt = AcGePoint3d(-DBL_MAX,-DBL_MAX,0);
	m_ucs = false;
}

TYRect::TYRect(AcGePoint3d lb, AcGePoint3d rt)
{
    m_lb = lb;
	m_rt = rt;
	m_ucs = false;
}



TYRect::~TYRect()
{

}

void TYRect::Add(AcGePoint3d pt)
{
	if (m_lb.x > pt.x)
		m_lb.x = pt.x;
	if (m_lb.y > pt.y)
		m_lb.y = pt.y;


	if (m_rt.x < pt.x)
		m_rt.x = pt.x;

	if (m_rt.y < pt.y)
		m_rt.y = pt.y;
}

void TYRect::Add(TYRect &other)
{
	Add(other.GetLB());
	Add(other.GetRT());
}

bool TYRect::IsPointIn(AcGePoint3d pt)
{
	/*ads_point apt,apt1,apt2;
	ads_point bpt,bpt1,bpt2;
	JHCOM_AcGeToads(pt,apt);
	JHCOM_AcGeToads(m_lb,apt1);
	JHCOM_AcGeToads(m_rt,apt2);

	acdbWcs2Ucs(apt, bpt,false);
	acdbWcs2Ucs(apt1, bpt1,false);
	acdbWcs2Ucs(apt2, bpt2,false);

	AcGePoint3d ppt,ppt1,ppt2;
	JHCOM_AdsToAcGe(bpt,ppt);
	JHCOM_AdsToAcGe(bpt1,ppt1);
	JHCOM_AdsToAcGe(bpt2,ppt2);

	double xx = (ppt.x - ppt1.x)*(ppt.x - ppt2.x);
	double yy = (ppt.y - ppt1.y)*(ppt.y - ppt2.y);
	if (xx <= 0 && yy <= 0) return true;
	return false;*/

	//¹«²î
    
	double xx = (pt.x - m_lb.x)*(pt.x - m_rt.x);
	double yy = (pt.y - m_lb.y)*(pt.y - m_rt.y);
	if (xx <= 0 && yy <= 0) return true;
	//¹«²î
	/*if(yy > 0)
	{
        pt.y += pt.y * 1.0/100000;
		yy = (pt.y - m_lb.y)*(pt.y - m_rt.y);
		if(yy < 0)
			return true;
	}*/
	return false;
}
bool TYRect::IsPointOn(AcGePoint3d pt, double tol)
{
	//¹«²î
	double xx = (pt.x - m_lb.x)*(pt.x - m_rt.x);
	double yy = (pt.y - m_lb.y)*(pt.y - m_rt.y);
	if (fabs(xx) < tol && yy < tol)
	{
		return true;
	}

	if (fabs(yy) < tol && xx < tol)
	{
		return true;
	}
	return false;
}
bool TYRect::IsRectIn(TYRect &other)
{
	return IsPointIn(other.GetLB()) && IsPointIn(other.GetRT());
}

AcGePoint3d TYRect::GetCenter()
{
    return AcGePoint3d((m_lb.x + m_rt.x)/2.0,(m_lb.y + m_rt.y)/2.0,(m_lb.z + m_rt.z)/2.0);
}


void TYRect::Draw(double offset, CString layout)
{
	acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite, NULL, NULL, true);

	AcGePoint3d lb = m_lb;
	AcGePoint3d rt = m_rt;
	AcGePoint3d lt = GetLT();
	AcGePoint3d rb = GetRB();

	if (!m_ucs)
	{
		lb.x -= offset;
		lb.y -= offset;
		lb.z = 0; 

		rt.x += offset;
		rt.y += offset;
		rt.z = 0;

		lt = lb;
		lt.y = rt.y;

		rb = rt;
		rb.y = lb.y;
	}

	vAcGePoint3d pnts;
	pnts.push_back(lt);
	pnts.push_back(lb);
	pnts.push_back(rb);
	pnts.push_back(rt);
	pnts.push_back(lt);


	MD2010_CreatePolyLine(pnts);

	acDocManager->unlockDocument(acDocManager->curDocument());
}

bool TYRect::IsSame(TYRect &other, double tol)
{
	if(m_ucs)
	{
		bool eq1 = JHCOM_isPoint3dEqual(m_lb, other.m_lb,tol);
		bool eq2 = JHCOM_isPoint3dEqual(m_rt, other.m_rt,tol);
		if(eq1 && eq2 )
		{
			return true;
		}
		return false;
	}
	else
	{
        bool eq1 = JHCOM_isPoint3dEqual(m_lb, other.m_lb,tol);
		bool eq2 = JHCOM_isPoint3dEqual(m_rt, other.m_rt,tol);
		if(eq1 && eq2)
		{
			return true;
		}
		return false;
	}
}