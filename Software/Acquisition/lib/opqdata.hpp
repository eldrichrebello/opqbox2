#ifndef ACQUISITION_OPQDATA_H
#define ACQUISITION_OPQDATA_H

#include <memory>
#include <chrono>
#include <string>
#include <iostream>
#include <chrono>

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
        } __attribute__((packed)) OPQ_Frame;

        typedef struct{

            std::vector< OPQ_Frame> frames;
            std::vector< std::chrono::time_point<std::chrono::high_resolution_clock > > read_time;
        } OPQ_Measurement;

        typedef std::shared_ptr<OPQ_Measurement> OPQMeasurementPtr;

        inline OPQMeasurementPtr make_measurement(){
            return std::make_shared< OPQ_Measurement >();
        }

        typedef std::shared_ptr< SyncQueue <OPQMeasurementPtr> > MeasurementQueue;

        inline MeasurementQueue make_measurement_queue(){
            return std::make_shared< SyncQueue <OPQMeasurementPtr> >();
        }

        typedef struct {
            float RMS;
            float frequency;
            uint16_t hist[HISTOGRAM_BINS];
            std::chrono::time_point<std::chrono::high_resolution_clock > start;
        }OPQ_Analysis;

        typedef std::shared_ptr<OPQ_Analysis> OPQAnalysisPtr;

        inline OPQAnalysisPtr make_analysis(){
            return std::make_shared< OPQ_Analysis >();
        }

        typedef std::shared_ptr< SyncQueue <OPQAnalysisPtr> > AnalysisQueue;

        inline AnalysisQueue make_analysis_queue(){
            return std::make_shared< SyncQueue <OPQAnalysisPtr> >();
        }

        inline std::string serialize_analysis_trigger_redis(OPQAnalysisPtr a){

            std::chrono::time_point<std::chrono::high_resolution_clock > epoch;
            auto elapsed = a->start - epoch;
            uint64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
            std::string ret = std::to_string(timestamp) +
                    ":" + std::to_string(a->RMS) +
                    ":" + std::to_string(a->frequency);
            return ret;
        }

    }
}

#endif //ACQUISITION_OPQDATA_H
