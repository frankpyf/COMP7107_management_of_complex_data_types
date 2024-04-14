#include <string>
#include <fstream>
#include <algorithm>

#define STRING(x) #x
#define XSTRING(x) STRING(x)

static const std::string data_path(XSTRING(DATA_PATH));
namespace comp7107
{
    void set_value(const std::string& src, std::string& dst)
    {
        dst = src;
    }

    void set_value(const std::string& src, int& dst)
    {
        dst = std::stoi(src);
    }

    void do_parse(const std::string&, size_t)
    {
        return;
    }

    template<typename Arg, typename ... Args>
    void do_parse(const std::string& str_line, size_t current_idx, Arg& arg0, Args& ... args)
    {
        while(str_line[current_idx] == ' ' || str_line[current_idx] == '\t')
        {
            ++current_idx;
        }
        size_t length = 0;
        while(str_line[current_idx + length] != '\t')
        {
            ++length;
        }
        set_value(str_line.substr(current_idx, length), arg0);
        
        do_parse(str_line, current_idx + length, args...);
    }

    template<typename ...Args>
    void parse(const std::string& str_line, Args&... args)
    {
        do_parse(str_line, 0, args...);
    };
    
} // namespace comp7107