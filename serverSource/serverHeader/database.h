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
	//见函数内注释
	std::string login(std::string& loginName, std::string& password);
	//见函数内注释
	std::string getAuthority(std::string& username);

public:
	//见函数内注释
	std::string updateTimeTable(std::string& token, std::string& projectName, std::string& duration, std::string& myDate);
	//见函数内注释
	std::vector<std::string> getTimeTable(std::string& token, std::string& starDate, std::string& endDate, std::string range, std::string total);

public:
	//见函数内注释
	std::string updateProject(std::string& token, std::string& projectName, std::string& manager);
	//见函数内注释
	std::vector<std::string> getProject(std::string& token);

public:
	//sql查询语句执行函数,查询结果存在results中
	int selectData(const std::string& sql, std::vector<std::string>& results);
	//sql增删改语句执行函数
	int executeSql(const std::string& sql);

private:
	_ConnectionPtr   HC_pConnection; // 数据库指针
	//_RecordsetPtr    HC_pRecordset; // 命令指针
	_CommandPtr      HC_pCommand; // 记录指针

private:
	//数据库断连重连函数
	bool reConnect();
    
};

#endif