#ifndef SOPERATION_H
#define SOPERATION_H

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "database.h"

#include <json/json.h>

#include <cryptopp/cryptlib.h>
#include <cryptopp/md5.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>

//Summary: ����˵�¼����
//Parameters:
//      value: json��ʽ������
//Return: token���������Ӧ˵��
std::string serverLogin(Json::Value& value, AdoAccess& dataBase);
//Summary: �ı�md5����
//Parameters:
//      str: ��Ҫ���ܵ��ı�
//Return: ���ܺ���ı�
std::string encryptMd5(std::string str);

#endif