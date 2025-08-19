#include "PupilsController.h"
#include <drogon/drogon.h>
#include <functional>
#include <string>
#include <memory>
using namespace std;
using namespace drogon;

// static auto dbClient = app().getDbClient("default");
void PupilsController::getPupils(const HttpRequestPtr& req, function<void (const HttpResponsePtr &)> &&callback)
{
    auto dbClient = app().getDbClient("default");
    string sql = "SELECT registration_num, surname, first_name FROM pupils";
    DbInterface dbPupils(dbClient, sql);
    // auto cbPtr = new function<void (const HttpResponsePtr &)>(move(callback));
    auto cbPtr = make_shared<function <void (const HttpResponsePtr&)> >(move(callback));
    dbPupils.getAll(
        [cbPtr](const Result& results)
        {
            Json::Value jsonArray(Json::arrayValue);
            for (const auto& row : results)
            {
                Json::Value jsonRow;
                Json::Value name;
                jsonRow["registration_num"] = row["registration_num"].as<int>();
                name["surname"] = row["surname"].as<std::string>();
                name["first_name"] = row["first_name"].as<std::string>();

                jsonRow["name"] = name;
                jsonArray.append(jsonRow);
            }

            if (results.empty())
            {
                Json::Value error;
                error["error"] = "There are currently no pupils";
                (*cbPtr)(HttpResponse::newHttpJsonResponse(error));
            }
            else
            {
                (*cbPtr)(HttpResponse::newHttpJsonResponse(jsonArray));
            }
            
        },
        [cbPtr](string err)
        {
            Json::Value error;
            error["error"] = err;
            (*cbPtr)(HttpResponse::newHttpJsonResponse(error));
        }
    );

    (void) req;
}

void PupilsController::getPupil(const HttpRequestPtr& req, function<void (const HttpResponsePtr &)> &&callback, int registration_num)
{
    auto dbClient = app().getDbClient("default");
    string sql = "SELECT registration_num, surname, first_name FROM pupils WHERE registration_num=$1";
    DbInterface dbPupils(dbClient, sql);
    // auto cbPtr = new function<void (const HttpResponsePtr &)>(move(callback));
    auto cbPtr = make_shared<function <void (const HttpResponsePtr&)> >(move(callback));
    dbPupils.getAll(
        [cbPtr](const Result& results)
        {
            Json::Value jsonArray(Json::arrayValue);
            for (const auto& row : results)
            {
                Json::Value jsonRow;
                Json::Value name;
                jsonRow["registration_num"] = row["registration_num"].as<int>();
                name["surname"] = row["surname"].as<std::string>();
                name["first_name"] = row["first_name"].as<std::string>();

                jsonRow["name"] = name;
                jsonArray.append(jsonRow);
            }

            if (results.empty())
            {
                Json::Value error;
                error["error"] = "The registration number does not exist !!!!!";
                (*cbPtr)(HttpResponse::newHttpJsonResponse(error));
            }
            else
            {
                (*cbPtr)(HttpResponse::newHttpJsonResponse(jsonArray));
            }
            
        },
        [cbPtr](string err)
        {
            Json::Value error;
            error["error"] = err;
            (*cbPtr)(HttpResponse::newHttpJsonResponse(error));
        },
        (int64_t) registration_num
    );

    (void) req;
}