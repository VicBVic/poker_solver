#include "ev_solver.hpp"
#include "poker_math.hpp"
#include "card_set.hpp"


void EVSolver::init()
{
	return;
}

void EVSolver::calculate_heads_up_matrix()
{
	return;
}

void EVSolver::read_heads_up_matrix(const char *filename)
{
	return;
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
