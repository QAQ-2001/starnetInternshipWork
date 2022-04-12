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
					rusultGet.ChangeType(VT_BSTR);	//统一转成字符串，否则下面有可能出错
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
			//ret = RefreshNum.lVal; //更新行数
			ret = 0;
		}
		catch (_com_error e)
		{
			std::string errorInfo(e.Description());
			std::cout << errorInfo << std::endl;
			//偶发连接错误处理
			//报:[DBNETLIB][ConnectionRead (recv()).]一般性网络错误。请检查网络文档
			if (errorInfo.find("DBNETLIB") != std::string::npos)
			{
				if (reConnect())
				{
					ret = executeSql(sql);//再次执行
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
	//查询loginName是否存在,存在将对应password和userName写入results中
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
	//若results为空,返回账号错误 否则将password和数据库中的password进行加密对比,相同返回根据userName产生的token
	//不同返回密码错误
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
	//查询loginName对应的prior,为空返回账号错误,否则返回prior
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
	//验证token
	std::string tokenResult = verifyToken(token);
	//token超时,提示重新登录
	if ("token expired" == tokenResult)
	{
		return "token expired";
	}
	//token验证成功
	if ("verify success" == tokenResult)
	{
		std::vector<std::string> results;
		std::string username;
		username = getUsername(token);
		try
		{
			//查询是否已经有相关的data记录
			std::string tmpSql = "SELECT * FROM [data] WHERE username = '" + username + "' AND myDate = #" + myDate
				+ "# AND projectName = '" + projectName + "'";
			AdoAccess::selectData(tmpSql, results);

			//若没有,插入该数据
			if (true == results.empty())
			{
				tmpSql = "INSERT INTO [data] (userName, projectName, duration, myDate) values('" + username +
					"','" + projectName + "','" + duration + "',#" + myDate + "#)";
				AdoAccess::executeSql(tmpSql);
				return "execute sql success";
			}

			//若有,更新该数据
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
	//验证token
	std::string tokenResult = verifyToken(token);
	//token超时,提示重新登录
	if ("token expired" == tokenResult)
	{
		results.emplace_back("token expired");
		return results;
	}

	try
	{
		if ("verify success" == tokenResult)
		{
			//返回数据的统计值,相同项的duration相加
			if (total == "total")
			{
				//下列四项属于数据库查询结果的存储结构
				//userName----和用户名有关的查询结果
				//groupName----和组名有关的查询结果
				//tmpProTimeResults----和项目名有关的时间中间查询结果
				//tmpUserTimeResults----和用户名有关的时间中间查询结果
				std::vector<std::string> userName;
				std::vector<std::string> groupName;
				std::vector<std::string> tmpProTimeResults;
				std::vector<std::string> tmpUserTimeResults;
				
				//返回所有人的
				if ("all" == range)
				{
					//拿到所有人的用户名
					std::string tmpSql = "SELECT userName FROM [user]";
					AdoAccess::selectData(tmpSql, userName);

					//对__userName做sql的in语句处理
					std::string __userName = "(";
					for (int i = 0; i < userName.size() - 1; i++)
					{
						__userName = __userName + "'" + split(userName[i], ",")[1] + "',";
					}
					__userName = __userName + "'" + split(userName[userName.size() - 1], ",")[1] + "')";

					//首位记录userName有多少条
					results.emplace_back("");

					//查找每个用户的项目总时间
					tmpSql = "SELECT userName, projectName, Sum(duration) FROM [data] WHERE userName IN " + __userName
						+ " AND myDate between #" + starDate + "# and #" + endDate + "#" + " GROUP BY userName, projectName;";
					AdoAccess::selectData(tmpSql, tmpUserTimeResults);
					results.insert(results.end(), tmpUserTimeResults.begin(), tmpUserTimeResults.end());

					//判空
					if (true == tmpUserTimeResults.empty())
					{
						results.emplace_back("no data");
						return results;
					}

					//首位记录userName有多少条
					results[0] = std::to_string(tmpUserTimeResults.size());

					//查找每个项目的总时间
					tmpSql = "SELECT projectName, Sum(duration) FROM [data] WHERE userName IN " + __userName
						+ " AND myDate between #" + starDate + "# and #" + endDate + "#" + " GROUP BY projectName;";
					AdoAccess::selectData(tmpSql, tmpProTimeResults);
					results.insert(results.end(), tmpProTimeResults.begin(), tmpProTimeResults.end());

					return results;
				}
				
				//返回某个组的
				if ("group" == range)
				{
					//拿到该成员所在的组的名称
					std::string _userName = getUsername(token);
					std::string tmpSql = "SELECT groupName FROM [user] WHERE userName = '" + _userName + "'";
					AdoAccess::selectData(tmpSql, groupName);
					std::string _groupName = split(groupName[0], ",")[1];

					//根据该组名称拿到该组所有的用户名
					tmpSql = "SELECT userName FROM [user] WHERE groupName = '" + _groupName + "'";
					AdoAccess::selectData(tmpSql, userName);

					//首位记录userName有多少条
					results.emplace_back("");

					//对__userName做sql的in语句处理
					std::string __userName = "(";
					for (int i = 0; i < userName.size() - 1; i++)
					{
						__userName = __userName + "'" + split(userName[i], ",")[1] + "',";
					}
					__userName = __userName + "'" + split(userName[userName.size() - 1], ",")[1] + "')";

					//查找每个用户的项目总时间
					tmpSql = "SELECT userName, projectName, Sum(duration) FROM [data] WHERE userName IN " + __userName
						+ " AND myDate between #" + starDate + "# and #" + endDate + "#" + " GROUP BY userName, projectName;";
					AdoAccess::selectData(tmpSql, tmpUserTimeResults);
					results.insert(results.end(), tmpUserTimeResults.begin(), tmpUserTimeResults.end());

					//判空
					if (true == tmpUserTimeResults.empty())
					{
						results.emplace_back("no data");
						return results;
					}

					//首位记录userName有多少条
					results[0] = std::to_string(tmpUserTimeResults.size());

					//查找每个项目的总时间
					tmpSql = "SELECT projectName, Sum(duration) FROM [data] WHERE userName IN " + __userName
						+ " AND myDate between #" + starDate + "# and #" + endDate + "#" + " GROUP BY projectName;";
					AdoAccess::selectData(tmpSql, tmpProTimeResults);
					results.insert(results.end(), tmpProTimeResults.begin(), tmpProTimeResults.end());

					return results;
				}
				
				//返回该用户的
				std::string _userName = getUsername(token);
				std::string tmpSql = "SELECT userName, projectName, Sum(duration) FROM [data] WHERE userName = '" +
					_userName + "' AND myDate between #" + starDate + "# and #" + endDate + "#" + " GROUP BY userName, projectName;";
				AdoAccess::selectData(tmpSql, results);

				//判空
				if (true == results.empty())
				{
					results.emplace_back("no data");
					return results;
				}

				return results;
			}

			//返回数据库中的所有条目

			//返回所有人的
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
			
			//返回某个组的
			if ("group" == range)
			{
				//同理,如上
				std::vector<std::string> userName;
				std::vector<std::string> groupName;

				//拿到该成员所在的组的名称
				std::string _username = getUsername(token);
				std::string tmpSql = "SELECT groupName FROM [user] WHERE userName = '" + _username + "'";
				AdoAccess::selectData(tmpSql, groupName);
				std::string _groupName = split(groupName[0], ",")[1];

				//根据该组名称拿到该组所有的用户名
				tmpSql = "SELECT userName FROM [user] WHERE groupName = '" + _groupName + "'";
				AdoAccess::selectData(tmpSql, userName);

				//对__userName做sql的in语句处理
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
			
			//返回该用户的
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
	//验证token
	std::string tokenResult = verifyToken(token);
	//token超时,提示重新登录
	if ("token expired" == tokenResult)
	{
		return "token expired";
	}

	if ("verify success" == verifyToken(token))
	{
		std::vector<std::string> results;
		try
		{
			//查询是否已经有相关的project记录
			std::string tmpSql = "SELECT * FROM [project] WHERE projectName = '" + projectName + "'";
			AdoAccess::selectData(tmpSql, results);

			//若没有,插入该数据
			if (true == results.empty())
			{
				tmpSql = "INSERT INTO [project] (projectName, manager) values('" +
					projectName + "','" + manager + "')";
				AdoAccess::executeSql(tmpSql);
				return "execute sql success";
			}

			//若有,更新该数据
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
	//验证token
	std::string tokenResult = verifyToken(token);
	//token超时,提示重新登录
	if ("token expired" == tokenResult)
	{
		results.emplace_back("token expired");
		return results;
	}
	//验证成功,返回所有项目名
	if ("verify success" == verifyToken(token))
	{
		std::string tmpSql = "SELECT * FROM [project]";
		AdoAccess::selectData(tmpSql, results);
		return results;
	}

}


