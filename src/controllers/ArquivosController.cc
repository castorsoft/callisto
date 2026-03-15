#include "ArquivosController.h"

void ArquivosController::index(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const
{
    Json::Value ret;

    ret["message"] = "Ohayoo";

    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}
