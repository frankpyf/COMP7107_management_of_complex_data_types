#include <algorithm>
#include <execution>
#include <vector>
#include <array>
#include <limits>
#include "assignment2.hpp"

int main(int argc, char* argv[])
{
    std::ifstream input_file(data_path + "/Beijing_restaurants.txt");
    std::string str_line;
    std::getline(input_file, str_line);

    std::vector<comp7107::Restaurant> restaurants;
    std::array<std::array<comp7107::Cell, 10>, 10> grid_info;

    double min_x = std::numeric_limits<double>::max();
    double min_y = std::numeric_limits<double>::max();
    double max_x = std::numeric_limits<double>::min();
    double max_y = std::numeric_limits<double>::min();
        
    while(std::getline(input_file, str_line))
    {
        std::string token = str_line.substr(0, str_line.find(' '));
        double x = std::stod(token);
        token = str_line.substr(str_line.find(' ') + 1, str_line.length());
        double y = std::stod(token);
        auto& restaurant = restaurants.emplace_back(restaurants.size() + 1, x, y);
            
        if(x < min_x)
            min_x = x;
        if(x > max_x)
            max_x = x;
        if(y < min_y)
            min_y = y;
        if(y > max_y)
            max_y = y;
    }
    input_file.close();

    float unit_length_x = (max_x - min_x) / 10;
    float unit_length_y = (max_y - min_y) / 10;

    // set belonging cell 
    // and get the number of records in each cell
    for(auto& restaurant : restaurants)
    {
        restaurant.cell_x = (restaurant.x - min_x) / unit_length_x;
        restaurant.cell_y = (restaurant.y - min_y) / unit_length_y;

        ++grid_info[restaurant.cell_x][restaurant.cell_y].num_of_records;
    }
    uint32_t characters = 0;
    std::sort(restaurants.begin(), restaurants.end(), comp7107::comp_with_cell_idx);

    std::ofstream grid_grid(data_path + "/grid.grid");
    uint32_t last_x = 10;
    uint32_t last_y = 10;
    for(const auto& restaurant : restaurants)
    {
        std::string out_str(std::to_string(restaurant.id) + ' ' + std::to_string(restaurant.x) + ' ' + std::to_string(restaurant.y) + '\n');
        // to avoid double from being rounded
        grid_grid << out_str;
        auto& belonging_grid = grid_info[restaurant.cell_x][restaurant.cell_y];
        if(last_x != restaurant.cell_x || last_y != restaurant.cell_y)
        {
            belonging_grid.begin_character_pos = characters;
            last_x = restaurant.cell_x;
            last_y = restaurant.cell_y;
        }
        characters += out_str.length();
    }

    std::ofstream grid_dir(data_path + "/grid.dir");
    grid_dir << std::to_string(min_x) << ' ' <<std::to_string(max_x) << ' '  << std::to_string(min_y) << ' ' <<std::to_string(max_y) << '\n';
    for(int i = 0; i < 10; ++i)
    {
        for(int j = 0; j < 10; ++j)
        {
            if(grid_info[i][j].num_of_records == 0)
                continue;
            grid_dir << i << ' ' << j << ' ' << grid_info[i][j].begin_character_pos << ' ' << grid_info[i][j].num_of_records << '\n';
        }
    }
    
    return 0;
}   