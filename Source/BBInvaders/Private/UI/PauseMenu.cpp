// by Dmitry Kolontay


#include "UI/PauseMenu.h"
#include "Components/Button.h"
#include "Player/BBInvadersPlayerController.h"

void UPauseMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//GetOwningPlayer()
	resumeButton->SetClickMethod(EButtonClickMethod::PreciseClick);
	toMainMenuButton->SetClickMethod(EButtonClickMethod::PreciseClick);
	exitButton->SetClickMethod(EButtonClickMethod::PreciseClick);

	exitButton->OnClicked.AddDynamic(
		CastChecked<ABBIPlayerController>(GetOwningPlayer()), &ABBIPlayerController::ExitGame);
}
