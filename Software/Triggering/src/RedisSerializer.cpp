#include "RedisSerializer.hpp"
#include "Settings.hpp"
#include "util.hpp"

#include <iostream>
#include <string>
#include <thread>
#include <opqdata.hpp>
#include <boost/log/trivial.hpp>

using namespace opq;
using namespace std;

RedisSerializer::RedisSerializer() {
    Settings *set = Settings::Instance();
    _boxId = set->getInt("box_id");

    std::string redisHost = set->getString("redis.host");
    uint16_t redisPort = (uint16_t)set->getInt("redis.port");
    std::string redisPass = set->getString("redis.auth");
    _redisRecordTtlS = set->getInt("redis.ttl.s");
    _redisRecordGcCnt = set->getInt("redis.gc.cnt");
    _trimCnt = 0;
    c = NULL;
    BOOST_LOG_TRIVIAL(info) << "Connecting to redis " + redisHost + " port " + to_string(redisPort);
    while(c == NULL){
        c = redisConnect(redisHost.c_str(), redisPort);
        if (c != NULL && c->err) {
            redisFree(c);
            c = NULL;
        }
        if(c == NULL)
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    BOOST_LOG_TRIVIAL(info) << "Connected to redis";

    redisReply* reply;
    if(redisPass != ""){
        reply = (redisReply*)redisCommand(c, "AUTH %s", redisPass.c_str());
        if (reply == NULL || reply->type == REDIS_REPLY_ERROR) {
            BOOST_LOG_TRIVIAL(fatal) << "Could not authenticate with redis!";
            exit(0);
        }
        BOOST_LOG_TRIVIAL(info) << "Authenticated with redis";
        freeReplyObject(reply);
    }

    reply = (redisReply*)redisCommand(c, "FLUSHALL");
    if(reply == NULL || reply->type == REDIS_REPLY_ERROR){
        BOOST_LOG_TRIVIAL(fatal) << "Could not authenticate with redis!";
        exit(0);
    }
    BOOST_LOG_TRIVIAL(info) << "Cleared Redis database";
    freeReplyObject(reply);
}

RedisSerializer::~RedisSerializer() {
    if(c != NULL)
        redisFree(c);
}

void RedisSerializer::sendToRedis(data::OPQMeasurementPtr measurement) {
    std::string message = util::serialize_to_protobuf(_boxId, measurement);
    if (measurement->timestamps.size() == 0) return;

    uint64_t _ts = util::crono_to_mili(measurement->timestamps[0]);
    _score = std::to_string(_ts);

    // Add to buffer
    redisAppendCommand(c, "ZADD %s %s %b", BUFFER_KEY, _score.c_str(), message.c_str(), message.length());

    // Perform GC
    uint64_t _endRange;
    if(++_trimCnt == _redisRecordGcCnt) {
        _endRange = _ts - (_redisRecordTtlS * MS_IN_S);
        redisAppendCommand(c, "ZREMRANGEBYSCORE %s -inf %s", BUFFER_KEY, std::to_string(_endRange).c_str());
        _trimCnt = 0;
    }

    redisReply* reply;
    redisGetReply(c,(void**)&reply);
    if (reply == NULL || reply->type == REDIS_REPLY_ERROR) {
        BOOST_LOG_TRIVIAL(fatal) << "Lost connection with redis";
        exit(0);
    }
    freeReplyObject(reply);
}

RedisSerializer& opq::operator<<(RedisSerializer& redis, data::OPQMeasurementPtr measurement)
{
    redis.sendToRedis(measurement);
    return redis;
}
