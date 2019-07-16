#pragma once

#include <dbents.h>


/*-----------------------------查找系列-------------------*/
bool vfind(AcDbObjectId &id, vAcDbObjectId &ids);
bool vfind(vAcDbObjectId &ids, AcDbObjectId &id, int &index);
bool vfind(vCString &strs, CString &str, int &index);
bool vfind(CString &str, vCString &strs);
bool vfind(AcGePoint3d &pt, vAcGePoint3d &pts, double tol = TOL);
bool vfind(double &pt, vdouble &pts, double tol = TOL);


/*-----------------------------计算极点系列-------------------*/
//这个函数在给的一系列点找到最左 最下， 最右，最上的四个坐标值，组成两个点给出
void MD2010_GetExtrmePoint(vAcGePoint3d &in, AcGePoint3d &out1, AcGePoint3d &out2);

//这个函数给一系列的曲线，计算这些曲线的所有点，写入容器
void MD2010_GetCurvePoints(vAcDbObjectId &curveIds, vAcGePoint3d &vpoints);

//这个函数给一个图层，计算出图层所有曲线的最左下 和 右上点， 用在地坪材质，以及算比例的时候用
void MD2010_ComputeExtrmePointsForPaper(WCHAR * layername, AcGePoint3d &lb, AcGePoint3d &rt);

//这个函数是在计算出来两个极点后，算出他们的中心点
void MD2010_ComputeExtrmeCenterForPaper(WCHAR * layername, AcGePoint3d &center);

//针对单个曲线应用以上方法，得到中心点
void MD2010_ComputeExtrmeCenterForPaper_SingleCurve(WCHAR * layername, AcDbObjectId curveId, AcGePoint3d &center);


/*-----------------------------计算相交系列-------------------*/

//给一个线段，然后给一些列的曲线，计算这个线段和这些曲线的所有交点
void MD2010_GetLineInstersectPoints(AcDbObjectId lineid, vAcDbObjectId &ids, vAcGePoint3d &pnts);

//给一个线段，和一系列的曲线，计算所有的交点后，找到两个不一样的点
bool MD2010_GetLineInstersectTwoPoints(AcDbObjectId lineid, vAcDbObjectId &ids, AcGePoint3d &a, AcGePoint3d &b);

//同MD2010_GetLineInstersectPoints，指针形式
void MD2010_GetLineInstersectPoints(AcDbLine *pline, vAcDbObjectId &ids, vAcGePoint3d &pnts);

//同上，指针形式
bool MD2010_GetLineInstersectTwoPoints(AcDbLine *pline, vAcDbObjectId &ids, AcGePoint3d &a, AcGePoint3d &b);

//一个直线和一系列的曲线，计算交点后，把交点按照X增加顺序排列
bool MD2010_GetLineInstersect_Row(AcDbLine *pline, vAcDbObjectId &ids, vAcGePoint3d &out);

//一个直线和一系列的曲线，计算交点后，把交点按照Y增加顺序排列
bool MD2010_GetLineInstersect_Coloum(AcDbLine *pline, vAcDbObjectId &ids, vAcGePoint3d &out);


/*--------------------其他函数---------------------------*/

//这个函数从给一个点，计算他周围的 一圈点，用于多媒体图 的生成
void MD2010_GetAroundPointsFromOnePoint(AcGePoint3d &pnt, vAcGePoint3d &pnts, double d, bool twosidesflag = true);

//所有点按照X增加排序
void MD2010_SortX(vAcGePoint3d &indata, vAcGePoint3d&outdata);

//所有点按照Y增加排序
void MD2010_SortY(vAcGePoint3d &indata, vAcGePoint3d&outdata);

//这个函数给一些列的点，把它组合成一个封闭多段线，计算他的面积
double MD2010_GetAreaFromPoints(vAcGePoint3d &vpoints);

//从一系列点中找到两个不一样的给出来
bool MD2010_GetTwoDiffPoints(vAcGePoint3d &pnts, AcGePoint3d &a, AcGePoint3d &b);

//这个函数 从一系列点钟，剔除重复的，得到干净的唯一的点的容器
void MD2010_RemoveCopyOfPoints(vAcGePoint3d &vpnts);
void MD2010_RemoveCopyOfPoints(vAcGePoint3d &vpnts, double tol);

//这个函数从一堆点中得到X不同的点 不考虑Y
void MD2010_GetDiffPointX(vAcGePoint3d &vpnts, vAcGePoint3d &opts, double tol);
//这个函数从一堆点中得到Y不同的点 不考虑X
void MD2010_GetDiffPointY(vAcGePoint3d &vpnts, vAcGePoint3d &opts, double tol);

