#ifndef DATABASE_H
#define DATABASE_H
#define NOMINMAX

#import "C:/Program Files/Common Files/System/ado/msado15.dll" no_namespace rename("EOF","adoEOF") rename("BOF","adoBOF")
#include "token.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <string>

//single instance?
//Meyers' Singleton?
class AdoAccess {
public:
	/*static AdoAccess& getInstance()
	{
		static AdoAccess instance;
		return instance;
	}*/
	
	AdoAccess();
	~AdoAccess();
public:
	//��������ע��
	std::string login(std::string& loginName, std::string& password);
	//��������ע��
	std::string getAuthority(std::string& username);

public:
	//��������ע��
	std::string updateTimeTable(std::string& token, std::string& projectName, std::string& duration, std::string& myDate);
	//��������ע��
	std::vector<std::string> getTimeTable(std::string& token, std::string& starDate, std::string& endDate, std::string range, std::string total);

public:
	//��������ע��
	std::string updateProject(std::string& token, std::string& projectName, std::string& manager);
	//��������ע��
	std::vector<std::string> getProject(std::string& token);

public:
	//sql��ѯ���ִ�к���,��ѯ�������results��
	int selectData(const std::string& sql, std::vector<std::string>& results);
	//sql��ɾ�����ִ�к���
	int executeSql(const std::string& sql);

private:
	_ConnectionPtr   HC_pConnection; // ���ݿ�ָ��
	//_RecordsetPtr    HC_pRecordset; // ����ָ��
	_CommandPtr      HC_pCommand; // ��¼ָ��

private:
	//���ݿ������������
	bool reConnect();
    
};

#endif