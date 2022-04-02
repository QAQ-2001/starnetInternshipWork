#include"database.h"

AdoAccess::AdoAccess() {
	HRESULT hr = CoInitialize(NULL);
	if (!SUCCEEDED(hr)) //����ֵ���жϳ�ʼ��COM�Ƿ�ɹ�������SUCCEEDED���ж�
	{
		std::cout << "COM fail" << std::endl;
	}
	try
	{
		HC_pConnection.CreateInstance(__uuidof(Connection));//�ȼ��� //HC_pConnection.CreateInstance("ADODB.Connection");
		//�˾������λ�����ݿ������ķ�����Ϣ
		_bstr_t strConnect = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=Database2.accdb;Persist Security Info=False ";
		//�������ݿ⣬�����롣
		HC_pConnection->Open(strConnect, "", "", adModeUnknown);	//�������ݿ�

		std::cout << "���ӳɹ�,���������ָ��" << std::endl;
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
			if (!FAILED(HC_pConnection.CreateInstance(_uuidof(Connection))))  //�������ӳ�ʱʱ��
			{
				HC_pConnection->CommandTimeout = 30;                  //�������ӳ�ʱֵ����λΪ��
				_bstr_t strConnect = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=Database2.accdb;Persist Security Info=False ";
				//�������ݿ⣬�����롣
				HC_pConnection->Open(strConnect, "", "", adModeUnknown);	//�������ݿ�

				std::cout << "���ӳɹ�,���������ָ��" << std::endl;
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
			//�Ľ���ֱ����Execute������_RecordsetPtr�ͺܿ죬�ܰ���
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
					//������ţ���0��ʼ
					_variant_t rusultGet = pRecordset->Fields->GetItem(long(i))->Value;

					char midData[MAXCHAR] = { 0 };
					rusultGet.ChangeType(VT_BSTR);//ͳһת���ַ��������������п��ܳ���
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
			//ż�����Ӵ�����
			//��:[DBNETLIB][ConnectionRead (recv()).]һ��������������������ĵ�
			//���� lsl-20200828
			//����˵����Data Source=10.237.103.28,1433������Ҫ���϶˿ں�
			if (errorInfo.find("DBNETLIB") != std::string::npos)
			{
				if (reConnect())
				{
					ret = selectData(sql, results);//�ٴ�ִ��
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


