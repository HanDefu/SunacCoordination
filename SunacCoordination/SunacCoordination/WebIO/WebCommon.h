#pragma once
#include "SunacCadWeb\soapArgumentSettingServiceSoapProxy.h"



inline void InitSoapTime(ArgumentSettingServiceSoapProxy &cadWeb)
{
	cadWeb.recv_timeout = 2;	//接受  (单位是秒)
	cadWeb.send_timeout = 2;	//发送  (单位是秒)
	cadWeb.connect_timeout = 2;	//连接(单位是秒)
}