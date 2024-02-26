#include <algorithm>
#include <execution>
#include <vector>
#include <array>
#include "assignment2.hpp"

int main()
{
    std::ifstream input_file("./Beijing_restaurants.txt");
    std::string str_line;
    std::getline(input_file, str_line);

    std::vector<comp7107::Restaurant> restaurants;
    std::array<std::array<std::pair<uint32_t, uint32_t>, 10>, 10> grid_info;
    for(int i = 0; i < 10; ++i)
    {
        for(int j = 0; j < 10; ++j)
        {
            grid_info[i][j] = std::pair<uint32_t, uint32_t>(std::numeric_limits<uint32_t>::max(), 0);
        }
    }

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
    float unit_length_x = (max_x - min_x) / 10;
    float unit_length_y = (max_y - min_y) / 10;
    ///////////////////////////////////////
    // Part 1: 
    ///////////////////////////////////////

    // set belonging cell 
    // and get the number of records in each cell
    for(auto& restaurant : restaurants)
    {
        restaurant.cell_x = (restaurant.x - min_x) / unit_length_x;
        restaurant.cell_y = (restaurant.y - min_y) / unit_length_y;

        ++grid_info[restaurant.cell_x][restaurant.cell_y].second;
    }
    uint32_t characters = 0;
    std::sort(restaurants.begin(), restaurants.end(), comp7107::comp_with_cell_idx);
    std::ofstream grid_grid("./grid.grid");
    for(const auto& restaurant : restaurants)
    {
        std::string out_str(std::to_string(restaurant.id) + ' ' + std::to_string(restaurant.x) + ' ' + std::to_string(restaurant.y) + '\n');
        // to avoid double from being rounded
        grid_grid << out_str;
        auto& belonging_grid = grid_info[restaurant.cell_x][restaurant.cell_y];
        belonging_grid.first = std::min(characters, belonging_grid.first);
        characters += out_str.length();
    }
    grid_grid.close();
    std::ofstream grid_dir("./grid.dir");
    grid_dir << std::to_string(min_x) << ' ' <<std::to_string(max_x) << ' '  << std::to_string(min_y) << ' ' <<std::to_string(max_y) << '\n';
    for(int i = 0; i < 10; ++i)
    {
        for(int j = 0; j < 10; ++j)
        {
            if(grid_info[i][j].second == 0)
                continue;
            grid_dir << i << ' ' << j << ' ' << grid_info[i][j].first << ' ' << grid_info[i][j].second << '\n';
        }
    }
    grid_dir.close();

    ///////////////////////////////////////
    // Part 2: 
    ///////////////////////////////////////
    


    return 0;
}   