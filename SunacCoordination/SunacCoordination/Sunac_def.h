#pragma once

typedef enum eWindowDimType
{
	SINGLE,//单个值
	MULTI,//多个值
	SCOPE,//范围值
	UNLIMIT//不限值
}eWindowDimType;

typedef enum eViewDir
{
	LIMINAN,//立面图
	PINGMIAN,//平面图
	CESHI,//侧视图
}eViewDir;

typedef enum eRailingType
{
	TIEYI,//铁艺
	BOLI,//玻璃
}eRailingType;


typedef enum eWindowDoorPos//门窗位置关系
{
    DUIKAI,//对开
	CHUIZHIKAI//垂直开
} eWindowDoorPos;


typedef enum eLingNingShuiGuanPos//冷凝水管位置
{
	CEBIAN,//侧边
	HOUBIAN//后边
}eLingNingShuiGuanPos;

typedef struct RCDimData
{
	eWindowDimType type;//值类型，单个值
	//如果是单个值 就取第一个，
	//如果是范围值 最小值是第一个 最大值是第二个
	//如果是系列值 逐个读取
	//不限 没有
	vdouble values;
}SRCDimData;
typedef std::vector<SRCDimData> vSRCDimData;
