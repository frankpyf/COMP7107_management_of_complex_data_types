#include <vector>
#include <array>
#include <iostream>
#include <string>
#include <bitset>
#include <memory>
#include <thread>
#include <sstream>

#define STRING(x) #x
#define XSTRING(x) STRING(x)

namespace comp7107
{
    /**
     * @brief size: 64 bytes  
     * would love to make them int or uint to save more space
     * but for nomalization, i have to make them float.
     * 
     * And...cpp doesn't have reflection, so i collect quantative attributes into an array
     */
    struct ForestCoverType
    {
        ForestCoverType() = default;
        std::array<float, 10> quantative_attributes{0.0f};
        // Wilderness area designation
        std::bitset<4> wilderness_area;
        // Soil Type
        std::bitset<40> soil_type;
        // Forest Cover Type designation
        int cover_type = 1;
    };

    std::ostream& operator<<(std::ostream& os, const ForestCoverType& forest)
    {
        for(const float& quantative : forest.quantative_attributes)
        {
            os << quantative << ", ";
        }
        os << forest.wilderness_area << ", " << forest.soil_type << ", " << forest.cover_type;
        return os;
    }

    bool operator<(const ForestCoverType& lhs, const ForestCoverType& rhs)
    {
        return lhs.quantative_attributes[0] < rhs.quantative_attributes[0];
    }

    /**
     * @brief Take in a str and convert it into a Forest Cover Type
     * 
     * @param str 
     * @param forest default Forest Cover Type entity
     */
    void string_to_forest_type(const std::string& str, ForestCoverType& forest)
    {
        std::stringstream ss(str);
        std::string num;
        auto split = [&]() -> int{
            std::getline(ss, num, ',');
            return std::stoi(num);
        };
        
        for(auto& quantative : forest.quantative_attributes)
        {
            quantative = split();
        }

        std::string s;
        for(int i = 0; i < 4; ++i)
        {
            std::getline(ss, num, ',');
            s += num;
        }
        forest.wilderness_area = std::bitset<4>(s);
        s.clear();

        for(int i = 0; i < 40; ++i)
        {
            std::getline(ss, num, ',');
            s += num;
        }
        forest.soil_type = std::bitset<40>(s);
        s.clear();

        forest.cover_type = split();
    }

    void normalize(std::vector<ForestCoverType>& sorted_records)
    {
        std::array<float, 10> min{std::numeric_limits<float>::max()};
        std::array<float, 10> max{0.0f};
        min[0] = sorted_records[0].quantative_attributes[0];
        max[0] = sorted_records.back().quantative_attributes[0];

        for(const auto& record : sorted_records)
        {
            for(int i = 1; i < 10; ++i)
            {
                min[i] = std::min(min[i], record.quantative_attributes[i]);
                max[i] = std::max(max[i], record.quantative_attributes[i]);
            }
        }
        for(auto& record : sorted_records)
        {
            for(int i = 0; i < 10; ++i)
            {
                record.quantative_attributes[i] = (record.quantative_attributes[i] - min[i]) / (max[i] - min[i]);
            }
        }
    }

    //////////////////////////////////
    // Solutions
    //////////////////////////////////

    /**
     * @brief Compute and return the number of observation record pairs whose difference in the first column is at most k. 
     * 
     * @param sorted_records sorted data table
     * @param k a small non-negative integer k
     * @return int
     */
    int num_of_pairs(const std::vector<ForestCoverType>& sorted_records, uint16_t k)
    {
        int right = 0;
        int nums = 0;

        // we are talking of pairs, so we only need to iterate to the element before the last element
        for(int left = 0; left < sorted_records.size() - 1; ++left)
        {
            while((sorted_records[right].quantative_attributes[0] - sorted_records[left].quantative_attributes[0]) <= k)
            {
                // shouldn't be able to move even further
                if(right == (sorted_records.size() - 1))
                    break;
                ++right;
            }
            while((sorted_records[right].quantative_attributes[0] - sorted_records[left].quantative_attributes[0]) > k)
            {
                --right;
            }
            nums += (right - left);
        }
        return nums;
    }
 
    /**
     * @brief compute similarity between two records. 
     * For quantative attributes, we compute their similarities based on their distance. See slides 91 of 01 datatypes.pptx
     * For binary attributes, we use jaccard to compute their similarities.
     * Finally, we use general approach for combining similarities.
     * 
     * @param f1 
     * @param f2 
     * @return float 
     */
    float similarity(const ForestCoverType& f1, const ForestCoverType& f2)
    {
        float similarity = 0.0f;
        int sum_delta = 0;
        // Quantative attributes
        for(int i = 0; i < 10; ++i)
        {
            float length = std::abs(f1.quantative_attributes[i] - f2.quantative_attributes[i]);
            similarity += 1.0f / (1.0f + length);
            ++sum_delta;
        }
        // Binary attributes
        for(int i = 0; i < 4; ++i)
        {
            if(f1.wilderness_area[i] == 0 && f2.wilderness_area[i] == 0)
                continue;
            similarity += f1.wilderness_area[i] * f2.wilderness_area[i];
            ++sum_delta;
        }
        for(int i = 0; i < 40; ++i)
        {
            if(f1.soil_type[i] == 0 && f2.soil_type[i] == 0)
                continue;
            similarity += f1.soil_type[i] * f2.soil_type[i];
            ++sum_delta;
        }
        return similarity / (sum_delta * 1.0f);
    }
} // namespace comp7107