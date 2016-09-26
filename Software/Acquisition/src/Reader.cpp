//
// Created by tusk on 6/27/16.
//
#include <fcntl.h>
#include <stdexcept>
#include <memory>
#include <unistd.h>
#include <iostream>
#include <chrono>

#include "Settings.hpp"
#include "Reader.hpp"
#include "opqdata.hpp"

using namespace std;
using namespace opq;
using namespace opq::data;


#ifdef OPQ_DEBUG

#include <math.h>
#include <time.h>
#include <stdlib.h>
#endif

Reader::Reader(MeasurementQueue &q){
    _q = q;
#ifndef OPQ_DEBUG
    _fd = ::open("/dev/opq0",O_RDONLY);
    if(_fd < 0){
       throw std::runtime_error("Could not open /dev/opq0");
    }
#else
    srand(time(NULL));
#endif
    auto settings = Settings::Instance();
    _frames_per_measurement = settings->getInt("frames_per_measurement");
    _fpm_callback_id = settings->onChangeCallback("frames_per_measurement",
    [=](OPQSetting s){this->_onFramesPerMeasurementChange(s);}
    );
    _running = false;
}

Reader::~Reader(){
    ::close(_fd);
    auto settings = Settings::Instance();
    settings->removeChangeCallback("frames_per_measurement", _frames_per_measurement);
}

bool Reader::start(){
    _t = std::thread([this]{readerLoop();});
}

bool Reader::stop(){
    _running = false;
    _t.join();
}

bool Reader::readFrame(opq::data::OPQCycle &frame){
#ifdef OPQ_DEBUG
    for(int i = 0; i< data::SAMPLES_PER_CYCLE; i++) {
        frame.data[i] = 16384*sin(2*M_PI*i/SAMPLES_PER_CYCLE) + rand()%200 - 100;
    }
    std::this_thread::sleep_for(std::chrono::microseconds(1000000/60));
    return true;
#else
    if(::read(_fd, &frame, sizeof(data::OPQCycle)) != sizeof(data::OPQCycle)) return false;
        return true;
#endif

}

void Reader::readerLoop(){
    _running = true;
    while(_running){
        int current_frame = 0;
        auto measurement = make_measurement();
        while(current_frame < _frames_per_measurement){
            measurement->cycles.push_back(OPQCycle());
            readFrame(measurement->cycles[current_frame]);
            measurement->timestamps.push_back(std::chrono::high_resolution_clock::now());
            current_frame++;
        }
        _q->push(measurement);
    }
}

void Reader::_onFramesPerMeasurementChange(OPQSetting s) {
    _frames_per_measurement = boost::get<int>(s);
}
