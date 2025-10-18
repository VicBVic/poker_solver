#include "poker_hand.hpp"

#include "card_format.hpp"

bool operator<(const poker_hand_t &a, const poker_hand_t &b)
{
	return a.to_ulong() < b.to_ulong();
}

bool operator>(const poker_hand_t &a, const poker_hand_t &b)
{
	return a.to_ulong() > b.to_ulong();
}

poker_hand_t max(const poker_hand_t &a, const poker_hand_t &b)
{
	return a > b ? a : b;
}

std::ostream &operator<<(std::ostream &os, const poker_hand_t &hand)
{
	int category = (hand >> 20).to_ulong();
	int values[5];
	for (int i = 0; i < 5; i++)
	{
		values[i] = (hand >> (16 - 4 * i)).to_ulong() & 0xF;
	}

	switch (category)
	{
	case S_FLUSH:
		os<<"Straight Flush, high card "<<verbose_rank(values[0]);
		break;
	case FOUR:
		os<<"Four of a Kind, "<<verbose_rank(values[0])<<"s, kicker "<<verbose_rank(values[1]);
		break;
	case FULL:
		os<<"Full House, "<<verbose_rank(values[0])<<"s full of "<<verbose_rank(values[1]);
		break;
	case FLUSH:
		os<<"Flush, high card "<<verbose_rank(values[0])<<", kickers "<<verbose_rank(values[1])<<", "<<verbose_rank(values[2])<<", "<<verbose_rank(values[3])<<", "<<verbose_rank(values[4]);
		break;
	case STRAIGHT:
		os<<"Straight, high card "<<verbose_rank(values[0]);
		break;
	case THREE:
		os<<"Three of a Kind, "<<verbose_rank(values[0])<<"s, kickers "<<verbose_rank(values[1])<<", "<<verbose_rank(values[2]);
		break;
	case TWO_PAIR:
		os<<"Two Pair, "<<verbose_rank(values[0])<<"s and "<<verbose_rank(values[1])<<"s, kicker "<<verbose_rank(values[2]);
		break;
	case ONE_PAIR:
		os<<"One Pair, "<<verbose_rank(values[0])<<"s, kickers "<<verbose_rank(values[1])<<", "<<verbose_rank(values[2])<<", "<<verbose_rank(values[3]);
		break;
	default:
		break;
	}
	return os;
}

poker_hand_t poker_hand_from_array(int values[5], int category)
{
	return (values[0]<<16) + (values[1]<<12) + (values[2]<<8) + 
			(values[3]<<4) + values[4] + (category<<20);
}

poker_hand_t get_kickers(card_set_t cards, int count)
{
	poker_hand_t kickers;
	for(int i=0;i<count;i++)
	{
		int rank = highest_rank(cards);
		if(rank == -1)
			break;
		kickers<<=4;
		kickers |= rank;
		cards &= ~filter_rank(cards, rank);
	}
	return kickers;
}

poker_hand_t best_straight_flush(card_set_t cards)
{
	poker_hand_t best_hand;
	for(int suit=0;suit< SUITS; suit++)
	{
		best_hand = max(best_hand, best_straight(filter_suit(cards, suit)));
	}

	if(best_hand.none())
		return best_hand;
	
	best_hand &= (1<<20)-1; // sterg categoria
	best_hand |= (S_FLUSH<<20); // adaug categoria

	return best_hand;
}

poker_hand_t best_four_of_a_kind(card_set_t cards)
{
	poker_hand_t best_hand;
	for(int rank=RANKS-1;rank>=0;rank--)
	{
		card_set_t four = filter_rank(cards, rank);
		if(four.count() >= 4)
		{
			best_hand = FOUR<<20; // adaug categoria
			best_hand |= rank<<16; // adaug valoarea mainii
			best_hand |= (get_kickers(cards & ~four, 1)<<12); // adaug kickerul
			break;
		}
	}
	return best_hand;
}

poker_hand_t best_full_house(card_set_t cards)
{
	poker_hand_t best_three = best_three_of_a_kind(cards);
	if(best_three.none())
		return best_three;

	int three_rank = (best_three>>16).to_ulong();
	three_rank &= 0xF;
	
	poker_hand_t best_pair = best_one_pair(cards & ~filter_rank(cards, three_rank)); 
	
	if(best_pair.none())
		return best_pair;

	int pair_rank = (best_pair>>16).to_ulong();
	pair_rank &= 0xF;

	poker_hand_t full_house = FULL<<20; // adaug categoria
	full_house |= three_rank<<16; // adaug valoarea mainii
	full_house |= pair_rank<<12; // adaug valoarea mainii

	return full_house;
}

