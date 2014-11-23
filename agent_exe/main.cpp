
#include <cacoon/agent.h>
#include <cacoon/transport_file.h>

#include <iosfwd>

using cacoon::agent;
using cacoon::transport::file;

int main(const int argc, const char **argv) {
    agent<file> agent(42);
    agent.start();

    int x;
    while(agent.is_running()) {
        std::cin >> x;
        if(x == 0) {
            agent.stop();
        }
    }

    return 0;
}
