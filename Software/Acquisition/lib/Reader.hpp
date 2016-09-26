//
// Created by tusk on 6/27/16.
//
#ifndef ACQUISITION_READER_H
#define ACQUISITION_READER_H
#include "opqdata.hpp"
#include "Settings.hpp"

namespace opq {

    /**
     * @brief This class is responsible for data acquisition.
     * Reader class communicates with the opq kernel driver and transfers ADC samples into userland.
     */
    class Reader {
    public:
        /**
         * @brief Creates a new Reader and set it to output OPQCycle into the queue q.
         * @param q output queue.
         */
        Reader(opq::data::MeasurementQueue &q);
        /**
         * @brief  Close the device and release the queue.
         */
        ~Reader();
        /**
         * @brief  Start the readout thread.
         */
        void start();
        /**
         * @brief Stop the readout thread.
         */
        void stop();
    private:
        void _onFramesPerMeasurementChange(OPQSetting s);

        int _frames_per_measurement;
        int _fpm_callback_id;

        void readerLoop();
        bool readFrame(opq::data::OPQCycle &frame);
        opq::data::MeasurementQueue _q;
        int _fd;
        std::thread _t;
        bool _running;
    };

}

#endif //ACQUISITION_READER_H
