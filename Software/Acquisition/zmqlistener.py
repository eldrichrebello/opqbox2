import logging
import os
import zmq
import zmq.auth
import opq_pb2
import google.protobuf
from redisscraper import RedisScraper
from zmq.auth.thread import ThreadAuthenticator


class ZmqListener:
    def __init__(self, settings):

        self.redis = RedisScraper(settings)
        self.id = settings.getKey("box_id")
        self.log = logging.getLogger('ZMQ')
        self.clientPath = settings.getKey("zmq.private_cert")
        self.serverPath = settings.getKey("zmq.server_cert")
        if not self.clientPath or not self.serverPath:
            self.log.fatal("zmq certificates not configured in the settings file")
            os._exit(1)

        self.host = settings.getKey("zmq.acq_host")

        self.ctx = zmq.Context()

        self.auth = ThreadAuthenticator(self.ctx)
        self.auth.start()

        #self.auth.allow('127.0.0.1')
        self.auth.configure_curve(domain='*', location=zmq.auth.CURVE_ALLOW_ANY)

        self.client = self.ctx.socket(zmq.REP)

        try:
            client_public, client_secret = zmq.auth.load_certificate(self.clientPath)
            self.client.curve_secretkey = client_secret
            self.client.curve_publickey = client_public

            server_public, _ = zmq.auth.load_certificate(self.serverPath)

            self.client.curve_serverkey = server_public
            self.client.connect(self.host)
        except IOError:
            self.log.fatal("Could not load client certificate")
            os._exit(1)
        except ValueError:
            self.log.fatal("Could not load client certificate")
            os._exit(1)

        self.log.info("ZMQ connected to " + self.host + " using certs " + self.clientPath)
        self.running = False
        self.handlers = {opq_pb2.RequestDataMessage.PING: self.ping,
                         opq_pb2.RequestDataMessage.READ: self.read}

    def ping(self, message):
        message.type = opq_pb2.RequestDataMessage.PONG
        message_buff = message.SerializeToString()
        self.log.info("Received a PING from server")
        self.client.send(message_buff)
        return True

    def read(self, message):
        self.log.debug("Received a data transfer request from server")
        try:
            if message.front == 0 or message.back == 0:
                message.type = opq_pb2.RequestDataMessage.ERROR
                message_buff = message.SerializeToString()
                self.log.info("Bad message from server")
                self.client.send(message_buff)
                return False
            cycles = self.redis.getRange(message.time - message.back, message.time + message.front)
            cycles.id = self.id
            cycles.mid = message.mid
            message_buff = cycles.SerializeToString()
            self.client.send(message_buff)
        except google.protobuf.message.DecodeError:
            self.log.fatal("Bad request from acquisition server.")
            return False

    def run(self):
        self.running = True
        logger1 = logging.getLogger('ZMQ')
        try:
            while self.running:
                message_buff = self.client.recv()
                message = opq_pb2.RequestDataMessage()
                message.ParseFromString(message_buff)
                self.handlers[message.type](message)
        except google.protobuf.message.DecodeError:
            self.log.fatal("Bad request from acquisition server.")
