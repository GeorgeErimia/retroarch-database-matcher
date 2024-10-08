#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <math.h>
using namespace std;

int levenshteinRecursive(const std::string& str1,
    const std::string& str2, int m, int n)
{

    // str1 is empty
    if (m == 0) {
        return n;
    }
    // str2 is empty
    if (n == 0) {
        return m;
    }

    if (str1[m - 1] == str2[n - 1]) {
        return levenshteinRecursive(str1, str2, m - 1,
            n - 1);
    }

    return 1
        + std::min(

            // Insert
            levenshteinRecursive(str1, str2, m, n - 1),
            std::min(

                // Remove
                levenshteinRecursive(str1, str2, m - 1,
                    n),

                // Replace
                levenshteinRecursive(str1, str2, m - 1,
                    n - 1)));
}

// Function to compute the Levenshtein distance
int levenshteinDistance(const string& s1, const string& s2) {
    int len1 = s1.size();
    int len2 = s2.size();

    // Ensure that len1 is always smaller to minimize memory usage
    if (len1 > len2) {
        return levenshteinDistance(s2, s1);
    }

    // Create two vectors to store current and previous row values
    vector<int> prevRow(len1 + 1);
    vector<int> currRow(len1 + 1);

    // Initialize the first row (base case for comparing with empty string)
    for (int i = 0; i <= len1; i++) {
        prevRow[i] = i;
    }

    // Loop through the second string (s2)
    for (int j = 1; j <= len2; j++) {
        currRow[0] = j; // Base case for an empty s1
        for (int i = 1; i <= len1; i++) {
            int insertion = prevRow[i] + 1;  // Cost of insertion
            int deletion = currRow[i - 1] + 1; // Cost of deletion
            int substitution = prevRow[i - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1); // Cost of substitution

            // Choose the minimum of the three operations
            currRow[i] = min({ insertion, deletion, substitution });
        }
        prevRow = currRow; // Move to the next row
    }

    return currRow[len1]; // The result is in the last cell of currRow
}