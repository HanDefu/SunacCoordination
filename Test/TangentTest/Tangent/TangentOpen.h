#pragma once

struct CTOpenData
{
	double width; 
	double height;
};


class CTangentOpen
{
public:
	HRESULT SetTangentOpenProp( AcDbObjectId p_winId, CTOpenData p_winData);

protected:
	HRESULT SetTangentOpenProp_TCH10_COM9_T20V5X64( AcDbObjectId p_winId, CTOpenData p_winData);

};


