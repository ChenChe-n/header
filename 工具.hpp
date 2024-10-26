//工具.hpp
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include <bit>
#include <Windows.h>
#include <unordered_map>
#include <map>
#include <set>
#include <cassert>

typedef char                i8;
typedef unsigned char       u8;
typedef short               i16;
typedef unsigned short      u16;
typedef int                 i32;
typedef unsigned int        u32;
typedef long long           i64;
typedef unsigned long long  u64;

typedef float               f32;
typedef double              f64;


//将编辑器内部字符串使用UTF-8
#pragma execution_character_set("utf-8")


namespace 工具 {
    namespace 终端 {
        //设置控制台为UTF_8编码
        // bool 设置控制台为UTF_8编码();

        class 设置控制台为UTF_8编码 {
        public:
            设置控制台为UTF_8编码();
            设置控制台为UTF_8编码(const 设置控制台为UTF_8编码& other) = delete;
            设置控制台为UTF_8编码 operator=(const 设置控制台为UTF_8编码& other) = delete;
            static bool 是否初始化;
        };
        inline 设置控制台为UTF_8编码 我是傻逼;


        // 函数：显示或隐藏光标
        void ShowCursor(bool showFlag);

        // 函数：将光标移动到指定位置 (x, y)
        void GotoXY(int x, int y);
    }

    namespace 计算 {
        // 快速二进制取模
        inline u64 快速二进制取模函数(u64 源数据, u8 截取位数) {
            return 源数据 & ((1ULL << 截取位数) - 1);
        }

        // 获取最近 2 的幂指数
        inline u8 获取最近2的幂指数(u64 x) {
            if (x <= 1) return 0;
            return std::bit_width(x - 1);
        }

        // 向上取整为 2 的幂
        inline u64 向上取整为2的幂(u64 x) {
            if (x <= 1) return 1;
            x--;
            x |= x >> 1;
            x |= x >> 2;
            x |= x >> 4;
            x |= x >> 8;
            x |= x >> 16;
            x |= x >> 32;
            return x + 1;
        }
    }

    namespace 容器 {
        template <typename T>
        class 循环队列 {
        public:
            // 默认构造函数
            循环队列()
                : 容器容量(16), 容器位数(4), 队列头(0), 队列尾(0), 当前元素数量(0) {
                内部数据.resize(16, nullptr);
            }

            // 带初始队列的构造函数
            循环队列(const std::vector<T*>& 队列)
                : 内部数据(std::move(队列)), 当前元素数量(内部数据.size()) {
                内部数据.resize(计算::向上取整为2的幂(当前元素数量), nullptr);
                容器容量 = 内部数据.size();
                容器位数 = 计算::获取最近2的幂指数(容器容量);
            }

            // 添加数据到队列
            bool 添加数据(T* 数据) {
                if (当前元素数量 >= 容器容量) {
                    扩容();
                }

                // 使用取模后的索引添加数据
                u64 索引 = 计算::快速二进制取模函数(队列尾, 容器位数);
                内部数据[索引] = 数据;
                队列尾++;
                当前元素数量++;
                return true;
            }

            // 获取队列头的数据
            T* 获取数据() {
                if (当前元素数量 == 0) return nullptr;

                // 环形访问数据并返回
                u64 索引 = 计算::快速二进制取模函数(队列头, 容器位数);
                T* 数据 = 内部数据[索引];
                内部数据[索引] = nullptr;

                队列头++;
                当前元素数量--;
                return 数据;
            }

            // 压缩容器到合理大小
            bool 压缩容器() {
                if (当前元素数量 * 2 > 容器容量) {
                    return false;  // 不需要压缩
                }

                // 将所有未消费的数据搬移到起点
                搬移数据(0);
                内部数据.resize(计算::向上取整为2的幂(当前元素数量));
                容器容量 = 内部数据.size();
                容器位数 = 计算::获取最近2的幂指数(容器容量);
                return true;
            }
        private:
            // 扩容函数：扩展到两倍大小
            void 扩容() {
                u64 新容量 = 容器容量 * 2;
                std::vector<T*> 新数据(新容量, nullptr);

                // 搬移未消费的元素到新容器
                搬移数据(新数据);

                内部数据 = std::move(新数据);
                容器容量 = 新容量;
                容器位数 += 1;
            }

