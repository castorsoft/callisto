#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class ArquivosController : public drogon::HttpController<ArquivosController>
{
public:
    METHOD_LIST_BEGIN
        //use METHOD_ADD to add your custom processing function here;
        ADD_METHOD_TO(ArquivosController::index, "/api/v1/arquivos", HttpMethod::Get);                  //path is /api/v1/User/{arg1}
        // METHOD_ADD(User::getDetailInfo, "/{id}/detailinfo", Get);  //path is /api/v1/User/{arg1}/detailinfo
        // METHOD_ADD(User::newUser, "/{name}", Post);                 //path is /api/v1/User/{arg1}
    METHOD_LIST_END

    void index(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const;
        // void create(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int userId) const;
        // void show(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int userId) const;
        // void update(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int userId) const;
        // void del(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int userId) const;

    ArquivosController() {
        LOG_DEBUG << "Criado ArquivosController";
    }
};
