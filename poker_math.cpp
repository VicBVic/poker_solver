#include "poker_math.hpp"

u_int64_t PokerMath::comb[CARDS+1][CARDS+1];
card_set_t PokerMath::all_hands[5][PokerMath::five_card_hand_count];

void PokerMath::init()
{
	calculate_combinations();
	generate_all_hands();
}

void PokerMath::calculate_combinations()
{
	for(int n=0;n<=CARDS;n++)
	{
		comb[n][0] = 1;
		comb[n][n] = 1;
		for(int k=1;k<n;k++)
			comb[n][k] = comb[n-1][k-1] + comb[n-1][k];
	}
}

u_int64_t PokerMath::n_choose_k(int n, int k)
{
	if(n < 0 || n > CARDS || k < 0 || k > n)
		return 0;
	return comb[n][k];
}

card_set_t PokerMath::kth_hand(int size, int index)
{
	if(size<=0) return card_set_t();
	return all_hands[size-1][index];
}

void PokerMath::generate_all_hands()
{
	for(int size=1;size<=5;size++)
	{
		u_int64_t total = n_choose_k(CARDS, size);
		card_set_t hand;
		
		for(int i=0;i<size;i++)
			hand.set(i);
		
		for(u_int64_t index=0;index<total;index++)
		{
			hand.reset();

			int bits_to_set = size;
			u_int64_t search_index = index;
			for(int card=0;card<CARDS && bits_to_set>0;card++)
			{
				u_int64_t combinations_with_card = n_choose_k(CARDS - card - 1, bits_to_set - 1);
				if(search_index < combinations_with_card)
				{
					hand.set(card);
					bits_to_set--;
				}
				else
				{
					search_index -= combinations_with_card;
				}
			}

			all_hands[size-1][index] = hand;
		}
	}
}