            // 搬移数据逻辑，支持压缩和扩容
            void 搬移数据(std::vector<T*>& 新数据) {
                for (u64 i = 0; i < 当前元素数量; ++i) {
                    u64 原索引 = 计算::快速二进制取模函数(队列头 + i, 容器位数);
                    新数据[i] = 内部数据[原索引];
                    内部数据[原索引] = nullptr;
                }
                队列头 = 0;
                队列尾 = 当前元素数量;
            }

            void 搬移数据(u64 新起点) {
                for (u64 i = 0; i < 当前元素数量; ++i) {
                    u64 原索引 = 计算::快速二进制取模函数(队列头 + i, 容器位数);
                    内部数据[新起点 + i] = 内部数据[原索引];
                    内部数据[原索引] = nullptr;
                }
                队列头 = 新起点;
                队列尾 = 新起点 + 当前元素数量;
            }

            std::vector<T*> 内部数据;  // 底层存储容器
            u8 容器位数;               // 容量的位数
            u64 队列头;                // 队列头指针
            u64 队列尾;                // 队列尾指针
            u64 容器容量;              // 当前容量
            u64 当前元素数量;           // 当前元素数量
        };


        namespace 内存分配器 {
            //enum 数据类型
            //{
            //    error_ = 0,
            //    bool_, 
            //    u8_,
            //    i8_,
            //    u16_,
            //    i16_,
            //    u32_,
            //    i32_,
            //    u64_,
            //    i64_,
            //    f64_,

            //    ptr_,

            //    char_,
            //    wchar_t_,
            //    char16_t_,
            //    char32_t_
            //};
            //class 分配器 {
            //private:
            //    struct 内存块信息
            //    {
            //        u64 起始位置_byet;
            //        u64 结束位置_byet;
            //        u32 数据长度_byet;
            //        bool 是否使用;
            //    };
            //    class 数据指针
            //    {
            //    public:
            //        数据指针(数据类型 数据类型, 分配器* 分配器指针, 内存块信息* 内存块信息指针);
            //    private:
            //        ~数据指针();
            //        数据类型    数据_类型_;
            //        分配器* 分配器_指针;
            //        内存块信息* 内存块_指针;
            //    };
            //    内存块信息* 添加内存块信息(u64 起始位置_byet, u64 结束位置_byet, u32 数据长度_byet, bool 是否使用);
            //    void        删除内存块信息();
            //public:
            //    分配器(u64 最大byet = 1024 * 1024, u64 初始byet = 4096, u64 扩容倍率 = 2);
            //    数据指针 malloc(数据类型 数据类型,u64 数量);
            //    数据指针 realloc(数据指针 数据指针, u64 数量);
            //    void free(数据指针 数据指针);
            //private:
            //    ~分配器();
            //private:
            //    std::vector<u8> 内存_存储_区域;
            //    u8* 内存_开始_指针 = nullptr;
            //    u8* 内存_结束_指针 = nullptr;
            //    u8* 内存_分配_指针 = nullptr;
            //    //当前导出的指针数
            //    u64 数据_指针_引用数 = 0;
            //    //连续数量为key，按大小排序，
            //    std::map <u64, 内存块信息*> 未使用连续内存块_{};
            //    //相对 内存_开始_指针 的byet为key，按地址排序，
            //    std::map<u64, 内存块信息*> 所有内存块信息_{};
            //    //内存块信息存储区,u64 为引用次数
            //    std::unordered_map<内存块信息*, u64> 内存块信息存储区{};
            //};
            enum 数据类型 {
                error_ = 0,
                bool_, u8_, i8_, char_,
                u16_, i16_,wchar_t_, char16_t_,
                u32_, i32_, char32_t_,
                u64_, i64_, f64_, ptr_,
            };

            class 分配器 {
            private:
                struct 内存块信息 {
                    u64 起始位置_byet;
                    u64 结束位置_byet;
                    u32 数据长度_byet;
                    bool 是否使用;
                };

