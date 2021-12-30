#pragma once
#include <thread>
#include <vector>
#include <memory>

#include "listener.h"
#include "variables.h"

class Router {
private:
    std::vector<Listener> _listeners;
    std::vector<std::thread> _threads;
    unsigned int _port;

public:
    Router(unsigned int port) : _port(port) {
        // Append the port to the URL base.
        this->url_base = DEFAULT_BASE_URL + std::to_string(port);
        LOG("Router(): Created router for port " << port);
    }

    std::string url_base;

    Listener& add_listener(std::string _url, std::string _route_name) {
        LOG("Adding route " << _route_name << " at " << _url);

        // This shared_ptr avoids the listener's m_impl (a unique_ptr) from being deleted.
        std::shared_ptr<http_listener> _inst_listener = std::make_shared<http_listener>(
            http_listener(U(url_base + _url))
        );

        // We create a new Listener using our instance of http_listener, and we move this
        // ptr's ownership to the Listener.
        Listener _listener = Listener(_url, _route_name, this->url_base, _inst_listener);
        this->_listeners.push_back(_listener);

        return this->_listeners.back();
    }

    void begin_listening() {
        for (auto& listener : _listeners) {
            _threads.push_back(std::thread([&listener]() {
                LOG("Starting listener for " << listener.route_name);
                listener.start();
            }));
        }

        LOG("Listening on " << this->_port);

        for (auto& thread : _threads) {
            // Join all threads and wait for them to finish.
            thread.join();
        }

        // Clear the threads vector.
        _threads.clear();

        // Clear the listeners vector.
        _listeners.clear();

        LOG("Closing router");
    }
};