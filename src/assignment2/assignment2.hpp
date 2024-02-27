#pragma once
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>

namespace comp7107
{
    struct Restaurant {
        uint32_t id = 0;
        double x = 0;
        double y = 0;
        uint16_t cell_x = 0;
        uint16_t cell_y = 0;

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
    };

    bool comp_with_cell_idx(const Restaurant& r1, const Restaurant& r2)
    {
        if(r1.cell_x < r2.cell_x)
            return true;
        else if(r1.cell_x > r2.cell_x)
            return false;
        
        if(r1.cell_y < r2.cell_y)
            return true;
        else if(r1.cell_y > r2.cell_y)
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


