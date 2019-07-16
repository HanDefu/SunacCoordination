#pragma once

#include <vector>
#include <gecomp3d.h>
#include <brshell.h>
#include <brcplx.h >

//----------------------------------Template Functions and special functions----------------------------------//
/*-------------------------------------------------------------------------
* Function Name   : JHCOM_vFind
* Description     : This function find one object in the related object vector,
*                   if exist, return the index, else return -1.
* Note            : NULL.
* Input           : B : Object to find.
*                   A : Object vector for finding.
* Output          : NULL.
* Return          : If exist, return the index, else return -1..
--------------------------------------------------------------------------*/
template<typename T> 
int JHCOM_vFind(T &B, std::vector<T>&A)
{
	for(int i = 0; i < (int)A.size(); i++)
	{
		if(A[i] == B)
			return i;
	}
	return -1;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_FindSame
* Description     : This function find same object of two vectors and put the same ones
*                   in another vector.
* Note            : NULL.
* Input           : A : First vector to find same Objects.
*                   B : Second vector to find same Objects.
*                   same : Vector to store same ones.
* Output          : NULL.
* Return          : How many same ones.
--------------------------------------------------------------------------*/
template<typename T> 
int JHCOM_FindSame(std::vector<T>&A, std::vector<T>&B, std::vector<T>&same)
{
	for (int i = 0; i < (int)A.size(); i++)
	{
		if(JHCOM_Find(B,A[i]) >=0)
			same.push_back(A[i]);
	}
	return (int)same.size();
}

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
bool              JHCOM_big(double a, double b, double tol = TOL);
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
bool              JHCOM_sma(double a, double b, double tol = TOL);
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
bool              JHCOM_equ(double a, double b, double tol = TOL);
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
int               JHCOM_cmp(double a, double b, double tol = TOL);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetMax
* Description     : Get maxsize value from double vector.
* Note            : If vector has no value, retur DBL_MIN.
* Input           : values : Double vector to find max one.
* Output          : NULL.
* Return          : max value.
--------------------------------------------------------------------------*/
double            JHCOM_GetMax(vdouble &values);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetMin
* Description     : Get minimize value from double vector.
* Note            : If vector has no value, return DBL_MAX.
* Input           : values : Double vector to find minimize one.
* Output          : NULL.
* Return          : minimize value.
--------------------------------------------------------------------------*/
double            JHCOM_GetMin(vdouble &values);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetInt45
* Description     : Get the related int value of double value according to "Àƒ…·ŒÂ»Î".
* Note            : If decimal bigger than 0.4 return bigger one, else return smaller int value.
* Input           : input : Double double to get int value.
* Output          : NULL.
* Return          : Int value.
--------------------------------------------------------------------------*/
int               JHCOM_GetInt45(double input);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_PointDistance
* Description     : This function get the distance of two AcGePoint3d points.
* Note            : NULL.
* Input           : a : First point.
*                   b : Second point.
* Output          : NULL.
* Return          : point distance.
--------------------------------------------------------------------------*/
double            JHCOM_PointDistance(AcGePoint3d a, AcGePoint3d b);

int JHCOM_Round(double f);

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
int JHCOM_GetOneCurveFromCurveVectorByDistance(AcGeCurve3d *firstCurve, vAcGeCurve3dP &vGeCurs, double dist, double tol);


/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetAngleFromThreePointForArc
* Description     : This is a method to calculate the angle from three point.
* Note            : NULL.
* Input           : first : first pnt, which is on the arc. 
*                   second : second pnt, which is on the arc. 
*                   tanp : we make two tangent line from first and second point, then there will be a intersection
*                   of these two tangent lines, this is that intersection point.
* Output          : NULL.
* Return          : angle.
--------------------------------------------------------------------------*/
double JHCOM_GetAngleFromThreePointForArc(AcGePoint3d first, AcGePoint3d tanp, AcGePoint3d second);
double JHCOM_GetAngleFromTwoPointsAndR(AcGePoint3d first, AcGePoint3d second, double rad);
/*-------------------------------------------------------------------------
* Function Name   : JHCOM_isPoint3dEqual
* Description     : Are the input two pnts same or not in a tolerance?
* Note            : NULL.
* Input           : a : first pnt. 
*                   b : second pnt. 
* Output          : NULL.
* Return          : Same or not.
--------------------------------------------------------------------------*/
bool JHCOM_isPoint3dEqual(AcGePoint3d &a, AcGePoint3d &b, double tol = 0.1);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_LoopCountCurve
* Description     : This is a good and stable method to find a loop from an array of curves.
* Note            : NULL.
* Input           : pCurves : Curve array to find loop. 
*                   seedOri : For the recircle reason we should give this additional parameter, you can just give it
*                             same with seedPnt., this point is fixed, in order to find the last closed loop.
*                   seedPnt : you can give it just same with seedOri, but this point will change every time in the find loop.
*                   findIndex : just a flag, you can give it to 0.
* Output          : loop -loop we find.
* Return          : Fail code.
--------------------------------------------------------------------------*/
int  JHCOM_LoopCountCurve(vAcGeCurve3dP &pCurves, AcGePoint3d &seedOri, AcGePoint3d seedPnt, int &findIndex, vAcGeCurve3dP &loop);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetOneSameCurveFromCurveVector
* Description     : This function get a similar same curve from a curve vector use statistic method.
* Note            : We just check 10 sample points on curve, and we just find one then return.
* Input           : firstCurve :Curve to find same curve. 
*                   vGeCurs : Candidate curves.
* Output          : NULL.
* Return          : Same curve index.
--------------------------------------------------------------------------*/
int JHCOM_GetOneSameCurveFromCurveVector(AcGeCurve3d *firstCurve, vAcGeCurve3dP &vGeCurs);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetPointNearestPoint
* Description     : This function get a nearest point from a point array.
* Note            : NULL.
* Input           : pt : seed point.
*                   pnts : pnts to fine nearest one.
* Output          : NULL.
* Return          : Nearest point index.
--------------------------------------------------------------------------*/
int JHCOM_GetPointNearestPoint(AcGePoint3d pt, vAcGePoint3d &pnts);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetPointNearestPoint
* Description     : This function get a nearest point from a point array.
* Note            : NULL.
* Input           : pt : seed point.
*                   pnts : pnts to fine nearest one.
* Output          : NULL.
* Return          : Nearest point index.
--------------------------------------------------------------------------*/
int JHCOM_GetPointNearestPoint(AcGePoint3d pt, AcGePoint3dArray &pointArray);

double Omit(double in);
double GetAngle(double Y,double Z);
double JHCOM_A2R(double angle);
double JHCOM_R2A(double rad);
