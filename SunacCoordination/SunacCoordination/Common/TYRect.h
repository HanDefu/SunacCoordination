#pragma once

class TYRect
{
public:
	TYRect();
	TYRect(AcGePoint3d lb, AcGePoint3d rt);
	virtual ~TYRect();

	AcGePoint3d GetLB()const {return m_lb;}//pt1
	AcGePoint3d GetRT()const {return m_rt;}//pt3
	AcGePoint3d GetLT()const {return AcGePoint3d(m_lb.x, m_rt.y, 0);}//pt2
	AcGePoint3d GetRB()const {return AcGePoint3d(m_rt.x, m_lb.y, 0);}//pt4
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


	bool m_ucs;
}; 

typedef std::vector<TYRect> vTYRect;
