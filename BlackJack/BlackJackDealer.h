/*
This class is a dealer for the common card game of Blackjack
This dealer will operate by these BlackJack rules:
1. Dealer will hit until he reaches a count of 17 or greater
2. There will be no insurance
3. Players will be allowed to take the following actions during their turn
	a.) "Hit" the player wishes to take 1 more card from the "shoe"
	b.) "Stay" the player wishes to keep their current total and allows the dealer to now play
	c.) "Split" if a player's first two cards are identical in value, the player may split them into two hands
	d.) "Double Down" if a player's first two cards equal 9,10, or 11 the player may double their bet in exchange for one, and only one
	     more card.
4. The Player will "Win" under the following conditions:
	a.) The Dealer "busts" with a total more than 21. (Pays 1 to 1)
	b.) The Player has a higher total than the dealer, without going over 21. (Pays 1 to 1)
	c.) The Player has a hand consisting of 5 cards and still has not gone over 21. (Pays 1 to 1);
	d.) Player gets "BlackJack", total of 21 on first two cards. (Pay 3 to 2).
5. The Player will "Lose" under the following conditions:
	a.) The Player "busts" with a total more than 21.
	b.) The Dealer has a higher total than the Player, without going over 21.
	c.) The Dealer has a hand consisting of 5 cards and still has not gone over 21.
	d.) The Dealer has "BlackJack", total of 21 on first two cards.
6. There will be a "Push" under the following condition:
	a.) The Player and Dealer have an equal hand value and neither is over 21. (Player is returned is original bet).
7. The Game ends when one of two conditions is met:
	a.) The player plays the Maximum Number of Hands
	b.) The player runs out of chips
*/
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <vector>

using namespace std;

#define NUMBER_OF_CARDS 10
#define PLAYER_STARTING_CHIPS 1000
#define NUMBER_OF_HANDS 1000
enum Card{
	ONE,
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
	KING,
	ACE
};

class Hand
{
	friend class BlackJackDealer;
public:
	//default Constructor
	Hand();
	// gets the value of the hand
	unsigned int GetValue()
	{
		return HandValue;
	}
	// Gets the number of cards in a hand
	unsigned int GetNumCards()
	{
		return NumCards;
	}
	// gets the array of cards that make up the hand
	std::vector<Card> GetCards()
	{
		std::vector<Card> TempCards = Cards;
		return TempCards;
	}
	// check to see if the hand is done
	bool IsDone()
	{
		return Done;
	}
private:
	std::vector<Card> Cards;
	unsigned int NumCards;
	unsigned int HandValue;
	bool IsPlayer;
	bool Done;
};

class BlackJackDealer
{

private:
	
	// handles a player "BlackJack"
	void PlayerBlackJack();
	//counts player loss
	void PlayerLoses();
	//counts player win
	void PlayerWins();
	//prints results
	void PrintResults();
	// runs dealer logic
	void PlayDealer();
	// checks for hand winner
	void CheckWinner();
	// checks if all the players hands have busted
	bool CheckPlayerBust();
	//gets a card from the dealing wheel and replaces it with a new card.
	__declspec(dllexport)  Card GetCard();
	unsigned int PlayerChips;
	Card Wheel[NUMBER_OF_CARDS];
	Hand DealerHand;
	unsigned int NextCardIndex;
	unsigned int PlayerBet;
	unsigned int OriginalPlayerBet;
	unsigned int wins;
	unsigned int losses;
	unsigned int pushes;
	unsigned int HandNumber;
	bool GameOver;
	std::vector<Hand*> PlayerHands;

public:
	//initializes the dealer
	__declspec(dllexport) BlackJackDealer();

	// function called when a hand "Hits"
	__declspec(dllexport) void Hit(Hand* hand);
	// function called when a hand "stays"
	__declspec(dllexport) void Stay(Hand* hand);
	// Call this function to start a new hand
	// Player will be dealt 2 cards and dealer will be delt 2 cards
	// will return nullptr when the game is over
	__declspec(dllexport) Hand* StartHand();
	// returns the value of the passed in card
	__declspec(dllexport) unsigned int GetCardValue(Card card);
	// sets player's bet value
	__declspec(dllexport) void SetPlayerBet(unsigned int bet);
	// gets player's bet value
	__declspec(dllexport) unsigned int GetPlayerBet()
	{
		return PlayerBet;
	}

	//get the number of chips the player has left
	__declspec(dllexport) unsigned int GetPlayerChipsRemaining()
	{
		return PlayerChips;
	}

	//get the number of chips the player has left
	__declspec(dllexport) unsigned int IsGameOver()
	{
		return GameOver;
	}
	// returns the value of the Dealer's face up card
	__declspec(dllexport) unsigned int GetDealerFaceUpCardValue();
	//returns the player hand at the specified index
	__declspec(dllexport) Hand* GetPlayerHand(unsigned int index)
	{
		return PlayerHands[index];
	}
	//returns the number of player hands
	__declspec(dllexport) unsigned int GetNumPlayerHands()
	{
		return PlayerHands.size();
	}
	// splits the players hand into two new ones
	// returns a new hand, does not delete the old one
	__declspec(dllexport) Hand* Split(Hand* hand);
	// lets the player double down for one more card
	__declspec(dllexport) void DoubleDown(Hand* hand);
	// lets the player walk away from the table
	__declspec(dllexport) void PlayerQuits();
	// checks if all of the player's hands are done
	__declspec(dllexport) bool CheckAllPlayerHandsDone();
};