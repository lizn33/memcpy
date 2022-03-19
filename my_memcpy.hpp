//c++11
namespace my
{
    using my_size_t = unsigned long int;
    void *memcpy(void *dest, void *src, my_size_t size)
    {   
        //处理非正常输入
        if(dest == nullptr || src == nullptr || dest == src || size <= 0)
            return nullptr;

        //如果size过大（如size = 10000000000），超出内存区域，
        //不需多做处理，此时编译器会抛出异常，函数外添加catch捕获即可
        
        my_size_t count;
        char *char_dest = reinterpret_cast<char *>(dest);
        char *char_src = reinterpret_cast<char *>(src);

        //可以使用long long int增加移动量, 为避免频繁访问类型长度消耗时间
        //使用无符号短整型记录其长度
        unsigned short int lli_len = sizeof(long long int);

        count = size;
        //剩余的无法用long long int移动的部分的数量
        my_size_t mod = count % lli_len;

        //可以移动的部分的数量
        count = count / lli_len;

        //从后往前移动
        if(char_src < char_dest && char_dest <= char_src + size)
        { 
            char *char_src_end = char_src + size - sizeof(char);
            char *char_dest_end = char_dest + size - sizeof(char);
            //count为0，则不必花费资源创建多余变量
            if(count)
            {
                //寄存器存放指针，加速，效果不明显
                register long long int *lli_dest_end = reinterpret_cast<long long int *>(char_dest + size - lli_len);
                register long long int *lli_src_end = reinterpret_cast<long long int *>(char_src + size - lli_len);
                //可以使用cpu流水线处理思维，一次处理多个，效果不明显，不使用
                // while(count > 2)
                // {
                //     *lli_dest_end-- = *lli_src_end--;
                //     *lli_dest_end-- = *lli_src_end--;
                //     *lli_dest_end-- = *lli_src_end--;
                //     count -= 3;
                // }
                while(count--)
                {
                    *lli_dest_end-- = *lli_src_end--;
                }
                char_src_end = reinterpret_cast<char *>(++lli_src_end) - sizeof(char);
                char_dest_end = reinterpret_cast<char *>(++lli_dest_end) - sizeof(char);
            }
            while(mod--)
            {
                *char_dest_end = *char_src_end;
                --char_src_end;
                --char_dest_end;
            }
        }
        //从前往后移动
        else
        {
            char *char_src_beg = char_src;
            char *char_dest_beg = char_dest;
            if(count)
            {
                register long long int *lli_dest_beg = reinterpret_cast<long long int *>(char_dest);
                register long long int *lli_src_beg = reinterpret_cast<long long int *>(char_src);
                //可以使用cpu流水线处理思维，一次处理多个，效果不明显，不使用
                // while(count > 2)
                // {
                //     *lli_dest_beg++ = *lli_src_beg++;
                //     *lli_dest_beg++ = *lli_src_beg++;
                //     *lli_dest_beg++ = *lli_src_beg++;
                //     count -= 3;
                // }
                while(count--)
                {
                    *lli_dest_beg++ = *lli_src_beg++;
                }
                char_src_beg = reinterpret_cast<char *>(lli_src_beg);
                char_dest_beg = reinterpret_cast<char *>(lli_dest_beg);
            }
            while(mod--)
            {
                *char_dest_beg = *char_src_beg;
                ++char_src_beg;
                ++char_dest_beg;
            }
        }// if

        return dest;
    }// memcpy
    
} // namespace my

