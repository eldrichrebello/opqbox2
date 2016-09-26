#ifndef ACQUISITION_UTILL_HPP
#define ACQUISITION_UTILL_HPP
#include <string>
#include "opq.pb.h"
#include "opqdata.hpp"
#include <chrono>

using std::string;

namespace opq{
    /**
     * OPQ Acqisition utility functions.
     */
    namespace util{

        /**
         * @brief Create a millisecond timestamp from a chrono::time_point.
         * @param time timestamp to convert.
         * @return millisecond timestamp.
         */
        inline uint64_t crono_to_mili(std::chrono::time_point<std::chrono::high_resolution_clock > time){
            std::chrono::time_point<std::chrono::high_resolution_clock > epoch;
            auto elapsed = time - epoch;
            return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        }

        /**
         * @brief serialize data::OPQAnalysisPtr using google's protobuf protocol.
         * @param id id of the client device.
         * @param message analysis result ready to be transmitted.
         * @return string containing a serialized data::OPQAnalysisPtr.
         */
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

        /**
         * @brief serialize data::OPQMeasurementPtr using google's protobuf protocol.
         * @param id id of the client device.
         * @param message analysis result ready to be transmitted.
         * @return string containing a serialized data::OPQMeasurementPtr.
         */
        inline string serialize_to_protobuf(int id, data::OPQMeasurementPtr message){
            proto::DataMesaage protomessage;
            protomessage.set_id(id);
            for (size_t mnum = 0; mnum< message->cycles.size();mnum++){
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
