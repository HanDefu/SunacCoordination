#pragma once
#include "Sunac_def.h"

//kitchenId厨房块参照的id
//shuiPen:"单盆600"/"单盆700"/双盆900/双盆1000/双盆1200
int Kitchen_SelectShuiPen(AcDbObjectId kitchenId, CString shuiPen);

//kitchenId厨房块参照的id
//shuiPen:"800"/"900"
int Kitchen_SelectZaoTai(AcDbObjectId kitchenId, CString zaoTai);

//KUq门窗对开设置门的位置
int Kitchen_KUq_DuiKai_SetDoorPos(AcDbObjectId kitchenId, double kaiJian);
//KUq门窗对开设置灶台的位置 
//zaoTaiType "800"/"900"
int Kitchen_KUq_DuiKai_SetZaoTaiPos(AcDbObjectId kitchenId, double jinshen, CString zaoTaiType);