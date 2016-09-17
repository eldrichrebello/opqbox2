//
// Created by tusk on 9/14/16.
//

#include "ZMQTrigger.hpp"
#include "Settings.hpp"
#include "opq.pb.h"
#include <czmq.h>
#include <opqdata.hpp>
#include <string>

using namespace opq;

ZMQTrigger::ZMQTrigger(opq::data::AnalysisQueue q) {
    _q = q;
    _running = false;
    auto set = Settings::Instance();
    Settings *s = Settings::Instance();
    _boxId = s->getInt("box_id");
    _host = s->getString("zmq_host");
    _server_cert_path = s->getString("zmq_server_cert");
    _private_cert_path = s->getString("zmq_private_cert");
    zsys_handler_set(NULL);
}

void ZMQTrigger::start() {
    _t = std::thread([this] { readerLoop(); });
}

bool ZMQTrigger::stop() {
    _running = false;
    _t.join();
}

void ZMQTrigger::readerLoop() {
    _running = true;
    zctx_t *ctx = zctx_new ();
    zauth_t *auth = zauth_new (ctx);
    zauth_set_verbose (auth, true);
    //We are only checking the identity of the client.
    zauth_configure_curve (auth, "*", CURVE_ALLOW_ANY);
    zcert_t *client_cert = zcert_load(_private_cert_path.c_str());
    //Load a cert here
    zcert_t *server_cert = zcert_load(_server_cert_path.c_str());
    if(client_cert == NULL || server_cert == NULL){
        std::cout <<"Bad news! No crets!" << std::endl;
        exit(0);
    }
    const char *server_key = zcert_public_txt (server_cert);

    //  Create and connect client socket
    void *client = zsocket_new (ctx, ZMQ_PUSH);
    zcert_apply (client_cert, client);
    zsocket_set_curve_serverkey (client, server_key);
    zsocket_connect (client, _host.c_str());
    while (_running) {
        auto message = _q->pop();
        proto::TriggerMessage protomessage;
        protomessage.set_id(_boxId);
        protomessage.set_frequency(message->frequency);
        protomessage.set_rms(message->RMS);

        std::string out;
        protomessage.SerializeToString(&out);
        zstr_send(client, out.c_str());
    }
    zcert_destroy (&client_cert);
    zcert_destroy (&server_cert);
    zauth_destroy (&auth);
    zctx_destroy (&ctx);
}
