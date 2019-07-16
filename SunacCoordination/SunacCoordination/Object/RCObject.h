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

	virtual RCTYPE GetType()const = 0;

public:
	CString m_name;
	void * m_userData;
};

typedef std::vector<RCObject> vRCObject;
typedef std::vector<RCObject*> vpRCObject;