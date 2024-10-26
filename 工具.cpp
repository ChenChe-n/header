#include "工具.hpp"

namespace 工具 {
    namespace 终端 {

        bool 设置控制台为UTF_8编码::是否初始化 = false;

        设置控制台为UTF_8编码::设置控制台为UTF_8编码() {
            if (not 是否初始化) {
                是否初始化 = true;
                SetConsoleOutputCP(CP_UTF8);
            }
            else {

            }
        }

        void ShowCursor(bool showFlag) {
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_CURSOR_INFO cursorInfo;
            GetConsoleCursorInfo(hOut, &cursorInfo);
            cursorInfo.bVisible = showFlag; // 设置光标可见性
            SetConsoleCursorInfo(hOut, &cursorInfo);
        }

        void GotoXY(int x, int y) {
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            COORD coord;
            coord.X = x;
            coord.Y = y;
            SetConsoleCursorPosition(hOut, coord);
        }
    }

    namespace 计算 {

    }

    namespace 容器 {
        namespace 内存分配器 {
 /*           分配器::数据指针::数据指针(数据类型 数据类型, 分配器* 分配器指针, 内存块信息* 内存块信息指针)
                :数据_类型_(数据类型), 分配器_指针(分配器指针), 内存块_指针(内存块信息指针)
            {
                分配器_指针->数据_指针_引用数++;
            }

            分配器::数据指针::~数据指针()
            {
                分配器_指针->数据_指针_引用数--;
            }

            分配器::内存块信息* 分配器::添加内存块信息(u64 起始位置_byet, u64 结束位置_byet, u32 数据长度_byet, bool 是否使用)
            {
                分配器::内存块信息* 内存块信息;

            }

            void 分配器::删除内存块信息()
            {

            }

            分配器::分配器(u64 最大byet, u64 初始byet, u64 扩容倍率)
                :内存_存储_区域(初始byet, 0)
            {
                内存_开始_指针 = &内存_存储_区域[0];
                内存_结束_指针 = 内存_开始_指针 + 最大byet;
                内存_分配_指针 = &内存_存储_区域.back();

            }

            分配器::数据指针 
                分配器::malloc(数据类型 数据类型, u64 数量)
            {

            }

            分配器::数据指针 
                分配器::realloc(数据指针 数据指针, u64 数量)
            {

            }

            void 分配器::free(数据指针 数据指针)
            {
            }


            分配器::~分配器()
            {
            }*/

            分配器::数据指针::数据指针(数据类型 数据类型, 分配器* 分配器指针, 内存块信息* 内存块信息指针)
                : 数据_类型_(数据类型), 分配器_指针(分配器指针), 内存块_指针(内存块信息指针) {
                分配器_指针->数据_指针_引用数++;
            }

            分配器::数据指针::~数据指针() {
                分配器_指针->数据_指针_引用数--;
            }

            分配器::分配器(u64 最大byet, u64 初始byet, u64 扩容倍率)
                : 内存_存储_区域(初始byet, 0), 扩容倍率_(扩容倍率) {
                内存_开始_指针 = &内存_存储_区域[0];
                内存_结束_指针 = 内存_开始_指针 + 最大byet;
                内存_分配_指针 = 内存_开始_指针;
            }

            分配器::~分配器() {
                for (auto& [_, 块] : 所有内存块信息_) {
                    delete 块;
                }
            }

            分配器::数据指针 分配器::malloc(数据类型 数据类型, u64 数量) {
                u64 数据长度;
                switch (数据类型)
                {
                case 工具::容器::内存分配器::error_:
                    数据长度 = 0;
                    break;
                case 工具::容器::内存分配器::bool_:
                case 工具::容器::内存分配器::u8_:
                case 工具::容器::内存分配器::i8_:
                case 工具::容器::内存分配器::char_:
                    数据长度 = 1;
                    break;
                case 工具::容器::内存分配器::u16_:
                case 工具::容器::内存分配器::i16_:
                case 工具::容器::内存分配器::wchar_t_:
                case 工具::容器::内存分配器::char16_t_:
                    数据长度 = 2;
                    break;
                case 工具::容器::内存分配器::u32_:
                case 工具::容器::内存分配器::i32_:
                case 工具::容器::内存分配器::char32_t_:
                    数据长度 = 4;
                    break;
                case 工具::容器::内存分配器::u64_:
                case 工具::容器::内存分配器::i64_:
                case 工具::容器::内存分配器::f64_:
                case 工具::容器::内存分配器::ptr_:
                    数据长度 = 8;
                    break;
                default:
                    break;
                }
                u64 所需字节 = 数量 * sizeof(数据类型);

                auto it = 未使用连续内存块_.lower_bound(所需字节);
                if (it != 未使用连续内存块_.end()) {
                    内存块信息* 块 = it->second;
                    块->是否使用 = true;
                    未使用连续内存块_.erase(it);
                    return 数据指针(数据类型, this, 块);
                }

                if (内存_分配_指针 + 所需字节 > 内存_结束_指针) {
                    扩展内存(所需字节);
                }

                u64 起始位置 = 内存_分配_指针 - 内存_开始_指针;
                内存块信息* 新块 = 添加内存块信息(起始位置, 起始位置 + 所需字节, 所需字节, true);
                内存_分配_指针 += 所需字节;

                return 数据指针(数据类型, this, 新块);
            }

