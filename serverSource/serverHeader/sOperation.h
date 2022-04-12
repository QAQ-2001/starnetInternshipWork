#ifndef SOPERATION_H
#define SOPERATION_H

#include "database.h"

#include <json/json.h>

//Summary: 服务端处理客户端登录操作
//Parameters: 
//	value: json格式的数据
//	database: 类AdoAccess(操作数据库的类)的引用
//Return:  token,prior----登录成功,返回一个token加prior用,分隔的字符串
//	database error----登录失败,数据库操作失败
//	loginName error----登录失败,登录账号错误
//	password error----登录失败,登录密码错误
//	lack data----登录失败,json数据缺失
std::string serverLogin(Json::Value& value, AdoAccess& database);

//Summary: 服务端处理客户端更新时间表操作
//Parameters:
//	value: json格式的数据集
//	database: 类AdoAccess(操作数据库的类)的引用
//Return:  execute sql success----更新成功
//	token expired----更新失败,token过期
//	execute fail----更新失败,时间表未更新
//	lack data----更新失败,json数据缺失
std::string serverUpdateTimeTable(Json::Value& value, AdoAccess& database);

//Summary: 服务端处理客户端请求时间表操作
//Parameters:
//	value: json格式的数据集
//	database: 类AdoAccess(操作数据库的类)的引用
//Return:  json格式string----请求成功
//	error----请求失败,数据库操作错误或token验证不通过
//	token expired----请求失败,token过期
//	no data----请求失败,数据库没有相关时间表记录
//	lack data----请求失败,json数据缺失
std::string serverGetTimeTable(Json::Value& value, AdoAccess& database);


std::string serverUpdateProject(Json::Value& value, AdoAccess& database);


std::string serverGetProject(Json::Value& value, AdoAccess& database);


#endif