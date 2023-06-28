#include <signal.h>
#include <iostream>

#include "OM.hpp"

int main(int argc, char** argv)
{
    std::cout << "OM is running with " <<  argc  << " arguments printed below:\n";
    for(int i = 0; i < argc; ++i) {
        std::cout << argv[i] << ", ";
    }
    std::cout << "\n";

    om::CreateTCPIPServer((uint16_t)2014);

    //while(true)
    {
        int sig;
        sigset_t signal_set;
        sigemptyset(&signal_set);
        sigaddset(&signal_set, SIGABRT);
        sigaddset(&signal_set, SIGTERM);
        sigaddset(&signal_set, SIGKILL);
        sigwait( &signal_set, &sig);
        //FIXME:: Handle signal
        sleep(10);
    }
    return 0;
}
