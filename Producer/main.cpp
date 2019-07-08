#include <cppkafka/cppkafka.h>
#include <iostream>
#include <ctime>
#include <string>
#include <chrono>

using namespace std;
using namespace cppkafka;
using namespace std::chrono;

uint64_t getMs() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}


int main() {

    // Create the config
    Configuration config = {
            { "metadata.broker.list", "127.0.0.1:9092" },
            {"linger.ms",2},
    };

    // Create the producer
    Producer producer(config);

    // Produce a message!
    string message = "STOP";
    string test = "test";
    long int now1 = getMs();
    int msgAmount = 100000;

    for(int i=0; i<msgAmount; i++){
        long int now = getMs();
        string time = to_string(now);
        producer.produce(MessageBuilder("test").partition(0).payload(time));
//        if(i%(msgAmount/300)==0){
//            producer.flush();
//        }
//        cout.flush();
//        cout << "<sending message:"<<now<<" counter:"<<i<<"\n";
    }

    producer.produce(MessageBuilder("test").partition(0).payload(message));
    producer.flush();
    long int now2 = getMs();

    cout <<"Sent "<<msgAmount<<" messages in:"<< float(now2-now1)/1000<<"s"<<"  TS: "<<now2;

}