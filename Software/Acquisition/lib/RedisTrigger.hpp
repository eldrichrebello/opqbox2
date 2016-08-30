//
// Created by tusk on 8/18/16.
//

#ifndef ACQUISITION_REDDIS_STORE_H
#define ACQUISITION_REDDIS_STORE_H

#include "opqdata.hpp"
#include <thread>

namespace opq {
    class RedisTrigger {
    public:
        RedisTrigger(opq::data::AnalysisQueue q);
        void start();
        bool stop();
    private:
        std::thread _t;
        bool _running;

        void readerLoop();
        opq::data::AnalysisQueue _q;

        std::string _redisHost;
        uint16_t _redisPort;
        std::string _redisPass;
        std::string _boxId;
        uint16_t _redisRecordLen;
    };
}

#endif //ACQUISITION_REDDIS_STORE_H
