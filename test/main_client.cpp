#include "app_client.h"
#include "List.h"
#include "Array.h"
#include "stdio.h"

struct AA
{
    int a;
    int b;
};

void test_list()
{
    kit::List<AA> list;
    for (int i = 1; i != 10; ++i)
    {
        AA item;
        item.a = i;
        item.b = i;
        bool ret = list.push(item);
        printf("%d\n", ret);
    }
}

void test_array()
{
    kit::Array<AA, 100> array;
}

void test_fun(int*& a)
{
    printf("%d\n", *a);
}

int main()
{
    App app;
    app.run();
    return 0;
}
