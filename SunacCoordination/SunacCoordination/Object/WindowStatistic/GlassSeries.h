#pragma once
#include <vector>

using namespace std;


class CGlassData
{
public:
	CGlassData();
	~CGlassData();

	CString m_sArea;	//区域
	CString m_sSubArea;//二级地区

	double m_heatInsulationCoefficient; //隔热系数
	double m_shadingCoefficient; //遮阳系数

	CString m_sSerailName;  //玻璃系列名称

	int m_glassCount;		//玻璃层数

	vector<CString> m_alSerials; //匹配的型材
};

class CGlassSeries
{
	CGlassSeries();
public:
	static CGlassSeries* Instance();
	virtual ~CGlassSeries();

	vector<CString> GetAllGlassSerials();

	//通过型材系列获取玻璃选型
	vector<CString> GetGlassSerialsByAlSerial(CString p_sAlSerial);

	//通过隔热系数获取玻璃选型
	vector<CString> GetGlassSerialsByHeatInsulationCoeff(double p_heatCoeff);

};