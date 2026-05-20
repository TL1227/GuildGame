#include "MenuWidget.h"

void UMenuWidget::Navigate(FVector2D Direction)
{
    //NOTE: having a seperate function for each direction is a bit silly looking
	//but it makes wiring up the blueprint less painfull

	if (Direction.X == 1.0f)
		Right();
	else if (Direction.X == -1.0f)
		Left();
	else if (Direction.Y == -1.0f)
		Up();
	else if (Direction.Y == 1.0f)
		Down();
}

void UMenuWidget::IncreaseIndex()
{
	if (++CurrentItemIndex >= ItemsCount)
		CurrentItemIndex = 0;
}

void UMenuWidget::DecreaseIndex()
{
	if (--CurrentItemIndex < 0)
		CurrentItemIndex = ItemsCount - 1;
}

