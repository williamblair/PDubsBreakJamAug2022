#ifndef HIGH_SCORE_DATA_H_INCLUDED
#define HIGH_SCORE_DATA_H_INCLUDED

#include <cstdint>

struct HighScoreData
{
    char name[3];
    uint64_t score;
};

#endif // HIGH_SCORE_DATA_H_INCLUDED
