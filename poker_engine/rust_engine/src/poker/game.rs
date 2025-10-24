mod player;
mod card_deck;

use std::{fmt::Error, vec};

use player::Player;
use card_deck::CardDeck;
use std::collections::HashMap;
use card_deck::Card;

enum Round{
	NotPlaying,
	Preflop,
	Flop,
	Turn,
	River,
	Showdown
}

struct Game{
	players : HashMap<i128, Player>,
	seats: Vec<i128>,
	chips_at_risk: Vec<i64>,
	hands: Vec<Vec<card_deck::Card>>,
	deck: CardDeck,
	seat_count: usize,
	button_position: usize,
	current_round: Round,
	active_player: i128,
}

impl Game {
	pub fn new(seat_count: usize) -> Game{

		Game { 
			seat_count: seat_count,
			players : HashMap::new(),
			deck: CardDeck::new(),
			seats: vec![0;seat_count],
			button_position: 0,
			chips_at_risk: Vec::new(),
			hands: Vec::new(),
			current_round: Round::NotPlaying,
			active_player: 0
		}
	}

	pub fn add_player(&mut self, player: Player, seat: usize) -> Result<(), &'static str> {
		if seat >= self.seat_count {
			return Err("invalid seat\n");
		}

		self.seats[seat] = player.id;
		self.players.insert(player.id, player);
		

		Ok(())
	}

	pub fn remove_player(&mut self, player:Player) -> Result<(), &'static str> {
		if !self.players.contains_key(&player.id) {
			return Err("player not in game\n");
		}

		self.players.remove(&player.id);
		self.seats[]


	}

	fn deal_hands(&mut self) {
		for i in 0..self.seat_count {
			if self.seats[i] == 0 {
				continue;
			}

			self.hands[i] = self.deck.deal_hand(2);
		}
	}
}