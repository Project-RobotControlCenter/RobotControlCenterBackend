//
// Created by elejandc on 11/11/24.
//

#ifndef APP_H
#define APP_H
#include <memory>
#include <string>


class App {
public:
    App(const App&) = delete;
    App& operator=(const App&) = delete;
    ~App();

    static bool initInstance(int argc, const char * argv[]) {
        if(!_instance) {
            _instance = std::unique_ptr<App>(new App(argc, argv));
        }
        return _instance != nullptr;
    }

private:
    static std::unique_ptr<App> _instance;

    std::string _db_local_ip;
    std::string _db_local_port;
    std::string _db_remove_ip;
    std::string _db_remove_port;
    std::string _db_password;
    std::string _frontend_websocket_port;
    std::string _robots_websocket_port;

    App(int argc, const char * argv[]);
};



#endif //APP_H
