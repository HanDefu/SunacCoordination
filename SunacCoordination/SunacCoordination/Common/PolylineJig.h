#pragma once
#include <vector>
#include "dbjig.h"

using namespace  std;

class CPolylineJig : public AcEdJig
{
public:
	CPolylineJig();
	virtual ~CPolylineJig();


	//获取正在操作的对象，以便在其它函数中设置参数
	bool doIt(vector<AcGePoint3d>  pts, AcGePoint3d& ptOut);

protected:
	virtual AcEdJig::DragStatus sampler();
	virtual Adesk::Boolean update();
	virtual AcDbEntity* entity() const;

	void Erase(AcDbObjectId entId);
	bool Init(vector<AcGePoint3d>&  pts);
	
protected:
	vector<AcGePoint3d>  m_pts;
	AcDbPolyline* m_pPolyline;
	AcDbObjectId m_polylineId;
	AcGePoint3d m_curPoint;		// 存储用户光标移动时点的临时位置
};