            void 分配器::free(数据指针 数据指针) {
                内存块信息* 块 = 数据指针.内存块_指针;
                块->是否使用 = false;
                未使用连续内存块_.emplace(块->数据长度_byet, 块);
                合并相邻内存块(块);
            }

            分配器::数据指针 分配器::realloc(数据指针 数据指针, u64 数量) {
                内存块信息* 块 = 数据指针.内存块_指针;
                u64 新大小 = 数量 * sizeof(数据指针.数据_类型_);

                if (块->数据长度_byet >= 新大小) {
                    return 数据指针;
                }

                auto 新指针 = malloc(数据指针.数据_类型_, 数量);
                std::memcpy(&内存_存储_区域[新指针.内存块_指针->起始位置_byet],
                    &内存_存储_区域[块->起始位置_byet], 块->数据长度_byet);
                free(数据指针);
                return 新指针;
            }

            void 分配器::扩展内存(u64 最少需要的字节) {
                u64 当前大小 = 内存_存储_区域.size();

                u64 新大小 = max((当前大小 * 扩容倍率_), (当前大小 + 最少需要的字节));
                if (新大小 > 内存_结束_指针 - 内存_开始_指针) {
                    throw std::bad_alloc();
                }
                内存_存储_区域.resize(新大小);
                内存_分配_指针 = &内存_存储_区域[当前大小];
            }

            void 分配器::合并相邻内存块(内存块信息* 块) {
                auto it = 所有内存块信息_.find(块->起始位置_byet);
                if (it != 所有内存块信息_.end()) {
                    auto next = std::next(it);
                    if (next != 所有内存块信息_.end() && !next->second->是否使用) {
                        块->结束位置_byet = next->second->结束位置_byet;
                        块->数据长度_byet += next->second->数据长度_byet;
                        删除内存块信息(next->second);
                    }
                }
            }

            分配器::内存块信息* 分配器::添加内存块信息(u64 起始位置, u64 结束位置, u32 数据长度, bool 是否使用) {
                auto* 块 = new 内存块信息{ 起始位置, 结束位置, 数据长度, 是否使用 };
                所有内存块信息_[起始位置] = 块;
                if (!是否使用) {
                    未使用连续内存块_.emplace(数据长度, 块);
                }
                return 块;
            }

            void 分配器::删除内存块信息(内存块信息* 块) {
                所有内存块信息_.erase(块->起始位置_byet);
                delete 块;
            }
        }
    }

    namespace 字符串 {


