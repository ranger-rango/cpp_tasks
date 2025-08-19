#pragma once
#include <drogon/orm/DbClient.h>
#include <jsoncpp/json/json.h>
#include <string>
#include <functional>
#include <vector>

using namespace std;
using namespace drogon;
using namespace drogon::orm;

class DbInterface
{
    private:
        drogon::orm::DbClientPtr dbClient;
        string sqlQuery;
    public:
        explicit DbInterface (const drogon::orm::DbClientPtr& db, const string& sql)
        : dbClient(db), sqlQuery(sql)
        {}

        template <typename... Args>
        void getAll(function<void(const Result&)> onSuccess, function<void(string)> onError, Args&&... args) const
        {
            dbClient -> execSqlAsync
            (
                sqlQuery,
                [onSuccess](const Result& results)
                {
                    onSuccess(results);
                },
                [onError](const DrogonDbException& e)
                {
                    onError(e.base().what());
                },
                forward<Args>(args)...
            );
        }
};