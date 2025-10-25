use rust_poker::hand_evaluator::{Hand, CARDS};
use rand::seq::{SliceRandom};


pub struct CardDeck{
	cards: Vec<Hand>
}

impl CardDeck{
	pub fn new() -> Self {
		
		let mut cards = Vec::<Hand>::new();

		for i in 0..52 {
			cards.push(CARDS[i]);
		}


		Self{cards: cards}
	}

	pub fn shuffle(&mut self) {
		let mut rng = rand::rng();

		self.cards.shuffle(&mut rng);
	}

	pub fn deal_top(&mut self) -> Option<Hand> {
		self.cards.pop()
	}

	pub fn deal_hand(&mut self, cnt:i32) -> Hand {
		let mut ans = Hand::default();

		for i in 0..cnt {
			if !self.cards.is_empty(){
				ans += self.cards.pop().expect("imposibil");
			}
		}

		ans
	}

	pub fn reset(&mut self) {
		self.cards.clear();

		for i in 0..52 {
			self.cards.push(CARDS[i]);
		}
	}

	pub fn burn(&mut self) {
		self.cards.pop();
	}
}
