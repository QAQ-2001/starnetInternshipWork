#ifndef TOKEN_H
#define TOKEN_H

#include <jwt-cpp/jwt.h>
#include <jwt-cpp/base.h>
#include <iostream>

//Summary: 根据user返回一个10分钟有效的token
//Parameters:
//       user: 用户的ID
//Return: token
std::string generateToken(std::string user);

//Summary: 根据user和时间验证token是否有效
//Parameters:
//       token: 令牌
//       user: 用户的ID
//Return: true-有效，false-无效
bool verifyToken(std::string token, std::string user);

#endif