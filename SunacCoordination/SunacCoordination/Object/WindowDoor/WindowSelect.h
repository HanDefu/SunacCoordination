#pragma once
#include <vector>
#include "AttrWindow.h"
#include "../../Common/TYRect.h"
using namespace std;

class CWinInCad
{
public:
	CWinInCad();

public:
	AcDbObjectId m_winId;
	AcDbObjectId m_rootId; //针对块中块的根节点
	bool m_bMirror;			//门窗块是否镜像
	double m_rotateAngle;  //门窗块的旋转角度
	AcGeMatrix3d m_mx;
};
//////////////////////////////////////////////////////////////////////////

class CWindowSelect
{
public:
	static vector<CWinInCad> SelectWindows(eViewDir p_view, bool p_bAllWindow = false);
	static vector<CWinInCad> SelectWindowsByRect(eViewDir p_view, TYRect p_rect);
	
	static bool IsReferenctMirror(const AcDbObjectId refId);	

protected:
	static vector<CWinInCad> GetWinsInObjectIds(const vector<AcDbObjectId>& p_ids, eViewDir p_view);


	static int FindWindowsDeep(const AcDbObjectId inputId, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CWinInCad> &outputIds);

	static int FindWindowInBlock(const AcDbObjectId inputId, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CWinInCad> &outputIds);
	static int FindWindowInArray(const AcDbObjectId inputId, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CWinInCad> &outputIds);

	static bool IsMxMirror(const AcGeMatrix3d p_parentMx); //通过矩阵判断是否做过镜像处理

};

//////////////////////////////////////////////////////////////////////////
class CWindowAndCount
{
public:
	CWindowAndCount();

public:
	AttrWindow winAtt;
	int nCount;

	AcDbObjectIdArray objIds;
};

class CWindowCountArray
{
	bool InitByWindowIds(const vAcDbObjectId& p_winIds);
public:
	bool InitByWindowIds(const vector<CWinInCad>& p_winIds);
	bool InitByWindowAtts(const vector<AttrWindow>& p_winAtts, const vector<AcDbObjectId>& p_ids);

	int GetCount()const { return (int)(m_winCountArray.size()); }

	const CWindowAndCount& GetWindow(int p_index) { return m_winCountArray[p_index]; }

private:
	vector<CWindowAndCount> m_winCountArray;
};