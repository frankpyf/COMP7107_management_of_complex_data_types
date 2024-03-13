#include <fstream>
#include "assignment2.hpp"
#include <queue>
#include <vector>
#include <array>
#include <sstream>
#include <unordered_set>

double comp7107::Cell::min_x = 0.0f;
double comp7107::Cell::max_x = 0.0f;
double comp7107::Cell::min_y = 0.0f;
double comp7107::Cell::max_y = 0.0f;

double q_x = 0.0f;
double q_y = 0.0f;

std::string grid_grid{};
std::array<comp7107::Cell, 100> cell_infos;
std::unordered_set<uint32_t> visited{};

namespace comp7107
{
    class NearestNeighbor 
    {
        template<typename T>
        struct comp_with_dist_to_q
        {
            bool operator() (const T& r1, const T& r2)
            {
                Restaurant r(q_x, q_y);
                return compute_euclidean_distance(r, r1) > compute_euclidean_distance(r, r2);
            }
        };
        using priority_queue_for_points = std::priority_queue<Restaurant, std::vector<Restaurant>, comp_with_dist_to_q<Restaurant>>;
        using priority_queue_for_cells  = std::priority_queue<Cell, std::vector<Cell>, comp_with_dist_to_q<Cell>>;
        // one heap for points
        priority_queue_for_points heap_p_;
        // one heap for cell
        priority_queue_for_cells heap_c_;

    public:
        NearestNeighbor()
        {
            // initialize with all the points within the cell that holds query point p
            uint32_t idx_x = (q_x - comp7107::Cell::min_x) * 10.f / (comp7107::Cell::max_x - comp7107::Cell::min_x);
            uint32_t idx_y = (q_y - comp7107::Cell::min_y) * 10.f / (comp7107::Cell::max_y - comp7107::Cell::min_y);
            std::cout << "pushed in cell :" << idx_x << ", " << idx_y << '\n';

            auto& belonging_cell = cell_infos[idx_x * 10 + idx_y];

            visited.insert(idx_x * 10 + idx_y);

            push_surrounding_cells(belonging_cell);

            uint32_t offset = belonging_cell.begin_character_pos;
            uint32_t iterations = belonging_cell.num_of_records;
            uint32_t pos = offset;
            for(int i = 0; i < iterations; ++i)
            {
                pos = grid_grid.find('\n', offset);
                auto line = grid_grid.substr(offset, pos - offset);
                comp7107::Restaurant cur_restaurant;
                comp7107::parse_string_by_blank(line, cur_restaurant.id, cur_restaurant.x, cur_restaurant.y);
                    
                heap_p_.push(cur_restaurant);

                offset = pos + 1;
            }
        };
        const Restaurant& get_next_nearest_restaurant()
        {
            const auto& restaurant = heap_p_.top();
            heap_p_.pop();
            Restaurant query_restaurant(q_x, q_y);
            std::cout << "euclidean distance: " << compute_euclidean_distance(restaurant, query_restaurant) << '\n';
            
            // deheap the next nearest cell, and add all the points in that cell to the heap
            if(heap_p_.empty())
            {
                if(heap_c_.empty())
                    return restaurant;
                auto& in_cell = heap_c_.top();
                heap_c_.pop();
                std::cout << "de-heap cell (" << in_cell.idx_x << ", " << in_cell.idx_y << ')';
                push_surrounding_cells(in_cell);

                uint32_t offset = in_cell.begin_character_pos;
                uint32_t iterations = in_cell.num_of_records;
                uint32_t pos = offset;
                for(int i = 0; i < iterations; ++i)
                {
                    pos = grid_grid.find('\n', offset);
                    auto line = grid_grid.substr(offset, pos - offset);
                    comp7107::Restaurant cur_restaurant;
                    comp7107::parse_string_by_blank(line, cur_restaurant.id, cur_restaurant.x, cur_restaurant.y);
                    
                    heap_p_.push(cur_restaurant);

                    offset = pos + 1;
                }
            }
            
            return restaurant;
        }
    private:
            
        void push_surrounding_cells(const Cell& in_cell)
        {
            for(int i = -1; i < 2; ++i)
            {
                for(int j = -1; j < 2; ++j)
                {
                    int idx_cur_x = in_cell.idx_x + i;
                    int idx_cur_y = in_cell.idx_y + j;

                    if(idx_cur_x < 0 || idx_cur_y < 0 || idx_cur_x > 9 || idx_cur_y > 9)
                        continue;

                    if(visited.find(idx_cur_x * 10 + idx_cur_y) != visited.end())
                        continue;
                        
                    heap_c_.push(cell_infos[idx_cur_x * 10 + idx_cur_y]);
                    visited.insert(idx_cur_x * 10 + idx_cur_y);
                    std::cout << "pushed in cell :" << idx_cur_x << ", " << idx_cur_y << '\n';
                }
            }        
        }
    };
} // namespace comp7107

int main(int argc, char* argv[])
{
    int k = 0;

    if(argc > 3)
    {
        k = std::stoi(argv[1]);
        q_x = std::stod(argv[2]);
        q_y = std::stod(argv[3]);
    }

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
        cell.begin_character_pos = begin_character_pos;
        cell.num_of_records = num_of_records;
        cell.idx_x = idx_x;
        cell.idx_y = idx_y;
    }
    input_file.close();

    input_file.open(data_path + "/grid.grid");
    std::ostringstream str_stm;
    str_stm << input_file.rdbuf() ;
    grid_grid = str_stm.str();

    comp7107::NearestNeighbor nn;

    for(int i = 0; i < k; ++i)
    {
        auto& cur_restaurant = nn.get_next_nearest_restaurant();
        std::cout << cur_restaurant.id << ' ' << std::to_string(cur_restaurant.x) << ' ' << std::to_string(cur_restaurant.y) << '\n';
    }
    
    return 0;
}