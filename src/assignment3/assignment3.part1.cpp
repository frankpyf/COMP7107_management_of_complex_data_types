#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>
#include "assignment3.hpp"

int main(int argc, char* argv[])
{
    ///////////////////
    // Task1.1
    ///////////////////
    std::ifstream input_file(data_path + "/soc-redditHyperlinks-title.tsv");
    std::string str_line;
    std::unordered_set<std::string> unique_subreddit;
    std::getline(input_file, str_line);

    while(std::getline(input_file, str_line))
    {
        std::string src_reddit;
        std::string tgt_reddit;
        comp7107::parse(str_line, '\t', src_reddit, tgt_reddit);
        if(src_reddit.length() > 0)
            unique_subreddit.insert(src_reddit);
        if(tgt_reddit.length() > 0)
            unique_subreddit.insert(tgt_reddit);
    }
    input_file.clear();
    input_file.seekg (0, std::ios::beg);

    std::vector<std::string> dict_vec(unique_subreddit.begin(), unique_subreddit.end());
    std::sort(dict_vec.begin(), dict_vec.end());

    std::unordered_map<std::string, int> dict;

    std::ofstream dict_tsv(data_path + "/dict.tsv");
    for(size_t idx = 0; idx < dict_vec.size(); ++idx)
    {
        dict_tsv << idx << '\t' << dict_vec[idx] << '\n';
        dict[dict_vec[idx]] = idx;
    }
    dict_tsv.close();

    ///////////
    // Task1.2
    ///////////
    std::getline(input_file, str_line);

    std::vector<std::vector<comp7107::AdjacencyInfo>> adjacency_lists(dict_vec.size());

    while(std::getline(input_file, str_line))
    {
        std::string src_reddit;
        std::string tgt_reddit;
        std::string timestamp;
        std::string post_id;
        int sentiment;

        comp7107::parse(str_line, '\t', src_reddit, tgt_reddit, post_id, timestamp, sentiment);
        int src_id = dict[src_reddit];
        int tgt_id = dict[tgt_reddit];

        std::tm unix_timestamp{};
        std::stringstream ss(timestamp);
        ss >> std::get_time(&unix_timestamp, "%Y-%m-%d %H:%M:%S");
        std::time_t t = mktime(&unix_timestamp);
        
        comp7107::AdjacencyInfo temp(t, tgt_id, sentiment);

        if(find(adjacency_lists[src_id].begin(), adjacency_lists[src_id].end(), temp) == adjacency_lists[src_id].end())
            adjacency_lists[src_id].push_back(std::move(temp));
    }
    input_file.close();
    std::ofstream graph_tsv(data_path + "/graph.tsv");
    for(size_t idx = 0; idx < adjacency_lists.size(); ++idx)
    {
        if(adjacency_lists[idx].empty())
            continue;
        std::sort(adjacency_lists[idx].begin(), adjacency_lists[idx].end(), [](const auto& lhs, const auto& rhs){
            if(lhs.unix_timestamp < rhs.unix_timestamp)
            {
                return true;
            }
            else if(lhs.unix_timestamp == rhs.unix_timestamp)
            {
                return lhs.id < rhs.id;
            }
            return false;
        });

        graph_tsv << idx << '\t';
        for(const auto& info : adjacency_lists[idx])
        {
            graph_tsv << info.unix_timestamp << ',' << info.id << ',' << info.sentiment << ' ';
        }
        graph_tsv << '\n';
    }
    graph_tsv.close();

    return 0;
}