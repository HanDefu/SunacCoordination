#pragma once
#include <vector>

using namespace std;


enum EAuxiliaryFrameType
{
	EAuxiliaryFrame_¸Ö¸½¿ò,
	EAuxiliaryFrame_½ÚÄÜ¸½¿ò,
};

class CAuxiliaryFrameData
{
public:
	CAuxiliaryFrameData();
	~CAuxiliaryFrameData();

	EAuxiliaryFrameType m_type;
	CString m_sName;
};

class CAuxiliaryFrameSeries
{
	CAuxiliaryFrameSeries();
public:
	static CAuxiliaryFrameSeries* Instance();
	virtual ~CAuxiliaryFrameSeries();

	vector<CString> GetAllAuxiliaryFrameSerials();
	vector<CString> GetAllAuxiliaryFrameSerials(EAuxiliaryFrameType p_type);

	vector<double>  GetPlugSlotSize(bool p_bHasAuxiliaryFrame); //Èû·ì³ß´ç
};