#pragma once
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>

#define STRING(x) #x
#define XSTRING(x) STRING(x)

static const std::string data_path(XSTRING(DATA_PATH));

namespace comp7107
{
    struct Restaurant {
        uint32_t id = 0;
        double x = 0;
        double y = 0;

        Restaurant() = default;
        Restaurant(double in_x, double in_y) 
            : x(in_x), y(in_y) {}
        Restaurant(uint32_t in_id, double in_x, double in_y)
            : id(in_id), x(in_x), y(in_y) {}
        ~Restaurant() = default;
    };

    struct Cell {
        uint32_t begin_character_pos = 0;
        uint32_t num_of_records = 0;

        static double min_x;
        static double min_y;

        static double max_x;
        static double max_y;
    };

    double compute_euclidean_distance(const Restaurant& r1, const Restaurant& r2)
    {
        return sqrt((r1.x - r2.x) * (r1.x - r2.x) + (r1.y - r2.y) * (r1.y - r2.y));
    }

    bool comp_with_cell_idx(const Restaurant& r1, const Restaurant& r2)
    {
        uint32_t r1_cell_x = (r1.x - Cell::min_x) * 10.f / (Cell::max_x - Cell::min_x);
        uint32_t r1_cell_y = (r1.y - Cell::min_y) * 10.f / (Cell::max_y - Cell::min_y);

        uint32_t r2_cell_x = (r2.x - Cell::min_x) * 10.f / (Cell::max_x - Cell::min_x);
        uint32_t r2_cell_y = (r2.y - Cell::min_y) * 10.f / (Cell::max_y - Cell::min_y);

        if(r1_cell_x < r2_cell_x)
            return true;
        else if(r1_cell_x > r2_cell_x)
            return false;
        
        if(r1_cell_y < r2_cell_y)
            return true;
        else if(r1_cell_y > r2_cell_y)
            return false;

        return r1.id < r2.id;
    }

    template<typename Arg0, typename ...Args>
    void do_parse(std::string& str, Arg0& arg, Args&&... args)
    {
        if(str.length() == 0)
            return;
        size_t pos = 0;
        std::string token;
        size_t idx = 0;
        if((pos = str.find(' ')) != std::string::npos) 
        {
            token = str.substr(0, pos);
            if(token.find('.') != std::string::npos)
                arg = std::stod(token);
            else
                arg = std::stoi(token);
            str.erase(0, pos + 1);
        }
        else
        {
            if(str.find('.') != std::string::npos)
                arg = std::stod(str);
            else
                arg = std::stoi(str);
        }
        (do_parse(str, args), ...);
    };
    
    template<class ...Args>
    void parse_string_by_blank(std::string str, Args&&... args)
    {
        (do_parse(str, args), ...);
    }
} // namespace comp7107


