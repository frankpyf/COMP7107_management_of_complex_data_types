#include "assignment1.hpp"
#include <unistd.h>
#include <fstream>
#include <random>

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
    sort(records.begin(), records.end());

    ///////////////////////////////////////
    // Part 1: see assignment1.hpp
    ///////////////////////////////////////
    int num_pairs = comp7107::num_of_pairs(records, k);\
    std::cout << "Among " << records.size() << " records" << '\n';
    std::cout << "There are " << num_pairs << " record pairs s.t |t1.elevation - t2.elevation| <= " << k << '\n';

    ////////////////////////////////////////////////////////
    // Part 2: 
    // I use cosine similarity for the first 10th attributes
    // And Jaccard for the binary attributes
    ////////////////////////////////////////////////////////
    comp7107::normalize(records);
    std::ofstream normalized_records("./normalized.data");
    for(const auto& record : records)
    {
        normalized_records << record << '\n';
    }

    std::srand(1000);
    float min_similarity = 1.0f;
    float max_similarity = 0.0f;
    float avg_similarity = 0.0f;
    constexpr int count = 499500;
    
    std::array<float, 1000> random_sample_idx{};

    for(int i = 0; i < 1000; ++i)
    {
        int sample_idx = i + std::rand() % 580;
        random_sample_idx[i] = sample_idx;
    }
    for(int i = 0; i < 1000; ++i)
    {
        for(int j = i + 1; j < 1000; ++j)
        {
            float similarity = comp7107::similarity(records[i], records[j]);
            min_similarity = std::min(similarity, min_similarity);
            max_similarity = std::max(similarity, max_similarity);
            avg_similarity += similarity;
        }
    }
    avg_similarity /= (1.0f * count);

    std::cout << "min similarity: " << min_similarity << '\n';
    std::cout << "max similarity: " << max_similarity << '\n';
    std::cout << "average similarity: " << avg_similarity << '\n';
    std::cout << std::flush;
    return 0;
}