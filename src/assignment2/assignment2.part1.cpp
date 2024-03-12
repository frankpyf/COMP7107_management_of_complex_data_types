#include <algorithm>
#include <execution>
#include <vector>
#include <array>
#include <limits>
#include "assignment2.hpp"

double comp7107::Cell::min_x = std::numeric_limits<double>::max();
double comp7107::Cell::min_y = std::numeric_limits<double>::max();
double comp7107::Cell::max_x = std::numeric_limits<double>::min();
double comp7107::Cell::max_y = std::numeric_limits<double>::min();

int main(int argc, char* argv[])
{
    std::ifstream input_file(data_path + "/Beijing_restaurants.txt");
    std::string str_line;
    std::getline(input_file, str_line);

    std::vector<comp7107::Restaurant> restaurants;
    std::array<comp7107::Cell, 100> grid_info;

    while(std::getline(input_file, str_line))
    {
        std::string token = str_line.substr(0, str_line.find(' '));
        double x = std::stod(token);
        token = str_line.substr(str_line.find(' ') + 1, str_line.length());
        double y = std::stod(token);
        auto& restaurant = restaurants.emplace_back(restaurants.size() + 1, x, y);
            
        if(x < comp7107::Cell::min_x)
            comp7107::Cell::min_x  = x;
        if(x > comp7107::Cell::max_x)
            comp7107::Cell::max_x = x;
        if(y < comp7107::Cell::min_y)
            comp7107::Cell::min_y = y;
        if(y > comp7107::Cell::max_y)
            comp7107::Cell::max_y = y;
    }
    input_file.close();


    // set belonging cell 
    // and get the number of records in each cell
    for(auto& restaurant : restaurants)
    {
        uint32_t cell_idx_x = (restaurant.x - comp7107::Cell::min_x) * 10.f / (comp7107::Cell::max_x - comp7107::Cell::min_x);
        uint32_t cell_idx_y = (restaurant.y - comp7107::Cell::min_y) * 10.f / (comp7107::Cell::max_y - comp7107::Cell::min_y);
        if(cell_idx_x == 10)
        {
            cell_idx_x = 9;
        }
        if(cell_idx_y == 10)
        {
            cell_idx_y = 9;
        }
        auto& cur_grid = grid_info[cell_idx_x * 10 + cell_idx_y];

        ++cur_grid.num_of_records;
        cur_grid.idx_x = cell_idx_x;
        cur_grid.idx_y = cell_idx_y;
    }
    uint32_t characters = 0;
    std::sort(restaurants.begin(), restaurants.end(), comp7107::comp_with_cell_idx);

    std::ofstream grid_grid(data_path + "/grid.grid");
    uint32_t last_x = 10;
    uint32_t last_y = 10;
    for(const auto& restaurant : restaurants)
    {
        uint32_t cell_idx_x = (restaurant.x - comp7107::Cell::min_x) * 10.f / (comp7107::Cell::max_x - comp7107::Cell::min_x);
        uint32_t cell_idx_y = (restaurant.y - comp7107::Cell::min_y) * 10.f / (comp7107::Cell::max_y - comp7107::Cell::min_y);
        if(cell_idx_x == 10)
        {
            cell_idx_x = 9;
        }
        if(cell_idx_y == 10)
        {
            cell_idx_y = 9;
        }

        std::string out_str(std::to_string(restaurant.id) + ' ' + std::to_string(restaurant.x) + ' ' + std::to_string(restaurant.y) + '\n');
        // to avoid double from being rounded
        grid_grid << out_str;
        auto& belonging_grid = grid_info[cell_idx_x * 10 + cell_idx_y];
        if(last_x != cell_idx_x || last_y != cell_idx_y)
        {
            belonging_grid.begin_character_pos = characters;
            last_x = cell_idx_x;
            last_y = cell_idx_y;
        }
        characters += out_str.length();
    }

    std::ofstream grid_dir(data_path + "/grid.dir");

    grid_dir << std::to_string(comp7107::Cell::min_x) << ' ' <<std::to_string(comp7107::Cell::max_x) 
    << ' '  << std::to_string(comp7107::Cell::min_y) << ' ' <<std::to_string(comp7107::Cell::max_y) << '\n';

    for(int i = 0; i < 10; ++i)
    {
        for(int j = 0; j < 10; ++j)
        {
            if(grid_info[i * 10 + j].num_of_records == 0)
                continue;
            grid_dir << i << ' ' << j << ' ' << grid_info[i * 10 + j].begin_character_pos << ' ' << grid_info[i * 10 + j].num_of_records << '\n';
        }
    }
    
    return 0;
}   