poker_hand_t best_flush(card_set_t cards)
{
	poker_hand_t best_hand;
	for(int suit=0;suit< SUITS; suit++)
	{
		card_set_t suited_cards = filter_suit(cards, suit);
		poker_hand_t this_colored_hand;
		if(suited_cards.count() >= 5)
		{
			this_colored_hand = FLUSH<<20; // adaug categoria
			this_colored_hand |= get_kickers(suited_cards, 5); // adaug kickerul
		}

		best_hand = max(best_hand, this_colored_hand);
	}
	return best_hand;
}

poker_hand_t best_straight(card_set_t cards)
{
	poker_hand_t best_hand;
	for(int start_rank=RANKS-1;start_rank>=4;start_rank--)
	{
		bool straight = true;
		for(int rank=start_rank;rank>start_rank-5;rank--)
			if(filter_rank(cards, rank).none())
				straight = false;
		if(straight)
		{
			best_hand = STRAIGHT<<20; // adaug categoria
			best_hand |= start_rank<<16; // adaug valoarea mainii
			break;
		}
	}

	// cazul special A2345
	if(best_hand.none())
	{
		bool straight = true;
		for(int rank=3;rank>=0;rank--)
			if(filter_rank(cards, rank).none())
				straight = false;
		if(straight && !filter_rank(cards, 12).none())
		{
			best_hand = STRAIGHT<<20; // adaug categoria
			best_hand |= (3<<16); // adaug valoarea mainii (5 high)
		}
	}

	return best_hand;
}

poker_hand_t best_three_of_a_kind(card_set_t cards)
{
	poker_hand_t best_hand;
	for(int rank=RANKS-1;rank>=0;rank--)
	{
		card_set_t three = filter_rank(cards, rank);
		if(three.count() >= 3)
		{
			best_hand = THREE<<20; // adaug categoria
			best_hand |= rank<<16; // adaug valoarea mainii
			best_hand |= (get_kickers(cards & ~three, 2))<<8; // adaug kickerul
			break;
		}
	}
	return best_hand;
}

poker_hand_t best_two_pair(card_set_t cards)
{
	poker_hand_t best_hand;
	int first_pair_rank = -1;
	for(int rank=RANKS-1;rank>=0;rank--)
	{
		card_set_t pair = filter_rank(cards, rank);
		if(pair.count() >= 2)
		{
			if(first_pair_rank == -1)
			{
				first_pair_rank = rank;
			}
			else
			{
				best_hand = TWO_PAIR<<20; // adaug categoria
				best_hand |= first_pair_rank<<16; // adaug valoarea mainii
				best_hand |= rank<<12; // adaug valoarea mainii
				best_hand |= get_kickers(cards & ~filter_rank(cards, first_pair_rank) & ~pair, 1)<<8; // adaug kickerul
				break;
			}
		}
	}
	return best_hand;
}

poker_hand_t best_one_pair(card_set_t cards)
{
	poker_hand_t best_hand;
	for(int rank=RANKS-1;rank>=0;rank--)
	{
		card_set_t pair = filter_rank(cards, rank);
		if(pair.count() >= 2)
		{
			best_hand = ONE_PAIR<<20; // adaug categoria
			best_hand |= rank<<16; // adaug valoarea mainii
			best_hand |= (get_kickers(cards & ~pair, 3))<<4; // adaug kickerul
			break;
		}
	}
	return best_hand;
}

poker_hand_t best_poker_hand(card_set_t cards)
{
	poker_hand_t best_hand;

	best_hand = max(best_hand, best_straight_flush(cards));
	best_hand = max(best_hand, best_four_of_a_kind(cards));
	best_hand = max(best_hand, best_full_house(cards));
	best_hand = max(best_hand, best_flush(cards));
	best_hand = max(best_hand, best_straight(cards));
	best_hand = max(best_hand, best_three_of_a_kind(cards));
	best_hand = max(best_hand, best_two_pair(cards));
	best_hand = max(best_hand, best_one_pair(cards));

	return best_hand;
}