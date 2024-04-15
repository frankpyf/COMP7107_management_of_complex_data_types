#include <string>
#include <fstream>
#include <algorithm>
#include <math.h>

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
    void set_value(const std::string& src, long& dst)
    {
        dst = std::stol(src);
    }

    void do_parse(const std::string&, char, size_t)
    {
        return;
    }

    template<typename Arg, typename ... Args>
    void do_parse(const std::string& str_line, char sep, size_t current_idx, Arg& arg0, Args& ... args)
    {
        while(str_line[current_idx] == ' ' || str_line[current_idx] == '\t' || str_line[current_idx] == sep)
        {
            ++current_idx;
        }
        size_t length = 0;
        while(((current_idx + length) < str_line.length()) && str_line[current_idx + length] != sep)
        {
            ++length;
        }
        set_value(str_line.substr(current_idx, length), arg0);
        do_parse(str_line, sep, current_idx + length, args...);
    }

    template<typename ...Args>
    void parse(const std::string& str_line, char sep, Args&... args)
    {
        do_parse(str_line, sep, 0, args...);
    };

    struct AdjacencyInfo {
        AdjacencyInfo(long in_timestamp, int in_id, int in_sentiment)
        : unix_timestamp(in_timestamp), id(in_id), sentiment(in_sentiment){}

        const bool operator==(const AdjacencyInfo& other) const
        {
            return unix_timestamp == other.unix_timestamp && id == other.id && sentiment == other.sentiment;
        }

        long unix_timestamp;
        int id;
        int sentiment;  
    };
} // namespace comp7107