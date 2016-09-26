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
#include <boost/log/trivial.hpp>

using namespace opq;
using namespace std;

RedisSerializer::RedisSerializer() {
    Settings *set = Settings::Instance();
    _boxId = set->getInt("box_id");

    std::string redisHost = set->getString("redis.host");
    uint16_t redisPort = (uint16_t)set->getInt("redis.port");
    std::string redisPass = set->getString("redis.auth");
    _redisRecordTTL = set->getInt("redis.ttl");
    _redisRecordRotation = set->getInt("redis.rotation");
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
    if (reply == NULL || reply->type == REDIS_REPLY_ERROR) {
        BOOST_LOG_TRIVIAL(fatal) << "Lost connection with redis";
        exit(0);
    }
    freeReplyObject(reply);
    if(setTTL) {
        redisGetReply(c, (void **) &reply);
        if (reply == NULL || reply->type == REDIS_REPLY_ERROR) {
            BOOST_LOG_TRIVIAL(fatal) << "Lost connection with redis";
            exit(0);
        }
        freeReplyObject(reply);
    }
}

RedisSerializer& opq::operator<<(RedisSerializer& redis, data::OPQMeasurementPtr measurement)
{
    redis.sendToRedis(measurement);
    return redis;
}