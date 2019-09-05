/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This file is the head file of class RCObject.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/

#pragma once
#include "RCObject.h"

enum RCTYPE {
	T_NONE=0,
	T_WINDOW = 1, //窗
	T_DOOR,//门
	T_KITCHEN,//厨房
	T_TOILET,//卫生间
	T_STDFACADE, //标准立面
	T_WATERPROOF, //防水
	T_STDFILL,//填充
	T_AIRCONDITION, //空调
	T_LANGAN,//栏杆
	T_XIANJIAO,//线脚
};

class RCObject
{
public:
	RCObject(void);
	RCObject(const RCObject&other);
	~RCObject(void);

	void SetName(CString  name) { m_name = name; }
	CString& GetName() { return m_name; }

	void SetUserData(void * userData){m_userData = userData;}
	void *GetUserData(){return m_userData;}

	virtual bool isEqualToAll(RCObject*other = 0);//所有数据完全一致
	virtual bool isEqualTo(RCObject*other = 0);//基础数据一致

	virtual RCTYPE GetType()const = 0;

public:
	CString m_name;
	void * m_userData;
	AcDbObjectId m_id;

public:
	//一般情况下一个RCObject是一个实例化的块对象
	//这里添加的一个属性用于表示当RCObject当作一个type时候，所有的和他相同的对象列表
	//主要是出门窗表时候要分类汇总
	vAcDbObjectId m_sameTypeIds;//这个需要包含他自己的id
};

typedef std::vector<RCObject> vRCObject;
typedef std::vector<RCObject*> vpRCObject;
