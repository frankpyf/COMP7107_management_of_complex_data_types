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

} // namespace comp7107


