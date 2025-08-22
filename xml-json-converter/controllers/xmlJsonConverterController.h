#pragma once
#include <drogon/HttpController.h>
#include <functional>
using namespace drogon;
using namespace std;

class xmlJsonConverterController : public drogon::HttpController<xmlJsonConverterController>
{
    public:
        METHOD_LIST_BEGIN
            ADD_METHOD_TO(xmlJsonConverterController::xmlJsonConverter, "/convert", Post);
            ADD_METHOD_TO(xmlJsonConverterController::xmlJsonPathConverter, "/pathconvert", Post);
        METHOD_LIST_END

        void xmlJsonConverter(const HttpRequestPtr& req, function<void (const HttpResponsePtr&)> &&callback);
        void xmlJsonPathConverter(const HttpRequestPtr& req, function<void (const HttpResponsePtr&)> &&callback);
};