// by Dmitry Kolontay


#include "CoreSystems/BBInvadersGameModeBase.h"
#include "UI/BBInvadersHUD.h"
#include "Player/BBInvadersPlayerController.h"
#include "Player/PlayerPawn.h"
#include "BBInvadersUtils.h"
#include "Player/MainMenuCameraActor.h"
#include "CoreSystems/AsteroidTracker.h"

ABBInvadersGameModeBase::ABBInvadersGameModeBase() :
	localController{ nullptr }, mapHalfSize{0.f}
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SetActorTickInterval(1.f);

}

void ABBInvadersGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	localController = CastChecked<ABBInvadersPlayerController>(NewPlayer);
}

void ABBInvadersGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABBInvadersGameModeBase::StartGameplay()
{
	UWorld* world{ GetWorld() };
	APlayerPawn* pawn{ BBInvadersUtils::GetFirstActor<APlayerPawn>(world) };

	mapHalfSize = pawn->CalcMapHalfSize();


	BBInvadersUtils::GetFirstActor<AAsteroidTracker>(world)->SetTrackArea(pawn->GetActorTransform(), mapHalfSize);

	localController->Possess(pawn);
	localController->SetViewTarget(pawn);

	SetActorTickEnabled(true);

	BBInvadersUtils::ForActorsOfClass<AMainMenuCameraActor>(world, [](AActor* actor) {actor->GetWorld(); });

	
}

void ABBInvadersGameModeBase::GoToMainMenu()
{
	
	//load level
	UWorld* world{ GetWorld() };
	AMainMenuCameraActor* menuCamera{ BBInvadersUtils::GetFirstActor<AMainMenuCameraActor>(world) };

	localController->SetViewTarget(menuCamera);
	localController->UnPossess();

	SetActorTickEnabled(false);
}

void ABBInvadersGameModeBase::TogglePause()
{
	//since we have only one player I decided not to bother doing it via PC
	if (IsPaused()) {
		ClearPause();
	}
	else {
		SetPause(localController.Get());
	}
}


/*void ABBInvadersGameModeBase::SetMapHalfSize(const FVector2D& size)
{
	check(size > FVector2D{0.f});
	mapHalfSize = size;
}*/



#if WITH_EDITOR
EDataValidationResult ABBInvadersGameModeBase::IsDataValid(TArray<FText>& ValidationErrors)
{
	Super::IsDataValid(ValidationErrors);

	/*if (mainMenuLevel) {
		int32 introCamerasCount{ 0 };
		for (TActorIterator<AActor> it{ mainMenuLevel->GetWorld(), AIntroCamera::StaticClass() }; it; ++it)
		{
			introCamerasCount++;
		}
		if (introCamerasCount != 1) {
			ValidationErrors.Add(FText::FromString("There should be exactly one intro camera in main level"));
		}
	}
	else {
		ValidationErrors.Add(FText::FromString("Invalid mainMenuLevel"));
	}*/

	if (HUDClass && HUDClass->IsChildOf<ABBInvadersHUD>()) {
		if (!HUDClass->IsInBlueprint()) {
			ValidationErrors.Add(FText::FromString("Use blueprinted ABBInvadersHUD"));
		}
	}
	else {
		ValidationErrors.Add(FText::FromString("Invalid HUDClass"));
	}

	if (PlayerControllerClass && PlayerControllerClass->IsChildOf<ABBInvadersPlayerController>()) {

	}
	else {
		ValidationErrors.Add(FText::FromString("Invalid PlayerControllerClass"));
	}

	return ValidationErrors.Num() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif

void ABBInvadersGameModeBase::BeginPlay()
{
	UWorld* world{ GetWorld() };

	localController->SetViewTarget(BBInvadersUtils::GetFirstActor<AMainMenuCameraActor>(world));

	APlayerPawn* pawn{ BBInvadersUtils::GetFirstActor<APlayerPawn>(world) };

	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	world->SpawnActor<AAsteroidTracker>(
		AAsteroidTracker::StaticClass(), pawn->GetActorTransform(), params);

	localController->GetBBInvadersHUD()->RequestBindings(*this);

	Super::BeginPlay();
}

void ABBInvadersGameModeBase::ChangeGameMode(bool bToMenu, bool bChangeView)
{
	SetActorTickEnabled(!bToMenu);
}
