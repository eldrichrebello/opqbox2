import os
import logging
import redis
import opq_pb2
from itertools import chain

def reserialize(serialized_measurements):
    def get_cycles(measurement):
        m = opq_pb2.DataMessage()
        m.ParseFromString(measurement)
        return m.cycles

    cycles = list(chain.from_iterable(map(get_cycles, serialized_measurements)))
    m = opq_pb2.DataMessage()
    m.cycles.extend(cycles)
    return m


class RedisScraper:
    def __init__(self, settings):
        self.log = logging.getLogger("Redis")
        redisHost = settings.getKey("redis.host")
        redisPort = settings.getKey("redis.port")
        redisAuth = settings.getKey("redis.auth")
        self.redis_buffer_key = settings.getKey("redis.key.measurementsbuffer")
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

    def get_range(self, start_ts_ms, end_ts_ms):
        measurements = self.redis.zrangebyscore(self.redis_buffer_key, start_ts_ms, end_ts_ms)
        return reserialize(measurements)


