#include"database.h"

AdoAccess::AdoAccess() {
	HRESULT hr = CoInitialize(NULL);
	if (!SUCCEEDED(hr)) //返回值可判断初始化COM是否成功，请用SUCCEEDED来判断
	{
		std::cout << "COM fail" << std::endl;
	}
	try
	{
		HC_pConnection.CreateInstance(__uuidof(Connection));//等价于 //HC_pConnection.CreateInstance("ADODB.Connection");
		//此句包含定位你数据库的所需的访问信息
		_bstr_t strConnect = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=Database2.accdb;Persist Security Info=False ";
		//连接数据库，无密码。
		HC_pConnection->Open(strConnect, "", "", adModeUnknown);	//连接数据库

		std::cout << "连接成功,并获得智能指针" << std::endl;
	}
	catch (_com_error e)
	{
		std::string errorInfo(e.Description());
		std::cout << errorInfo << std::endl;
	}
}

bool AdoAccess::reConnect()
{
	if (NULL == HC_pConnection || adStateClosed == HC_pConnection->State)
	{
		try
		{  //Connecting
			if (!FAILED(HC_pConnection.CreateInstance(_uuidof(Connection))))  //设置连接超时时间
			{
				HC_pConnection->CommandTimeout = 30;                  //设置连接超时值，单位为秒
				_bstr_t strConnect = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=Database2.accdb;Persist Security Info=False ";
				//连接数据库，无密码。
				HC_pConnection->Open(strConnect, "", "", adModeUnknown);	//连接数据库

				std::cout << "连接成功,并获得智能指针" << std::endl;
				return true;
			}
		}
		catch (_com_error e)
		{
			std::string errorInfo(e.Description());
			std::cout << errorInfo << std::endl;
		}
	}
	return false;
}

AdoAccess::~AdoAccess() {
	/*if (HC_pRecordset != NULL)
		HC_pRecordset->Close();*/
	HC_pConnection->Close();
	::CoUninitialize();
}

int AdoAccess::selectData(const std::string& sql, std::vector<std::string>& results)
{
	int ret = -1;

	if (sql.size() > 0)
	{
		if (NULL == HC_pConnection || adStateClosed == HC_pConnection->State)
		{
			reConnect();
		}

		try
		{
			_variant_t RefreshNum;
			//改进，直接用Execute，返回_RecordsetPtr就很快，很棒！
			_RecordsetPtr pRecordset = HC_pConnection->Execute(_bstr_t(sql.c_str()), &RefreshNum, adCmdText);
			long line = 0;
			while (NULL != pRecordset && !pRecordset->adoEOF && adStateClosed != pRecordset->State)
			{
				long count = pRecordset->Fields->Count;
				++line;
				std::stringstream ss;
				ss << line;
				for (long i = 0; i < count; ++i)
				{
					//按列序号，从0开始
					_variant_t rusultGet = pRecordset->Fields->GetItem(long(i))->Value;

					char midData[MAXCHAR] = { 0 };
					rusultGet.ChangeType(VT_BSTR);//统一转成字符串，否则下面有可能出错
					WideCharToMultiByte(CP_ACP, 0, rusultGet.bstrVal, -1, midData, MAXCHAR, NULL, NULL);
					ss << "," << midData;
				}
				results.emplace_back(ss.str());
				pRecordset->MoveNext();
			}
			pRecordset = NULL;
			ret = 0;
		}
		catch (_com_error e)
		{
			std::string errorInfo(e.Description());
			std::cout << errorInfo << std::endl;
			//偶发连接错误处理
			//报:[DBNETLIB][ConnectionRead (recv()).]一般性网络错误。请检查网络文档
			//处理 lsl-20200828
			//网上说的是Data Source=10.237.103.28,1433，后面要加上端口号
			if (errorInfo.find("DBNETLIB") != std::string::npos)
			{
				if (reConnect())
				{
					ret = selectData(sql, results);//再次执行
				}
				if (ret != 0)
				{
					std::cout << "retry Execute error,error:" << errorInfo << " sql:" << sql << std::endl;
				}
			}
		}
	}
	return ret;
}

std::string AdoAccess::login(std::string& username, std::string& password)
{
	std::vector<std::string> results;
	try
	{
		std::string tmpSql = "SELECT * FROM [user] where userName='" + username + "' and password='" + password + "';";
		AdoAccess::selectData(tmpSql, results);
	}
	catch (_com_error e)
	{
		std::string errorInfo(e.Description());
		return errorInfo;
	}
	try
	{
		if (true == results.empty())
		{
			std::string tmpSql = "SELECT * FROM [user] where userName='" + username + "';";
			AdoAccess::selectData(tmpSql, results);
			if (true == results.empty())
			{
				return "username error";
			}	
			else
			{
				return "password error";
			}
		}
		else
		{
			return generateToken(username);
		}	
	}
	catch (_com_error e)
	{
		std::string errorInfo(e.Description());
		return errorInfo;
	}
}


