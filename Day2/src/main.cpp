#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#define SHOULD_DEBUG 1

void ReadAndParsePasswordDatabase(const std::string& p_databaseName, std::vector<std::pair<uint16_t, uint16_t>>& p_bounds, std::vector<char>& p_keys, std::vector<std::string>& p_entries)
{
    std::ifstream file(p_databaseName);
    std::string line;

    while (std::getline(file, line))
    {   
#if (SHOULD_DEBUG == 1)
        std::cout << line << '\n';
#endif
        uint16_t firstNumStrIndex {0};
        uint16_t secondNumStrIndex {0};
        uint16_t keyIndex {0};
        
        if (line[firstNumStrIndex] == '\0')
            continue;
        
        while (line[firstNumStrIndex] != '-' && line[firstNumStrIndex] != '\0')
            firstNumStrIndex++;

        secondNumStrIndex = firstNumStrIndex;
        
        while (line[secondNumStrIndex] != ' ' && line[secondNumStrIndex] != '\0')
            secondNumStrIndex++;

        while (line[keyIndex] != ':' && line[keyIndex] != '\0')
            keyIndex++;
        
        if (line[firstNumStrIndex] == '\0' || line[secondNumStrIndex] == '\0' || line[keyIndex] == '\0')
        { 
            std::cerr << "Wait. That's illegal! Someone managed to send their password using their own rules !" << '\n';
            std::cerr << "DATABASE IS CORRUPTED ? SEARCH FOR " << line << " AND DELETE THIS LINE." << '\n';
            return;
        }
        if (line[firstNumStrIndex] == '-' && line[secondNumStrIndex] == ' ' && line[keyIndex] == ':')
        {
            //calculate bounds
            const std::string firstTextNumbers{line.substr(0, firstNumStrIndex)};
            const std::string secondTextNumbers{line.substr(firstNumStrIndex + 1, secondNumStrIndex - firstNumStrIndex - 1)};

            uint16_t max {0};
            uint16_t min {0};
            
            for (uint16_t i = 0; i < firstTextNumbers.size(); i++)
            {
                char digit = firstTextNumbers[i] - 48;
                min = (min * 10) + digit;  
            }
            
            for (uint16_t i = 0; i < secondTextNumbers.size(); i++)
            {
                char digit = secondTextNumbers[i] - 48;
                max = (max * 10) + digit;
            }

            p_bounds.push_back(std::make_pair(min,max));
#if (SHOULD_DEBUG == 1) 
            std::cout << "min is " << p_bounds[p_bounds.size() - 1].first << '\n';
            std::cout << "max is " << p_bounds[p_bounds.size() - 1].second << '\n';
#endif
            p_keys.push_back(line[keyIndex - 1]);
#if (SHOULD_DEBUG == 1)
            std::cout << "key is " << p_keys[p_keys.size() - 1] << '\n';
#endif
            const std::string entries{line.substr(keyIndex, line.size() - keyIndex)};
#if (SHOULD_DEBUG == 1)
            std::cout << "string is " << entries << '\n' << '\n';
#endif
            if (entries.size() == 0)
            { 
                std::cerr << "Wait. That's illegal! Someone managed to send their password using their own rules !" << '\n';
                std::cerr << "DATABASE IS CORRUPTED ? SEARCH FOR " << line << " AND DELETE THIS LINE." << '\n';
                continue;
            }
            
            p_entries.push_back(entries);
        }
    }
}

void ValidatePasswords(const std::vector<std::pair<uint16_t, uint16_t>>& p_bounds, const std::vector<char>& p_keys, const std::vector<std::string>& p_entries)
{
    unsigned int totalSledValidated{0};
    unsigned int totalTobogganValidated{0};

    if (p_bounds.size() != p_keys.size() || p_bounds.size() != p_entries.size())
    {
        std::cerr << "Something went wrong when going to validate the passwords. Data is corrupted." << '\n';
        std::cerr << "bounds size is : " << p_bounds.size() << " keys size is : " << p_keys.size() << " entries size is : " << p_entries.size() << '\n';
        return;
    }
    for (unsigned int i = 0; i < p_entries.size(); i++)
    {
#if (SHOULD_DEBUG)
        std::cout << "-----------\n" << "Searching for " << p_keys[i] << " in " << p_entries[i] << ". Min is " << p_bounds[i].first << " and max is " << p_bounds[i].second << '\n';
#endif
        const char& key {p_keys[i]};
        unsigned int currentIndex {0};
        uint16_t matchesFoundForSled {0};
        bool isSledValid {false};
        bool isTobogganValid {true};
        
        while (p_entries[i][currentIndex] != '\0')
        {    
            if (p_entries[i][currentIndex] == key)
                matchesFoundForSled++;
            currentIndex++;
        }
        
        if (matchesFoundForSled >= p_bounds[i].first && matchesFoundForSled <= p_bounds[i].second)
        {    
            totalSledValidated++;
            isSledValid = true;
        }

        if ((p_entries[i][p_bounds[i].first + 1] == key && p_entries[i][p_bounds[i].second + 1] == key) || (p_entries[i][p_bounds[i].first + 1] != key && p_entries[i][p_bounds[i].second + 1] != key))
            isTobogganValid = false;
        else
            totalTobogganValidated++;

#if (SHOULD_DEBUG == 1)
        std::cout << matchesFoundForSled << " Found For Sled. Password is valid for Sled ? : " << isSledValid << " Password is valid for Sled ? : " << isTobogganValid << '\n';
#endif
    }
    std::cout << "SLED PASSWORD DATABASE CORRECTLY READ AND VALIDATED. THERE WERE " << totalSledValidated << " PASSWORDS VALIDATED" << '\n';
    std::cout << "TOBOGGAN PASSWORD DATABASE CORRECTLY READ AND VALIDATED. THERE WERE " << totalTobogganValidated << " PASSWORDS VALIDATED" << '\n';
}

int main ()
{
    std::vector<std::pair<uint16_t, uint16_t>> bounds;
    std::vector<char> keys;
    std::vector<std::string> entries;
    ReadAndParsePasswordDatabase("input.txt", bounds, keys, entries);
    ValidatePasswords(bounds, keys, entries);
    return EXIT_SUCCESS;
}
