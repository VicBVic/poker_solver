#pragma once

#include <bitset>

#define SUITS 4
#define RANKS 13
#define CARDS 52

typedef std::bitset<52> card_set_t;

std::ostream& operator<<(std::ostream& os, const card_set_t& cards);
std::istream& operator>>(std::istream& is, card_set_t& cards);

int bit_of_rank_suit(int rank, int set);

card_set_t filter_rank(card_set_t cards, int rank);
card_set_t filter_suit(card_set_t cards, int suit);
bool filter_rank_suit(card_set_t cards, int rank, int suit);

int highest_rank(card_set_t cards);

int compare_high_card(card_set_t a, card_set_t b);