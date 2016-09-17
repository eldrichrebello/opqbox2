#include <iostream>
#include <ZMQTrigger.hpp>
#include "Reader.hpp"
#include "Settings.hpp"
#include "LocalAnalysis.hpp"
#include "RedisTrigger.hpp"

#include <csignal>

using namespace std;

namespace
{
    volatile std::sig_atomic_t gSignalStatus;
}

void bail_handler( int signum )
{
    cout << "Exiting" << endl;
    gSignalStatus = 0;
}

int main() {
    gSignalStatus = 1;
    auto settings = opq::Settings::Instance();
    settings->loadFromFile("settings.set");
    auto readerQueue = opq::data::make_measurement_queue();
    auto analysisQueue = opq::data::make_analysis_queue();
    opq::Reader r(readerQueue);
    opq::LocalAnalysis l(readerQueue, analysisQueue);
    opq::ZMQTrigger t(analysisQueue);
    t.start();
    l.start();
    r.start();
    std::signal(SIGINT, bail_handler);
    while(gSignalStatus != 0){
        sleep(1);
    }

    t.stop();
    l.stop();
    r.stop();
    exit(0);
    return 0;
}