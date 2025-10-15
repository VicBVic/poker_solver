#pragma once

#include "card_set.hpp"

class PokerMath
{
	static const int five_card_hand_count = 2598960;
	static u_int64_t comb[CARDS+1][CARDS+1];
	static card_set_t all_hands[5][five_card_hand_count];
	
	static void calculate_combinations();
	static void generate_all_hands();

public:

	static void init();

	static u_int64_t n_choose_k(int n, int k);
	static card_set_t kth_hand(int size, int index);

};