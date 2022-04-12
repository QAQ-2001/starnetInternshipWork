#ifndef TOKEN_H
#define TOKEN_H
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <jwt-cpp/jwt.h>
#include <jwt-cpp/base.h>
#include <iostream>

#include <cryptopp/cryptlib.h>
#include <cryptopp/md5.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
//Summary: 根据user返回一个10分钟有效的token
//Parameters:
//       user: 用户的名称
//Return: token
std::string generateToken(std::string& user);

//Summary: 根据user和时间验证token是否有效
//Parameters:
//       token: 令牌
//       user: 用户的ID
//Return:  verify success----验证成功
//	invalid token supplied----提供一个非正常格式的jwt
//	claim value does not match expected value----jwt格式正确但是username错误
//	token expired----jwt超时失效
//	verify success----jwt验证成功
std::string verifyToken(std::string& token);

//Summary: 根据token取得username
//Parameters:
//       token: 令牌
//Return:  invalid token supplied----提供一个非正常格式的jwt
//	claim value does not match expected value----jwt格式正确但是username错误
//	token expired----jwt超时失效
//	verify success----jwt验证成功
std::string getUsername(std::string& token);

//Summary: 文本md5加密
//Parameters:
//      str: 需要加密的文本
//Return: 加密后的文本
std::string encryptMd5(std::string str);

//Summary: 对str根据part分割成片段，片段存在vector<string>中返回
//Parameters:
//      str: 需要分割的字符串
//		part:用来进行分割的字符串
//Return: 分割后的字符串
std::vector<std::string> split(const std::string str, const std::string part);

#endif