#include <iostream>
#include <fstream>
#include <vector>

#define TARGET_NUMBER 2020

void ReadFileToVectorOfUint(const std::string& p_filename, std::vector<uint16_t>& p_entries)
{
    std::ifstream file(p_filename);
    uint16_t dynamicNumber;
    while (file >> dynamicNumber)
        p_entries.push_back(dynamicNumber);
}

int ResolveCalculations(const std::string& p_filename)
{
    std::vector<uint16_t> entries;
    std::pair<uint16_t, uint16_t> pair;
    uint16_t triple[3]{ 0, 0, 0 };

    bool tripleFound = false;

    ReadFileToVectorOfUint(p_filename, entries);

    for (uint16_t i = 0; i < entries.size(); ++i)
    {
        for (uint16_t j = 0; j < entries.size(); ++j)
        {
            for (uint16_t k = 0; k < entries.size(); ++k)
            {
                if (!tripleFound && (entries[i] + entries[j] + entries[k]) == 2020)
                {
                    triple[0] = entries[i];
                    triple[1] = entries[j];
                    triple[2] = entries[k];
                    std::cout << "Triple is : " << triple[0] << " and " << triple[1] << " and " << triple[2] << '\n';
                    int product = triple[0] * triple[1] * triple[2];
                    std::cout << "Triple multiplied returns " << product << '\n';
                    tripleFound = true;
                }
            }

            if ((entries[i] + entries[j]) == 2020)
            {
                pair = std::make_pair(entries[i], entries[j]);
                std::cout << "Pair is : " << pair.first << " and " << pair.second << '\n';
                int product = pair.first * pair.second;
                std::cout << "Pair multiplied returns " << product << '\n';
            }

            if (pair.first != 0 && triple[0] != 0)
                return EXIT_SUCCESS;
        }
    }
    std::cerr << "Error : Something went wrong. Either your file was badly inserted or there were no compatible numbers inside of it.\n";
    std::cout << "You were looking for " << TARGET_NUMBER << '\n';
    return EXIT_FAILURE;
}

int main()
{
    return ResolveCalculations("Expense Report.txt");
}