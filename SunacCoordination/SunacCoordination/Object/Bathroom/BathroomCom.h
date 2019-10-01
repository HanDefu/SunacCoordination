#pragma once
#include "..\..\Sunac_def.h"



//-------------通用函数-----//

//BathroomId厨房块参照的id
//taiPen:"Sunac_台盆_650"/"Sunac_台盆_750"/Sunac_台盆_800/Sunac_台盆_900/Sunac_台盆_1000
int Bathroom_SelectTaiPen(AcDbObjectId BathroomId, CString taiPen);

//BathroomId厨房块参照的id
//maTong:Sunac_马桶750/Sunac_马桶800
int Bathroom_SelectBingXiang(AcDbObjectId BathroomId, CString maTong);

//-------------------TI3---------------//
//yLen y方向总长（长边尺寸）
//自动选择马桶
int TI3_SetMaTongPos(AcDbObjectId BathroomId, double yLen);

//根据输入的长边尺寸和马桶的宽度 自动筛选合适的台盆宽度
//maTongWidth 750/800
vCString TI3_GetTaiPens(AcDbObjectId BathroomId, double yLen, double maTongWidth);

//TI3_G

//Y 取值： 950或者1050---注意如果选择了950 台盆最大为900 不能为1000--
//注意：盥洗区和台盆有个关系
int TI3_G_SetHuanXiQuY(AcDbObjectId BathroomId, double yLen);


//-------------------TI4---------------//
//yLen y方向总长（长边尺寸）
//自动选择马桶
int TI4_SetMaTongPos(AcDbObjectId BathroomId, double yLen);
//根据输入的长边尺寸和马桶的宽度 自动筛选合适的台盆宽度
//maTongWidth 750/800
vCString TI4_GetTaiPens(AcDbObjectId BathroomId, double yLen, double maTongWidth);
//Y 取值： 950或者1050---注意如果选择了950 台盆最大为900 不能为1000--
//注意：盥洗区和台盆有个关系
int TI4_G_SetHuanXiQuY(AcDbObjectId BathroomId, double yLen);


//-------------------TL---------------//
//linYuFangY 900/1000
//根据输入的长边尺寸和淋浴房的宽度 自动筛选合适的台盆宽度
vCString TL4_GetTaiPens(AcDbObjectId BathroomId, double yLen, double linYuFangY);




