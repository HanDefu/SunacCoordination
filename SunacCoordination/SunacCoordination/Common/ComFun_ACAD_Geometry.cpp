#include "stdafx.h"
#include "ComFun_ACAD_Geometry.h"
#include <dbents.h>
#include <dbspline.h>
#include <dbmline.h>
#include "ComFun_ACAD_Common.h"
#include "ComFun_Math.h"

bool vfind(vAcDbObjectId &ids, AcDbObjectId &id, int &index)
{
    for(int i = 0; i < (int)ids.size(); i++)
	{
		if(ids[i] == id) 
		{
			index = i;
			return true;
		}
	}
	index = (int)ids.size();
	return false;
}

bool vfind(vCString &strs, CString &str, int &index)
{
    for(int i = 0; i < (int)strs.size(); i++)
    {
        if(strs[i].Compare(str) == 0) 
        {
            index = i;
            return true;
        }
    }
    index = (int)strs.size();
    return false;
}

bool vfind(AcDbObjectId &id, vAcDbObjectId &ids)
{
	for(int i = 0; i < (int)ids.size(); i ++)
		if(id == ids[i]) return true;
	return false;
}

bool vfind(CString &str, vCString &strs)
{
    for(int i = 0; i < (int)strs.size(); i ++)
        if(strs[i].Compare(str) == 0) return true;
    return false;
}

bool vfind(AcGePoint3d &pt, vAcGePoint3d &pts, double tol)
{
	for(int i = 0; i < (int)pts.size(); i ++)
		if(JHCOM_isPoint3dEqual(pt, pts[i], tol)) return true;
	return false;
}

bool vfind(double &d, vdouble &ds, double tol)
{
	for(int i = 0; i < (int)ds.size(); i ++)
		if(JHCOM_equ(d, ds[i], tol)) return true;
	return false;
}

double MD2010_GetAreaFromPoints(vAcGePoint3d &vpoints)
{
	AcGePoint3dArray array3d;
	for(int i = 0; i < (int)vpoints.size(); i++)
	{
		array3d.append(AcGePoint3d(vpoints[i].x,vpoints[i].y,vpoints[i].z));
	}
	AcDb2dPolyline *pplolyline = new AcDb2dPolyline(AcDb::k2dSimplePoly,array3d,0,Adesk::kTrue);
	double area = 0.0;
	pplolyline->getArea(area);
	delete pplolyline;
	return area;
}

void MD2010_RemoveCopyOfPoints(vAcGePoint3d &vpnts)
{
    for(int i = 0; i < (int)vpnts.size(); i++)
	{
		for(int j = i+1; j < (int)vpnts.size(); j++)
		{
			if(vpnts[j] == vpnts[i])
				vpnts.erase(vpnts.begin()+j);
		}
	}

	int len = (int)vpnts.size();
	if(len > 2)
	{
		if(JHCOM_isPoint3dEqual(vpnts[len -1], vpnts[len -2]))
			vpnts.erase(vpnts.begin()+len-1);
	}
}

void MD2010_RemoveCopyOfPoints(vAcGePoint3d &vpnts, double tol)
{
	for(int i = 0; i < (int)vpnts.size(); i++)
	{
		for(int j = i+1; j < (int)vpnts.size(); j++)
		{
			if(JHCOM_isPoint3dEqual(vpnts[j], vpnts[i], tol))
				vpnts.erase(vpnts.begin()+j);
		}
	}
    
	int len = (int)vpnts.size();
	if(len > 2)
	{
		if(JHCOM_isPoint3dEqual(vpnts[len -1], vpnts[len -2], tol))
			vpnts.erase(vpnts.begin()+len-1);
	}
	
}