                class 数据指针 {
                public:
                    数据指针(数据类型 数据类型, 分配器* 分配器指针, 内存块信息* 内存块信息指针);
                    ~数据指针();

                    u64 地址() const { return 内存块_指针->起始位置_byet; }
                    数据类型 数据_类型_;
                    分配器* 分配器_指针;
                    内存块信息* 内存块_指针;
                };

                std::multimap<u64, 内存块信息*> 未使用连续内存块_;
                std::map<u64, 内存块信息*> 所有内存块信息_;
                std::unordered_map<内存块信息*, u64> 内存块信息存储区_;

                std::vector<u8> 内存_存储_区域;
                u8* 内存_开始_指针 = nullptr;
                u8* 内存_结束_指针 = nullptr;
                u8* 内存_分配_指针 = nullptr;

                u64 数据_指针_引用数 = 0;
                u64 扩容倍率_;

            public:
                分配器(u64 最大byet, u64 初始byet = 4096, u64 扩容倍率 = 2);
                ~分配器();

                数据指针 malloc(数据类型 数据类型, u64 数量);
                数据指针 realloc(数据指针 数据指针, u64 数量);
                void free(数据指针 数据指针);

            private:
                void 扩展内存(u64 最少需要的字节);
                void 合并相邻内存块(内存块信息* 块);
                内存块信息* 添加内存块信息(u64 起始位置, u64 结束位置, u32 数据长度, bool 是否使用);
                void 删除内存块信息(内存块信息* 块);
            };
        }
    }
    namespace 字符串 {
        //将utf-8 转换为 Unicode码点
        std::u32string utf8_to_unicode(const std::string& utf8);
        // 将 Unicode 码位转换为 UTF-8
        std::string unicode_to_utf8(const std::u32string& unicode);
        // 将 UTF-8 字符串转换为宽字符（wchar_t）
        std::wstring utf8_to_wstring(const std::string& utf8);
        //将utf8字符串存储到string
        std::string char8_t_To_string(const char8_t* u8str);
        //将utf8字符串转换为gbk字符串
        std::string utf8_to_gbk(const std::string& utf8Str);
        // 过滤连续特殊字符
        std::u32string 按连续特殊字符过滤(const std::u32string& str, const std::u32string& 特殊字符 = { ' ', '\t', '\n' }, const std::u32string& 替换目标 = { ' ' });
        // 按特殊字符分隔字符串
        std::vector<std::u32string> 按特殊字符分隔字符串(const std::u32string& str, const std::u32string& 特殊字符串 = {' '});
    }

    namespace 文件 {
        //将utf8文件读取到string
        std::string file_to_string(const std::string& filename);

        //将string写入文件
        void string_to_file(const std::string& filename, const std::string& content);

        //将数组写入文件
        template <typename T>
        void write_Vector_To_File(const std::string& filename, const std::vector<T>& vec) {
            auto _filename = 字符串::utf8_to_gbk(filename); // 转换为GBK编码
            std::ofstream file(_filename, std::ios::binary);
            if (!file.is_open()) {
                std::cerr << "Failed to open file: " << _filename << std::endl;
                return;
            }

            size_t size = vec.size();
            file.write(reinterpret_cast<const char*>(&size), sizeof(size)); // 写入向量大小
            file.write(reinterpret_cast<const char*>(vec.data()), size * sizeof(T)); // 写入向量数据
            file.close();
        }

        //从文件读取数组
        template <typename T>
        std::vector<T> read_Vector_From_File(const std::string& filename) {
            auto _filename = 字符串::utf8_to_gbk(filename); // 转换为GBK编码
            std::ifstream file(_filename, std::ios::binary);
            if (!file.is_open()) {
                std::cerr << "Failed to open file: " << _filename << std::endl;
                return {};
            }

            size_t size;
            file.read(reinterpret_cast<char*>(&size), sizeof(size)); // 读取向量大小
            std::vector<T> vec(size);
            file.read(reinterpret_cast<char*>(vec.data()), size * sizeof(T)); // 读取向量数据
            file.close();

            return vec;
        }

    }

