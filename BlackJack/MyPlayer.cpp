#include "BlackJackDealer.h"


int main()
{
	BlackJackDealer Dealer = BlackJackDealer();
	unsigned int MyChips = Dealer.GetPlayerChipsRemaining();
	unsigned int MyBet = 10;
	Dealer.SetPlayerBet(MyBet);
	Hand* MyHand = Dealer.StartHand();

	//MyHand will be null when the game is over
	while (MyHand)
	{
		MyChips = Dealer.GetPlayerChipsRemaining();
		std::vector<Card> MyCards = MyHand->GetCards();
		unsigned int DealerFaceUp = Dealer.GetDealerFaceUpCardValue();
		while (!MyHand->GetDoneState())
		{
			if ((MyHand->GetValue() >= 9 && MyHand->GetValue() <= 11) && MyHand->GetNumCards() == 2)
				Dealer.DoubleDown(MyHand);
			else
			{
				if (MyHand->GetValue() < 17 && !MyHand->GetDoneState())
					Dealer.Hit(MyHand);

				if (MyHand->GetValue() >= 17
					&& !MyHand->GetDoneState())
					Dealer.Stay(MyHand);
			}
		}
		// if the player wants to walk away at any point this is how
		/*if (Dealer.GetPlayerChipsRemaining() >= 1000)
		{
			Dealer.PlayerQuits();
			break;
		}*/
		//else
			MyHand = Dealer.StartHand();
	}
	cin.get();
}