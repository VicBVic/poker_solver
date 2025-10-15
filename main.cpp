#include <iostream>
#include <unordered_map>
#include <cassert>

#include "card_set.hpp"
#include "poker_hand.hpp"
#include "ev_solver.hpp"
#include "poker_math.hpp"

using namespace std;


int main()
{
	
	cout<<"Poker hand solver demo\n";
	cout<<"=====================\n\n";
	cout<<"initializing poker math...\n";
	
	int time = clock();
	PokerMath::init();
	
	cout<<"done in "<<(clock()-time)/1000.0<<"ms\n";

	cout<<"initializing EV solver...\n";
	time = clock();
	EVSolver::init();
	cout<<"done in "<<(clock()-time)/1000.0<<"ms\n";

	
	card_set_t hero, villain, cards;
	cout<<"Enter hero's cards: \n";
	cin>>hero;
	if(cin.fail()) return 0;
	cout<<"Enter villain's cards: \n";
	cin>>villain;
	if(cin.fail()) return 0;
	cout<<"Enter table cards: \n";
	cin>>cards;
	if(cin.fail()) return 0;

	if((hero & villain).any() || (hero & cards).any() || (villain & cards).any())
	{
		cerr<<"Error: duplicate cards\n";
		return 0;
	}

	cout<<"Calculating EV....\n";

	time = clock();
	EVResponse result = EVSolver::get_heads_up_ev(hero, villain, cards);

	cout<<"Done in "<<time/1000.0<<" ms \n";

	cout<<"Hero wins "<<result.win_percentage<<"% of runouts\n";
	cout<<"Hero draws "<<result.draw_percentage<<"% of drawouts\n";
	return 0;
}