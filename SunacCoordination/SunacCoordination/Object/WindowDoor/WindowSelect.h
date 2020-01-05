#pragma once

class CWinInCad
{
public:
	CWinInCad();

public:
	AcDbObjectId m_winId;
	bool m_bMirror;
	AcGeMatrix3d m_mx;
};
//////////////////////////////////////////////////////////////////////////

class CWindowSelect
{
public:
	static vector<CWinInCad> SelectWindows(eViewDir p_view, bool p_bAllWindow = false);


protected:
	static int FindWindowsDeep(const AcDbObjectId inputId, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CWinInCad> &outputIds);

	static int FindWindowInBlock(const AcDbObjectId inputId, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CWinInCad> &outputIds);
	static int FindWindowInArray(const AcDbObjectId inputId, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CWinInCad> &outputIds);

	static bool IsWindowMirror(const AcDbObjectId inputId, const eViewDir viewDir, const AcGeMatrix3d p_parentMx);	

};