/**
 * \file NPuzzleSetting.hpp
 * \brief Define the demonstration and moving cost for the N-Puzzle problem.
 */
#ifndef __NPUZZLE_SETTING__
#define __NPUZZLE_SETTING__

namespace NPuzzle
{
    //! The n-puzzle demonstration, could be change to 15, 25, etc.
    constexpr int demonstration = 8;
    //! The cost of every operation (up, down, left, right) on blank
    constexpr int moveCost = 1;
    /**
     * \brief An n-puzzle demonstration would generate a \c AxA matrix,
     * where A equals to <tt>sqrt(demonstration + 1)</tt>.
     */
    const int matrixDemonstration = (int)sqrt(demonstration + 1);
}

#endif