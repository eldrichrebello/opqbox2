//
// Created by tusk on 6/27/16.
//
#ifndef ACQUISITION_READER_H
#define ACQUISITION_READER_H
#include "opqdata.hpp"
#include "Settings.hpp"

namespace opq {

    class Reader {
    public:
        Reader(opq::data::MeasurementQueue &q);
        ~Reader();
        bool start();
        bool stop();
    private:
        void _onFramesPerMeasurementChange(OPQSetting s);

        int _frames_per_measurement;
        int _fpm_callback_id;

        void readerLoop();
        bool readFrame(opq::data::OPQ_Frame &frame);
        opq::data::MeasurementQueue _q;
        int _fd;
        std::thread _t;
        bool _running;
    };

}

#endif //ACQUISITION_READER_H