void MD2010_GetExtrmePoint(vAcGePoint3d &in, AcGePoint3d &out1, AcGePoint3d &out2)
{
	if(((int)in.size()) <1) return;
	out2 = out1 = in[0];
	for(int i = 1; i < (int)in.size(); i++)
	{
        if(in[i].x < out1.x) out1.x = in[i].x;
		if(in[i].y < out1.y) out1.y = in[i].y;
		if(in[i].x > out2.x) out2.x = in[i].x;
		if(in[i].y > out2.y) out2.y = in[i].y;
	}
}

void MD2010_GetCurvePoints(vAcDbObjectId &curveIds, vAcGePoint3d &vpoints)
{
	AcDbObject *pEnt = NULL;
	for(int i = 0; i < (int)curveIds.size(); ++i)
	{
		AcGePoint3d pnt;
        acdbOpenAcDbObject(pEnt,curveIds[i],AcDb::kForRead);
	    
		AcDbLine *pline = AcDbLine::cast(pEnt);
		AcDbArc *parc = AcDbArc::cast(pEnt);
		AcDbSpline *pspline = AcDbSpline::cast(pEnt);
		AcDb2dPolyline *ppolyline = AcDb2dPolyline::cast(pEnt);
		AcDb3dPolyline *ppolyline3d = AcDb3dPolyline::cast(pEnt);
		AcDbMline *pmline = AcDbMline::cast(pEnt);
		AcDbPolyline *ppolyline0 = AcDbPolyline::cast(pEnt);
		if(pline)
		{   
			pline->getStartPoint(pnt);
			vpoints.push_back(pnt);
			pline->getEndPoint(pnt);
			vpoints.push_back(pnt);
		}
		else if(parc)
		{
			parc->getStartPoint(pnt);
			vpoints.push_back(pnt);
			parc->getEndPoint(pnt);
			vpoints.push_back(pnt);
		}
		else if(pspline)
		{
			pspline->getStartPoint(pnt);
			vpoints.push_back(pnt);
			pspline->getEndPoint(pnt);
			vpoints.push_back(pnt);
		}
		else if(ppolyline)
		{
			AcDbObjectIterator* itor =  ppolyline->vertexIterator();
			for (itor->start(); !itor->done(); itor->step())
			{
				AcDbEntity* pEnt = itor->entity();
				AcDb2dVertex *p2d = AcDb2dVertex::cast(pEnt);
				if (p2d != NULL)
				{
					vpoints.push_back(p2d->position());
				}
				pEnt->close(); 
			}
		}
		else if (pmline)
		{
			for(int j = 0; j < pmline->numVertices(); j ++)
			{
				vpoints.push_back(pmline->vertexAt(j));
			}
		}
		else if (ppolyline0)
		{
			int mnum = ppolyline0->numVerts();
			for(int j = 0; j < mnum; j ++)
			{
				AcGePoint2d p2d;
				ppolyline0->getPointAt(j,p2d);
				AcGePoint3d p3d = AcGePoint3d(p2d.x,p2d.y,0);
				vpoints.push_back(p3d);
			}
		}
		pEnt->close();
	}

	MD2010_RemoveCopyOfPoints(vpoints);
}
void MD2010_ComputeExtrmePointsForPaper(WCHAR * layername, AcGePoint3d &rectlb, AcGePoint3d &rectrt)
{
	vAcDbObjectId curveIds;
	vAcGePoint3d vpoints;
    MD2010_CycleAllTypedObjectsInLayer( layername, ACDB_CURVE, curveIds);   
	MD2010_GetCurvePoints(curveIds, vpoints);
	MD2010_GetExtrmePoint(vpoints, rectlb, rectrt);
}

void MD2010_ComputeExtrmeCenterForPaper(WCHAR * layername, AcGePoint3d &center)
{
	AcGePoint3d rectlb, rectrt;
	vAcDbObjectId curveIds;
	vAcGePoint3d vpoints;
    MD2010_CycleAllTypedObjectsInLayer( layername, ACDB_CURVE, curveIds);   
	MD2010_GetCurvePoints(curveIds, vpoints);
	MD2010_GetExtrmePoint(vpoints, rectlb, rectrt);
	center.x = (rectlb.x + rectrt.x)/2;
	center.y = (rectlb.y + rectrt.y)/2;
	center.z = (rectlb.z + rectrt.z)/2;
}

