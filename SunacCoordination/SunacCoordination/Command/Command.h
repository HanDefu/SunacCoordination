#pragma once

//创建/移除工具栏
void CADPalette_AddP();
void CADPalette_RemoveP();

void CMD_Login();

//窗户
void CMD_SUNACWINDOW();
void CMD_SUNACDOOR();

//统计算量
void CMD_SUNACSTATISTICS();

//厨房
void CMD_SUNACKITCHEN();
void CMD_SUNACBATHROOM();

void CMD_SUNACRAILING();//栏杆
void CMD_SUNACMOLDINGS(); //线脚

//填充材质
void CMD_SUNACFILLING();

//空调
void CMD_SUNACAIRCONDITIONER();

//标准立面
void CMD_SUNACFACADE();

//防水构造
void CMD_SUNACWATERPROOF();

//退出前关闭所有非模态对话框
void CloseModelessDialogs();
