#include <cppkafka/cppkafka.h>
#include <iostream>
#include <ctime>
#include <string>
#include <chrono>
#include <vector>


using namespace std;
using namespace cppkafka;
using namespace std::chrono;

uint64_t getMs() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void displayResults (vector <long> res){
    for(auto const& value: res) {
        cout << value <<endl;
    }
    cout<<endl<<"---------------------------"<<endl;
}


int main() {
    string topic = "test";
    vector <long> res;
    string stop = "STOP";

    Configuration config = {
            { "metadata.broker.list", "127.0.0.1:9092"},
            {"fetch.wait.max.ms" , 1},
            {"group.id", topic},
            {"fetch.wait.max.ms", 1},
            {"enable.auto.commit", false},
            {"auto.offset.reset", "latest"}
    };

            // Construct from some config we've defined somewhere
    Consumer consumer(config);

    consumer.subscribe({ topic });

    cout << "Consuming messages from topic " << topic << endl;

    auto ms = std::chrono::microseconds(10);

// Now loop forever polling for messages
    while (true) {
        // Try to consume a message

        Message msg = consumer.poll();
        if (msg) {

            if(msg.get_payload()==stop){
                displayResults(res);
            }
            // If we managed to get a message
            if (msg.get_error()) {
                // Ignore EOF notifications from rdkafka
                if (!msg.is_eof()) {
                    cout << "[+] Received error notification: " << msg.get_error() << endl;
                }
            }
            else {
                // Print the key (if any)
                if (msg.get_key()) {
                    //cout << msg.get_key() << " -> ";
                }
                // Print the payload
                res.push_back(getMs()-msg.get_timestamp().get().get_timestamp().count());
                //cout << msg.get_payload()<<" OFSET: "<< msg.get_offset() <<" TIME: "<<getMs() - msg.get_timestamp().get().get_timestamp().count()<< endl;
                // Now commit the message
                consumer.commit(msg);
            }
        }
    }

}

//msg.get_timestamp().get().get_timestamp().count()