void MD2010_ComputeExtrmeCenterForPaper_SingleCurve(WCHAR * layername, AcDbObjectId curveId, AcGePoint3d &center)
{
    AcGePoint3d rectlb, rectrt;
    vAcDbObjectId curveIds;
    vAcGePoint3d vpoints;
    //MD2010_CycleAllTypedObjectsInLayer( layername, ACDB_CURVE, curveIds);   
    curveIds.push_back(curveId);
    MD2010_GetCurvePoints(curveIds, vpoints);
    MD2010_GetExtrmePoint(vpoints, rectlb, rectrt);
    center.x = (rectlb.x + rectrt.x)/2;
    center.y = (rectlb.y + rectrt.y)/2;
    center.z = (rectlb.z + rectrt.z)/2;
}

bool MD2010_GetTwoDiffPoints(vAcGePoint3d &pnts, AcGePoint3d &a, AcGePoint3d &b)
{
	if(((int)pnts.size()) <= 1) return false;
	a = pnts[0];
	for(int i = 1; i < (int)pnts.size(); i++)
	{
		if(pnts[i] != a)
		{
			b = pnts[i];
			return true;
		}
	}

	return false;
}

void MD2010_GetLineInstersectPoints(AcDbLine *pline, vAcDbObjectId &ids, vAcGePoint3d &pnts)
{
	AcDbObject *pEnt = NULL;
	AcGePoint3dArray points;
	for(int i = 0; i < (int)ids.size(); i++)
	{
        acdbOpenAcDbObject(pEnt,ids[i],AcDb::kForRead);
		AcDbCurve *pcurve = AcDbCurve::cast(pEnt);
		if(pcurve == NULL) 
		{
			pEnt->close();
			continue;
		}
		pline->intersectWith(pcurve, AcDb::kOnBothOperands, points);
		for(int j = 0; j < points.length(); j++ )
			pnts.push_back(points.at(j));
		points.removeAll();
		pEnt->close();
	}
}
void MD2010_SortX(vAcGePoint3d &indata, vAcGePoint3d&outdata)
{
    int len =(int)indata.size();
    vAcGePoint3d indataccopy;
	for (int i = 0; i < len; i++)
	{
		indataccopy.push_back(indata[i]);
	}
	for (int i = 0; i < len; i++)
	{
		outdata.push_back(indataccopy[i]);
		for (int j = i + 1; j < len; j++ )
		{
			if (indataccopy[j].x < outdata[i].x)
			{
				AcGePoint3d temp = outdata[i];
				outdata[i] = indataccopy[j];
				indataccopy[j] = temp;
			}
			
		}
	}
	
}
void MD2010_SortY(vAcGePoint3d &indata, vAcGePoint3d&outdata)
{
	int len =(int)indata.size();
	vAcGePoint3d indataccopy;
	for (int i = 0; i < len; i++)
	{
		indataccopy.push_back(indata[i]);
	}
	for (int i = 0; i < len; i++)
	{
		outdata.push_back(indataccopy[i]);
		for (int j = i + 1; j < len; j++ )
		{
			if (indataccopy[j].y < outdata[i].y)
			{
				AcGePoint3d temp = outdata[i];
				outdata[i] = indataccopy[j];
				indataccopy[j] = temp;
			}

		}
	}
}

bool MD2010_GetLineInstersect_Row(AcDbLine *pline, vAcDbObjectId &ids, vAcGePoint3d &out)
{
	vAcGePoint3d pnts;
	MD2010_GetLineInstersectPoints(pline, ids, pnts);
	/*if (((int)pnts.size())%2 != 0)
	{
		return false;
	}*///暂时去掉20110410
	MD2010_SortX(pnts, out);
    return true;
}

