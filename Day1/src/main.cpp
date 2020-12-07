#include <iostream>
#include <fstream>
#include <vector>
#include <MyMath.h>

#define TARGET_NUMBER 2020

int main()
{
    std::vector<uint16_t> entries;
    std::ifstream file("Expense Report.txt");
    uint16_t dynamicNumber;
    std::pair<uint16_t, uint16_t> pair;
    uint16_t triple[3]{ 0, 0, 0 };
    bool pairFound = false;

    while (file >> dynamicNumber)
    {
        entries.push_back(dynamicNumber);
        if (MyMath::Add(entries[0], dynamicNumber) == TARGET_NUMBER)
        {
            pair = std::make_pair(entries[0], dynamicNumber);
            break;
        }
    }
    for (uint16_t i = 0; i < entries.size(); ++i)
    {
        for (uint16_t j = 0; j < entries.size(); ++j)
        {
            if (MyMath::Add(entries[i], entries[j]) == 2020)
            {
                pair = std::make_pair(entries[i], entries[j]);
                std::cout << "Pair is : " << pair.first << " and " << pair.second << '\n';
                int mult = MyMath::Multiply(pair.first, pair.second);
                std::cout << "Pair multiplied returns " << mult << '\n';
                return EXIT_SUCCESS;
            }
        }
    }
    return EXIT_FAILURE;
}