    namespace 线程 {
        // 等效队列数据容器
        template <typename T>
        class 任务队列数据容器 {
        private:
            std::vector<T*> 队列容器; // 使用智能指针来管理任务
            std::mutex 队列任务锁;                   // 用于保护队列的互斥锁
            size_t 当前起始容器位置 = 0;              // 队列的起始位置
            size_t 当前结束容器位置 = 0;              // 队列的结束位置
            size_t 队列任务数 = 0;                    // 当前队列中的任务数量
            size_t 进行任务数 = 0;                    // 正在进行的任务数量

        public:
            // 构造函数
            任务队列数据容器() {
                队列容器.resize(8); // 初始大小设置为8
            }

            // 添加任务到队列
            bool 添加任务(T* 任务数据指针) {
                if (!任务数据指针) {
                    return false; // 检查空指针
                }

                std::lock_guard<std::mutex> lock(队列任务锁); // 自动锁定和解锁

                // 如果任务数已达到容器容量，则进行扩容
                if (队列任务数 >= 队列容器.size()) {
                    size_t 当前大小 = 队列容器.size();
                    队列容器.resize(当前大小 * 2); // 扩容容器

                    // 如果当前起始位置在队列的后面一半，数据已环绕
                    if (当前起始容器位置 > 当前结束容器位置) {
                        // 将从起始位置到末尾的数据，拷贝到新容器的后半部分
                        size_t 第一段长度 = 当前大小 - 当前起始容器位置;
                        for (size_t i = 0; i < 当前结束容器位置; ++i) {
                            队列容器[当前大小 + i] = 队列容器[i]; // 将队列头部的元素移到新空间
                            队列容器[i] = nullptr;                 // 清空旧位置的数据
                        }
                        当前结束容器位置 += 当前大小; // 更新结束位置
                    }
                }

                // 将任务数据指针添加到队列中
                队列容器[当前结束容器位置] = 任务数据指针;

                // 更新结束位置，循环使用队列
                当前结束容器位置 = (当前结束容器位置 + 1) % 队列容器.size();
                队列任务数++; // 更新任务数
                return true;
            }

            // 从队列弹出第一个任务，并将获取任务计数器+1
            T* 获取任务() {
                std::lock_guard<std::mutex> lock(队列任务锁); // 自动锁定和解锁

                if (队列任务数 == 0) {
                    return nullptr; // 队列为空时返回空指针
                }

                // 获取队列中的第一个任务
                auto 任务 = 队列容器[当前起始容器位置];
                队列容器[当前起始容器位置] = nullptr; // 清空该位置

                // 更新起始位置
                当前起始容器位置 = (当前起始容器位置 + 1) % 队列容器.size();
                队列任务数--; // 更新任务数
                进行任务数++; // 正在进行的任务数+1

                return 任务; // 返回任务
            }

            // 将获取任务计数器-1，表示任务完成
            bool 结束任务() {
                std::lock_guard<std::mutex> lock(队列任务锁); // 自动锁定和解锁
                if (进行任务数 > 0) {
                    进行任务数--; // 任务结束，减少进行任务数
                    return true;
                }
                return false;
            }

            // 获取当前进行任务的数量
            size_t 当前获取任务数() const {
                std::lock_guard<std::mutex> lock(队列任务锁); // 加锁以保护读取
                return 进行任务数; // 返回正在进行的任务数量
            }

            // 获取当前队列中任务的数量
            size_t 当前队列任务数() const {
                std::lock_guard<std::mutex> lock(队列任务锁); // 加锁以保护读取
                return 队列任务数; // 返回当前队列中的任务数量
            }
        };
        //等效单写多读数据容器，ns级别，读取时拷贝，同时只能一个线程写入或多个线程读取
        template <typename T>
        class 单写多读数据容器 {
        private:
            T 数据;  // 容器数据
            std::atomic<bool> 是否有线程写入{ false };  // 用于指示是否有线程正在写入
            std::atomic<size_t> 正在读取线程数{ 0 };    // 当前正在读取的线程数
            std::atomic<bool> 即将写入{ false };         // 用于告诉读线程写操作即将开始

        public:
            单写多读数据容器() : 数据(T()) {}
            单写多读数据容器(T 初始化数据) : 数据(初始化数据) {}

