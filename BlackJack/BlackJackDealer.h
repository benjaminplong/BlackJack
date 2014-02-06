/*
This class is a dealer for the common card game of Blackjack
This dealer will operate by these BlackJack rules:
1. Dealer will hit until he reaches a count of 17 or greater
2. There will be no insurance
3. Players will be allowed to take the following actions during their turn
	a.) "Hit" the player wishes to take 1 more card from the "shoe"
	b.) "Stay" the player wishes to keep their current total and allows the dealer to now play
4. The Player will "Win" under the following conditions:
	a.) The Dealer "busts" with a total more than 21. (Pays 1 to 1)
	b.) The Player has a higher total than the dealer, without going over 21. (Pays 1 to 1)
	c.) The Player has a hand consisting of 5 cards and still has not gone over 21. (Pays 1 to 1);
5. The Player will "Lose" under the following conditions:
	a.) The Player "busts" with a total more than 21.
	b.) The Dealer has a higher total than the Player, without going over 21.
	c.) The Dealer has a hand consisting of 5 cards and still has not gone over 21.
6. There will be a "Push" under the following condition:
	a.) The Player and Dealer have an equal hand value and neither is over 21. (Player is returned is original bet).
*/
#include <cstdlib>
#include <time.h>

#define NUMBER_OF_CARDS 10
enum Card{
	ACE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	TEN,
	JACK,
	QUEEN,
	KING
};

struct Hand
{
	Card Cards[5];
	unsigned int NumCards;
	unsigned int HandValue;
};
class BlackJackDealer
{
	//initializes the dealer
	BlackJackDealer();
	//gets a card from the dealing wheel and replaces it with a new card.
	Card GetCard();
	// function called when player "Hits"
	Card PlayerHits();
private:
	unsigned int PlayerChips;
	Card Wheel[NUMBER_OF_CARDS];
	Hand PlayerHand;
	Hand DealerHand;
	unsigned int NextCardIndex;
};