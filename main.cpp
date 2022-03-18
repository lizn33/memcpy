#include <cstring>
#include <iostream>
#include <random>
#include <time.h>
#include "my_memcpy.hpp"
using std::cout;
using std::endl;
using std::memcpy;
int main()
{
    //经过实验，现在g++/msvc编译器的memcpy函数处理有交叉区域的内存时
    //都不会错误覆盖，即达到了memmove的纠错效果。
    //所以以下示例代码以标准库的memcpy函数的处理结果为正确标准, 处理
    //结束后，比较结果，查看是否一致, 比较方式为strcmp函数

    //本函数若输入错误会返回空值或抛出异常

    //两个字串，标准库处理str，自己的函数处理back
    char str[1024];
    char back[1024];
    size_t len = 500;

    //随机数填充字符串
    for(int i = 0; i < len; i++)
    {
        str[i] = rand() % 10 + '0';
    }
    str[2 * len] = '\0';

    //使两个待处理函数的内容一致
    memcpy(&back[0], &str[0], 1024);
    cout << strcmp(str, back) << endl;
    //cout << str << endl << endl;
    bool flag;

    //第一种情况，src在低位，dest在高位，两块区域不相交
    memcpy(&str[100], &str[0], 50);
    my::memcpy(&back[100], &back[0], 50);
    cout << "情况一，处理正确则输出0：" << strcmp(str, back) << endl;

    //第二种情况，src在低位，dest在高位，两块区域相交
    //
    //此情况网络资料认为memcpy会出现问题，但实际上编译器的实现中纠正了错误
    memcpy(&str[100], &str[0], 150);
    my::memcpy(&back[100], &back[0], 150);
    cout << "情况二，处理正确则输出0：" << strcmp(str, back) << endl;

    
    //第三种情况，src在高位，dest在低位，两块区域相交
    memcpy(&str[0], &str[100], 150);
    my::memcpy(&back[0], &back[100], 150);
    cout << "情况三，处理正确则输出0：" << strcmp(str, back) << endl;

    //第四种情况，src在高位，dest在低位，两块区域不相交
    memcpy(&str[0], &str[100], 50);
    my::memcpy(&back[0], &back[100], 50);
    cout << "情况四，处理正确则输出0：" << strcmp(str, back) << endl;

    //第五种情况：函数返回值检测
    memcpy(&str[0], &str[100], 50);
    flag = (my::memcpy(&back[0], &back[100], 50) == &back[0]);
    cout << "情况五，处理正确则输出1：" << flag << endl;

    //第六种情况：异常输入
    flag = (my::memcpy(nullptr, &back[100], 50) == nullptr);
    cout << "情况六，处理正确则输出1：" << flag << endl;

    //和标准库函数比较时间差异
    size_t long_str_len = 1000000;
    char *long_str_a = new char[long_str_len];
    char *long_str_b = new char[long_str_len];
    clock_t start, end;

    //标准库函数运行时长
    start = clock();
    memcpy(long_str_a, long_str_b, long_str_len);
    end = clock();
    cout << "标准库函数运行时长(s): " << std::fixed << (double)(end - start)/CLOCKS_PER_SEC << endl;

    //个人函数运行时长
    start = clock();
    my::memcpy(long_str_a, long_str_b, long_str_len);
    end = clock();
    cout << "个人函数运行时长(s): " << std::fixed << (double)(end - start)/CLOCKS_PER_SEC << endl;

    return 0;
}