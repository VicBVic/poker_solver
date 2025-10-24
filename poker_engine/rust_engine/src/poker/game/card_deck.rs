use std::fmt::{self, write, Display, Formatter};

use rand::seq::{SliceRandom};

pub struct Card{
	rank: u8,
	suit: u8,	
}

impl Display for Card {
	
	fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
		let suit = match self.suit {
			1 => "Hearts",
			2 => "Spades",
			3 => "Clubs",
			_ => "Diamonds"
		};

		let rank = match self.rank {
			1 => "Ace",
			13 => "King",
			12 => "Queen",
			11 => "Jack",
			10 => "Ten",
			_ => &self.rank.to_string()
		};

		write!(f, "{} of {}", rank, suit)
	}
}


pub struct CardDeck{
	cards: Vec<Card>
}

impl CardDeck{
	pub fn new() -> Self {
		
		let mut cards = Vec::<Card>::new();

		for suit in 1..=4 {
			for rank in 1..=13 {
				cards.push(Card { rank:rank, suit: suit });
			}
		}


		Self{cards: cards}
	}

	pub fn shuffle(&mut self) {
		let mut rng = rand::rng();

		self.cards.shuffle(&mut rng);
	}

	pub fn deal_top(&mut self) -> Option<Card> {
		self.cards.pop()
	}

	pub fn deal_hand(&mut self, cnt:i32) -> Vec<Card> {
		let mut ans = Vec::<Card>::new();

		for i in 0..cnt {
			if !self.cards.is_empty(){
				ans.push(self.cards.pop().expect("How"));
			}
		}

		ans
	}

	pub fn reset(&mut self) {
		self.cards.clear();

		for suit in 1..=4 {
			for rank in 1..=13 {
				self.cards.push(Card { rank:rank, suit: suit });
			}
		}
	}

	pub fn burn(&mut self) {
		self.cards.pop();
	}
}

impl Display for CardDeck{
	fn fmt(&self, f: &mut Formatter) ->fmt::Result{
		
		let mut result = fmt::Result::Ok(());
		
		for card in &self.cards {
			result = result.and(write!(f, "{}; ", card));
		}

		result.and(write!(f, "\n"))
	}
}