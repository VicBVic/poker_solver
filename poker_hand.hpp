#pragma once

#include <bitset>
#include <iostream>
#include "card_set.hpp"

#define S_FLUSH 7
#define FOUR 6
#define FULL 5
#define FLUSH 4
#define STRAIGHT 3
#define THREE 2
#define TWO_PAIR 1
#define ONE_PAIR 0

// 8 maini importante:
// straight flush
// four of a kind
// etc.
// --> 3 biti
// 5 valori de maxim 13 pentru maini de tip flush
// --> 4*5 biti

// total: 3 + 20 = 23 biti

typedef std::bitset<23> poker_hand_t;

bool operator<(const poker_hand_t& a, const poker_hand_t& b);
bool operator>(const poker_hand_t& a, const poker_hand_t& b);
poker_hand_t max(const poker_hand_t& a, const poker_hand_t& b);

std::ostream& operator<<(std::ostream& os, const poker_hand_t& hand);

poker_hand_t poker_hand_from_array(int values[5], int category);
poker_hand_t get_kickers(card_set_t cards, int count);

poker_hand_t best_straight_flush(card_set_t cards);
poker_hand_t best_four_of_a_kind(card_set_t cards);
poker_hand_t best_full_house(card_set_t cards);
poker_hand_t best_flush(card_set_t cards);
poker_hand_t best_straight(card_set_t cards);
poker_hand_t best_three_of_a_kind(card_set_t cards);
poker_hand_t best_two_pair(card_set_t cards);
poker_hand_t best_one_pair(card_set_t cards);



poker_hand_t best_poker_hand(card_set_t cards);