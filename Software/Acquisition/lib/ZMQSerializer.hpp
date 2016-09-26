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
    class ZMQSerializer {
    public:
        ZMQSerializer();
        ~ZMQSerializer();
        void sendToZMQ(data::OPQAnalysisPtr message);
    private:
        int _boxId;
        zctx_t *_ctx;
        zauth_t *_auth;
        zcert_t *_client_cert;
        zcert_t *_server_cert;
        void *client;
    };

    ZMQSerializer& operator<<(ZMQSerializer& zmq, data::OPQAnalysisPtr message);
};

#endif //ACQUISITION_ZMQSERIALIZER_HPP
