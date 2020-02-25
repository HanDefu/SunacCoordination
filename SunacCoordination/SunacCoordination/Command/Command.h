#pragma once

void SendCommandToCAD(CString cmd); //此函数尚未调通

//创建/移除工具栏
void CADPalette_AddP();
void CADPalette_RemoveP();

void CMD_Login();
void CMD_ShowCADPalette();
void CMD_SetUp();

//窗户
void CMD_SunacWindow();
void CMD_SunacDoor();
void CMD_SunacWindowAdvanceDesign();//门窗深化设计
void CMD_SunacWindowDetail();//门窗详图
void CMD_SunacWindowsTable();//门窗表
void CMD_SunacFloorWindowsTable();//地面门窗表
void CMD_SunacWindowsStatistics();//统计算量
void CMD_SunacWindowFloorSetting();//门窗楼层设置
void CMD_SunacWindowTop2Front();//门窗平面到立面
void CMD_SunacWindowFront2Top();//门窗立面到平面
void CMD_SunacWinAutoId();
void CMD_SunacNoHighlight();

//厨房
void CMD_SunacKitchen();
void CMD_SunacBathroom();
void CMD_SunacKitchenBathroomStatistic();

//栏杆
void CMD_SunacRailing();
void CMD_SunacRailingDetail();
void CMD_SunacRailingStatistic();

//空调
void CMD_SunacAirconditioner();
void CMD_SunacAirconditionerStatistic();

void CMD_SunacMoldings(); //线脚

//填充材质
void CMD_SunacFilling();

//标准立面
void CMD_SunacFacade();

//防水构造
void CMD_SunacWaterproof();


//退出前关闭所有非模态对话框
void CloseModelessDialogs();
