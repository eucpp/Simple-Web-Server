#include <chrono>
#include <thread>
#include <vector>

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

std::chrono::steady_clock::time_point now() {
    return std::chrono::steady_clock::now();
}

#if !(defined(PRECISE_GC_SERIAL) || defined(PRECISE_GC_CMS))
template <typename Function, typename... Args>
    std::thread create_thread(Function&& f, Args&&... args)
    {
        return std::thread(std::forward<Function>(f), std::forward<Args>(args)...);
    };
#endif

int main(int argc, const char* argv[]) {

    #if defined(PRECISE_GC_SERIAL) || defined(PRECISE_GC_CMS)
//        enable_logging(gc_loglevel::INFO);
        set_threads_available(1);
        register_main_thread();
    #endif

    HttpServer server;
    server.config.port=8080;

    server.resource["^/test$"]["GET"]=[](ptr_t(HttpServer::Response) response, ptr_t(HttpServer::Request) request) {
        auto content=request->content.string();
        *pin(response) << "HTTP/1.1 200 OK\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
    };

    std::thread server_thread = create_thread([&server](){
        server.start();
    });
    std::this_thread::sleep_for(std::chrono::seconds(1));

    size_t threads_num = std::thread::hardware_concurrency();

    auto tm = now();

    std::vector<std::thread> threads(threads_num);
    for (auto& thread: threads) {
        thread = create_thread([] () {
            const size_t ITER_COUNT = 1000;

            HttpClient client("localhost:8080");
            for (size_t i = 0; i < ITER_COUNT; ++i) {
                auto rsp = client.request("GET", "/test");
            }
        });
    };

    for (auto& thread: threads) {
        thread.join();
    };

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now() - tm).count();
    std::cout << "Completed in " << elapsed << " ms" << std::endl;

    server.stop();
    server_thread.join();

    return 0;
}