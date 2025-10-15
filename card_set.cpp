#include "card_set.hpp"
#include "card_format.hpp"

#include <iostream>
#include <cassert>
#include <cstring>

std::ostream &operator<<(std::ostream &os, const card_set_t &cards)
{
	for(int card=CARDS-1;card>=0;card--)
		if(cards.test(card))
		{
			os << human_readable_rank(card/4) << human_readable_suit(card%4) << ' ';
		}

	return os;
}

std::istream &operator>>(std::istream &is, card_set_t &cards)
{
	cards.reset();
	char buffer[100], *p;
	is.getline(buffer, 100);
	p = strtok(buffer, " ");

	while(p)
	{
		if(strlen(p) != 2)
		{
			is.setstate(std::ios::failbit);
			return is;
		}
		int rank = rank_from_char(p[0]);
		int suit = suit_from_char(p[1]);
		if(rank == -1 || suit == -1)
		{
			is.setstate(std::ios::failbit);
			return is;
		}
		cards.set(bit_of_rank_suit(rank, suit));
		p = strtok(nullptr, " ");
	}


	return is;
}

int bit_of_rank_suit(int rank, int suit)
{
	return rank*SUITS + suit;
}

card_set_t filter_rank(card_set_t cards, int rank)
{
	card_set_t filter;
	for(int suit=0;suit<SUITS;suit++)
		filter.set(bit_of_rank_suit(rank, suit));
	return filter & cards;
}

card_set_t filter_suit(card_set_t cards, int suit)
{
	card_set_t filter;
	for(int rank=0;rank<RANKS;rank++)
		filter.set(bit_of_rank_suit(rank, suit));
	return filter & cards;
}

bool filter_rank_suit(card_set_t cards, int rank, int suit)
{
	return cards.test(bit_of_rank_suit(rank, suit));
}

int highest_rank(card_set_t cards)
{
	for(int card=CARDS-1;card>=0;card--)
		if (cards.test(card))
			return card / SUITS;
	return -1;
}

int compare_high_card(card_set_t a, card_set_t b)
{
	for(int rank = RANKS-1; rank >= 0; rank--)
	{
		card_set_t a_rank = filter_rank(a, rank);
		card_set_t b_rank = filter_rank(b, rank);
		if(a_rank.any() && !b_rank.any())
			return 1;
		if(!a_rank.any() && b_rank.any())
			return -1;
	}
	return 0;
}
