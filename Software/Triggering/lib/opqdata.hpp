#ifndef ACQUISITION_OPQDATA_H
#define ACQUISITION_OPQDATA_H

#include <memory>
#include <string>
#include <iostream>
#include <unistd.h>
#include <cmath>
#include "SyncQueue.hpp"


namespace opq {

    /**
     * Common data types.
     */
    namespace data {

        ///Grid cycles per second
        const static int CYCLES_PER_SEC = 60;
        ///Sampling rate per cycle
        const static int SAMPLES_PER_CYCLE = 200;
        const static int HISTOGRAM_BINS = 100;

        /** @struct OPQCycle
        *  @brief This structure represents an AC cycle
        *  @var OPQCycle::data
        *  Contains 16 bit signed ADC samples
        *  @var OPQCycle::zero_crossing_high
        *  Reserved
        *  @var OPQCycle::zero_crossing_low
        *  Reserved
        */

        typedef struct OPQCycle{
            int16_t data[SAMPLES_PER_CYCLE];
            int32_t zero_crossing_high;
            int32_t zero_crossing_low;
        } __attribute__((packed)) OPQCycle;

        inline bool readCycle(int fd, opq::data::OPQCycle &cycle){
#ifdef OPQ_DEBUG
            for(int i = 0; i< data::SAMPLES_PER_CYCLE; i++) {
                cycle.data[i] = 16384*sin(2*M_PI*i/SAMPLES_PER_CYCLE) + rand()%200 - 100;
            }
            std::this_thread::sleep_for(std::chrono::microseconds(1000000/60));
            return true;
#else
            if(::read(fd, &cycle, sizeof(data::OPQCycle)) != sizeof(data::OPQCycle)) return false;
            return true;
#endif
        }

        ///@brief A collection of cycles and their timestamps.
        typedef struct {
            ///Contains measurememtns for several cycles.
            std::vector<OPQCycle> cycles;
            ///Contains timestamps for each measurement.
            std::vector<std::chrono::time_point<std::chrono::high_resolution_clock> > timestamps;
        } OPQMeasurement;

        ///@brief Smartpointer for OPQMeasurement.
        typedef std::shared_ptr<OPQMeasurement> OPQMeasurementPtr;

        /**
         * @brief Creates an empty OPQMeasurementPtr.
         * @return empty OPQMeasurementPtr.
         */
        inline OPQMeasurementPtr make_measurement() {
            return std::make_shared<OPQMeasurement>();
        }

        ///@brief A sycronized queue of OPQMeasurementPtr wrapped in a smart pointer.
        typedef std::shared_ptr<SyncQueue<OPQMeasurementPtr> > MeasurementQueue;

        /**
         * @brief Creates an empty MeasurementQueue.
         * @return MeasurementQueue.
         */
        inline MeasurementQueue make_measurement_queue() {
            return std::make_shared<SyncQueue<OPQMeasurementPtr> >();
        }

        ///@brief Result of the local analysis thread.
        typedef struct {
            ///Computed RMS.
            float RMS;
            ///Computed frequency.
            float frequency;
            uint16_t hist[HISTOGRAM_BINS];
            ///UNIX timestamp of the first opqcycle rtransfer.
            std::chrono::time_point<std::chrono::high_resolution_clock> start;
        } OPQAnalysis;

        ///@brief Smart pointer of OPQAnalysis.
        typedef std::shared_ptr<OPQAnalysis> OPQAnalysisPtr;

        /**
         * @brief Create an new OPQAnalysisPtr.
         * @return OPQAnalysisPtr.
         */
        inline OPQAnalysisPtr make_analysis() {
            return std::make_shared<OPQAnalysis>();
        }

        ///A sycronized queue of OPQAnalysisPtr wrapped in a smart pointer.
        typedef std::shared_ptr<SyncQueue<OPQAnalysisPtr> > AnalysisQueue;

        /**
         * @brief Create a empty AnalysisQueue.
         * @return AnalysisQueue.
         */
        inline AnalysisQueue make_analysis_queue() {
            return std::make_shared<SyncQueue<OPQAnalysisPtr> >();
        }


    }
}

#endif //ACQUISITION_OPQDATA_H
