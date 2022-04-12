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

AdoAccess::~AdoAccess() {
	/*if (HC_pRecordset != NULL)
		HC_pRecordset->Close();*/
	HC_pConnection->Close();
	::CoUninitialize();
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
					rusultGet.ChangeType(VT_BSTR);	//ͳһת���ַ��������������п��ܳ���
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

int AdoAccess::executeSql(const std::string& sql)
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
			HC_pConnection->Execute(_bstr_t(sql.c_str()), &RefreshNum, adCmdText);
			//ret = RefreshNum.lVal; //��������
			ret = 0;
		}
		catch (_com_error e)
		{
			std::string errorInfo(e.Description());
			std::cout << errorInfo << std::endl;
			//ż�����Ӵ�����
			//��:[DBNETLIB][ConnectionRead (recv()).]һ��������������������ĵ�
			if (errorInfo.find("DBNETLIB") != std::string::npos)
			{
				if (reConnect())
				{
					ret = executeSql(sql);//�ٴ�ִ��
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

std::string AdoAccess::login(std::string& loginName, std::string& password)
{
	std::vector<std::string> results;
	//��ѯloginName�Ƿ����,���ڽ���Ӧpassword��userNameд��results��
	try
	{
		std::string tmpSql = "SELECT password, userName FROM [user] WHERE loginName = '" + loginName + "'";
		AdoAccess::selectData(tmpSql, results);
	}
	catch (_com_error e)
	{
		std::string errorInfo(e.Description());
		std::cout << errorInfo << std::endl;
		return "database error";
	}
	//��resultsΪ��,�����˺Ŵ��� ����password�����ݿ��е�password���м��ܶԱ�,��ͬ���ظ���userName������token
	//��ͬ�����������
	try
	{
		if (true == results.empty())
		{
			return "loginName error";
		}
		else
		{
			std::vector<std::string> result = split(results[0], ",");
			std::string userName = result[2];
			std::string passwordDB = encryptMd5(result[1]);
			if (0 == passwordDB.compare(password))
			{
				return generateToken(userName);
			}
			return "password error";
		}	
	}
	catch (_com_error e)
	{
		std::string errorInfo(e.Description());
		std::cout << errorInfo << std::endl;
		return "database error";
	}
}

std::string AdoAccess::getAuthority(std::string& loginName)
{
	std::vector<std::string> results;
	//��ѯloginName��Ӧ��prior,Ϊ�շ����˺Ŵ���,���򷵻�prior
	try
	{
		std::string tmpSql = "SELECT authority FROM [user] WHERE loginName = '" + loginName + "'";
		AdoAccess::selectData(tmpSql, results);
		if (true == results.empty())
		{
			return "loginName error";
		}
		std::vector<std::string> result = split(results[0], ",");
		return result[1];
	}
	catch (_com_error e)
	{
		std::string errorInfo(e.Description());
		std::cout << errorInfo << std::endl;
		return "database error";
	}
}

std::string AdoAccess::updateTimeTable(std::string& token, std::string& projectName, std::string& duration, std::string& myDate)
{
	//��֤token
	std::string tokenResult = verifyToken(token);
	//token��ʱ,��ʾ���µ�¼
	if ("token expired" == tokenResult)
	{
		return "token expired";
	}
	//token��֤�ɹ�
	if ("verify success" == tokenResult)
	{
		std::vector<std::string> results;
		std::string username;
		username = getUsername(token);
		try
		{
			//��ѯ�Ƿ��Ѿ�����ص�data��¼
			std::string tmpSql = "SELECT * FROM [data] WHERE username = '" + username + "' AND myDate = #" + myDate
				+ "# AND projectName = '" + projectName + "'";
			AdoAccess::selectData(tmpSql, results);

			//��û��,���������
			if (true == results.empty())
			{
				tmpSql = "INSERT INTO [data] (userName, projectName, duration, myDate) values('" + username +
					"','" + projectName + "','" + duration + "',#" + myDate + "#)";
				AdoAccess::executeSql(tmpSql);
				return "execute sql success";
			}

			//����,���¸�����
			tmpSql = "UPDATE data SET duration = '" + duration + "' WHERE projectName = '" + projectName + "' AND username = '"
				+ username + "' AND myDate = #" + myDate + "#";
			AdoAccess::executeSql(tmpSql);
			return "execute sql success";
		}
		catch (_com_error e)
		{
			std::string errorInfo(e.Description());
			std::cout << errorInfo << std::endl;
			return "database error";
		}
	}
	return "token error";
}

std::vector<std::string> AdoAccess::getTimeTable(std::string& token, std::string& starDate, std::string& endDate, std::string range, std::string total)
{
	std::vector<std::string> results;
	//��֤token
	std::string tokenResult = verifyToken(token);
	//token��ʱ,��ʾ���µ�¼
	if ("token expired" == tokenResult)
	{
		results.emplace_back("token expired");
		return results;
	}

	try
	{
		if ("verify success" == tokenResult)
		{
			//�������ݵ�ͳ��ֵ,��ͬ���duration���
			if (total == "total")
			{
				//���������������ݿ��ѯ����Ĵ洢�ṹ
				//userName----���û����йصĲ�ѯ���
				//groupName----�������йصĲ�ѯ���
				//tmpProTimeResults----����Ŀ���йص�ʱ���м��ѯ���
				//tmpUserTimeResults----���û����йص�ʱ���м��ѯ���
				std::vector<std::string> userName;
				std::vector<std::string> groupName;
				std::vector<std::string> tmpProTimeResults;
				std::vector<std::string> tmpUserTimeResults;
				
				//���������˵�
				if ("all" == range)
				{
					//�õ������˵��û���
					std::string tmpSql = "SELECT userName FROM [user]";
					AdoAccess::selectData(tmpSql, userName);

					//��__userName��sql��in��䴦��
					std::string __userName = "(";
					for (int i = 0; i < userName.size() - 1; i++)
					{
						__userName = __userName + "'" + split(userName[i], ",")[1] + "',";
					}
					__userName = __userName + "'" + split(userName[userName.size() - 1], ",")[1] + "')";

					//��λ��¼userName�ж�����
					results.emplace_back("");

					//����ÿ���û�����Ŀ��ʱ��
					tmpSql = "SELECT userName, projectName, Sum(duration) FROM [data] WHERE userName IN " + __userName
						+ " AND myDate between #" + starDate + "# and #" + endDate + "#" + " GROUP BY userName, projectName;";
					AdoAccess::selectData(tmpSql, tmpUserTimeResults);
					results.insert(results.end(), tmpUserTimeResults.begin(), tmpUserTimeResults.end());

					//�п�
					if (true == tmpUserTimeResults.empty())
					{
						results.emplace_back("no data");
						return results;
					}

					//��λ��¼userName�ж�����
					results[0] = std::to_string(tmpUserTimeResults.size());

					//����ÿ����Ŀ����ʱ��
					tmpSql = "SELECT projectName, Sum(duration) FROM [data] WHERE userName IN " + __userName
						+ " AND myDate between #" + starDate + "# and #" + endDate + "#" + " GROUP BY projectName;";
					AdoAccess::selectData(tmpSql, tmpProTimeResults);
					results.insert(results.end(), tmpProTimeResults.begin(), tmpProTimeResults.end());

					return results;
				}
				
				//����ĳ�����
				if ("group" == range)
				{
					//�õ��ó�Ա���ڵ��������
					std::string _userName = getUsername(token);
					std::string tmpSql = "SELECT groupName FROM [user] WHERE userName = '" + _userName + "'";
					AdoAccess::selectData(tmpSql, groupName);
					std::string _groupName = split(groupName[0], ",")[1];

					//���ݸ��������õ��������е��û���
					tmpSql = "SELECT userName FROM [user] WHERE groupName = '" + _groupName + "'";
					AdoAccess::selectData(tmpSql, userName);

					//��λ��¼userName�ж�����
					results.emplace_back("");

					//��__userName��sql��in��䴦��
					std::string __userName = "(";
					for (int i = 0; i < userName.size() - 1; i++)
					{
						__userName = __userName + "'" + split(userName[i], ",")[1] + "',";
					}
					__userName = __userName + "'" + split(userName[userName.size() - 1], ",")[1] + "')";

					//����ÿ���û�����Ŀ��ʱ��
					tmpSql = "SELECT userName, projectName, Sum(duration) FROM [data] WHERE userName IN " + __userName
						+ " AND myDate between #" + starDate + "# and #" + endDate + "#" + " GROUP BY userName, projectName;";
					AdoAccess::selectData(tmpSql, tmpUserTimeResults);
					results.insert(results.end(), tmpUserTimeResults.begin(), tmpUserTimeResults.end());

					//�п�
					if (true == tmpUserTimeResults.empty())
					{
						results.emplace_back("no data");
						return results;
					}

					//��λ��¼userName�ж�����
					results[0] = std::to_string(tmpUserTimeResults.size());

					//����ÿ����Ŀ����ʱ��
					tmpSql = "SELECT projectName, Sum(duration) FROM [data] WHERE userName IN " + __userName
						+ " AND myDate between #" + starDate + "# and #" + endDate + "#" + " GROUP BY projectName;";
					AdoAccess::selectData(tmpSql, tmpProTimeResults);
					results.insert(results.end(), tmpProTimeResults.begin(), tmpProTimeResults.end());

					return results;
				}
				
				//���ظ��û���
				std::string _userName = getUsername(token);
				std::string tmpSql = "SELECT userName, projectName, Sum(duration) FROM [data] WHERE userName = '" +
					_userName + "' AND myDate between #" + starDate + "# and #" + endDate + "#" + " GROUP BY userName, projectName;";
				AdoAccess::selectData(tmpSql, results);

				//�п�
				if (true == results.empty())
				{
					results.emplace_back("no data");
					return results;
				}

				return results;
			}

			//�������ݿ��е�������Ŀ

			//���������˵�
			if ("all" == range)
			{
				std::string tmpSql = "SELECT userName, projectName, duration, myDate FROM [data] WHERE myDate between #" 
					+ starDate + "# and #" + endDate + "#";
				AdoAccess::selectData(tmpSql, results);

				if (true == results.empty())
				{
					results.emplace_back("no data");
					return results;
				}

				return results;
			}
			
			//����ĳ�����
			if ("group" == range)
			{
				//ͬ��,����
				std::vector<std::string> userName;
				std::vector<std::string> groupName;

				//�õ��ó�Ա���ڵ��������
				std::string _username = getUsername(token);
				std::string tmpSql = "SELECT groupName FROM [user] WHERE userName = '" + _username + "'";
				AdoAccess::selectData(tmpSql, groupName);
				std::string _groupName = split(groupName[0], ",")[1];

				//���ݸ��������õ��������е��û���
				tmpSql = "SELECT userName FROM [user] WHERE groupName = '" + _groupName + "'";
				AdoAccess::selectData(tmpSql, userName);

				//��__userName��sql��in��䴦��
				std::string __userName = "(";
				for (int i = 0; i < userName.size() - 1; i++)
				{
					__userName = __userName + "'" + split(userName[i], ",")[1] + "',";
				}
				__userName = __userName + "'" + split(userName[userName.size() - 1], ",")[1] + "')";


				tmpSql = "SELECT userName, projectName, duration, myDate FROM [data] WHERE userName IN " 
					+ __userName + " AND myDate between #" + starDate + "# and #" + endDate + "#";
				AdoAccess::selectData(tmpSql, results);

				if (true == results.empty())
				{
					results.emplace_back("no data");
					return results;
				}

				return results;
			}
			
			//���ظ��û���
			std::string username;
			username = getUsername(token);
			std::string tmpSql = "SELECT userName, projectName, duration, myDate FROM [data] WHERE userName = '" +
				username + "' AND myDate between #" + starDate + "# and #" + endDate + "#";
			AdoAccess::selectData(tmpSql, results);

			if (true == results.empty())
			{
				results.emplace_back("no data");
				return results;
			}

			return results;
		}
	}
	catch (_com_error e)
	{
		std::string errorInfo(e.Description());
		std::cout << errorInfo << std::endl;
		results.emplace_back("database error");
		return results;
	}

	results.emplace_back("token error");
	return results;
}

std::string AdoAccess::updateProject(std::string& token, std::string& projectName, std::string& manager)
{
	//��֤token
	std::string tokenResult = verifyToken(token);
	//token��ʱ,��ʾ���µ�¼
	if ("token expired" == tokenResult)
	{
		return "token expired";
	}

	if ("verify success" == verifyToken(token))
	{
		std::vector<std::string> results;
		try
		{
			//��ѯ�Ƿ��Ѿ�����ص�project��¼
			std::string tmpSql = "SELECT * FROM [project] WHERE projectName = '" + projectName + "'";
			AdoAccess::selectData(tmpSql, results);

			//��û��,���������
			if (true == results.empty())
			{
				tmpSql = "INSERT INTO [project] (projectName, manager) values('" +
					projectName + "','" + manager + "')";
				AdoAccess::executeSql(tmpSql);
				return "execute sql success";
			}

			//����,���¸�����
			tmpSql = "UPDATE project SET manager = '" + manager + "' WHERE projectName = '" + projectName + "'";
			AdoAccess::executeSql(tmpSql);
			return "execute sql success";
		}
		catch (_com_error e)
		{
			std::string errorInfo(e.Description());
			std::cout << errorInfo << std::endl;
			return "database error";
		}
	}

	return "token error";
}

std::vector<std::string> AdoAccess::getProject(std::string& token)
{
	std::vector<std::string> results;
	//��֤token
	std::string tokenResult = verifyToken(token);
	//token��ʱ,��ʾ���µ�¼
	if ("token expired" == tokenResult)
	{
		results.emplace_back("token expired");
		return results;
	}
	//��֤�ɹ�,����������Ŀ��
	if ("verify success" == verifyToken(token))
	{
		std::string tmpSql = "SELECT * FROM [project]";
		AdoAccess::selectData(tmpSql, results);
		return results;
	}

}