        std::u32string utf8_to_unicode(const std::string& utf8) {
            std::u32string utf32_string;
            size_t i = 0;

            while (i < utf8.size()) {
                unsigned char c = utf8[i];

                // 根据 UTF-8 的字节格式确定字符长度
                if (c < 0x80) {
                    // 1-byte character (ASCII)
                    utf32_string += static_cast<char32_t>(c);
                    i += 1;
                }
                else if ((c >> 5) == 0b110) {
                    // 2-byte character
                    if (i + 1 >= utf8.size() || (utf8[i + 1] & 0b11000000) != 0b10000000)
                        throw std::runtime_error("Invalid UTF-8 sequence.");
                    char32_t unicode = ((c & 0b11111) << 6) | (utf8[i + 1] & 0b111111);
                    utf32_string += unicode;
                    i += 2;
                }
                else if ((c >> 4) == 0b1110) {
                    // 3-byte character
                    if (i + 2 >= utf8.size() ||
                        (utf8[i + 1] & 0b11000000) != 0b10000000 ||
                        (utf8[i + 2] & 0b11000000) != 0b10000000)
                        throw std::runtime_error("Invalid UTF-8 sequence.");
                    char32_t unicode = ((c & 0b1111) << 12) | ((utf8[i + 1] & 0b111111) << 6) | (utf8[i + 2] & 0b111111);
                    utf32_string += unicode;
                    i += 3;
                }
                else if ((c >> 3) == 0b11110) {
                    // 4-byte character
                    if (i + 3 >= utf8.size() ||
                        (utf8[i + 1] & 0b11000000) != 0b10000000 ||
                        (utf8[i + 2] & 0b11000000) != 0b10000000 ||
                        (utf8[i + 3] & 0b11000000) != 0b10000000)
                        throw std::runtime_error("Invalid UTF-8 sequence.");
                    char32_t unicode = ((c & 0b111) << 18) | ((utf8[i + 1] & 0b111111) << 12) |
                        ((utf8[i + 2] & 0b111111) << 6) | (utf8[i + 3] & 0b111111);
                    utf32_string += unicode;
                    i += 4;
                }
                else {
                    // Invalid UTF-8 character
                    throw std::runtime_error("Invalid UTF-8 character.");
                }
            }

            return utf32_string;
        }

        std::string unicode_to_utf8(const std::u32string& unicode) {
            std::string utf8;

            for (char32_t codepoint : unicode) {
                if (codepoint <= 0x7F) {
                    // 1 byte
                    utf8.push_back(static_cast<char>(codepoint));
                }
                else if (codepoint <= 0x7FF) {
                    // 2 bytes
                    utf8.push_back(static_cast<char>((codepoint >> 6) | 0xC0));
                    utf8.push_back(static_cast<char>((codepoint & 0x3F) | 0x80));
                }
                else if (codepoint <= 0xFFFF) {
                    // 3 bytes
                    utf8.push_back(static_cast<char>((codepoint >> 12) | 0xE0));
                    utf8.push_back(static_cast<char>(((codepoint >> 6) & 0x3F) | 0x80));
                    utf8.push_back(static_cast<char>((codepoint & 0x3F) | 0x80));  // 修正这里
                }
                else if (codepoint <= 0x10FFFF) {
                    // 4 bytes
                    utf8.push_back(static_cast<char>((codepoint >> 18) | 0xF0));
                    utf8.push_back(static_cast<char>(((codepoint >> 12) & 0x3F) | 0x80));
                    utf8.push_back(static_cast<char>(((codepoint >> 6) & 0x3F) | 0x80));
                    utf8.push_back(static_cast<char>((codepoint & 0x3F) | 0x80));  // 修正这里
                }
                else {
                    // Invalid codepoint (could be handled as an error)
                    continue;  // 这里保持不变
                }
            }

            return utf8;
        }

        std::wstring utf8_to_wstring(const std::string& utf8) {
            std::wstring wide_string;
            size_t i = 0;

            while (i < utf8.size()) {
                unsigned char c = utf8[i];

                // 根据 UTF-8 字节格式确定字符长度
                if (c < 0x80) {
                    // 1-byte character (ASCII)
                    wide_string += static_cast<wchar_t>(c);
                    i += 1;
                }
                else if ((c >> 5) == 0b110) {
                    // 2-byte character
                    if (i + 1 >= utf8.size()) throw std::runtime_error("Invalid UTF-8 sequence.");
                    wchar_t wchar = ((c & 0b11111) << 6) | (utf8[i + 1] & 0b111111);
                    wide_string += wchar;
                    i += 2;
                }
                else if ((c >> 4) == 0b1110) {
                    // 3-byte character
                    if (i + 2 >= utf8.size()) throw std::runtime_error("Invalid UTF-8 sequence.");
                    wchar_t wchar = ((c & 0b1111) << 12) | ((utf8[i + 1] & 0b111111) << 6) | (utf8[i + 2] & 0b111111);
                    wide_string += wchar;
                    i += 3;
                }
                else if ((c >> 3) == 0b11110) {
                    // 4-byte character
                    if (i + 3 >= utf8.size()) throw std::runtime_error("Invalid UTF-8 sequence.");
                    wchar_t wchar = ((c & 0b111) << 18) | ((utf8[i + 1] & 0b111111) << 12) |
                        ((utf8[i + 2] & 0b111111) << 6) | (utf8[i + 3] & 0b111111);
                    // 将 UTF-32 转换为 UTF-16
                    if (wchar >= 0x10000) {
                        wchar_t surrogate1 = 0xD800 + ((wchar - 0x10000) >> 10);
                        wchar_t surrogate2 = 0xDC00 + ((wchar - 0x10000) & 0x3FF);
                        wide_string += surrogate1;
                        wide_string += surrogate2;
                    }
                    else {
                        wide_string += wchar;
                    }
                    i += 4;
                }
                else {
                    // Invalid UTF-8 character
                    throw std::runtime_error("Invalid UTF-8 character.");
                }
            }

            return wide_string;
        }

