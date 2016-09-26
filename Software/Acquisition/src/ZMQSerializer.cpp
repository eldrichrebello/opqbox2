#include "ZMQSerializer.hpp"
#include "Settings.hpp"
#include "util.hpp"
#include "boost/log/trivial.hpp"
using namespace opq;

ZMQSerializer::ZMQSerializer(){
    Settings *set = Settings::Instance();
    _boxId = set->getInt("box_id");
    std::string host = set->getString("zmq.host");;
    std::string server_cert_path = set->getString("zmq.server_cert");;
    std::string private_cert_path = set->getString("zmq.private_cert");

    _ctx = (zctx_t*)zctx_new ();
    _auth = zauth_new (_ctx);
    zauth_set_verbose (_auth, true);
    //We are only checking the identity of the client.
    zauth_configure_curve (_auth, "*", CURVE_ALLOW_ANY);
    _client_cert = (zcert_t*)zcert_load(private_cert_path.c_str());
    //Load a cert here
    _server_cert = (zcert_t*)zcert_load(server_cert_path.c_str());
    if(_client_cert == NULL || _server_cert == NULL){
        BOOST_LOG_TRIVIAL(fatal) << "Could not load certificates" ;
        exit(0);
    }
    const char *server_key = zcert_public_txt (_server_cert);
    //  Create and connect client socket
    client = zsocket_new (_ctx, ZMQ_PUSH);
    zcert_apply (_client_cert, client);
    zsocket_set_curve_serverkey (client, server_key);
    zsocket_connect (client, host.c_str());
    BOOST_LOG_TRIVIAL(info) << "Sending data Trigger data to "  + host;

    zsys_handler_set(NULL);
}

ZMQSerializer::~ZMQSerializer(){
    zcert_destroy (&_client_cert);
    zcert_destroy (&_server_cert);
    zauth_destroy (&_auth);
    zctx_destroy (&_ctx);
}

void ZMQSerializer::sendToZMQ(data::OPQAnalysisPtr message){
    std::string out = util::serialize_to_protobuf(_boxId,message);
    zstr_send(client, out.c_str());
}

ZMQSerializer& opq::operator<<(ZMQSerializer& zmq, data::OPQAnalysisPtr message)
{
    zmq.sendToZMQ(message);
    return zmq;
}