#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <sstream>
#include <chrono>

// Общие ресурсы
std::mutex m;
std::condition_variable cv;
int data = 0;           // общий буфер (1 элемент)
bool ready = false;     
bool done = false;      

// Лог
std::ofstream logfile("output.log", std::ios::app);

// Производитель
void producer() {
    for (int i = 1; i <= 10; ++i) {
        {
            std::lock_guard<std::mutex> lock(m);
            data = i;
            ready = true;
        }
        cv.notify_one();    
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    {
        std::lock_guard<std::mutex> lock(m);
        done = true;        
    }
    cv.notify_one();        
}

// Потребитель
void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [] { return ready || done; });

        if (ready) {
            std::ostringstream oss;
            oss << "Получено: " << data;
            std::cout << oss.str() << "\n";
            logfile << oss.str() << "\n";
            logfile.flush();
            ready = false;
        }

        if (done) break;
    }
}

int main() {
    std::thread p(producer);
    std::thread c(consumer);

    p.join();
    c.join();

    std::cout << "Программа завершена\n";
    return 0;
}
