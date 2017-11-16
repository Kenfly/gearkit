#include <cstdio>
#include <vector>
#include <unordered_map>
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <sys/times.h>
#include <sys/time.h>
#include "gearkit.h"
#include "app_server.h"
#include "debugfunc.h"
#include "Queue.h"

#include <unistd.h>


void test(int a[])
{
    printf("%d\n", sizeof(a));
}

void test_hl_vec()
{
    const int test_cnt = 100000;
    const int run_cnt = 100000000;
    kit::Ref* p = kit::Socket::create();
    int index;
    kit::HashListDef hl[test_cnt];
    kit::HashList_init(hl, test_cnt);
    std::vector<void*> vec;

    KIT_BEGIN(hash_list);
    for(int i = 1; i != test_cnt; ++i)
    {
        kit::HashList_addElement(hl, i, p);
    }

    KIT_RUN(run_cnt);
    KIT_END(hash_list, "[HashList] init cost:");

    KIT_BEGIN(hash_list_list);
    index = 0;
    while (index = kit::HashList_next(hl, index))
    {
        void* pp = kit::HashList_getElement(hl, index);
    }
    KIT_RUN(run_cnt);

    KIT_END(hash_list_list, "[HashList] list cost:");

    KIT_BEGIN(hash_index);
    for (int i = 1; i != test_cnt; ++i)
    {
        void* pp = kit::HashList_getElement(hl, i);
    }
    KIT_RUN(run_cnt);
    KIT_END(hash_index, "[HashList] index cost:");

    printf("\n");

    KIT_BEGIN(vector);
    for(int i = 1; i != test_cnt; ++i)
    {
        vec.push_back(p);
    }
    KIT_RUN(run_cnt);
    KIT_END(vector, "[vector] init cost:");

    KIT_BEGIN(vector_list);
    std::vector<void*>::iterator ix = vec.begin();
    while (ix != vec.end())
    {
        void* pp = *ix;
        ++ix;
    }
    KIT_RUN(run_cnt);
    KIT_END(vector_list, "[vector] list cost:");

    KIT_BEGIN(vector_index);
    for (int i = 1; i != test_cnt; ++i)
    {
        void* pp = vec[i];
    }
    KIT_RUN(run_cnt);
    KIT_END(vector_index, "[vector] index cost:");

    printf("\n");

    // unorder map
    std::unordered_map<int, void*> umap;
    KIT_BEGIN(unordered_map_init);
    for (int i = 1; i != test_cnt; ++i)
    {
        umap[i] = p;
    }
    KIT_RUN(run_cnt);
    KIT_END(unordered_map_init, "[unorder_map] init cost:");

    KIT_BEGIN(unordered_map_list);
    for (std::unordered_map<int, void*>::iterator ix = umap.begin(); ix != umap.end(); ++ix)
    {
        void* pp = ix->second;
    }
    KIT_RUN(run_cnt);
    KIT_END(unordered_map_list, "[unorder_map] list cost");

    KIT_BEGIN(map_index);
    for (int i = 1; i != test_cnt; ++i)
    {
        void* pp = umap[i];
    }
    KIT_RUN(run_cnt);
    KIT_END(map_index, "[unorder_map] index cost:");

    printf("ref cnt:%d\n", p->getReferenceCount());
}

void test_tick()
{
    const int TEST_CNT = 0x7FFFFF;

    KIT_BEGIN(tick);
    for (int i = 0; i < TEST_CNT; ++i)
    {
        //KIT_TICK(A);
    }
    KIT_END(tick, "tick:");

    KIT_BEGIN(msec);
    for (int i = 0; i < TEST_CNT; ++i)
    {
        kit::time();
    }
    KIT_END(msec, "msec:");


    struct timespec ts;
    KIT_BEGIN(time);
    for (int i = 0; i < TEST_CNT; ++i)
    {
        clock_gettime(CLOCK_REALTIME, &ts);
    }
    KIT_END(time, "time:");


    KIT_BEGIN(clock);
    for (int i = 0; i < TEST_CNT; ++i)
    {
        clock();
    }
    KIT_END(clock, "clock:");

    struct timeval tv;
    struct timezone tz;
    KIT_BEGIN(gettimeofday);
    for (int i = 0; i < TEST_CNT; ++i)
    {
        gettimeofday(&tv, &tz);
    }
    KIT_END(gettimeofday, "gettimeofday:");

    KIT_BEGIN(times);
    for (int i = 0; i < TEST_CNT; ++i)
    {
        times(NULL);
    }
    KIT_END(times, "times:");

}

void test_hash_list()
{
    kit::HashListDef a[100];
    HashList_init(a, 100);

    kit::Ref* p = (kit::Ref*)1;

    kit::HashList_addElement(a, 1, p);
    kit::HashList_addElement(a, 2, p);
    kit::HashList_addElement(a, 88, p);
    kit::HashList_addElement(a, 4, p);
    kit::HashList_delElement(a, 2);
    kit::HashList_addElement(a, 2, p);

    kit::Server* cc = (kit::Server*)kit::HashList_getElement(a, 1);
    kit::HashList_print(a);
}

class AT : public kit::Ref
{
public:
};

void test_server()
{
    kit::Server* server1 = kit::Server::create();
    kit::Server* server2 = kit::Server::create();

    server1->setTimeval(2000);
    server1->startup("127.0.0.1", 3333);

    server2->setTimeval(2000);
    server2->startup("127.0.0.1", 3334);

    sleep(10);
    server1->shutdown();
    server2->shutdown();
}

#include <sys/epoll.h>

void test_que()
{
    typedef struct epoll_event Event;
    kit::Queue<struct epoll_event, 100> que;
    struct epoll_event event;
    for (int i = 0; i != 1000; ++i)
    {
        event.data.fd = i;
        bool ret = que.push(event);
        printf("push:%d\n", int(ret));
    }
    
    for (int i = 0; i != 1000; ++i)
    {
        if (que.pop(event))
        {
            LOG("pop::%d\n", event.data.fd);
        }
    }
    LOG("count:%d\n", que.count());
}

void start_server()
{
    App app;
    app.run();
}

void test_array()
{
    const int size = 10;
    int a[size];
    int* p = a;
    for (int i = 0; i != size; ++i)
    {
        a[i] = i;
    }

    memcpy(p, p + 2, sizeof(int) * 2);

    for (int i = 0; i != size; ++i)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}

int main(int argc, char* argv[])
{
    //test_que();
    //test_array();
    start_server();
    return 0;
}

