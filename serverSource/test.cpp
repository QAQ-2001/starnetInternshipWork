#define NOMINMAX

#include<iostream>
#include"server.h"

using namespace std;
//using namespace ADODB;

int main()
{
	//AdoAccess dataBase;

	////初始化
	//dataBase.OnInitADOConn();
	//dataBase.HC_pRecordset.CreateInstance("ADODB.Recordset");

	//try
	//{
	//	char sql[300];
	//	memset(sql, 0, 300);
	//	strncat_s(sql, "SELECT * FROM [user];", 22);
	//	dataBase.HC_pRecordset->Open(sql, dataBase.HC_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	//}
	//catch (_com_error* e)
	//{
	//	cout << e->ErrorMessage() << endl;
	//	if (dataBase.HC_pConnection->State)
	//	{
	//		dataBase.HC_pConnection->Close();
	//		dataBase.HC_pConnection = NULL;
	//	}
	//	dataBase.ExitConnect();
	//	return 0;
	//}
	//try
	//{
	//	//若结果为空，结束   
	//	if (dataBase.HC_pRecordset->adoBOF)
	//	{
	//		printf("表内数据为空！");
	//		if (dataBase.HC_pConnection->State)
	//		{
	//			dataBase.HC_pRecordset->Close();
	//			dataBase.HC_pRecordset = NULL;
	//			dataBase.HC_pConnection->Close();
	//			dataBase.HC_pConnection = NULL;
	//		}
	//		dataBase.ExitConnect();
	//		return 0;
	//	}
	//	cout << "---------------------------------" << endl;
	//	cout << "数据库的内容为:" << endl;
	//	//游标定位到第一条记录
	//	dataBase.HC_pRecordset->MoveFirst();
	//	_variant_t var[3]; //从结果集中取出的数据放到var中
	//	char* t1[3];
	//	while (!dataBase.HC_pRecordset->adoEOF)
	//	{
	//		var[0] = dataBase.HC_pRecordset->GetCollect("userId");
	//		if (var[0].vt != VT_NULL)
	//		{
	//			t1[0] = _com_util::ConvertBSTRToString((_bstr_t)var[0]);
	//		}
	//		printf(t1[0]);
	//		printf("\t");
	//		var[1] = dataBase.HC_pRecordset->GetCollect("userName");
	//		if (var[1].vt != VT_NULL)
	//		{
	//			t1[1] = _com_util::ConvertBSTRToString((_bstr_t)var[1]);
	//		}
	//		printf(t1[1]);
	//		printf("\t");
	//		var[2] = dataBase.HC_pRecordset->GetCollect("password");
	//		if (var[2].vt != VT_NULL)
	//		{
	//			t1[2] = _com_util::ConvertBSTRToString((_bstr_t)var[2]);
	//		}
	//		printf(t1[2]);
	//		printf("\n");
	//		dataBase.HC_pRecordset->MoveNext();
	//	}
	//}
	//catch (_com_error* e)
	//{
	//	cout << e->ErrorMessage() << endl;
	//}

	Server server;

	return 0;
}
