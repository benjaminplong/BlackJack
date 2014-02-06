#include "BlackJackDealer.h"

#define PLAYER_STARTING_CHIPS 1000

// initializes the Dealer to start the game.
BlackJackDealer::BlackJackDealer()
	:PlayerChips(PLAYER_STARTING_CHIPS)
{
	PlayerHand = Hand();
	DealerHand = Hand();
	srand(time(NULL));
	for(int i = 0; i < NUMBER_OF_CARDS; i++)
	{
		Wheel[i] = static_cast<Card>(rand()%13);
	}
}

//gets the next card from the dealing wheel and loads its replacment into the wheel.
Card BlackJackDealer::GetCard()
{
	Card next = Wheel[NextCardIndex % NUMBER_OF_CARDS];
	Wheel[NextCardIndex % NUMBER_OF_CARDS] = static_cast<Card>(rand()%13);
	NextCardIndex++;
	return next;
}

// Handles a Player "Hit", gets them a card and totals their hand
// also checks for 5 card win.
Card BlackJackDealer::PlayerHits()
{
	Card next = GetCard();
}