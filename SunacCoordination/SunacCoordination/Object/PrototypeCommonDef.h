#pragma once


enum EKitchType
{
	E_KITCH_U,	//U型
	E_KITCH_L,	//L型
	E_KITCH_I,	//I型
	E_KITCH_OTHER = 0xf, //其他
	E_KITCH_ALL = 0xff,	 //所有类型
};

enum EBathroomType
{
	E_BATHROOM_U,	//U型
	E_BATHROOM_L,	//L型
	E_BATHROOM_I,	//I型
	E_BATHROOM_OTHER = 0xf, //其他
	E_BATHROOM_ALL = 0xff,	 //所有类型
};

enum E_DIRECTION
{
	E_DIR_BOTTOM= 0,
	E_DIR_RIGHT,
	E_DIR_TOP,
	E_DIR_LEFT,
	E_DIR_UNKNOWN
};

//楼层区间
enum E_FLOOR_RANGE
{
	E_FLOOR_1_7 = 0,
	E_FLOOR_8_14,
	E_FLOOR_15_21,
	E_FLOOR_22_28,
	E_FLOOR_29_35,
};

const double c_airVentSize[] = { 350, 400, 450, 500, 550 };
