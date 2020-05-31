#pragma once

struct CTOpenData
{
	double width; 
	double height;
	double bottomHeight; //¥∞Ã®∏ﬂ
	CString sWinCode;	//√≈¥∞±‡∫≈ 
};

struct CTWallData
{
	AcDbExtents extents;
	double leftT; //◊Û≤‡«Ω∫Ò
	double rightT;//”“≤‡«Ω∫Ò
	double thick; //«Ω∫Ò
};

class CTangentOpen
{
public:
	static HRESULT SetTangentOpenProp( AcDbObjectId p_winId, CTOpenData p_winData);

	static HRESULT InsertWinOpenning(AcGePoint3d p_centerPt, CTOpenData p_winData, AcDbObjectId &p_tWinOpenIdOut);

	static HRESULT GetTangentWallData(AcDbObjectId p_id, CTWallData& p_walldataOut);

protected:
	//static HRESULT SetTangentOpenProp_TCH10_COM9_T20V5X64( AcDbObjectId p_winId, CTOpenData p_winData);

};


