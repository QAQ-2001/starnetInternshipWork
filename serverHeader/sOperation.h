#ifndef SOPERATION_H
#define SOPERATION_H

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "database.h"

#include <json/json.h>

#include <cryptopp/cryptlib.h>
#include <cryptopp/md5.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>

//Summary: 服务端登录操作
//Parameters:
//      value: json格式的数据
//Return: token或情况的相应说明
std::string serverLogin(Json::Value& value, AdoAccess& dataBase);
//Summary: 文本md5加密
//Parameters:
//      str: 需要加密的文本
//Return: 加密后的文本
std::string encryptMd5(std::string str);

#endif