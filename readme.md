安全性保证：
注意dest和src的地址，合理选择从前向后复制或从后向前复制
增加函数执行速度方式：
1、使用register关键字修饰频繁使用的变量（提速效果：不明显）
2、使用long long int指针来增加一次复制内存的量（提速效果：明显）
3、借助cpu处理时是流水线，一次复制多个内存（提速效果：不明显）