//
// Created by tusk on 9/25/16.
//

#include "RedisSerializer.hpp"
#include "Settings.hpp"
#include "util.hpp"

#include <iostream>
#include <string>
#include <thread>
#include <opqdata.hpp>

using namespace opq;
using namespace std;

RedisSerializer::RedisSerializer() {
    Settings *set = Settings::Instance();
    _boxId = set->getInt("box_id");

    std::string redisHost = set->getString("redis.host");
    uint16_t redisPort = (uint16_t)set->getInt("redis.port");
    std::string redisPass = set->getString("redis.auth");
    _redisRecordTTL = (uint16_t)set->getInt("redis.ttl");
    _redisRecordRotation = (uint16_t)set->getInt("redis.rotation");
    c = NULL;
    while(c == NULL){
        c = redisConnect(redisHost.c_str(), redisPort);
        if (c != NULL && c->err) {
            redisFree(c);
            c = NULL;
        }
        if(c == NULL)
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    redisReply* reply = (redisReply*)redisCommand(c, "AUTH %s", redisPass.c_str());
    if (reply == NULL) {
        throw std::runtime_error("Could not connect to redis!");
    }
    freeReplyObject(reply);
    reply = (redisReply*)redisCommand(c, "FLUSHALL");
    if(reply == NULL){
        throw std::runtime_error("Could not connect to redis!");
    }
    freeReplyObject(reply);
}

RedisSerializer::~RedisSerializer() {
    if(c != NULL)
        redisFree(c);
}

void RedisSerializer::sendToRedis(data::OPQMeasurementPtr measurement) {

    std::string message = util::serialize_to_protobuf(_boxId, measurement);
    if (measurement->timestamps.size() == 0) return;
    auto difference = std::chrono::duration_cast<std::chrono::seconds>(measurement->timestamps[0] - _lastRotation).count();
    bool setTTL = false;
    if(difference > _redisRecordRotation){
        _lastRotation = measurement->timestamps[0];
        setTTL = true;
        _key = std::to_string(util::crono_to_mili(measurement->timestamps[0]));
    }
    redisAppendCommand(c, "LPUSH %s %s",_key.c_str(), message.c_str());
    if(setTTL){
        string ttlCmd = "EXPIRE " + _key + " " + std::to_string(_redisRecordTTL);
        redisAppendCommand(c, ttlCmd.c_str());
    }
    redisReply* reply;
    redisGetReply(c,(void**)&reply);
    if (reply == NULL) {
        throw std::runtime_error("Lost connection to redis!");
    }
    freeReplyObject(reply);
    if(setTTL) {
        redisGetReply(c, (void **) &reply);
        if (reply == NULL) {
            throw std::runtime_error("Lost connection to redis!");
        }
        freeReplyObject(reply);
    }
}

RedisSerializer& opq::operator<<(RedisSerializer& redis, data::OPQMeasurementPtr measurement)
{
    redis.sendToRedis(measurement);
    return redis;
}