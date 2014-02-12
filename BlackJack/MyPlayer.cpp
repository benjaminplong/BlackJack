#include "BlackJackDealer.h"


int main()
{
	BlackJackDealer Dealer = BlackJackDealer();
	unsigned int MyChips = Dealer.GetPlayerChipsRemaining();
	unsigned int MyBet = 10;
	Dealer.SetPlayerBet(MyBet);

	std::vector<Hand*> MyHands;
	unsigned int currentHand = 0;
	//MyHand will be null when the game is over
	while (!Dealer.IsGameOver())
	{
		// if MyHands vector is not empty then clear it
		if (!MyHands.empty())
			MyHands.clear();
		// start hand
		MyHands.push_back(Dealer.StartHand());
		currentHand = 0; 
		MyChips = Dealer.GetPlayerChipsRemaining();
		unsigned int DealerFaceUp = Dealer.GetDealerFaceUpCardValue();
		while (!Dealer.CheckAllPlayerHandsDone())
		{
			Hand* MyHand = MyHands[currentHand];
			std::vector<Card> MyCards = MyHand->GetCards();
			if ((MyHand->GetValue() >= 9 && MyHand->GetValue() <= 11) && MyHand->GetNumCards() == 2)
				Dealer.DoubleDown(MyHand);
			else if (Dealer.GetCardValue(MyCards[0]) == Dealer.GetCardValue(MyCards[1]))
			{
				MyHands.push_back(Dealer.Split(MyHand));
				//increment current hand index for next execution.
			}
			else
			{
				if (MyHand->GetValue() < 17 && !MyHand->IsDone())
					Dealer.Hit(MyHand);

				if (MyHand->GetValue() >= 17
					&& !MyHand->IsDone())
					Dealer.Stay(MyHand);
			}

			//our current hand is done, but we have more hands to play so bump the
			// current hand count to play the next one
			if (MyHand->IsDone()
				&& !Dealer.CheckAllPlayerHandsDone())
			{
				currentHand++;
			}
		}
		// if the player wants to walk away at any point this is how
		/*if (Dealer.GetPlayerChipsRemaining() >= 1000)
		{
			Dealer.PlayerQuits();
			break;
		}*/
		//else
		//last hand is over so clear hands vector and start over
	}
	cin.get();
}