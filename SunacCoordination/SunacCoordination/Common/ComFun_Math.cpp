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
* Function Name   : JHCOM_big
* Description     : Dose a bigger than b with tol?
* Note            : NULL.
* Input           : a : First value to compare.
*                   b : Second value to compare.
*                   tol : tolerance.
* Output          : NULL.
* Return          : Bigger : true, else false.
--------------------------------------------------------------------------*/
bool JHCOM_big(double a, double b, double tol)
{
	if(a - b > tol) return true;
	return false;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_sma
* Description     : Dose a smaller than b with tol?
* Note            : NULL.
* Input           : a : First value to compare.
*                   b : Second value to compare.
*                   tol : tolerance.
* Output          : NULL.
* Return          : Smaller : true, else false.
--------------------------------------------------------------------------*/
bool JHCOM_sma(double a, double b, double tol)
{
	if(b - a > tol) return true;
	return false;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_equ
* Description     : Dose a equal to b with tol?
* Note            : NULL.
* Input           : a : First value to compare.
*                   b : Second value to compare.
*                   tol : tolerance.
* Output          : NULL.
* Return          : equal : true, else false.
--------------------------------------------------------------------------*/
bool JHCOM_equ(double a, double b, double tol)
{
	if(!JHCOM_big(a,b,tol) && !JHCOM_sma(a,b,tol)) return true;
	return false;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_cmp
* Description     : Compare a and b.
* Note            : NULL.
* Input           : a : First value to compare.
*                   b : Second value to compare.
*                   tol : tolerance.
* Output          : NULL.
* Return          : bigger:1, equal : 0,  smaller : -1.
--------------------------------------------------------------------------*/
int JHCOM_cmp(double a, double b, double tol)
{
	if(JHCOM_big(a,b,tol)) return 1;
	if(JHCOM_sma(a,b,tol)) return -1;
	return 0;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetMax
* Description     : Get maxsize value from double vector.
* Note            : If vector has no value, retur DBL_MIN.
* Input           : values : Double vector to find max one.
* Output          : NULL.
* Return          : max value.
--------------------------------------------------------------------------*/
double JHCOM_GetMax(vdouble &values)
{
	if(values.size() == 0)
		return DBL_MIN;
	double max = values[0];
	for(UINT i = 1; i < values.size(); i++)
	{
		if(values[i] > max)
		{
			max = values[i];
		}
	}
	return max;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetMin
* Description     : Get minimize value from double vector.
* Note            : If vector has no value, retur DBL_MAX.
* Input           : values : Double vector to find minimize one.
* Output          : NULL.
* Return          : minimize value.
--------------------------------------------------------------------------*/
double JHCOM_GetMin(vdouble &values)
{
	if(values.size() == 0)
		return DBL_MAX;
	double min = values[0];
	for(UINT i = 1; i < values.size(); i++)
	{
		if(values[i] < min)
		{
			min = values[i];
		}
	}
	return min;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetInt45
* Description     : Get the related int value of double value according to "四舍五入".
* Note            : If decimal bigger than 0.4 return bigger one, else return smaller int vlue.
* Input           : input : Double double to get int value.
* Output          : NULL.
* Return          : Int value.
--------------------------------------------------------------------------*/
int JHCOM_GetInt45(double input)
{
    int ini = (int)input;
	if(input - ini > 0.4)
		ini++;
	else
		ini;
	return ini;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_PointDistance
* Description     : This function get the distance of two AcGePoint3d points.
* Note            : NULL.
* Input           : a : First point.
*                   b : Second point.
* Output          : NULL.
* Return          : point distance.
--------------------------------------------------------------------------*/
double JHCOM_PointDistance(AcGePoint3d a, AcGePoint3d b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) +
		        (a.y - b.y) * (a.y - b.y) +
				(a.z - b.z) * (a.z - b.z) );
}


double Omit(double in)
{
	if ( in < 0.001 && in > -0.001 )
		return 0.000;
	else
		return ((long)(in*1000))/1000.0;
}

double GetAngle(double Y,double Z)
{
	double d_Angle;
	double i_Angle;
	//		string path = "d:\\angle.txt";
	//		ofstream outfile(path.c_str(),ios_base::app);


	if(Omit(Z)>0)
	{
		d_Angle = atan(Y/Z);
		i_Angle=d_Angle*180.0/PI;
		if(Omit(Y)>0)
			i_Angle = i_Angle-360;
	}
	else if(Omit(Z) == 0)
	{
		if(Omit(Y)>0)
			i_Angle = -270;
		else if(Omit(Y)<0)
			i_Angle = -90;
		else
		{
			i_Angle = 0;
		}
	}
	else
	{
		d_Angle = atan(Y/Z);
		i_Angle = d_Angle*180.0/PI -180;
	}
	//		outfile<<Y<<" "<<Z<<" "<<i_Angle<<endl;
	return i_Angle;//return (-360,0]
}


int JHCOM_Round(double f)
{
	int up=(int)ceil(f);
	int down=(int)floor(f);
	if (fabs(f-down)>fabs(up-f))
		return up;
	else
		return down;
}
double JHCOM_GetAngleFromThreePointForArc(AcGePoint3d first, AcGePoint3d tanp, AcGePoint3d second)
{
	AcGePoint3d mid((first.x + second.x)/2.0,(first.y + second.y)/2.0,(first.z + second.z)/2.0);
	double dis1 = JHCOM_PointDistance(mid, tanp);
	double dis2 = JHCOM_PointDistance(mid, first);
	double angle = atan(dis1/dis2) * 2;
	return angle;
}

double JHCOM_GetAngleFromTwoPointsAndR(AcGePoint3d first, AcGePoint3d second, double rad)
{
	double dis = JHCOM_PointDistance(first, second)/2.0;
	return asin(dis/rad) * 2;
}

bool JHCOM_isPoint3dEqual(AcGePoint3d &a, AcGePoint3d &b, double tol)
{
	if (fabs(a.x - b.x) < tol  &&
		fabs(a.y - b.y) < tol &&
		fabs(a.z - b.z) < tol)
	{
		return true;
	}
	else
		return false;
}

//递归搜索
int JHCOM_LoopCountCurve(vAcGeCurve3dP &pCurves, AcGePoint3d &seedOri, AcGePoint3d seedPnt, int &findIndex, vAcGeCurve3dP &loop)
{
	if(pCurves.size() == 0 && JHCOM_isPoint3dEqual(seedOri,seedPnt))//含一个曲线都没有的情况
		return 0;//寻找完毕


	if(pCurves.size() == 0)//含一个曲线都没有的情况
		return 1;//寻找完毕 但是没找到

	int ret = 0;
	AcGePoint3d d3s, d3e;
	AcGePoint3d seedOld = seedPnt;
	int index = 0;
	for (int j = 0; j < (int)pCurves.size(); j++)
	{
		d3s = JHCOM_GetCurveStart(pCurves[j]);
		d3e = JHCOM_GetCurveEnd(pCurves[j]);

		if (JHCOM_isPoint3dEqual(seedPnt, d3e))
		{
			seedPnt = d3s;
			loop.push_back(pCurves[j]);
			pCurves.erase(pCurves.begin() + j--);
			index=j+1;
			ret = JHCOM_LoopCountCurve(pCurves, seedOri, seedPnt, index, loop);
			if(ret == -1)
			{
				seedPnt = seedOld;
				loop.pop_back();
				if(index<=j)
					j=index-1;
			}
			else if(ret == 0)
				return 0;
		}

		if (JHCOM_isPoint3dEqual(seedPnt, d3s))
		{
			seedPnt = d3e;
			loop.push_back(pCurves[j]);
			pCurves.erase(pCurves.begin() + j--);
			index=j+1;
			ret = JHCOM_LoopCountCurve(pCurves, seedOri, seedPnt, index, loop);
			if(ret == -1)
			{
				seedPnt = seedOld;
				loop.pop_back();
				if(index<=j)
					j=index-1;
			}
			else if(ret == 0)
				return 0;
		}
	}//找到一个相邻的点,放进来,

	if (JHCOM_isPoint3dEqual(seedOri,seedPnt))
	{
		return 0;
	}

	return -1;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetOneCurveFromCurveVectorByDistance
* Description     : This function get a similar parallel curve (same distance on several points)
*                   from a curve vector use statistic method.
* Note            : We just check 10 sample points on curve, and we just find one then return.
* Input           : firstCurve :Curve to find parallel curve. 
*                   vGeCurs : Candidate curves.
*                   dist:parallel distance.
*                   tol : distance tolerance.
* Output          : NULL.
* Return          : Finded curve index.
--------------------------------------------------------------------------*/
int JHCOM_GetOneCurveFromCurveVectorByDistance(AcGeCurve3d *firstCurve, vAcGeCurve3dP &vGeCurs, double dist, double tol)
{
	AcGePoint3dArray pointArray;
	firstCurve->getSamplePoints(10,pointArray);
	double dis = 0;
	for (UINT i = 0; i < vGeCurs.size(); i++)
	{
		int isSame = 0;
		for (int j = 0; j < pointArray.length(); j++)
		{
			AcGePoint3d pt = vGeCurs[i]->closestPointTo(pointArray[j]);
			dis = JHCOM_PointDistance(pt, pointArray[j]);
			if(fabs(dis - dist) < tol)
				isSame++;
		}
		if (isSame == pointArray.length())
		{
			return i;
		}
	}
	return -1;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetOneSameCurveFromCurveVector
* Description     : This function get a similar same curve from a curve vector use statistic method.
* Note            : We just check 10 sample points on curve, and we just find one then return.
* Input           : firstCurve :Curve to find same curve. 
*                   vGeCurs : Candidate curves.
* Output          : NULL.
* Return          : Same curve index.
--------------------------------------------------------------------------*/
int JHCOM_GetOneSameCurveFromCurveVector(AcGeCurve3d *firstCurve, vAcGeCurve3dP &vGeCurs)
{
	AcGePoint3dArray pointArray;
	firstCurve->getSamplePoints(10,pointArray);
	double dis = 0;
    for (UINT i = 0; i < vGeCurs.size(); i++)
    {
		int isSame = 0;
		for (int j = 0; j < pointArray.length(); j++)
		{
			AcGePoint3d pt = vGeCurs[i]->closestPointTo(pointArray[j]);
			dis = JHCOM_PointDistance(pt, pointArray[j]);
			if(dis < TOL*10)
				isSame++;
		}
		if (isSame == pointArray.length())
		{
			return i;
		}
    }
	return -1;
}

int JHCOM_GetPointNearestPoint(AcGePoint3d pt, vAcGePoint3d &pnts)
{
	double minidist = DBL_MAX;
	int index = 0;
	for (UINT i = 0; i < pnts.size(); i++)
	{
		double curdis = pt.distanceTo(pnts[i]);
		if(curdis < minidist)
		{
			index = i;
			minidist = curdis;
		}
	}
	return index;
}

int JHCOM_GetPointNearestPoint(AcGePoint3d pt, AcGePoint3dArray &pointArray)
{
	double minidist = DBL_MAX;
	int index = 0;
	for (int i = 0; i < pointArray.length(); i++)
	{
		double curdis = pt.distanceTo(pointArray[i]);
		if(curdis < minidist)
		{
			index = i;
			minidist = curdis;
		}
	}
	return index;
}

double JHCOM_A2R(double angle)
{
	return angle*PI/180;
}

double JHCOM_R2A(double rad)
{
	return rad*180/PI;
}

bool MD2010_CheckPointIsInRect(AcGePoint3d pt1, AcGePoint3d pt3, AcGePoint3d pt)
{
	double xx = (pt.x - pt1.x)*(pt.x - pt3.x);
	double yy = (pt.y - pt1.y)*(pt.y - pt3.y);
	if (xx <= 0 && yy <= 0) return true;
	return false;
}
