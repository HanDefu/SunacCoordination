#pragma once

#include <vector>
#include <gecomp3d.h>
#include <brshell.h>
#include <brcplx.h >

/*
    本文件主要涉及矩形阵列，路径阵列和环形阵列的一些操作，
	任何一个阵列他是通过图块展示出来的，一般涉及两个id一个是选择或者遍历出来的arrayid，
	这个arrayid可以转换成一个blockreference，然后进行炸开。

	通过arrayid能够拿到actionid，actionid是控制这个阵列的参数的，通过这个actionid的parameter可以实现阵列的参数修改

	注意：动态块不是actionbody --经过测试TYCOM_GetActionId 无法成功

	注意：actionbody 重写了自己的 explode
*/

//判断一个选择或者遍历出来的id是不是阵列
bool TYCOM_IsArray(AcDbObjectId arrayid);
AcDbObjectId TYCOM_GetActionId(AcDbObjectId arrayid);
int TYCOM_ModifyRectArrayParm_DEMO(AcDbObjectId actionid);


/*
经过验证：下属函数所得到的结果和TYCOM_GetArrayObjects 的id是一致的
TYCOM_CycleBlockReferenceEntites(eId,ids2);---方法1

AcDbObjectId actionID = TYCOM_GetActionId(arrayid);---方法2
TYCOM_GetArrayObjects(actionID,ids1);

*/
int TYCOM_GetArrayObjects(AcDbObjectId actionid, vAcDbObjectId &ids);

int TYCOM_CreateRectArrayDemo();
int TYCOM_CreatePolarArrayDemo();
int TYCOM_CreatePathArrayDemo();



//一个双层阵列 内部一层是3*4个window， 外面一层是2*2个内层的
/*
ads_name ename;
ads_point pt;
if (acedEntSel(L"\nSelect a dynamic block reference: ", ename, pt) != RTNORM)
{
acutPrintf(L"\nError selecting entity.");
return;
}
AcDbObjectId eId;
acdbGetObjectId(eId, ename);

vAcDbObjectId ids1,ids2,ids3,ids4;
AcDbObjectId actionID = TYCOM_GetActionId(eId);
TYCOM_GetArrayObjects(actionID,ids1);//得到4个对象 每个对象都是一个块 不是arrayid

for(int i = 0; i < ids1.size(); i++)
{
    TYCOM_CycleBlockReferenceEntites(ids1[0],ids2);//通过这个方法 得到这一个块内部的 arrayid

    TYCOM_IsArray(ids2[0]);
    actionID = TYCOM_GetActionId(ids2[0]);
    TYCOM_GetArrayObjects(actionID,ids3);//这个是12个内部的图块 还不是window

    TYCOM_CycleBlockReferenceEntites(ids3[0],ids4);//通过图块得到对应的window
    TY_IsWindow(ids4[0],eViewDir::E_VIEW_ALL);
}
*/