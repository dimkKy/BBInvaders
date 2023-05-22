// by Dmitry Kolontay


#include "UI/MainMenu.h"
#include "Components/Button.h"
#include "Player/BBInvadersPlayerController.h"

void UMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	newGameButton->SetClickMethod(EButtonClickMethod::PreciseClick);
	exitButton->OnClicked.AddDynamic(
		CastChecked<ABBInvadersPlayerController>(GetOwningPlayer()), &ABBInvadersPlayerController::ExitGame);
	//GetOwningPlayer()
}
