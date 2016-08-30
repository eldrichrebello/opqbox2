//
// Created by tusk on 8/17/16.
//

#ifndef ACQUISITION_METRICS_H
#define ACQUISITION_METRICS_H

#include <vector>
#include <stdint.h>
#include <thread>

#include "AntialiasDownsamplingFilter.hpp"
#include "LowPassFilter.hpp"
#include "opqdata.hpp"


namespace opq {
    class LocalAnalysis {
    public:
        LocalAnalysis(opq::data::MeasurementQueue  inq, opq::data::AnalysisQueue outq);
        void start();
        bool stop();
    private:
        enum LocalAnalysisState {
            INITIALIZING_DOWNSAMPLING_FILTER,
            INITIALIZING_LOWPASS_FILTER,
            RUNNING
        };

        LocalAnalysisState _state;


        void readerLoop();
        float rmsVoltage(int16_t data[]);

        static const uint16_t DECIMATION_FACTOR = 10;
        void initFilter(opq::data::OPQMeasurementPtr &m);

        uint32_t _samplesProcessed;
        AntialiasDownsamplingFilter adf;

        LowPassFilter lpf;

        std::vector<float> _downSampled;
        void calculateFrequency();

        opq::data::MeasurementQueue _inQ;
        opq::data::AnalysisQueue _outQ;
        bool _running;
        std::thread _t;


    };
}


#endif //ACQUISITION_METRICS_H
