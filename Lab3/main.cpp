#include <iostream>
#include <vector>
#include <thread>
#include <latch>
#include <syncstream>
#include <chrono>
#include <string>

const int count_a = 7;
const int count_b = 8;
const int count_c = 6;
const int count_d = 7;
const int count_e = 6;
const int count_f = 8;
const int count_g = 6;
const int count_h = 4;
const int count_i = 5;
const int count_j = 5;

std::latch done_a(1);
std::latch done_b(1);
std::latch done_c(1);
std::latch done_d(1);
std::latch done_f(1);
std::latch done_e(1);
std::latch done_g(1);

void f(char set_name, int action_number)
{
    std::osyncstream(std::cout) << "From set " << set_name << " done action " << action_number << "." << std::endl;
}

void run_task(char name, int count)
{
    for (int i = 1; i <= count; ++i) f(name, i);
}

// a -> d -> i
void worker1()
{
    run_task('a', count_a);
    done_a.count_down();

    run_task('d', count_d);
    done_d.count_down(); 

    done_f.wait();
    run_task('i', count_i);
}

// b -> f -> h
void worker2()
{
    run_task('b', count_b);
    done_b.count_down();

    done_c.wait();
    run_task('f', count_f);
    done_f.count_down(); 

    done_d.wait();
    run_task('h', count_h);
}

// c -> e
void worker3()
{
    run_task('c', count_c);
    done_c.count_down();

    done_a.wait();
    run_task('e', count_e);
    done_e.count_down();
}

// g -> j
void worker4()
{
    done_b.wait();
    done_c.wait();
    run_task('g', count_g);
    done_g.count_down();

    done_e.wait();
    run_task('j', count_j);
}

int main()
{ 
    std::cout << "The calculation has begun." << std::endl;

    {
        std::jthread t1(worker1);
        std::jthread t2(worker2);
        std::jthread t3(worker3);
        std::jthread t4(worker4);
    }

    std::cout << "The calculation has ended.";

    return 0;
}