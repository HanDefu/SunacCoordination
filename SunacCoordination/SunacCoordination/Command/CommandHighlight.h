#pragma once
#include "../Sunac_def.h"


class CCommandHighlight
{
	CCommandHighlight();
public:
	~CCommandHighlight();

	static CCommandHighlight *GetInstance()
	{
		static CCommandHighlight instance;
		return &instance;
	}

	void WindowDoorHighlight(vAcDbObjectId p_windoorIds);
	void WindowDoorNoHighlight();
	
private:
	vAcDbObjectId m_winHighlights;
};

