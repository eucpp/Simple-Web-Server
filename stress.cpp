#include "server_http.hpp"
#include "client_http.hpp"

#ifdef PRECISE_GC_SERIAL
#include <liballocgc/liballocgc.hpp>
using namespace allocgc;
using namespace allocgc::serial;
#endif

#ifdef PRECISE_GC_CMS
#include <liballocgc/liballocgc.hpp>
    using namespace allocgc;
    using namespace allocgc::cms;
#endif

#include "macro.hpp"

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
typedef SimpleWeb::Client<SimpleWeb::HTTP> HttpClient;

#if !(defined(PRECISE_GC_SERIAL) || defined(PRECISE_GC_CMS))
template <typename Function, typename... Args>
    std::thread create_thread(Function&& f, Args&&... args)
    {
        return std::thread(std::forward<Function>(f), std::forward<Args>(args)...);
    };
#endif

int main(int argc, const char* argv[]) {

    #if defined(PRECISE_GC_SERIAL) || defined(PRECISE_GC_CMS)
    //        enable_logging(gc_loglevel::DEBUG);
        register_main_thread();
    #endif

    HttpServer server;
    server.config.port=8080;

    server.resource["^/string$"]["POST"]=[](ptr_t(HttpServer::Response) response, ptr_t(HttpServer::Request) request) {
        //Retrieve string:
        auto content=request->content.string();
        //request->content.string() is a convenience function for:
        //stringstream ss;
        //ss << request->content.rdbuf();
        //string content=ss.str();

        *pin(response) << "HTTP/1.1 200 OK\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
    };
}