#include"sOperation.h"

std::string serverLogin(Json::Value& value, AdoAccess& database)
{
    //json格式数据转换加判空
    std::string loginName = value["userArray"][0]["loginName"].asString();
    std::string password = value["userArray"][0]["password"].asString();
    if (loginName.empty() || password.empty())
    {
        return "lack data";
    }
    //result是否是token,是加上prior返回,不是直接返回result
    std::string result = database.login(loginName, password);
    if (std::string::npos == result.find("."))
    {
        return result;
    }
    std::string authority = database.getAuthority(loginName);
    result = result + "," + authority;

    return result;
}

std::string serverUpdateTimeTable(Json::Value& value, AdoAccess& database)
{
    //json格式数据转换加判空
    std::string token = value["token"].asString();
    if (token.empty())
    {
        return "lack data";
    }

    std::string result;
    int flag = 0;
    Json::Value projects = value["projects"];

    for (int i = 0; i < (int)projects.size(); i++)
    {
        //json格式数据转换加判空
        std::string projectName = projects[i]["projectName"].asString();
        std::string duration = projects[i]["duration"].asString();
        std::string myDate = projects[i]["myDate"].asString();
        if (projectName.empty() || duration.empty() || myDate.empty())
        {
            return "lack data";
        }
        //调用函数
        result = database.updateTimeTable(token, projectName, duration, myDate);
        if ("token expired" == result) 
        {
            return "token expired";
        }
        if (result != "execute sql success")
        {
            flag = 1;
        }
    }

    if (flag == 1)
    {
        return "execute fail";
    }
    
    return result;
}

std::string serverGetTimeTable(Json::Value& value, AdoAccess& database)
{
    //返回的结果,json格式的string
    std::string result;
    //函数返回结果的储存结构
    std::vector<std::string> results;

    //json格式数据转换加判空
    Json::Value timeTable = value["timeTable"];
    std::string token = value["token"].asString();
    std::string startDate = timeTable[0]["startDate"].asString();
    std::string endDate = timeTable[0]["endDate"].asString();
    std::string range = timeTable[0]["range"].asString();
    std::string total = timeTable[0]["total"].asString();
    if (token.empty() || startDate.empty() || endDate.empty() || token.empty())
    {
        return "lack data";
    }

    results = database.getTimeTable(token, startDate, endDate, range, total);
    //返回结果判断
    if (results[0] == "token expired")
    {
        return "token expired";
    }
    if (results[0] == "no data")
    {
        return "no data";
    }
    if (results[0] == "database error" || results[0] == "token error")
    {
        return "error";
    }
    //如果要统计且为全体或小组统计
    if ("total" == total && false == range.empty())
    {
        std::vector<std::string> resultsLine;
        Json::Value root;
        Json::Value userData;
        Json::Value projectData;
        Json::StreamWriterBuilder writerBuilder; // 新式API
        std::ostringstream os;
        writerBuilder.settings_["emitUTF8"] = true;

        int userCount = atoi(results[0].c_str());

        for (int i = 1; i < userCount + 1; i++)
        {
            resultsLine = split(results[i], ",");
            userData["userName"] = resultsLine[1];
            userData["projectName"] = resultsLine[2];
            userData["duration"] = resultsLine[3];
            root["userData"].append(userData);
        }

        for (int i = userCount + 1; i < results.size(); i++)
        {
            resultsLine = split(results[i], ",");
            projectData["projectName"] = resultsLine[1];
            projectData["duration"] = resultsLine[2];
            root["projectData"].append(projectData);
        }

        std::unique_ptr<Json::StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
        jsonWriter->write(root, &os); // json-->stringstream
        std::string result = os.str(); // 转为string

        return result;
    }
    //如果要统计且为个人统计
    if ("total" == total)
    {
        std::vector<std::string> resultsLine;
        Json::Value root;
        Json::Value userData;
        Json::StreamWriterBuilder writerBuilder; // 新式API
        std::ostringstream os;
        writerBuilder.settings_["emitUTF8"] = true;

        for (int i = 0; i < results.size(); i++)
        {
            resultsLine = split(results[i], ",");
            userData["userName"] = resultsLine[1];
            userData["projectName"] = resultsLine[2];
            userData["duration"] = resultsLine[3];
            root["userData"].append(userData);
        }

        std::unique_ptr<Json::StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
        jsonWriter->write(root, &os); // json-->stringstream
        std::string result = os.str(); // 转为string

        return result;
    }
    //不进行统计,数据库结果直接输出
    std::vector<std::string> resultsLine;
    Json::Value root;
    Json::Value timeTable;
    Json::StreamWriterBuilder writerBuilder; // 新式API
    std::ostringstream os;
    writerBuilder.settings_["emitUTF8"] = true;

    for (int i = 0; i < results.size(); i++)
    {
        resultsLine = split(results[i], ",");
        timeTable["userName"] = resultsLine[1];
        timeTable["projectName"] = resultsLine[2];
        timeTable["duration"] = resultsLine[3];
        timeTable["myDate"] = resultsLine[4];
        root["timeTable"].append(timeTable);
    }

    std::unique_ptr<Json::StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
    jsonWriter->write(root, &os); // json-->stringstream
    result = os.str(); // 转为string

    return result;
}

std::string serverUpdateProject(Json::Value& value, AdoAccess& database)
{
    std::string token = value["token"].asString();
    if (token.empty())
    {
        return "lack data";
    }

    std::string result;
    int flag = 0;
    Json::Value projects = value["projects"];

    for (int i = 0; i < (int)projects.size(); i++)
    {
        std::string projectName = projects[i]["projectName"].asString();
        std::string manager = projects[i]["manager"].asString();
        if (projectName.empty() || manager.empty())
        {
            return "lack data";
        }

        result = database.updateProject(token, projectName, manager);
        if (result != "execute sql success")
        {
            flag = 1;
        }
    }

    if (flag == 1)
    {
        return "execute fail";
    }

    return result;
}

std::string serverGetProject(Json::Value& value, AdoAccess& database)
{
    std::string token = value["token"].asString();
    if (token.empty())
    {
        return "lack data";
    }

    //返回的结果,json格式的string
    std::string result;
    //函数返回结果的储存结构
    std::vector<std::string> results;

    results = database.getProject(token);

    std::vector<std::string> resultsLine;
    Json::Value root;
    Json::Value projects;
    Json::StreamWriterBuilder writerBuilder; // 新式API
    std::ostringstream os;
    writerBuilder.settings_["emitUTF8"] = true;

    for (int i = 0; i < results.size(); i++)
    {
        resultsLine = split(results[i], ",");
        projects["projectName"] = resultsLine[1];
        projects["manager"] = resultsLine[2];
        root["projects"].append(projects);
    }

    std::unique_ptr<Json::StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
    jsonWriter->write(root, &os); // json-->stringstream
    result = os.str(); // 转为string

    return result;
}