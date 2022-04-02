#ifndef DATABASE_H
#define DATABASE_H
#define NOMINMAX

#import "C:/Program Files/Common Files/System/ado/msado15.dll" no_namespace rename("EOF","adoEOF") rename("BOF","adoBOF")
#include "token.h"

#include <iostream>
#include <vector>
#include <sstream>

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
	std::string login(std::string& username, std::string& password);
	int selectData(const std::string& sql, std::vector<std::string>& results);

private:
	_ConnectionPtr   HC_pConnection; // ���ݿ�ָ��
	//_RecordsetPtr    HC_pRecordset; // ����ָ��
	_CommandPtr      HC_pCommand; // ��¼ָ��

private:
	bool reConnect();
    
};

#endif