        std::string char8_t_To_string(const char8_t* u8str) {
            return std::string(reinterpret_cast<const char*>(u8str));
        }

        std::string utf8_to_gbk(const std::string& utf8Str) {
            // 计算转换为宽字符所需的缓冲区大小
            int wideCharSize = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, nullptr, 0);
            if (wideCharSize == 0) {
                throw std::runtime_error("Failed to convert UTF-8 to wide char.");
            }

            // 分配缓冲区并进行转换
            std::wstring wideStr(wideCharSize, 0);
            MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, &wideStr[0], wideCharSize);

            // 计算转换为GBK所需的缓冲区大小
            int gbkSize = WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
            if (gbkSize == 0) {
                throw std::runtime_error("Failed to convert wide char to GBK.");
            }

            // 分配缓冲区并进行转换
            std::string gbkStr(gbkSize, 0);
            WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, &gbkStr[0], gbkSize, nullptr, nullptr);

            return gbkStr;
        }

        std::u32string 按连续特殊字符过滤(const std::u32string& str, const std::u32string& 特殊字符, const std::u32string& 替换目标) {
            std::u32string 输出;
            std::unordered_map<char32_t, bool> 特殊字符表;

            // 初始化特殊字符表
            for (auto c : 特殊字符) {
                特殊字符表[c] = true;
            }

            bool 当前是否出现特殊字符 = false;

            for (auto c : str) {
                if (特殊字符表.count(c)) {
                    当前是否出现特殊字符 = true;
                }
                else {
                    if (当前是否出现特殊字符) {
                        if (输出.size() != 0)
                        {
                            输出 += 替换目标;
                        }
                        当前是否出现特殊字符 = false;
                    }
                    输出 += c;// 添加当前字符
                }
            }
            return 输出; // 返回结果
        }

        std::vector<std::u32string> 按特殊字符分隔字符串(const std::u32string& str, const std::u32string& 特殊字符串) {
            std::vector<std::u32string> 输出;
            size_t 左指针 = 0;

            while (左指针 < str.size()) {
                // 查找特殊字符串的出现位置
                size_t 右指针 = str.find(特殊字符串, 左指针);

                // 如果找到特殊字符串
                if (右指针 != std::u32string::npos) {
                    // 将左指针到右指针之间的内容保存到输出中
                    if (右指针 > 左指针) { // 确保不保存空字符串
                        输出.push_back(str.substr(左指针, 右指针 - 左指针));
                    }
                    // 移动左指针到特殊字符串的结束位置
                    左指针 = 右指针 + 特殊字符串.size();
                }
                else {
                    // 如果没有找到特殊字符串，将剩余的内容添加到输出中
                    if (左指针 < str.size()) {
                        输出.push_back(str.substr(左指针));
                    }
                    break; // 退出循环
                }
            }

            return 输出;
        }


    }

    namespace 文件 {



        std::string file_to_string(const std::string& filename) {
            auto _filename = 字符串::utf8_to_gbk(filename); // 转换为GBK编码
            std::ifstream file(_filename, std::ios::binary); // 以二进制模式打开文件
            if (!file.is_open()) {
                std::cerr << "Failed to open file: " << _filename << std::endl;
                return "";
            }

            std::ostringstream contents;
            contents << file.rdbuf(); // 将文件内容读入字符串流
            return contents.str(); // 返回字符串
        }

        void string_to_file(const std::string& filename, const std::string& content) {
            auto _filename = 字符串::utf8_to_gbk(filename); // 转换为GBK编码
            std::ofstream file(_filename, std::ios::binary); // 以二进制模式打开文件
            if (!file.is_open()) {
                std::cerr << "Failed to open file: " << _filename << std::endl;
                return;
            }

            file << content; // 写入内容
            file.close(); // 关闭文件
        }

    }


}

