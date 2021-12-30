#pragma once
#include <cpprest/http_listener.h>
#include <thread>
#include <string>
#include <memory>

#include "router.h"
#include "variables.h"

using namespace web::http::experimental::listener;
using namespace web::http;
using namespace web;

class Listener {
private:
    std::shared_ptr<http_listener> _listener;

public:
    Listener(
        std::string _url,
        std::string _route_name,
        std::string _base_url,
        std::shared_ptr<http_listener> inst_listener
    )
        : url(_url),
          route_name(_route_name),
          base_url(_base_url),
          _listener(std::move(inst_listener)) {}

    std::string url;
    std::string route_name;
    std::string base_url;

    // Starts the listener after binding all supported methods.
    void start() {
        _listener->open().wait();
        while(true);
    }

    // Several abstractions to enforce the use of the correct methods,
    // and to make the code more readable.

    // Returns a reference to the private instance of http_listener.
    http_listener& get_listener() {
        return *_listener;
    }

    // Allows the listener to receive GET requests and forwards them to the
    // provided function.
    template <typename F>
    void get(F handler) {
        // Create wrapper method to call the handler.
        auto &route_name = this->route_name;
        _listener->support(methods::GET, [handler, route_name](http_request&& request) {
            LOG("[GET::" << route_name << "] " + request.request_uri().to_string());
            handler(std::forward<http_request>(request));
        });
    }

    // Allows the listener to receive POST requests and forwards them to the
    // provided function.
    template <typename F>
    void post(F handler) {
        // Create wrapper method to call the handler.
        auto &route_name = this->route_name;
        _listener->support(methods::POST, [handler, route_name](http_request&& request) {
            LOG("[POST::" << route_name << "] " + request.request_uri().to_string());
            handler(std::forward<http_request>(request));
        });
    }

    // Allows the listener to receive PUT requests and forwards them to the
    // provided function.
    template <typename F>
    void put(F handler) {
        // Create wrapper method to call the handler.
        auto &route_name = this->route_name;
        _listener->support(methods::PUT, [handler, route_name](http_request&& request) {
            LOG("[PUT::" << route_name << "] " + request.request_uri().to_string());
            handler(std::forward<http_request>(request));
        });
    }

    // Allows the listener to receive DELETE requests and forwards them to the
    // provided function.
    template <typename F>
    void del(F handler) {
        // Create wrapper method to call the handler.
        auto &route_name = this->route_name;
        _listener->support(methods::DEL, [handler, route_name](http_request&& request) {
            LOG("[DELETE::" << route_name << "] " + request.request_uri().to_string());
            handler(std::forward<http_request>(request));
        });
    }

    // Allows you to specify a handler that uses the method provided by <method>.
    // This function does not create a wrapper method, so the handler is passed
    // through perfect forwarding.
    template<typename F>
    void use(const http::method& method, F&& handler) {
        _listener->support(method, std::forward<F>(handler));
    }
};