#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <queue>
#include <iomanip>
#include <chrono>
#include <sstream>
#include <stack>
#include "assignment3.hpp"

int main(int argc, char* argv[])
{
    if(argc < 4)
    {
        std::cout << "Please type in all the info in the command line" << '\n';
        return 0;
    }
    std::unordered_map<std::string, int> nodes;
    std::vector<std::string> nodes_vec;
    std::unordered_map<int, std::vector<comp7107::AdjacencyInfo>> adjacency_lists;
    
    //////////////////////////////////////////
    // file io
    {
        std::string str_line;
        std::ifstream input_file(data_path + "/dict.tsv");
        while(std::getline(input_file, str_line))
        {
            int id;
            std::string name;
            comp7107::parse(str_line, '\t', id, name);
            nodes[name] = id;
            nodes_vec.push_back(name);
        }
        input_file.close();

        input_file.open(data_path + "/graph.tsv");

        while(std::getline(input_file, str_line))
        {
            int src_id;
            std::string following_stuff;
            // custom parse
            comp7107::parse(str_line, '\t', src_id);
            str_line.erase(0, str_line.find('\t') + 1);

            while(str_line.length() > 0)
            {
                std::string cur_info = str_line.substr(0, str_line.find(' '));
                if(cur_info.length() == 0)
                    break;
                long timestamp;
                int tgt_id;
                int sentiment;
                comp7107::parse(cur_info, ',', timestamp, tgt_id, sentiment);
                adjacency_lists[src_id].emplace_back(timestamp, tgt_id, sentiment);
                str_line.erase(0, str_line.find(' ') + 1);
            }
        }

    }
    // end of file io
    ///////////////////////////
    std::string src(argv[1]);
    std::string dst(argv[2]);
    std::string date(argv[3]);
    std::string time(argv[4]);
    date += time;

    std::tm unix_timestamp{};
    {
        std::stringstream ss(date);
        ss >> std::get_time(&unix_timestamp, "%Y-%m-%d %H:%M:%S");
    }
    std::time_t t = mktime(&unix_timestamp);
    
    int src_id = nodes[src];
    int dst_id = nodes[dst];
    long start_time = t;
    std::cout << "src id: " << src_id << '\n';
    std::cout << "dst id: " << dst_id << '\n';
    std::cout << "start timestamp: " << start_time << '\n';


    std::vector<long> dist(nodes.size(), std::numeric_limits<long>::max());
    // key: dst_id, value: src_id
    std::unordered_map<int, int> path;
    dist[src_id] = start_time;

    auto cmp = [&](int lhs, int rhs)
    {
        return dist[lhs] < dist[rhs];
    };

    std::vector<int> q;
    q.push_back(src_id);

    std::unordered_set<int> visited{};
    std::unordered_set<int> inserted{};

    while(!q.empty())
    {
        sort(q.begin(), q.end(), cmp);

        int closest_id = q.front();
        q.erase(q.begin());

        visited.insert(closest_id);

        if(closest_id == dst_id)
        {
            // do something
            char time_buf[80];
            std::tm ts;
            ts = *localtime(&dist[closest_id]);
            strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", &ts);
            std::string temp(time_buf);
            std::cout << "Arrival Time: " << temp << '\n';
            std::stack<std::string> path_stack{};

            int next_id = path[dst_id];
            while(next_id != src_id)
            {
                path_stack.push(nodes_vec[next_id]);
                next_id = path[next_id];
            }
            path_stack.push(nodes_vec[next_id]);
            std::cout << "Path: ";
            while(!path_stack.empty())
            {
                auto& cur_name = path_stack.top();
                std::cout << cur_name << " -> ";
                path_stack.pop();
            }
            std::cout << nodes_vec[dst_id] << '\n';
            return 0;
        }

        for(const auto& info : adjacency_lists[closest_id])
        {   
            if(visited.find(info.id) != visited.end() || info.unix_timestamp < dist[closest_id] || info.sentiment != 1)
                continue;
            
            if(info.unix_timestamp <= dist[info.id])
            {
                dist[info.id] = info.unix_timestamp;

                path[info.id] = closest_id;
            }
            
            if(inserted.find(info.id) == inserted.end())
            {
                q.push_back(info.id);
                inserted.insert(info.id);
            }
        }
    }
    std::cout << "No Path Found" << '\n';
    return 0;
}