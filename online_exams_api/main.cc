#include <iostream>
#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <string>
using namespace std;
using namespace drogon;
using namespace drogon::orm;

int main() {
    app().loadConfigFile("config.json");

    app().registerHandler(
        "/pupils",
        [](const HttpRequestPtr & req, function<void (const HttpResponsePtr &)> &&callback)
        {
            auto dbClient = app().getDbClient("default");

            auto cbPtr = new function<void (const HttpResponsePtr &)>(std::move(callback));

            dbClient->execSqlAsync(
                "SELECT registration_num, surname, first_name FROM pupils",
                [cbPtr](const Result& results)
                {
                    Json::Value json;
                    if (results.empty())
                    {
                        json["error"] = "Pupils not found";
                        (*cbPtr)(HttpResponse::newHttpJsonResponse(json));
                        return;
                    }
                    
                    Json::Value jsonArray(Json::arrayValue);

                    for (const auto & row : results)
                    {
                        Json::Value jsonRow;
                        jsonRow["registration_num"] = row["registration_num"].as<int>();
                        jsonRow["surname"] = row["surname"].as<string>();
                        jsonRow["first_name"] = row["first_name"].as<string>();

                        jsonArray.append(jsonRow);
                    }
                    (*cbPtr)(HttpResponse::newHttpJsonResponse(jsonArray));
                    
                },
                [cbPtr](const DrogonDbException& e)
                {
                    Json::Value err;
                    err["error"] = e.base().what();
                    (*cbPtr)(HttpResponse::newHttpJsonResponse(err));
                }
            );

            (void) req;
        },
        {Get}
    );

    app().run();
    return 0;
}
