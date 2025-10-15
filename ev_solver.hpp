#pragma once

#include "card_set.hpp"
#include "poker_hand.hpp"

#include <vector>

struct EVResponse{
	double win_percentage;
	double draw_percentage;
	int winning_hands;
	int drawing_hands;
	int total_hands;
};

class EVSolver
{
	static const int total_card_combinations = 133784560; // 52 choose 7
	static poker_hand_t hand_rankings_mem[total_card_combinations];
	static int heads_up_matrix_win[CARDS][CARDS][CARDS][CARDS];
	static int heads_up_matrix_tie[CARDS][CARDS][CARDS][CARDS];
	
	static bool heads_up_matrix_computed;

	public:
	static void init();
	
	static void calculate_heads_up_matrix();
	
	static void read_heads_up_matrix(const char* filename);
	static EVResponse get_heads_up_ev(card_set_t hero_cards, card_set_t villain_cards, card_set_t community_cards = card_set_t());
	
	static EVResponse get_multiway_ev(card_set_t hero_cards, const std::vector<card_set_t>& opponents_hands, card_set_t community_cards = card_set_t());


};