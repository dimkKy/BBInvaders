// by Dmitry Kolontay


#include "UI/BBInvadersHUD.h"
#include "UI/GametimeUI.h"
#include "UI/MainMenu.h"
#include "UI/PauseMenu.h"
#include "Player/PlayerPawn.h"
#include "Player/MainMenuCameraActor.h"
#include "CoreSystems/BBInvadersGameModeBase.h"

#include "Components/Button.h"

ABBInvadersHUD::ABBInvadersHUD() :
	gametimeUI{ nullptr },
	mainMenu{ nullptr }
{
}

void ABBInvadersHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	gametimeUI = CreateWidget<UGametimeUI>(PlayerOwner, gametimeUIClass);
	mainMenu = CreateWidget<UMainMenu>(PlayerOwner, mainMenuClass);

	
}

void ABBInvadersHUD::OnViewTargetChange(const AActor& newTarget)
{
	if (newTarget.IsA(APlayerPawn::StaticClass())) {
		if (mainMenu->IsInViewport()) {
			mainMenu->RemoveFromParent();
		}
		if (!gametimeUI->IsInViewport()) {
			gametimeUI->AddToViewport();
		}

		FInputModeGameAndUI inputMode;
		inputMode.SetHideCursorDuringCapture(false);
		PlayerOwner->SetInputMode(inputMode);

		return;
	}

	if (newTarget.IsA(AMainMenuCameraActor::StaticClass())) {
		if (gametimeUI->IsInViewport()) {
			gametimeUI->RemoveFromParent();
		}
		if (!mainMenu->IsInViewport()) {
			mainMenu->AddToViewport();
		}

		PlayerOwner->SetInputMode(FInputModeUIOnly::FInputModeUIOnly());

		return;
	}
}

void ABBInvadersHUD::RequestBindings(ABBInvadersGameModeBase& gameMode)
{
	mainMenu->newGameButton->OnClicked.AddDynamic(
		&gameMode, &ABBInvadersGameModeBase::StartGameplay);

	gametimeUI->pauseMenu->toMainMenuButton->OnClicked.AddDynamic(
		&gameMode, &ABBInvadersGameModeBase::GoToMainMenu);

	gametimeUI->pauseButton->OnClicked.AddDynamic(
		&gameMode, &ABBInvadersGameModeBase::TogglePause);
	gametimeUI->pauseMenu->resumeButton->OnClicked.AddDynamic(
		&gameMode, &ABBInvadersGameModeBase::TogglePause);

	//mainMenu->exitButton->OnClicked.AddDynamic(&gameMode, &ABBInvadersGameModeBase::ExitGame);
	

	//UBlueprint
	//gametimeUI
}

#if WITH_EDITOR
EDataValidationResult ABBInvadersHUD::IsDataValid(TArray<FText>& ValidationErrors)
{
	Super::IsDataValid(ValidationErrors);

	if (!IsValid(gametimeUIClass))
		ValidationErrors.Add(FText::FromString("Invalid gametimeUI class"));
	if (!IsValid(mainMenuClass))
		ValidationErrors.Add(FText::FromString("Invalid mainMenu class"));

	return ValidationErrors.Num() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif