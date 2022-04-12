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
//Summary: ����user����һ��10������Ч��token
//Parameters:
//       user: �û�������
//Return: token
std::string generateToken(std::string& user);

//Summary: ����user��ʱ����֤token�Ƿ���Ч
//Parameters:
//       token: ����
//       user: �û���ID
//Return:  verify success----��֤�ɹ�
//	invalid token supplied----�ṩһ����������ʽ��jwt
//	claim value does not match expected value----jwt��ʽ��ȷ����username����
//	token expired----jwt��ʱʧЧ
//	verify success----jwt��֤�ɹ�
std::string verifyToken(std::string& token);

//Summary: ����tokenȡ��username
//Parameters:
//       token: ����
//Return:  invalid token supplied----�ṩһ����������ʽ��jwt
//	claim value does not match expected value----jwt��ʽ��ȷ����username����
//	token expired----jwt��ʱʧЧ
//	verify success----jwt��֤�ɹ�
std::string getUsername(std::string& token);

//Summary: �ı�md5����
//Parameters:
//      str: ��Ҫ���ܵ��ı�
//Return: ���ܺ���ı�
std::string encryptMd5(std::string str);

//Summary: ��str����part�ָ��Ƭ�Σ�Ƭ�δ���vector<string>�з���
//Parameters:
//      str: ��Ҫ�ָ���ַ���
//		part:�������зָ���ַ���
//Return: �ָ����ַ���
std::vector<std::string> split(const std::string str, const std::string part);

#endif