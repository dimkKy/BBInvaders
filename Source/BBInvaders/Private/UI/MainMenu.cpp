// by Dmitry Kolontay


#include "UI/MainMenu.h"
#include "Components/Button.h"
#include "Player/BBIPlayerController.h"

void UMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	newGameButton->SetClickMethod(EButtonClickMethod::PreciseClick);
	exitButton->OnClicked.AddDynamic(
		CastChecked<ABBIPlayerController>(GetOwningPlayer()), &ABBIPlayerController::ExitGame);
	//GetOwningPlayer()
}
