mod player;
mod card_deck;

use std::{fmt::Error, vec};

use player::Player;
use player::NULL_ID;
use card_deck::CardDeck;
use rust_poker::hand_evaluator::{Hand, evaluate};
use std::collections::HashMap;

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
	hands: Vec<Hand>,
	deck: CardDeck,
	seat_count: usize,
	button_position: usize,
	current_round: Round,
	active_player: i128,
	blinds: Vec<i64>,
	last_bet_amount: i64,
	total_pot: i64
}

impl Game {


	pub fn new(seat_count: usize, blinds:Vec<i64>) -> Game{

		Game {
			seat_count: seat_count,
			players : HashMap::new(),
			deck: CardDeck::new(),
			seats: vec![0;seat_count],
			button_position: 0,
			chips_at_risk: Vec::new(),
			hands: Vec::new(),
			current_round: Round::NotPlaying,
			active_player: NULL_ID,
			blinds: blinds,
			last_bet_amount: 0,
			total_pot: 0
		}
	}

	pub fn add_player(&mut self, player: Player) -> Result<(), &'static str> {
		if player.current_seat >= self.seat_count {
			return Err("invalid seat\n");
		}

		self.seats[player.current_seat] = player.id;
		self.players.insert(player.id, player);
		

		Ok(())
	}

	pub fn remove_player(&mut self, player:Player) -> Result<(), &'static str> {
		if !self.players.contains_key(&player.id) {
			return Err("player not in game\n");
		}

		self.players.remove(&player.id);
		self.seats[player.current_seat] = NULL_ID;

		Ok(())
	}

	pub fn init_round(&mut self) -> Result<(), &'static str> {
		
		self.deck.shuffle();
		self.deal_hands();
		
		Ok (())
	}

	pub fn place_bet(&mut self, player_id:i128, bet_amount: i64) -> Result<(), &'static str> {
		if !self.players.contains_key(&player_id) {
			Err("Player not in game\n")
		} else if self.active_player != player_id {
			Err("Not active player\n")
		} else if bet_amount < 0 || bet_amount > self.players[&player_id].stack {
			Err("Invalid bet amount\n")
		} else if self.last_bet_amount != 0 {
			Err("Invalid action\n")
		}
		else {

			let active_player = self.players.get(&player_id).expect("impossible");

			self.chips_at_risk[active_player.current_seat] = bet_amount;

			self.last_bet_amount = bet_amount;

			Ok (())
		}
	}

	pub fn place_raise(&mut self, player_id:i128, raise_amount: i64) -> Result<(), &'static str> {

		if !self.players.contains_key(&player_id) {
			Err("Player not in game\n")
		} else if self.active_player != player_id {
			Err("Not active player\n")
		} else if raise_amount < 0 || raise_amount > self.players[&player_id].stack {
			Err("Invalid raise amount\n")
		} else if raise_amount < self.last_bet_amount * 2 {
			Err("Invalid action\n")
		}
		else {

			let active_player = self.players.get(&player_id).expect("impossible");

			self.chips_at_risk[active_player.current_seat] = raise_amount;

			self.last_bet_amount = raise_amount;

			Ok (())
		}
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