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

    printf("input:\n");
    for (int i = 1; i != 10; ++i)
    {
        AA item;
        item.a = i;
        item.b = i;
        array.add(i + 1, item);
        printf("%d %d\n", item.a, item.b);
    }
    printf("output:\n");
    array.clear();
    for (int i = 7; i != 10; ++i)
    {
        AA item;
        item.a = i;
        item.b = i;
        array.add(i + 1, item);
    }
    AA item;
    for (array.resetNext(); array.next(item);)
    {
        printf("%d %d\n", item.a, item.b);
    }
}

int main()
{
    App app;
    //app.run();
    test_array();

    return 0;
}