            // 禁止拷贝构造和赋值
            单写多读数据容器(const 单写多读数据容器&) = delete;
            单写多读数据容器& operator=(const 单写多读数据容器&) = delete;

            // 写入数据 (只允许一个线程写入，写入时禁止其他线程读取)
            inline void 写入数据(const T& 新数据) {
                //避免2个线程以上同时进行写入
                while (即将写入) {
                    {};
                }

                // 首先设置即将写入标志，防止新读线程进入
                即将写入 = true;

                // 自旋等待所有读线程完成
                while (正在读取线程数 > 0) {
                    {};
                }

                // 设置写入标志，表示写操作正在进行
                是否有线程写入 = true;

                // 执行写入操作
                数据 = 新数据;

                // 写入完成后清除标志
                是否有线程写入 = false;
                即将写入 = false;  // 解除即将写入状态
            }

            // 读取数据 (多个线程可以同时读，除非写入占用)
            inline T 读取数据() {
                // 如果有线程即将进行写入，或者当前正处于写入中，等待写操作完成
                while (即将写入 or 是否有线程写入) {
                    {};
                }

                // 增加读取线程计数
                ++正在读取线程数;

                // 读取数据
                T 读到的数据 = 数据;

                // 读取完成，减少读取线程计数
                --正在读取线程数;

                return 读到的数据;
            }
        };
        //等效单线程数据容器，ns级别，但多线程冲突下性能不好，没有拷贝，同时只能一个线程操作一个容器
        template <typename T>
        class 等效单线程数据容器 {
        private:
            T 数据;
            std::atomic_flag 标签 = ATOMIC_FLAG_INIT;  // 自旋锁
            std::atomic<bool> 是否使用中{ false };
            std::thread::id 拥有者ID;

        public:
            等效单线程数据容器() : 数据(T()) {}
            等效单线程数据容器(T 初始化数据) : 数据(初始化数据) {
                拥有者ID = std::thread::id();
            }

            // 禁止拷贝构造和赋值
            等效单线程数据容器(const 等效单线程数据容器&) = delete;
            等效单线程数据容器& operator=(const 等效单线程数据容器&) = delete;

            // 尝试申请所有权
            inline bool 尝试申请所有权() {
                // 尝试获取锁
                if (标签.test_and_set(std::memory_order_acquire)) {
                    return false;//已有占用返回
                }

                // 成功获取锁后，检查是否在使用中
                if (是否使用中) {
                    标签.clear(std::memory_order_release);  // 释放锁
                    return false;  // 如果在使用中，返回 false
                }

                是否使用中 = true;  // 设置状态为使用中
                拥有者ID = std::this_thread::get_id();  // 记录当前线程ID
                return true;  // 成功获取所有权
            }

            // 申请所有权
            inline bool 申请所有权() {
                // 尝试获取锁
                while (标签.test_and_set(std::memory_order_acquire)) {
                    // 自旋等待
                }

                // 成功获取锁后，检查是否在使用中
                if (是否使用中) {
                    标签.clear(std::memory_order_release);  // 释放锁
                    return false;  // 如果在使用中，返回 false
                }

                是否使用中 = true;  // 设置状态为使用中
                拥有者ID = std::this_thread::get_id();  // 记录当前线程ID
                return true;  // 成功获取所有权
            }

            // 释放所有权
            inline bool 释放所有权() {
                if (拥有者ID == std::this_thread::get_id()) {
                    拥有者ID = std::thread::id();  // 重置拥有者ID
                    是否使用中 = false;  // 重置状态
                    标签.clear(std::memory_order_release);  // 释放锁
                    return true;  // 成功释放所有权
                }
                return false;  // 释放失败，可能是因为不是持有者
            }

            // 获取数据
            inline T* 获取数据() {
                if (拥有者ID == std::this_thread::get_id()) {
                    return &数据;  // 返回数据指针
                }
                return nullptr;  // 如果不是持有者，返回 nullptr
            }

            ~等效单线程数据容器() {
                while (是否使用中) {
                    std::this_thread::yield();  // 等待所有权释放
                }
            }
        };
    }
}
