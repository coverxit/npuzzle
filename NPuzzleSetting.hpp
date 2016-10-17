#ifndef __NPUZZLE_SETTING__
#define __NPUZZLE_SETTING__

namespace NPuzzle
{
    // The n-puzzle demonstration, could be change to 15, 25, etc.
    constexpr int demonstration = 8;
    // The cost of every operation (up, down, left, right) on blank
    constexpr int moveCost = 1;
    // An n-puzzle demonstration would generate a AxA matrix,
    // where A equals to sqrt(demonstration + 1)
    const int matrixDemonstration = (int)sqrt(demonstration + 1);
}

#endif