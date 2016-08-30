#include <iostream>
#include "Reader.hpp"
#include "Settings.hpp"
#include "LocalAnalysis.hpp"
#include "RedisTrigger.hpp"
using namespace std;

int main() {

    auto settings = opq::Settings::Instance();
    settings->loadFromFile("settings.set");

    settings->saveToFile("set1.set");
    auto readerQueue = opq::data::make_measurement_queue();
    auto analysisQueue = opq::data::make_analysis_queue();
    opq::Reader r(readerQueue);
    opq::LocalAnalysis l(readerQueue, analysisQueue);
    opq::RedisTrigger t(analysisQueue);
    l.start();
    r.start();
    t.start();
    while(true){
        sleep(1);
    }
    return 0;
}