import os
import logging
import redis
import opq_pb2


class RedisScraper:
    def __init__(self, settings):
        self.log = logging.getLogger("Redis")
        redisHost = settings.getKey("redis.host")
        redisPort = settings.getKey("redis.port")
        redisAuth = settings.getKey("redis.auth")
        if not redisHost or not redisPort:
            self.log.fatal("redis parameters not configured")
            os._exit(0)
        port = 0
        try:
            port = int(redisPort)
        except ValueError:
            self.log.fatal("redis port should be a number")
            os._exit(0)

        self.redis = redis.StrictRedis(host=redisHost, port=port, password=redisAuth)
        if not redis:
            self.log.fatal("Could not connect to redis")
            os._exit(0)
        self.log.info("Connected to Redis")

    def getRange(self, low, high):
        key_strs = self.redis.keys()
        keys = map(lambda x: int(x), key_strs)
        keys = sorted(keys)
        readout_keys = []
        last_key = 0
        for key in keys:
            if low < key < high:
                if len(readout_keys) == 0:
                    readout_keys.append(last_key)
                readout_keys.append(key)
            elif key > high:
                readout_keys.append(key)
                break
            last_key = key

        message = opq_pb2.DataMessage()
        i = 0
        for key in readout_keys:
            buffers = self.redis.lrange(key, 0, -1)
            for buffer in buffers:
                data = opq_pb2.DataMessage()
                data.ParseFromString(buffer)
                i += len(data.cycles)
                message.cycles.extend(filter(lambda x: low < x.time < high, data.cycles))
        return message
