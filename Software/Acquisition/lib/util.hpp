//
// Created by tusk on 8/18/16.
//

#ifndef ACQUISITION_UTILL_HPP
#define ACQUISITION_UTILL_HPP
#include <string>
#include "opq.pb.h"
#include "opqdata.hpp"
#include <chrono>

using std::string;

namespace opq{
    namespace util{

        inline uint64_t crono_to_mili(std::chrono::time_point<std::chrono::high_resolution_clock > time){
            std::chrono::time_point<std::chrono::high_resolution_clock > epoch;
            auto elapsed = time - epoch;
            return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        }


        inline string serialize_to_protobuf(int id, data::OPQAnalysisPtr message){
             proto::TriggerMessage protomessage;
             protomessage.set_id(id);
             protomessage.set_frequency(message->frequency);
             protomessage.set_rms(message->RMS);

             protomessage.set_time(crono_to_mili(message->start));

             std::string out;
             protomessage.SerializeToString(&out);
             return out;
         }

        inline string serialize_to_protobuf(int id, data::OPQMeasurementPtr message){
            proto::DataMesaage protomessage;
            protomessage.set_id(id);
            for (int mnum = 0; mnum< message->cycles.size();mnum++){
                auto &cycle = message->cycles[mnum];
                proto::Cycle *protocycle = protomessage.add_cycles();
                for(size_t i =0; i < data::SAMPLES_PER_CYCLE; i++){
                    protocycle->add_data(cycle.data[i]);
                }
                protocycle->set_time(crono_to_mili(message->timestamps[mnum]));
            }
            std::string out;
            protomessage.SerializeToString(&out);
            return out;
        }

    }
}
#endif //ACQUISITION_UTILL_HPP
