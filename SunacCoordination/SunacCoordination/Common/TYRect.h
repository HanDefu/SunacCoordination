#pragma once

class TYRect
{
public:
	TYRect();
	TYRect(AcGePoint3d lb, AcGePoint3d rt);
	TYRect(AcGePoint3d pt1, //左下
		AcGePoint3d pt2,//左上
		AcGePoint3d pt3,//右上
		AcGePoint3d pt4);//右下
	virtual ~TYRect();

	AcGePoint3d GetLB(){return m_lb;}//pt1
	AcGePoint3d GetRT(){return m_rt;}//pt3
	AcGePoint3d GetLT(){return AcGePoint3d(m_lb.x,m_rt.y,0);}//pt2
	AcGePoint3d GetRB(){return AcGePoint3d(m_rt.x,m_lb.y,0);}//pt4
	void SetLB(AcGePoint3d lb){m_lb = lb;}
	void SetRT(AcGePoint3d rt){m_rt = rt;}

	bool IsPointIn(AcGePoint3d pt);
	bool IsPointOn(AcGePoint3d pt, double tol = TOL);
	bool IsRectIn(TYRect &other);
	void Add(AcGePoint3d pt);
	void Add(TYRect &other);
	AcGePoint3d GetCenter();
	void Draw(double offset, CString layout);

	bool IsSame(TYRect &other, double tol = 10);

	double GetWidth(){return fabs(m_rt.x - m_lb.x);}
	double GetHeight(){return fabs(m_rt.y - m_lb.y);}

public:
	AcGePoint3d m_lb;//左下角点 pt1
	AcGePoint3d m_rt;//右上角点 pt2

	AcGePoint3d m_lt;//左上 pt3
	AcGePoint3d m_rb;//右下 pt4

	bool m_ucs;
}; 

typedef std::vector<TYRect> vTYRect;