bool MD2010_GetLineInstersect_Coloum(AcDbLine *pline, vAcDbObjectId &ids, vAcGePoint3d &out)
{
	vAcGePoint3d pnts;
	MD2010_GetLineInstersectPoints(pline, ids, pnts);
	/*if (((int)pnts.size())%2 != 0)
	{
		return false;
	}*///临时去掉20110410
	MD2010_SortY(pnts, out);
	return true;
}

void MD2010_GetLineInstersectPoints(AcDbObjectId lineid, vAcDbObjectId &ids, vAcGePoint3d &pnts)
{
	AcDbObject *pEnt = NULL;
	acdbOpenAcDbObject(pEnt,lineid,AcDb::kForRead);
	AcDbLine *pline = AcDbLine::cast(pEnt);
	if(pline == NULL) 
		return;
	MD2010_GetLineInstersectPoints(pline, ids, pnts);
	pline->close();
}

bool MD2010_GetLineInstersectTwoPoints(AcDbLine *pline, vAcDbObjectId &ids, AcGePoint3d &a, AcGePoint3d &b)
{
	vAcGePoint3d pnts;
	MD2010_GetLineInstersectPoints(pline, ids, pnts);
	return MD2010_GetTwoDiffPoints(pnts, a, b);
}

bool MD2010_GetLineInstersectTwoPoints(AcDbObjectId lineid, vAcDbObjectId &ids, AcGePoint3d &a, AcGePoint3d &b)
{
	AcDbObject *pEnt = NULL;
	acdbOpenAcDbObject(pEnt,lineid,AcDb::kForRead);
	AcDbLine *pline = AcDbLine::cast(pEnt);
	if(pline == NULL) 
		return false;
	bool value = MD2010_GetLineInstersectTwoPoints(pline, ids, a, b);
	pline->close();
	return value;
}
void MD2010_GetAroundPointsFromOnePoint(AcGePoint3d &pnt, vAcGePoint3d &pnts, double d, bool twosidesflag)
{
	AcGePoint3d temp;
	if (twosidesflag == true)
	{
		temp = AcGePoint3d(pnt.x + 1 * d, pnt.y + 1 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + -1 * d, pnt.y + -1 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + -1 * d, pnt.y + 1 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 1 * d, pnt.y + -1 * d, 0.0);
		pnts.push_back(temp);

		temp = AcGePoint3d(pnt.x + 3 * d, pnt.y + 1 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + -3 * d, pnt.y + -1 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + -3 * d, pnt.y + 1 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 3 * d, pnt.y + -1 * d, 0.0);
		pnts.push_back(temp);

		temp = AcGePoint3d(pnt.x + 1 * d, pnt.y + 3 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + -1 * d, pnt.y + -3 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + -1 * d, pnt.y + 3 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 1 * d, pnt.y + -3 * d, 0.0);
		pnts.push_back(temp);

		temp = AcGePoint3d(pnt.x + 3 * d, pnt.y + 3 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + -3 * d, pnt.y + -3 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + -3 * d, pnt.y + 3 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 3 * d, pnt.y + -3 * d, 0.0);
		pnts.push_back(temp);

		//以上是4×4
		//以下是6×6
		temp = AcGePoint3d(pnt.x + 5 * d, pnt.y - 5 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 5 * d, pnt.y -3 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 5 * d, pnt.y - 1 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 5 * d, pnt.y + 1 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 5 * d, pnt.y + 3 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 5 * d, pnt.y + 5 * d, 0.0);
		pnts.push_back(temp);

		temp = AcGePoint3d(pnt.x - 5 * d, pnt.y - 5 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 5 * d, pnt.y -3 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 5 * d, pnt.y - 1 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 5 * d, pnt.y + 1 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 5 * d, pnt.y + 3 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 5 * d, pnt.y + 5 * d, 0.0);
		pnts.push_back(temp);


		temp = AcGePoint3d(pnt.x - 1 * d, pnt.y - 5 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 3 * d, pnt.y - 5 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 1 * d, pnt.y - 5 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 3 * d, pnt.y - 5 * d, 0.0);
		pnts.push_back(temp);

		temp = AcGePoint3d(pnt.x - 1 * d, pnt.y + 5 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 3 * d, pnt.y + 5 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 1 * d, pnt.y + 5 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 3 * d, pnt.y + 5 * d, 0.0);
		pnts.push_back(temp);
	}
	else
	{
		temp = AcGePoint3d(pnt.x + 0 * d, pnt.y - 6 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 0 * d, pnt.y -4 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 0 * d, pnt.y -2 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 0 * d, pnt.y -0 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 0 * d, pnt.y + 2 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 0 * d, pnt.y + 4 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 0 * d, pnt.y + 6 * d, 0.0);
		pnts.push_back(temp);

		temp = AcGePoint3d(pnt.x + 2 * d, pnt.y - 6 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 2 * d, pnt.y -4 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 2 * d, pnt.y -2 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 2 * d, pnt.y -0 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 2 * d, pnt.y + 2 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 2 * d, pnt.y + 4 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 2 * d, pnt.y + 6 * d, 0.0);
		pnts.push_back(temp);

		temp = AcGePoint3d(pnt.x + 4 * d, pnt.y - 6 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 4 * d, pnt.y -4 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 4 * d, pnt.y -2 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 4 * d, pnt.y -0 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 4 * d, pnt.y + 2 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 4 * d, pnt.y + 4 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 4 * d, pnt.y + 6 * d, 0.0);
		pnts.push_back(temp);

		temp = AcGePoint3d(pnt.x + 6 * d, pnt.y - 6 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 6 * d, pnt.y -4 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 6 * d, pnt.y -2 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 6 * d, pnt.y -0 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 6 * d, pnt.y + 2 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 6 * d, pnt.y + 4 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x + 6 * d, pnt.y + 6 * d, 0.0);
		pnts.push_back(temp);

		temp = AcGePoint3d(pnt.x - 2 * d, pnt.y - 6 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 2 * d, pnt.y -4 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 2 * d, pnt.y -2 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 2 * d, pnt.y -0 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 2 * d, pnt.y + 2 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 2 * d, pnt.y + 4 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 2 * d, pnt.y + 6 * d, 0.0);
		pnts.push_back(temp);

		temp = AcGePoint3d(pnt.x - 4 * d, pnt.y - 6 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 4 * d, pnt.y -4 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 4 * d, pnt.y -2 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 4 * d, pnt.y -0 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 4 * d, pnt.y + 2 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 4 * d, pnt.y + 4 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 4 * d, pnt.y + 6 * d, 0.0);
		pnts.push_back(temp);

		temp = AcGePoint3d(pnt.x - 6 * d, pnt.y - 6 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 6 * d, pnt.y -4 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 6 * d, pnt.y -2 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 6 * d, pnt.y -0 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 6 * d, pnt.y + 2 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 6 * d, pnt.y + 4 * d, 0.0);
		pnts.push_back(temp);
		temp = AcGePoint3d(pnt.x - 6 * d, pnt.y + 6 * d, 0.0);
		pnts.push_back(temp);
	}


}

void MD2010_GetDiffPointX(vAcGePoint3d &vpnts, vAcGePoint3d &opts, double tol)
{
	vdouble ds;
	for(int i = 0; i < (int)vpnts.size(); i++)
	{
		if (!vfind(vpnts[i].x, ds))
		{
		    opts.push_back(vpnts[i]);
			ds.push_back(vpnts[i].x);
		}
	}
}

void MD2010_GetDiffPointY(vAcGePoint3d &vpnts, vAcGePoint3d &opts, double tol)
{
	vdouble ds;
	for(int i = 0; i < (int)vpnts.size(); i++)
	{
		if (!vfind(vpnts[i].y, ds))
		{
			opts.push_back(vpnts[i]);
			ds.push_back(vpnts[i].y);
		}
	}
}
