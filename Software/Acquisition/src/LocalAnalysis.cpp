//
// Created by tusk on 8/17/16.
//

#include "LocalAnalysis.hpp"
#include "opqdata.hpp"
#include "Settings.hpp"
#include <hiredis/hiredis.h>

#include <math.h>
#include <algorithm>
#include <iostream>
#include <opqdata.hpp>

using namespace opq;
using namespace opq::data;
using namespace std;

LocalAnalysis::LocalAnalysis(opq::data::MeasurementQueue inQ, opq::data::AnalysisQueue outQ) {
    _inQ = inQ;
    _outQ = outQ;
    _state = INITIALIZING_DOWNSAMPLING_FILTER;
    AntialiasDownsamplingFilter_init(&adf);
    LowPassFilter_init(&lpf);
}

void LocalAnalysis::start(){
    _t = std::thread([this]{readerLoop();});
}

bool LocalAnalysis::stop(){
    _running = false;
    _t.join();
}

void LocalAnalysis::readerLoop() {
    _running = true;
    auto settings = Settings::Instance();
    float calConstant = settings->getFloat("acquisition_calibration_constant");

    while(_running){
        opq::data::OPQMeasurementPtr m = _inQ->pop();
        if(_state == RUNNING){
            opq::data::OPQAnalysisPtr analysis = opq::data::make_analysis();
            _downSampled.clear();
            analysis->RMS = 0;
            for (auto&& frame : m->frames){
                analysis->RMS +=rmsVoltage(frame.data);

                for( int i = 0; i < SAMPLES_PER_CYCLE; i++) {
                    AntialiasDownsamplingFilter_put(&adf, frame.data[i]);
                    if(i% DECIMATION_FACTOR == 0){

                        float antialiased_value = AntialiasDownsamplingFilter_get(&adf);
                        LowPassFilter_put(&lpf, antialiased_value);
                        _downSampled.push_back(LowPassFilter_get(&lpf));
                    }
                }
            }
            analysis->start = m->read_time[0];
            analysis->RMS /= m->frames.size();
            analysis->RMS /= calConstant;

            std::vector<float> zeroCrossings;
            float last = FP_NAN;
            float next = 0;
            for(int i = 0; i< _downSampled.size(); i++) {
                if (last != FP_NAN) {
                    if (last < 0 && _downSampled[i] > 0) {
                        next = _downSampled[i];
                        zeroCrossings.push_back(1.0f*i+ (last)/(next-last));
                    }
                }
                last = _downSampled[i];
            }
            float accumulator  = 0;
            for(int i = 1; i< zeroCrossings.size(); i++){
                accumulator += zeroCrossings[i] - zeroCrossings[i-1];
            }
            accumulator /= zeroCrossings.size() - 1;
            analysis->frequency = SAMPLES_PER_CYCLE *
                    CYCLES_PER_SEC /
                    DECIMATION_FACTOR /
                    accumulator;
            _outQ->push(analysis);
            //Store sample in redis

        }
        else{
            initFilter(m);
        }
    }
}


float LocalAnalysis::rmsVoltage(int16_t data[]) {
    float av = 0;
    for( int i = 0; i < opq::data::SAMPLES_PER_CYCLE; i++) {
        av += 1.0*data[i]*data[i];
    }
    return sqrtf((float)av/opq::data::SAMPLES_PER_CYCLE);
}


void LocalAnalysis::initFilter(opq::data::OPQMeasurementPtr &m) {
    if(_state == INITIALIZING_DOWNSAMPLING_FILTER) {
        for (auto frame : m->frames) {
            for (int i = 0; i < opq::data::SAMPLES_PER_CYCLE; i++) {
                AntialiasDownsamplingFilter_put(&adf, frame.data[i]);
                _samplesProcessed++;
            }
        }
        if (_samplesProcessed > ANTIALIASDOWNSAMPLINGFILTER_TAP_NUM) {
            _state = INITIALIZING_LOWPASS_FILTER;
            _samplesProcessed = 0;
        }
    }
    else if(_state == INITIALIZING_LOWPASS_FILTER){
        for (auto frame : m->frames) {
            for (int i = 0; i < opq::data::SAMPLES_PER_CYCLE; i++) {
                AntialiasDownsamplingFilter_put(&adf, frame.data[i]);
                if(i% DECIMATION_FACTOR == 0){
                    LowPassFilter_put(&lpf, AntialiasDownsamplingFilter_get(&adf));
                    _samplesProcessed++;
                }
            }
        }
        if (_samplesProcessed > INITIALIZING_LOWPASS_FILTER) {
            _state = RUNNING;
            _samplesProcessed = 0;
        }
    }
}

void LocalAnalysis::calculateFrequency() {

}