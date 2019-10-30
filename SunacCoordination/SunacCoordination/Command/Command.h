#pragma once

void SendCommandToCAD(CString cmd); //此函数尚未调通

//创建/移除工具栏
void CADPalette_AddP();
void CADPalette_RemoveP();

void CMD_Login();

//窗户
void CMD_SunacWindow();
void CMD_SunacDoor();
void CMD_SunacWindowAdvanceDesign();//门窗深化设计
void CMD_SunacWindowsStatistics();//统计算量
void CMD_SunacWindowDetail();//门窗详图

//厨房
void CMD_SunacKitchen();
void CMD_SunacBathroom();

//栏杆
void CMD_SunacRailing();
void CMD_SunacRailingStatistic();

//空调
void CMD_SunacAirconditioner();

void CMD_SunacMoldings(); //线脚

//填充材质
void CMD_SunacFilling();

//标准立面
void CMD_SunacFacade();

//防水构造
void CMD_SunacWaterproof();


//退出前关闭所有非模态对话框
void CloseModelessDialogs();
