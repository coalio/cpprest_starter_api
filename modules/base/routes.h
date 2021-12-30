#pragma once
#include <memory>

#include "router.h"
#include "listener.h"

// Define routes here.
/*
    Format:
        Create multiple listeners uusing Router::add_listener(path, name), this will return you a reference
        to the listener, from which you can define your supported methods.

        Uste Listener::get, Listener::post, Listener::put, Listener::del to specify the supported methods for the
        listener. Or use Listener::use and specify a method and a function. Information about their corresponding
        function signatures and examples of usage can be found in listener.h and in this file.

        Using a lambda function is the best way to bind a function to a listener. The lambda function must send a response
        to the client using request.reply(status_code, value).

        Then call Router::begin_listening() to start every listener in a separate thread.
*/
/*
    - What about this file?
        - The function "run_routes" at the bottom of this file is the main function, it will
          run all the route methods defined above.
*/

Router main_router = Router(8000);

// Health routes
void health_methods() {
    main_router.add_listener("/health", "health")
    .get([](http_request request) {
        request.reply(status_codes::OK, "OK");
    });
}

// Runs all defined routes and starts listening.
void run_routes() {
    LOG("Setting up routes");
    health_methods();
    main_router.begin_listening();
}