#pragma once
#include <vector>
#include <utility>
#include <iostream>
#include <map>
#include <cstdlib>  // For qsort

// Comparator function for qsort
int cmp(const void* a, const void* b)
{
    const std::pair<std::string, int>* pairA = (const std::pair<std::string, int>*)a;
    const std::pair<std::string, int>* pairB = (const std::pair<std::string, int>*)b;

    // Compare the second values (integers)
    if (pairA->second < pairB->second) return -1;
    else if (pairA->second > pairB->second) return 1;
    else return 0;
}

// Function to sort the map according to value in (key-value) pairs using qsort
pair<string, int> getSortedMapFirstValue(std::map<std::string, int>& M)
{
    // Declare vector of pairs
    std::vector<std::pair<std::string, int>> A;

    // Copy key-value pair from Map to vector of pairs
    for (const auto& it : M) {
        A.push_back(it);
    }

    // Sort using qsort
    qsort(&A[0], A.size(), sizeof(std::pair<std::string, int>), cmp);

    //// Print the sorted value
    //for (const auto& it : A) {
    //    std::cout << it.first << ' ' << it.second << std::endl;
    //}

    return A[0];
}
