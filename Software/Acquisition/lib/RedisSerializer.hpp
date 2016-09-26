//
// Created by tusk on 9/25/16.
//

#ifndef ACQUISITION_REDISSERIALIZER_HPP
#define ACQUISITION_REDISSERIALIZER_HPP
#include <iostream>
#include <string>
#include "opqdata.hpp"

#include <hiredis/hiredis.h>

namespace opq {

    class RedisSerializer {
    public:
        RedisSerializer();

        ~RedisSerializer();

        void sendToRedis(data::OPQMeasurementPtr measurement);
    private:
        redisContext* c;
        int _boxId;
        std::chrono::time_point<std::chrono::high_resolution_clock > _lastRotation;
        std::string _key;
        uint64_t _redisRecordTTL;
        uint64_t _redisRecordRotation;
    };

    RedisSerializer& operator<<(RedisSerializer& redis, data::OPQMeasurementPtr measurement);
};
#endif //ACQUISITION_REDISSERIALIZER_HPP
