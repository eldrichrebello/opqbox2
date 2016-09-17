//
// Created by tusk on 8/18/16.
//

#include "RedisTrigger.hpp"
#include "Settings.hpp"
#include <hiredis/hiredis.h>

using namespace std;
using namespace opq;

RedisTrigger::RedisTrigger(opq::data::AnalysisQueue q) {
    _q = q;
    _running = false;
    auto set = Settings::Instance();
    _boxId = set->getString("box_id");
    _redisHost = set->getString("redis_trigger_host");
    _redisPort = (uint16_t)set->getInt("redis_trigger_port");
    _redisRecordLen = (uint16_t)set->getInt("redis_trigger_depth");
    _redisPass = set->getString("redis_auth");
}

void RedisTrigger::start() {
    _t = std::thread([this] { readerLoop(); });
}

bool RedisTrigger::stop() {
    _running = false;
    _t.join();
}

void RedisTrigger::readerLoop() {
    _running = true;
    std::string triggerKey = "box:" + _boxId + ":trg";
    redisReply *reply;
    redisContext* c = NULL;
    while(c == NULL && _running){
        _q->clear();
        c = redisConnect(_redisHost.c_str(), _redisPort);
        if (c != NULL && c->err) {
            redisFree(c);
            c = NULL;
        }
        if(c == NULL)
            this_thread::sleep_for(chrono::milliseconds(1000));
    }
    reply = (redisReply*)redisCommand(c, "AUTH %s", _redisPass.c_str());
    freeReplyObject(reply);

    reply = (redisReply*)redisCommand(c, "DEL %s", triggerKey.c_str());
    freeReplyObject(reply);
    std::string trimCommand = "LTRIM " + triggerKey + " 0 " + std::to_string(_redisRecordLen);
    while (_running) {
        auto message = _q->pop();
        auto messageBuffer = opq::data::serialize_analysis_trigger_redis(message);
        redisAppendCommand(c, "LPUSH %s %s",triggerKey.c_str(), messageBuffer.c_str());
        redisAppendCommand(c, trimCommand.c_str());
        redisGetReply(c,(void**)&reply);
        freeReplyObject(reply);
        redisGetReply(c,(void**)&reply);
        freeReplyObject(reply);

    }
    redisFree(c);
}
