#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>
#include <cstdlib>
#include <ctime>

using std::cout;
using std::endl;
using std::string;
using std::vector;

std::mutex print_lock;

string get_time() {
    time_t now = time(nullptr);
    tm* t = localtime(&now);
    char buf[16];
    strftime(buf, sizeof(buf), "%H:%M:%S", t);
    return string(buf);
}

struct Train {
    int id;
    vector<string> route;
    int index;
    bool active;
};

int rnd(int a, int b) {
    return a + (std::rand() % (b - a + 1));
}

void worker(Train* t, vector<std::mutex>& station_mutexes)
{
    while (t->active)
    {
        string station = t->route[t->index];

        {
            std::lock_guard<std::mutex> lk(print_lock);
            cout << "[" << get_time() << "] Train "
                 << t->id << " MOVING TO " << station << endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(rnd(1000, 2000)));

        {
            std::lock_guard<std::mutex> station_guard(station_mutexes[t->index]);

            {
                std::lock_guard<std::mutex> lk(print_lock);
                cout << "[" << get_time() << "] Train "
                     << t->id << " ARRIVED at " << station << endl;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(rnd(1000, 2000)));

            {
                std::lock_guard<std::mutex> lk(print_lock);
                cout << "[" << get_time() << "] Train "
                     << t->id << " LEAVING " << station << endl;
            }
        }

        t->index = (t->index + 1) % t->route.size();
    }

    std::lock_guard<std::mutex> lk(print_lock);
    cout << "[" << get_time() << "] Train " << t->id << " STOPPED" << endl;
}

int main() {
    std::srand((unsigned)std::time(nullptr));

    vector<string> route = { "Dushanbe", "Gissar", "Regar", "Pakhtaabad" };

    vector<Train*> trains;
    vector<std::thread> threads;
    vector<std::mutex> station_mutexes(route.size());

    for (int i = 1; i <= 8; ++i) {
        Train* t = new Train();
        t->id = i;
        t->route = route;
        t->index = 0;
        t->active = true;
        trains.push_back(t);
    }

    for (auto* t : trains)
        threads.emplace_back(worker, t, std::ref(station_mutexes));

    cout << "PRESS ENTER TO STOP..." << endl;
    std::cin.get();

    for (auto* t : trains)
        t->active = false;

    for (auto& th : threads)
        if (th.joinable())
            th.join();

    cout << "Simulation finished!" << endl;

    for (auto* t : trains) delete t;
}
