#ifndef ACQUISITION_OPQDATA_H
#define ACQUISITION_OPQDATA_H

#include <memory>
#include <string>
#include <iostream>


#include "SyncQueue.hpp"

namespace opq{
    namespace data{
        const static int CYCLES_PER_SEC = 60;
        const static int SAMPLES_PER_CYCLE = 200;
        const static int HISTOGRAM_BINS = 100;
        typedef struct{
            int16_t data[SAMPLES_PER_CYCLE];
            int32_t zero_crossing_high;
            int32_t zero_crossing_low;
        } __attribute__((packed)) OPQCycle;

        typedef struct{

            std::vector<OPQCycle> cycles;
            std::vector< std::chrono::time_point<std::chrono::high_resolution_clock > > timestamps;
        } OPQMeasurement;

        typedef std::shared_ptr<OPQMeasurement> OPQMeasurementPtr;

        inline OPQMeasurementPtr make_measurement(){
            return std::make_shared<OPQMeasurement>();
        }

        typedef std::shared_ptr< SyncQueue <OPQMeasurementPtr> > MeasurementQueue;

        inline MeasurementQueue make_measurement_queue(){
            return std::make_shared< SyncQueue <OPQMeasurementPtr> >();
        }

        typedef struct {
            float RMS;
            float frequency;
            uint16_t hist[HISTOGRAM_BINS];
            std::vector< std::chrono::time_point<std::chrono::high_resolution_clock > > read_time;
            std::chrono::time_point<std::chrono::high_resolution_clock > start;
        } OPQAnalysis;

        typedef std::shared_ptr<OPQAnalysis> OPQAnalysisPtr;

        inline OPQAnalysisPtr make_analysis(){
            return std::make_shared<OPQAnalysis>();
        }

        typedef std::shared_ptr< SyncQueue <OPQAnalysisPtr> > AnalysisQueue;

        inline AnalysisQueue make_analysis_queue(){
            return std::make_shared< SyncQueue <OPQAnalysisPtr> >();
        }


    }
}

#endif //ACQUISITION_OPQDATA_H
