#include "assignment1.hpp"
#include <unistd.h>
#include <fstream>
#include <random>
#include <assert.h>
#include <algorithm>

int main(int argc, char* argv[])
{
    int ch = -1;
    uint16_t k = 0;
    while((ch = getopt(argc, argv, "k:")) != -1)
    {
        switch(ch)
        {
            case 'k':
                k = std::stoi(optarg);
                break;
        }
    }

    std::vector<comp7107::ForestCoverType> records;

    std::ifstream input_file("./covtype.data");
    std::string str_line;
    while(std::getline(input_file, str_line))
    {
        auto& cur_record = records.emplace_back();
        comp7107::string_to_forest_type(str_line, cur_record);
    }
    std::sort(records.begin(), records.end());

    ///////////////////////////////////////
    // Part 1: 
    ///////////////////////////////////////
    int num_pairs = comp7107::num_of_pairs(records, k);\
    std::cout << "Among " << records.size() << " records" << '\n';
    std::cout << "There are " << num_pairs << " record pairs s.t |t1.elevation - t2.elevation| <= " << k << '\n';
    std::cout << '\n';

    ///////////////////////////////////////
    // Part 2: 
    ///////////////////////////////////////
    comp7107::normalize(records);
    std::ofstream normalized_records("./normalized.data");
    for(const auto& record : records)
    {
        normalized_records << record << '\n';
    }

    auto compute_and_report_similarity = [&records](const std::array<size_t, 1000>& in_idx){
        float min_similarity = 1.0f;
        float max_similarity = 0.0f;
        float avg_similarity = 0.0f;
        constexpr int count = 499500;

        for(int i = 0; i < 1000; ++i)
        {
            for(int j = i + 1; j < 1000; ++j)
            {
                int lhs = in_idx[i];
                int rhs = in_idx[j];
                float similarity = comp7107::similarity(records[lhs], records[rhs]);
                min_similarity = std::min(similarity, min_similarity);
                max_similarity = std::max(similarity, max_similarity);
                avg_similarity += similarity;
            }
        }
        avg_similarity /= (1.0f * count);

        std::cout << "min similarity: " << min_similarity << '\n';
        std::cout << "max similarity: " << max_similarity << '\n';
        std::cout << "average similarity: " << avg_similarity << '\n';
        std::cout << '\n';
    };

    // 2.2
    std::srand(1000);
    std::array<size_t, 1000> sample_idx{};
    for(int i = 0; i < 1000; ++i)
    {
        int random_idx = i + std::rand() % 580;
        sample_idx[i] = random_idx;
    }
    std::cout << "similarity report of random samples: " << '\n';
    compute_and_report_similarity(sample_idx);

    // 2.3
    // Choose 1000 samples that have the same cover type
    auto get_sample_idx_of_cover_type = [&records](std::array<size_t, 1000>& sample_idx, int cover_type){
        assert(cover_type >= 1 && cover_type <=7);
        int num = 0;
        for(int i = 0; i < records.size(); ++i)
        {
            if(records[i].cover_type != cover_type)
                continue;
            sample_idx[num++] = i;
            if(num == 1000)
                break;
        }
    };
    for(int i = 1; i < 8; ++i)
    {
        std::cout << "similarity report of 1000 samples that have cover type" << i << ':' << '\n';
        get_sample_idx_of_cover_type(sample_idx, i);
        compute_and_report_similarity(sample_idx);
        std::cout << '\n';
    }

    std::cout << std::flush;
    return 0;
}