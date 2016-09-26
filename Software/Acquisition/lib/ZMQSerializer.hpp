//
// Created by tusk on 9/25/16.
//

#ifndef ACQUISITION_ZMQSERIALIZER_HPP
#define ACQUISITION_ZMQSERIALIZER_HPP

#include <iostream>
#include <string>
#include "opqdata.hpp"
#include <czmq.h>

namespace opq {
    /**
     * @brief zeromq connector to the acquisition network service.
     * All of the configuration parameters are loaded via the Settings object.
     */
    class ZMQSerializer {
    public:
        /**
         * @brief Creates a new encrypted ZMQ connection.
         */
        ZMQSerializer();
        /**
         * @brief Closes the ZMQ connection.
         */
        ~ZMQSerializer();

        /**
         * @brief Send an analysis result to the network.
         * @param message message to be sent.
         */
        void sendToZMQ(data::OPQAnalysisPtr message);
    private:
        int _boxId;
        zctx_t *_ctx;
        zauth_t *_auth;
        zcert_t *_client_cert;
        zcert_t *_server_cert;
        void *client;
    };

    ///@brief Overloaded left shift operator for convenience.
    ZMQSerializer& operator<<(ZMQSerializer& zmq, data::OPQAnalysisPtr message);
};

#endif //ACQUISITION_ZMQSERIALIZER_HPP
