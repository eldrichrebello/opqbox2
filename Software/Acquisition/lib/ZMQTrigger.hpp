//
// Created by tusk on 9/14/16.
//

#ifndef ACQUISITION_ZMQTRIGGER_HPP
#define ACQUISITION_ZMQTRIGGER_HPP

#include "opqdata.hpp"
#include <thread>
#include <string>

namespace opq {
    /**
     * @brief ZeroMQ Trigger delivery service.
     */
    class ZMQTrigger {
    public:
        /**
         * @brief Creates a new ZeroMQ connection.
         * OPQAnalysisPtr object from the queue q will be serialized and sent over the network.
         * @param q input queue.
         */
        ZMQTrigger(opq::data::AnalysisQueue q);

        /**
         * @brief Start the ZMQ thread.
         */
        void start();

        /**
         * @brief Stop the ZMQ thread.
         */
        void stop();
    private:
        int _boxId;

        std::thread _t;
        bool _running;

        void readerLoop();
        opq::data::AnalysisQueue _q;

        std::string _host;
        std::string _server_cert_path;
        std::string _private_cert_path;
    };
}

#endif //ACQUISITION_ZMQTRIGGER_HPP
