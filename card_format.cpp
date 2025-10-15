#include "card_format.hpp"

#include <cassert>

char human_readable_rank(int rank)
{
	switch (rank)
	{
		case 12: return 'A';
		case 11: return 'K';
		case 10: return 'Q';
		case 9: return 'J';
		case 8: return 'T';
		default:
			assert(rank >= 0 && rank <= 8);
			return '2' + rank;
	}
}

char human_readable_suit(int suit)
{
	switch (suit)
	{
		case 0: return 'c'; // clubs
		case 1: return 'd'; // diamonds
		case 2: return 'h'; // hearts
		case 3: return 's'; // spades
		default:
			assert(false);
			return '?'; // :(
	}
}

std::string verbose_rank(int rank)
{
	switch (rank)
	{
		case 12: return "Ace";
		case 11: return "King";
		case 10: return "Queen";
		case 9: return "Jack";
		case 8: return "Ten";
		case 7: return "Nine";
		case 6: return "Eight";
		case 5: return "Seven";
		case 4: return "Six";
		case 3: return "Five";
		case 2: return "Four";
		case 1: return "Three";
		case 0: return "Two";
		default:
			assert(false);
			return "?"; // :(
	}
}

std::string verbose_suit(int suit)
{
	switch (suit)
	{
		case 0: return "Clubs";
		case 1: return "Diamonds";
		case 2: return "Hearts";
		case 3: return "Spades";
		default:
			assert(false);
			return "?"; // :(
	}
}

int rank_from_char(char c)
{
	switch (c)
	{
		case 'A': return 12;
		case 'K': return 11;
		case 'Q': return 10;
		case 'J': return 9;
		case 'T': return 8;
		default:
			if (c >= '2' && c <= '9')
				return c - '2';
			else
			{
				assert(false);
				return -1; // :(
			}
	}
}

int suit_from_char(char c)
{
	switch (c)
	{
		case 'c': return 0; // clubs
		case 'd': return 1; // diamonds
		case 'h': return 2; // hearts
		case 's': return 3; // spades
		default:
			assert(false);
			return -1; // :(
	}
}