#ifndef TOKEN_H
#define TOKEN_H

#include <jwt-cpp/jwt.h>
#include <jwt-cpp/base.h>
#include <iostream>

//Summary: ����user����һ��10������Ч��token
//Parameters:
//       user: �û���ID
//Return: token
std::string generateToken(std::string user);

//Summary: ����user��ʱ����֤token�Ƿ���Ч
//Parameters:
//       token: ����
//       user: �û���ID
//Return: true-��Ч��false-��Ч
bool verifyToken(std::string token, std::string user);

#endif