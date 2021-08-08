#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <string>
#include <chrono>
#include <ctime>
#include <thread>
#include <ratio>

using namespace std;

int run_publisher() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        cout << "socket not created" << endl;
        return -1;
    }

    //create hint structure for the server we are connecting
    int port = 54000; 
    string ipaddress = "127.0.0.1";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipaddress.c_str(), &hint.sin_addr);

    while (true)
    {
        string s;

        cout << "Enter message: " << flush;
        getline(cin, s);

        for (int i = 0; i < 10; i++)
        {
            this_thread::sleep_for(chrono::seconds(1));
            long int drn= chrono::duration_cast<chrono::microseconds> (
                            chrono::steady_clock::now().time_since_epoch()).count();
            string time1 = to_string(drn);
            string serial = ":";
            string interm = time1.append(serial);
            string sendstr = interm.append(s);
            int bytes = sendto(sock, sendstr.c_str(), sendstr.size() + 1, 0, (sockaddr *)&hint, sizeof(hint));
            cout << "Published " << bytes << " bytes on port 54000 " << endl;
        }
    }

    close(sock);
    return 0;
}

int main() {
    return run_publisher();
}

