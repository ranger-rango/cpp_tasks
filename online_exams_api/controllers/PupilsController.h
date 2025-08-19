#pragma once
#include <drogon/HttpController.h>
#include "../models/DbInterface.h"
using namespace std;
using namespace drogon;

class PupilsController : public HttpController<PupilsController>
{
    private:

    public:
        METHOD_LIST_BEGIN
            ADD_METHOD_TO(PupilsController::getPupils, "/pupils", Get);
            ADD_METHOD_TO(PupilsController::getPupil, "/pupil/{registration_num}", Get);
        METHOD_LIST_END

        void getPupils(const drogon::HttpRequestPtr& req, function<void (const drogon::HttpResponsePtr &)> &&callback);
        void getPupil(const drogon::HttpRequestPtr& req, function<void (const drogon::HttpResponsePtr &)> &&callback, int regisration_num);
};