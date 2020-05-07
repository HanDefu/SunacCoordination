#pragma once
#include "../Sunac_def.h"


//与文档关联绑定
class CObjsHighlight
{
public:
	CObjsHighlight();
	~CObjsHighlight();

	void Highlight(vAcDbObjectId p_sunacIds);
	void NoHighlight();
	
	static void HighlightObject(const AcDbObjectId& objId, bool highlight);

private:
	vAcDbObjectId m_sunacHighlights;
};

