#include "ev_solver.hpp"
#include "poker_math.hpp"
#include "card_set.hpp"

#include <filesystem>
#include <cassert>


const char EVSolver::heads_up_matrix_location[] = "data/heads_up_matrix";
int EVSolver::heads_up_matrix[2][total_hole_combinations][total_hole_combinations];

void EVSolver::init()
{
	if(std::filesystem::exists(heads_up_matrix_location))
	{
		read_heads_up_matrix();
	}
	else
	{
		calculate_heads_up_matrix();
		save_heads_up_matrix();
	}
	return;
}

void EVSolver::calculate_heads_up_matrix()
{
	return;
}

void EVSolver::read_heads_up_matrix()
{
	FILE *in = fopen(heads_up_matrix_location,"rb");
	assert(in);

	int read_bytes = fread(heads_up_matrix, 1, sizeof(heads_up_matrix), in);
	assert(read_bytes == sizeof(heads_up_matrix));

	assert(fclose(in)==0);
}

void EVSolver::save_heads_up_matrix()
{
	FILE *out = fopen(heads_up_matrix_location, "wb");

	assert(out);

	int checksum = fwrite(heads_up_matrix, 1, sizeof(heads_up_matrix), out);
	assert(checksum == sizeof(heads_up_matrix));

	assert(fclose(out)==0);
}

EVResponse EVSolver::get_heads_up_ev(card_set_t hero_cards, card_set_t villain_cards, card_set_t community_cards)
{
	card_set_t all_cards = hero_cards | villain_cards | community_cards;

	int to_be_dealt_cards = 5 - community_cards.count();

	EVResponse response = {0, 0, 0, 0, 0};

	for(int i=0;i<PokerMath::n_choose_k(CARDS, to_be_dealt_cards);i++)
	{
		card_set_t additional_community = PokerMath::kth_hand(to_be_dealt_cards, i);

		if((additional_community & all_cards).any())
		continue;

		//std::cerr<<"here "<<i<<' '<<additional_community<<'\n';

		card_set_t full_community = community_cards | additional_community;
		card_set_t all_hero = hero_cards | full_community;
		card_set_t all_villain = villain_cards | full_community;
		
		poker_hand_t hero_best = best_poker_hand(all_hero);
		poker_hand_t villain_best = best_poker_hand(all_villain);
		
		if(hero_best > villain_best)
		{
			response.winning_hands++;
		}
		else if(hero_best == villain_best)
		{
			int high_card_cmp = compare_high_card(all_hero, all_villain);
			
			if(high_card_cmp > 0) response.winning_hands++;
			else if(high_card_cmp == 0) response.drawing_hands++;
		}
		
		response.total_hands++;
	}

	response.win_percentage = response.winning_hands*100.0 / response.total_hands;
	response.draw_percentage = response.drawing_hands*100.0 / response.total_hands;

	return response;
}

EVResponse EVSolver::get_multiway_ev(card_set_t hero_cards, const std::vector<card_set_t> &opponents_hands, card_set_t community_cards)
{
	return EVResponse();
}
