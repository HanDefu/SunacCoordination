#pragma once
#include "Sunac_def.h"



//-------------通用函数-----//

//toiletId厨房块参照的id
//taiPen:"Sunac_台盆_650"/"Sunac_台盆_750"/Sunac_台盆_800/Sunac_台盆_900/Sunac_台盆_1000
int Toilet_SelectTaiPen(AcDbObjectId toiletId, CString taiPen);

//toiletId厨房块参照的id
//maTong:Sunac_马桶750/Sunac_马桶800
int Toilet_SelectBingXiang(AcDbObjectId toiletId, CString maTong);




