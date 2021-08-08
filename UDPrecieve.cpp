//UDP reciever
//recieve message -> first divides time stamp , construct time
//latency, how much to time to recieve message

//blocking vs non blocking / asynchronus calls
//confiure debugger

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstring>

using namespace std;

int run_receiver() {
    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketfd <= 0) {
        cout << "port not opened" << endl;
        return -1;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    u_int opt = 1;
    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, 
                    (char*) &opt, sizeof(opt)) < 0){
       perror("Reusing ADDR");
       return 1;
    }
    cout << "Setting socket reuseable ... Ok" << endl;


    if (bind(socketfd, (sockaddr *)&hint, sizeof(hint)) == -1) {
        cout << "cannot bind to  IP/PORT";
        return -2;
    }
    cout << "Binding socket to port 54000 ... Ok" << endl;

    sockaddr_in client;
    socklen_t clientsize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    char buf[4096];
    int bytes;
    string userinput;

    cout << "Waiting for udp data..." << endl;
    while (true) {
        int bytesin = recvfrom(socketfd, buf, 2048, 0, (sockaddr *)&client, &clientsize);
        if (bytesin == errno) {
            cout << "error recieving from client" << endl;
            continue;
        }

        long int endtime = chrono::duration_cast<chrono::microseconds>(
            chrono::steady_clock::now().time_since_epoch()).count();
        char clientIP[256];
        inet_ntop(AF_INET, &client.sin_addr, clientIP, 256);

        string recieve(buf);
        string ticks;
        string quote;

        for (int i = 0; recieve[i] != '\0'; i++) {
            if (recieve[i] == ':') {
                ticks = recieve.substr(0, i);
                quote = recieve.substr(i + 1, recieve.length() - 1);
                break;
            } else {
                continue;
            }
        }

        long int totaltime = endtime - stol(ticks, nullptr, 10);
        cout << "Time to recieve message: " << totaltime 
             << ", text: " << quote << endl
             << endl;
    }
    return 0;
}

int main() {
    run_receiver();
}
