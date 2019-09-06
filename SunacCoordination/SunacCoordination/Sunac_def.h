#pragma once

#define  SUNAC_DEBUG
#define  SUNAC_ATTRIBUTE_ENTITY L"SUNAC_ATTRIBUTE_ENTITY"
#define  SUNAC_COMMON_STR_LEN (256)
#define  FILE_VERSION ((Adesk::Int32)1)

#define  WORK_LOCAL//是否本地工作模式
#define CHECK_USE//是否检测权限

typedef enum eRCType
{
    WINDOW,//外窗
	DOOR,//门
	KITCHEN,//厨房
	TOILET,//卫生间
	AIRCON,//空调
	RAILING,//栏杆
	TYPENUM
}eRCType;

typedef enum eWindowDimType
{
	SINGLE,//单个值
	MULTI,//多个值
	SCOPE,//范围值
	UNLIMIT,//不限值
	CALC,//公式计算值
	NOVALUE
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
	AIRCON_CEBIAN,//侧边
	AIRCON_HOUBIAN//后边
}eLingNingShuiGuanPos;

typedef enum eWindowCalcType//门窗算量类型
{
	CALC_XINGCAI,//型材
	CALC_BOLI,//玻璃
	CALC_WUJIN,//五金
	CALC_FUCAI//辅材
}eWindowCalcType;

typedef struct SCalcData//门窗算量数据
{
    eWindowCalcType calcType;
	CString formula;
}SCalcData;

typedef std::vector<eWindowCalcType> vSCalcData;

typedef struct RCDimData
{
	eWindowDimType type;//值类型，单个值
	//如果是单个值 就取第一个，
	//如果是范围值 最小值是第一个 最大值是第二个
	//如果是系列值 逐个读取
	//不限 没有
	vdouble values;
	double defaultValue;
	CString prompt;
}SRCDimData;
typedef std::vector<SRCDimData> vSRCDimData;
