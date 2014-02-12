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
pushes(0),
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
	_ASSERT(!hand->Done);

	if (hand->IsPlayer)
		cout << "Player Hits" << endl;
	else
		cout << "Dealer Hits" << endl;
	Card card = GetCard();
	_ASSERT(card != ONE);
	hand->Cards.push_back(card);
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
				break;
			}
		}
	}
	
	// if devaluing all the aces still causes
	// a bust then that hand is done
	if (hand->HandValue > 21
		|| (hand->NumCards == 5
		&& hand->HandValue <= 21))
	{
		Stay(hand);
	}
}
// function called when a hand "stays"
void BlackJackDealer::Stay(Hand* hand)
{
	if (hand->Done)
		return;

	if (hand->IsPlayer)
		cout << "Player Stays" << endl;
	else
		cout << "Dealer Stays" << endl;

	hand->Done = true;
	if (CheckAllPlayerHandsDone())
	{
		if (hand->IsPlayer)
			PlayDealer();
		else
			CheckWinner();
	}
}
// Call this function to start a new hand
// Player will be dealt 2 cards and dealer will be delt 2 cards
Hand* BlackJackDealer::StartHand()
{
	// will be true if the player has run out of chips or
	// has played the maximum number of hands
	if (GameOver)
		return nullptr;
	//make sure the last hand has finished before starting a new one
	_ASSERT(PlayerHands.empty());
	// set the players bet back to the last value "set" by the player
	// comes into play after the player has doubled down or split hands
	PlayerBet = OriginalPlayerBet;
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
	Player->Cards.push_back(PlayerCardOne);
	Player->HandValue += GetCardValue(PlayerCardOne);
	Player->NumCards++;

	Card DealerCardOne = GetCard();
	DealerHand.Cards.push_back(DealerCardOne);
	DealerHand.HandValue += GetCardValue(DealerCardOne);
	DealerHand.NumCards++;

	Card PlayerCardTwo = GetCard();
	Player->Cards.push_back(PlayerCardTwo);
	Player->HandValue += GetCardValue(PlayerCardTwo);
	Player->NumCards++;

	Card DealerCardTwo = GetCard();
	DealerHand.Cards.push_back(DealerCardTwo);
	DealerHand.HandValue += GetCardValue(DealerCardTwo);
	DealerHand.NumCards++;

	// add the players hand to the tracking vector
	PlayerHands.push_back(Player);
	// blackjack
	if (Player->HandValue == 21)
	{
		Player->Done = true;
		DealerHand.Done = true;
		PlayerBlackJack();
	}
	//Dealer BlackJack
	else if (DealerHand.HandValue == 21)
	{
		Player->Done = true;
		DealerHand.Done = true;
		PlayerLoses();
		//need to do a special clear here because of dealer blackjack
		PlayerHands.clear();
	}
	
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
		OriginalPlayerBet = bet;
}
// handles a player "BlackJack"
void BlackJackDealer::PlayerBlackJack()
{
	//set dealer to done because player won
	DealerHand.Done = true;
	PlayerChips += static_cast<int>(PlayerBet * 1.5);
	wins++;
	cout << "Player BlackJack!" << endl;

	PlayerHands.clear();
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
	cout << ("The Player's final statistics are:") << endl;
	cout << "Total Chips:\t" << PlayerChips << endl;
	cout << "Wins:\t" << wins << endl;
	cout << "Losses:\t" << losses << endl;
	cout << "Pushes:\t" << pushes << endl;

	GameOver = true;
}
// checks for hand winner
void BlackJackDealer::CheckWinner()
{
	for (Hand* hand : PlayerHands)
	{
		if (hand->HandValue > 21)
		{
			cout << "Player Bust: \t" << hand->GetValue() << endl;
			PlayerLoses();
		}
		else if (DealerHand.HandValue > 21)
		{
			cout << "Dealer Bust: \t" << DealerHand.HandValue << endl;
			PlayerWins();
		}
		else if (hand->HandValue > DealerHand.HandValue)
		{
			cout << "Player Wins: \t" << hand->GetValue() << " to " << DealerHand.HandValue << endl;
			PlayerWins();
		}
		else if (hand->HandValue < DealerHand.HandValue)
		{
			cout << "Player Lost: \t" << DealerHand.HandValue << " to " << hand->GetValue() << endl;
			PlayerLoses();
		}
		else
		{
			cout << "Push" << endl;
			pushes++;
		}
	}

	PlayerHands.clear();
}
// runs dealer logic
void BlackJackDealer::PlayDealer()
{
	if (!CheckPlayerBust())
	{
		while (DealerHand.HandValue < 17 && !DealerHand.Done)
		{
			Hit(&DealerHand);
		}
		//if dealer did not bust or win on 5 cards stay
		if (!DealerHand.Done)
			Stay(&DealerHand);
	}
	else
		Stay(&DealerHand);
}
// returns the value of the Dealer's face up card
unsigned int BlackJackDealer::GetDealerFaceUpCardValue()
{
	return GetCardValue(DealerHand.Cards[1]);
}

// checks if all of the player's hands are done
bool BlackJackDealer::CheckAllPlayerHandsDone()
{
	bool done = true;
	for (Hand* hand : PlayerHands)
	{
		done &= hand->Done;
	}

	return done;
}
// checks if all the players hands have busted
bool BlackJackDealer::CheckPlayerBust()
{
	bool bust = true;
	for (Hand* hand : PlayerHands)
	{
		bust &= (hand->HandValue > 21);
	}

	return bust;
}

//Lets the player split the passed in hand
Hand* BlackJackDealer::Split(Hand* hand)
{
	cout << "Player Splits" << endl;
	_ASSERT(hand->NumCards == 2);
	_ASSERT(GetCardValue(hand->Cards[0]) == GetCardValue(hand->Cards[1]));

	Hand* newHand = new Hand();
	newHand->IsPlayer = true;
	Card card = hand->Cards[1];
	newHand->Cards.push_back(card);
	newHand->NumCards++;
	newHand->HandValue += GetCardValue(card);

	hand->Cards.pop_back();

	Card newCardOne = GetCard();
	hand->Cards.push_back(newCardOne);
	hand->HandValue -= GetCardValue(card);
	hand->HandValue += GetCardValue(newCardOne);

	Card newCardTwo = GetCard();
	newHand->Cards.push_back(newCardTwo);
	newHand->NumCards++;
	newHand->HandValue += GetCardValue(newCardTwo);

	PlayerHands.push_back(newHand);
	return newHand;
}

// lets the player double down for one more card
void BlackJackDealer::DoubleDown(Hand* hand)
{
	cout << "Player DoublesDown" << endl;
	_ASSERT(hand->NumCards == 2);
	_ASSERT(hand->GetValue() == 9 || hand->GetValue() == 10 || hand->GetValue() == 11);
	//doubles the players bet
	PlayerBet *= 2;
	Hit(hand);

	Stay(hand);
}

//lets the player walk away
void BlackJackDealer::PlayerQuits()
{
	_ASSERT(!GameOver);
	cout << "PlayerQuits" << endl;
	GameOver = true;
	PrintResults();
}