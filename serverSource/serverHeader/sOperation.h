#ifndef SOPERATION_H
#define SOPERATION_H

#include "database.h"

#include <json/json.h>

//Summary: ����˴���ͻ��˵�¼����
//Parameters: 
//	value: json��ʽ������
//	database: ��AdoAccess(�������ݿ����)������
//Return:  token,prior----��¼�ɹ�,����һ��token��prior��,�ָ����ַ���
//	database error----��¼ʧ��,���ݿ����ʧ��
//	loginName error----��¼ʧ��,��¼�˺Ŵ���
//	password error----��¼ʧ��,��¼�������
//	lack data----��¼ʧ��,json����ȱʧ
std::string serverLogin(Json::Value& value, AdoAccess& database);

//Summary: ����˴���ͻ��˸���ʱ������
//Parameters:
//	value: json��ʽ�����ݼ�
//	database: ��AdoAccess(�������ݿ����)������
//Return:  execute sql success----���³ɹ�
//	token expired----����ʧ��,token����
//	execute fail----����ʧ��,ʱ���δ����
//	lack data----����ʧ��,json����ȱʧ
std::string serverUpdateTimeTable(Json::Value& value, AdoAccess& database);

//Summary: ����˴���ͻ�������ʱ������
//Parameters:
//	value: json��ʽ�����ݼ�
//	database: ��AdoAccess(�������ݿ����)������
//Return:  json��ʽstring----����ɹ�
//	error----����ʧ��,���ݿ���������token��֤��ͨ��
//	token expired----����ʧ��,token����
//	no data----����ʧ��,���ݿ�û�����ʱ����¼
//	lack data----����ʧ��,json����ȱʧ
std::string serverGetTimeTable(Json::Value& value, AdoAccess& database);


std::string serverUpdateProject(Json::Value& value, AdoAccess& database);


std::string serverGetProject(Json::Value& value, AdoAccess& database);


#endif