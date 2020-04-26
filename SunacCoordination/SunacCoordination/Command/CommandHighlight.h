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

	void SunacHighlight(vAcDbObjectId p_sunacIds);
	void SunacNoHighlight();
	
private:
	vAcDbObjectId m_sunacHighlights;
};

