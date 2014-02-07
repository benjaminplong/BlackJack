#include "BlackJackDealer.h"

// initializes the Dealer to start the game.

Hand::Hand()
:NumCards(0),
IsPlayer(false),
HandValue(0),
Done(false)
{

}
BlackJackDealer::BlackJackDealer()
:PlayerChips(PLAYER_STARTING_CHIPS),
PlayerBet(0),
wins(0),
losses(0),
HandNumber(0),
NextCardIndex(0),
GameOver(false)
{
	srand(time(NULL));
	for(int i = 0; i < NUMBER_OF_CARDS; i++)
	{
		// add +1 to skip the special "ONE" enum when drawing cards
		Card card = static_cast<Card>(rand()%13 + 1);
		_ASSERT(card != ONE);
		Wheel[i] = card;
	}
}

//gets the next card from the dealing wheel and loads its replacment into the wheel.
Card BlackJackDealer::GetCard()
{
	Card next = Wheel[NextCardIndex % NUMBER_OF_CARDS];
	// add +1 to skip the special "ONE" enum when drawing cards
	Wheel[NextCardIndex % NUMBER_OF_CARDS] = static_cast<Card>(rand()%13+1);
	NextCardIndex++;
	return next;
}

// Handles a Hand "Hits", gets them a card and totals their hand
// also checks for 5 card win.
void BlackJackDealer::Hit(Hand* hand)
{
	//if trying to "Hit" on a hand
	// that previously Stayed or busted
	// return out
	if (hand->Done)
		return;

	Card card = GetCard();
	_ASSERT(card != ONE);
	hand->Cards[hand->NumCards] = card;
	hand->NumCards++;
	hand->HandValue += GetCardValue(card);

	//bust
	if (hand->HandValue > 21)
	{
		// check for an ace and turn it into a 
		// value of 1 if the hand has busted
		for (unsigned int i = 0; i < hand->NumCards; i++)
		{
			if (hand->Cards[i] == ACE)
			{
				//set the value to one so we don't count it again
				hand->Cards[i] = ONE;
				hand->HandValue -= 10;
			}
		}
	}
	
	// if devaluing all the aces still causes
	// a bust then that hand loses
	if (hand->HandValue > 21)
	{
		hand->Done = true;
		if (hand->IsPlayer)
		{
			//set dealer to done because player busted
			DealerHand.Done = true;
			PlayerLoses();
		}
		else
			PlayerWins();
	}
	else if (hand->NumCards == 5
		&& hand->HandValue <= 21)
	{
		hand->Done = true;
		if (hand->IsPlayer)
		{
			//set dealer to done because player won
			DealerHand.Done = true;
			PlayerWins();
		}
		else
			PlayerLoses();
	}
}
// function called when a hand "stays"
void BlackJackDealer::Stay(Hand* hand)
{
	hand->Done = true;
	if (hand->IsPlayer)
		PlayDealer();
	else
		CheckWinner();
}
// Call this function to start a new hand
// Player will be dealt 2 cards and dealer will be delt 2 cards
Hand* BlackJackDealer::StartHand()
{
	// will be true if the player has run out of chips or
	// has played the maximum number of hands
	if (GameOver)
		return nullptr;
	// if player wants to bet more than his total chips
	// then he will bet all of his remaining chips
	if (PlayerBet > PlayerChips)
		PlayerBet = PlayerChips;

	HandNumber++;
	PlayerHands.clear();
	DealerHand = Hand();
	Hand* Player = new Hand();
	Player->IsPlayer = true;
	Card PlayerCardOne = GetCard();
	Player->Cards[Player->NumCards] = PlayerCardOne;
	Player->HandValue += GetCardValue(PlayerCardOne);
	Player->NumCards++;

	Card DealerCardOne = GetCard();
	DealerHand.Cards[DealerHand.NumCards] = DealerCardOne;
	DealerHand.HandValue += GetCardValue(DealerCardOne);
	DealerHand.NumCards++;

	Card PlayerCardTwo = GetCard();
	Player->Cards[Player->NumCards] = PlayerCardTwo;
	Player->HandValue += GetCardValue(PlayerCardTwo);
	Player->NumCards++;

	Card DealerCardTwo = GetCard();
	DealerHand.Cards[DealerHand.NumCards] = DealerCardTwo;
	DealerHand.HandValue += GetCardValue(DealerCardTwo);
	DealerHand.NumCards++;

	// blackjack
	if (Player->HandValue == 21)
	{
		Player->Done = true;
		PlayerBlackJack();
	}
	//Dealer BlackJack
	else if (DealerHand.HandValue == 21)
	{
		Player->Done = true;
		DealerHand.Done = true;
		PlayerLoses();
	}
	PlayerHands.push_back(Player);
	return Player;
}
// returns the value of the passed in card
unsigned int BlackJackDealer::GetCardValue(Card card)
{
	switch (card)
	{
	case Card::ACE:
		return 11;
	case Card::JACK:
	case Card::QUEEN:
	case Card::KING:
		return 10;
	default:
		return static_cast<unsigned int>(card)+1;
	}
}

// sets player's bet value
void BlackJackDealer::SetPlayerBet(unsigned int bet)
{
	if (bet < PlayerChips)
		PlayerBet = bet;
}
// handles a player "BlackJack"
void BlackJackDealer::PlayerBlackJack()
{
	//set dealer to done because player won
	DealerHand.Done = true;
	PlayerChips += static_cast<int>(PlayerBet * 1.5);
	wins++;
}
//counts player loss
void BlackJackDealer::PlayerLoses()
{
	PlayerChips -= PlayerBet;
	losses++;
	//game over
	if (PlayerChips == 0
		|| HandNumber > NUMBER_OF_HANDS)
	{
		PrintResults();
	}
}
//counts player win
void BlackJackDealer::PlayerWins()
{
	wins++;
	PlayerChips += PlayerBet;
	if (HandNumber > NUMBER_OF_HANDS)
		PrintResults();
}
//prints results
void BlackJackDealer::PrintResults()
{
	cout << ("The Players final statistics are:") << endl;
	cout << "Total Chips:\t" << PlayerChips << endl;
	cout << "Wins:\t" << wins << endl;
	cout << "Losses:\t" << losses << endl;
	cout << "Win Percentage:\t" << static_cast<float>(wins / losses) << endl;

	GameOver = true;
}
// checks for hand winner
void BlackJackDealer::CheckWinner()
{
	for (Hand* hand : PlayerHands)
	{
		if (hand->HandValue > DealerHand.HandValue)
			PlayerWins();
		else if (hand->HandValue < DealerHand.HandValue)
			PlayerLoses();
	}
}
// runs dealer logic
void BlackJackDealer::PlayDealer()
{
	while (DealerHand.HandValue < 17)
	{
		Hit(&DealerHand);
	}
	//if dealer did not bust or win on 5 cards stay
	if (!DealerHand.Done)
		Stay(&DealerHand);
}
// returns the value of the Dealer's face up card
unsigned int BlackJackDealer::GetDealerFaceUpCardValue()
{
	return GetCardValue(DealerHand.Cards[1]);
}
