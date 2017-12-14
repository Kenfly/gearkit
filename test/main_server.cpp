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
#include "List.h"
#include "Map.h"
#include "Array.h"

#include <unistd.h>
#include <limits.h>
#include "QList.h"


void test(int a[])
{
    printf("%d\n", sizeof(a));
}

void test_hl_vec()
{
    g_RefPool->push(new kit::RefPool());
    const int test_cnt = 100000;
    const int run_cnt = 100000000;
    int index;

    std::vector<int> vec;
    KIT_BEGIN(vector);
    for(int i = 1; i != test_cnt; ++i)
    {
        vec.push_back(i);
    }
    KIT_RUN(run_cnt);
    KIT_END(vector, "[vector] init cost:");

    KIT_BEGIN(vector_list);
    std::vector<int>::iterator ix = vec.begin();
    while (ix != vec.end())
    {
        int a = *ix;
        ++ix;
    }
    KIT_RUN(run_cnt);
    KIT_END(vector_list, "[vector] list cost:");

    KIT_BEGIN(vector_index);
    for (int i = 1; i != test_cnt; ++i)
    {
        int p = vec[i];
    }
    KIT_RUN(run_cnt);
    KIT_END(vector_index, "[vector] index cost:");

    printf("\n");

    // unorder map
    std::unordered_map<int, int> umap;
    KIT_BEGIN(unordered_map_init);
    for (int i = 1; i != test_cnt; ++i)
    {
        umap[i] = i;
    }
    KIT_RUN(run_cnt);
    KIT_END(unordered_map_init, "[unorder_map] init cost:");

    KIT_BEGIN(unordered_map_list);
    for (std::unordered_map<int, int>::iterator ix = umap.begin(); ix != umap.end(); ++ix)
    {
        int pp = ix->second;
    }
    KIT_RUN(run_cnt);
    KIT_END(unordered_map_list, "[unorder_map] list cost");

    KIT_BEGIN(map_index);
    for (int i = 1; i != test_cnt; ++i)
    {
        int pp = umap[i];
    }
    KIT_RUN(run_cnt);
    KIT_END(map_index, "[unorder_map] index cost:");

    printf("\n");
    kit::List<int> mylist;
    KIT_BEGIN(LIST_init);
    for (int i = 1; i != test_cnt; ++i)
    {
        mylist.push(i);
    }
    KIT_RUN(run_cnt);
    KIT_END(LIST_init, "[LIST] init cost:");

    KIT_BEGIN(LIST_list2);
    for (kit::List<int>::Iterator ix = mylist.begin(); ix != mylist.end(); ++ix)
    {
        int pp = *ix;
    }
    KIT_RUN(run_cnt);
    KIT_END(LIST_list2, "[LIST] list2 cost:");

    printf("\n");

    kit::Map<int, int, 512> mymap;
    KIT_BEGIN(map_init);
    for (int i = 1; i != test_cnt; ++i)
    {
        mymap.set(i, i);
    }
    KIT_RUN(run_cnt);
    KIT_END(map_init, "[map] init cost:");

    int pppp = 0;
    KIT_BEGIN(map_list);
    for (int i = 1; i != test_cnt; ++i)
    {
        mymap.get(i, pppp);
    }
    KIT_RUN(run_cnt);
    KIT_END(map_list, "[map] list1 cost:");
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
}

class AT : public kit::Ref
{
public:
};

void test_server()
{
    kit::Server* server1 = kit::Server::create();
    kit::Server* server2 = kit::Server::create();

    server1->startup("127.0.0.1", 3333);

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
    typedef kit::Array<int, 512> MyArray;
    MyArray array;
    array.set(4, 4);
    array.set(511, 511);
    array.set(33, 33);

    int ggg;
    array.get(33, ggg);
    printf("get::%d\n", ggg);
    array.del(33);
    for(MyArray::Iterator ix = array.begin(); ix != array.end(); ++ix)
    {
        printf("%d\n", *ix);
    }
    printf("count:%d\n", array.count());
}

void test_map()
{
    kit::Map<int, int, 10> map;
    map.set(3, 3);
    map.set(9, 9);
    map.set(13, 13);
    map.set(14, 14);

    int a = 111;
    bool ret = false;
    ret = map.get(3, a);
    printf("ret:%d, %d\n", (int)ret, a);
    ret = map.get(14, a);
    printf("ret:%d, %d\n", (int)ret, a);
}

template<typename A, typename B>
void test_typename(A a, B b)
{
    printf("%d\n", a % b);
}


void test_pqueue()
{
    kit::MaxQueue<int, 100> que;
    for (int i = 1; i != 10; ++i)
    {
        que.push(i);
    }

    printf("\n");
    printf("cnt:%d\n", que.count());
    int a;
    que.pop(a);
    printf(":%d\n", a);
    que.pop(a);
    printf(":%d\n", a);
    que.pop(a);
    printf(":%d\n", a);
    que.pop(a);
    printf(":%d\n", a);
    que.pop(a);
    printf(":%d\n", a);
}

struct aaa
{
    union {
        uint8_t a[4];
        uint32_t b;
    };
};

void test_qlist()
{
    kit::QList<int, 200> que;
    for (int i = 1; i != 100; ++i)
        que.push(i);
    int o;
    bool ret;
    for (int i = 1; i <= 200; ++i)
    {
        ret = que.pop(o);
        if (ret)
            printf("%d\n", o);
    }
}

int main(int argc, char* argv[])
{
    //test_que();
    //test_array();
    //test_hl_vec();
    //test_map();
    //test_typename(33, 22);
    //test_pqueue();
    //test_qlist();
    
    start_server();
    return 0;
}

