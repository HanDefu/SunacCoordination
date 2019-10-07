#pragma once

#include <vector>
#include "../Object/Railing/AttrRailing.h"


class CRailingWebData
{
public:
	bool GetRailingById(CString p_sId, AttrRailing& value)const;

	std::vector<AttrRailing> GetRailings(eRailingType type);
	std::vector<AttrRailing> GetAllRailings();

protected:

};

