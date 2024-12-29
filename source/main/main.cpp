#include "../_aux/d7.hpp"
#include "../cpp-httplib/httplib.h"
#include <fftw3.h>
#include <rtl-sdr.h>


std::mutex console_mtx;
httplib::Server& configure_paths(httplib::Server& server) 
{
    server.Get("/", [](const httplib::Request& req, httplib::Response& res) {
        res.status = 200;
        res.set_content("Hello, World!", "text/plain");
    });
    return server;
}

extern  void test_fftw3(int argc, char *argv[]);
extern  void test_rtl_device();

int main(int argc, char *argv[]) 
{
    test_fftw3(argc, argv);

    std::cout << std::endl;
#ifdef _DEBUG
    std::cout << get_gradien_colorized("### DEBUG MODE ###", CONSOLE::CYAN,  CONSOLE::BLUE) << std::endl;
#endif
    std::cout << PUSH_GREEN("<< RTL SCANER >>") << std::endl;


    // test_rtl_device();

    httplib::Server server;
    configure_paths(server);

  
    int port = 9911;
    std::string host = "127.0.0.1";
    // host = "0.0.0.0";
    std::cout << "> Run server on port " << CONSOLE::COLORS::YELLOW << host << ":" << port << CONSOLE::COLORS::RESET << std::endl;
    server.listen(host, port);
    
    std::cout << PUSH_GREEN("<< Done! >>") << std::endl;
    return 0;
} 