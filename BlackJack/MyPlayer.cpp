#include "BlackJackDealer.h"


int main()
{
	BlackJackDealer Dealer = BlackJackDealer();
	unsigned int MyChips = Dealer.GetPlayerChipsRemaining();
	unsigned int MyBet = 10;
	Dealer.SetPlayerBet(MyBet);

	unsigned int currentHand = 0;
	//MyHand will be null when the game is over
	while (!Dealer.IsGameOver())
	{
		// start hand
		currentHand = 0; 
		MyChips = Dealer.GetPlayerChipsRemaining();
		Dealer.StartHand();
		unsigned int DealerFaceUp = Dealer.GetDealerFaceUpCardValue();
		while (!Dealer.CheckAllPlayerHandsDone())
		{
			Hand* MyHand = Dealer.GetPlayerHand(currentHand);
			std::vector<Card> MyCards = MyHand->GetCards();
			//if ((MyHand->GetValue() >= 9 && MyHand->GetValue() <= 11) && MyHand->GetNumCards() == 2)
			//	Dealer.DoubleDown(MyHand);
			//else if (Dealer.GetCardValue(MyCards[0]) == Dealer.GetCardValue(MyCards[1]))
			//{
			//	Dealer.Split(MyHand);
			//	//increment current hand index for next execution.
			//}
			//else
			//{
				if (MyHand->GetValue() < 17 && !MyHand->IsDone())
					Dealer.Hit(MyHand);

				if (MyHand->GetValue() >= 17
					&& !MyHand->IsDone())
					Dealer.Stay(MyHand);
			//}

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
	}
}