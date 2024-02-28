#include <algorithm>
#include <execution>
#include <vector>
#include <array>
#include <limits>
#include <sstream>
#include "assignment2.hpp"
double comp7107::Cell::min_x = 0.0f;
double comp7107::Cell::max_x = 0.0f;
double comp7107::Cell::min_y = 0.0f;
double comp7107::Cell::max_y = 0.0f;

int main(int argc, char* argv[])
{
    double x_low    = 0.0f;
    double x_high   = 0.0f;
    double y_low    = 0.0f;
    double y_high   = 0.0f;

    if (argc > 4)
    {
        x_low   = std::stod(argv[1]);
        x_high  = std::stod(argv[2]);
        y_low   = std::stod(argv[3]);
        y_high  = std::stod(argv[4]);
    }
    std::array<comp7107::Cell, 100> cell_infos;

    std::ifstream input_file(data_path + "/grid.dir");
    std::string str_line;
    std::getline(input_file, str_line);

    comp7107::parse_string_by_blank(str_line, comp7107::Cell::min_x, comp7107::Cell::max_x, comp7107::Cell::min_y, comp7107::Cell::max_y);

    while(std::getline(input_file, str_line))
    {
        uint32_t idx_x = 0;
        uint32_t idx_y = 0;
        uint32_t begin_character_pos = 0;
        uint32_t num_of_records = 0;

        comp7107::parse_string_by_blank(str_line, idx_x, idx_y, begin_character_pos, num_of_records);

        auto& cell = cell_infos[idx_x * 10 + idx_y];
        cell.idx_x = idx_x;
        cell.idx_y = idx_y;
        cell.begin_character_pos = begin_character_pos;
        cell.num_of_records = num_of_records;
    }
    input_file.close();

    input_file.open(data_path + "/grid.grid");
    std::ostringstream str_stm;
    str_stm << input_file.rdbuf() ;
    std::string grid_grid = str_stm.str();
    
    int lower_x_idx     = (x_low - comp7107::Cell::min_x) * 10.f / (comp7107::Cell::max_x - comp7107::Cell::min_x);
    int higher_x_idx    = (x_high - comp7107::Cell::min_x) * 10.f / (comp7107::Cell::max_x - comp7107::Cell::min_x);
    int lower_y_idx     = (y_low - comp7107::Cell::min_y) * 10.f / (comp7107::Cell::max_y - comp7107::Cell::min_y);
    int higher_y_idx    = (y_high - comp7107::Cell::min_y) * 10.f / (comp7107::Cell::max_y - comp7107::Cell::min_y);

    std::ofstream query_result(data_path + "/query_result.txt");
    uint32_t count = 0;
    for(int i = lower_x_idx; i <= higher_x_idx; ++i)
    {
        for(int j = lower_y_idx; j <= higher_y_idx; ++j)
        {
            std::cout << "cell: " << i << ", " << j << " is intersected with query window" << '\n';
            uint32_t offset = cell_infos[i * 10 + j].begin_character_pos;
            uint32_t iterations = cell_infos[i * 10 + j].num_of_records;
            uint32_t pos = offset;
            for(int k = 0; k < iterations; ++k)
            {
                pos = grid_grid.find('\n', offset);
                auto line = grid_grid.substr(offset, pos - offset);
                // Entirely covered by the window
                if(i > lower_x_idx && i < higher_x_idx && j > lower_y_idx && j < higher_y_idx)
                {
                    query_result << line << '\n';
                    ++count;
                }
                // Partially covered by the window
                else
                {
                    comp7107::Restaurant cur_restaurant;
                    comp7107::parse_string_by_blank(line, cur_restaurant.id, cur_restaurant.x, cur_restaurant.y);
                    
                    if(cur_restaurant.x >= x_low && cur_restaurant.x <= x_high 
                    && cur_restaurant.y >= y_low && cur_restaurant.y <= y_high)
                    {
                        query_result << line << '\n';
                        ++count;
                    }
                }
                offset = pos + 1;
            }
        }
    }
    std::cout << "There are " << count << " records in the query window" << '\n';
    return 0;
}   