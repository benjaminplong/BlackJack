#include "BlackJackDealer.h"


void main()
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
		unsigned int DealerFaceUp = Dealer.GetDealerFaceUpCardValue();
		while (MyHand->GetValue() < 17
			&& !MyHand->GetDoneState())
		{
			Dealer.Hit(MyHand);
		}
		if (!MyHand->GetDoneState())
			Dealer.Stay(MyHand);
		MyHand = Dealer.StartHand();
	}
}