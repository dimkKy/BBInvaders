// by Dmitry Kolontay


#include "CoreSystems/BBInvadersGameModeBase.h"
#include "UI/BBInvadersHUD.h"
#include "Player/BBInvadersPlayerController.h"
#include "Player/BBInvadersPlayerState.h"
#include "BBInvadersUtils.h"
#include "Player/MainMenuPawn.h"
#include "CoreSystems/BBInvadersProjectile.h"
#include "CoreSystems/BBInvadersGameStateBase.h"
#include "Environment/Invader.h"
#include "Environment/AdvancedInvader.h"
#include "Environment/Asteroid.h"
#include "Environment/Orbit.h"
#include "Containers/List.h"
#include "CoreSystems/ProjectileDataAsset.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

ABBInvadersGameModeBase::ABBInvadersGameModeBase() :
	localController{ nullptr }
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	//SetActorTickInterval(2.f);
}

void ABBInvadersGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* player)
{
	Super::HandleStartingNewPlayer_Implementation(player);
	localController = CastChecked<ABBInvadersPlayerController>(player);
}

void ABBInvadersGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ShrinkOrbits(DeltaTime);

	/*check(localController.IsValid());
	APawn* playerPawn{ localController->GetPawn() };
	check(playerPawn);

	
	//A(x-x0) + B(y-y0) + C(z-z0) = 0
	//A*x + B*y + C*z = A*x0 + B*y0 + C*z0
	FVector gamePlaneNormal{ playerPawn->GetActorUpVector() };
	FVector gamePlanePoint{ playerPawn->GetActorLocation() * gamePlaneNormal };
	

	FPlane gameAreaPlane{ gamePlaneNormal, gamePlanePoint.X + gamePlanePoint.Y + gamePlanePoint.Z };
	//add orbit
	if (true && orbits.Num() < maxOrbits) {
		//spawn orbit
	}

	//spawn asteroid
	if (true) {

	}
	//spawn advanced invader
	if (true) {

	}*/
}

void ABBInvadersGameModeBase::StartGameplay()
{
	localController->Possess(GetGameState<ABBInvadersGameStateBase>()->Refresh());
	SetActorTickEnabled(true);
}

void ABBInvadersGameModeBase::GoToMainMenu()
{
	ClearPause();
	//load level
	UWorld* world{ GetWorld() };
	AMainMenuPawn* menuPawn{
		BBInvadersUtils::GetFirstActor<AMainMenuPawn>(world) };

	localController->Possess(menuPawn);
	SetActorTickEnabled(false);

	BBInvadersUtils::ForActorsOfClass<
		AInvader, AOrbit, AAsteroid, ABBInvadersProjectile>
		(world, [](AActor* actor) {actor->Destroy(); });
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

void ABBInvadersGameModeBase::OnGameOver()
{
	GoToMainMenu();
}

#if WITH_EDITOR
EDataValidationResult ABBInvadersGameModeBase::IsDataValid(FDataValidationContext& context) const
{
	Super::IsDataValid(context);

	/*if (mainMenuLevel) {
		int32 introCamerasCount{ 0 };
		for (TActorIterator<AActor> it{ mainMenuLevel->GetWorld(), AIntroCamera::StaticClass() }; it; ++it)
		{
			introCamerasCount++;
		}
		if (introCamerasCount != 1) {
			context.AddError(FText::FromString("There should be exactly one intro camera in main level"));
		}
	}
	else {
		context.AddError(FText::FromString("Invalid mainMenuLevel"));
	}*/

	if (HUDClass && HUDClass->IsChildOf<ABBInvadersHUD>()) {
		if (!HUDClass->IsInBlueprint()) {
			context.AddError(FText::FromString("Use blueprinted ABBInvadersHUD"));
		}
	}
	else {
		context.AddError(FText::FromString("Invalid HUDClass"));
	}

	if (PlayerControllerClass && 
		PlayerControllerClass->IsChildOf<ABBInvadersPlayerController>()) {

	}
	else {
		context.AddError(FText::FromString("Invalid PlayerControllerClass"));
	}

	if (GameStateClass &&
		GameStateClass->IsChildOf<ABBInvadersGameStateBase>()) {

	}
	else {
		context.AddError(FText::FromString("Invalid GameStateClass"));
	}

	if (PlayerStateClass &&
		PlayerStateClass->IsChildOf<ABBInvadersPlayerState>()) {

	}
	else {
		context.AddError(FText::FromString("Invalid PlayerStateClass"));
	}

	return context.GetNumErrors() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}


void ABBInvadersGameModeBase::_SpawnNewAdvancedInvader() const
{
	check(SpawnNewAdvancedInvader());
}

void ABBInvadersGameModeBase::_SpawnNewAsteroid() const
{
	check(SpawnNewAsteroid());
}

void ABBInvadersGameModeBase::_SpawnNewOrbit()
{
	check(SpawnNewOrbit());
}
#endif

void ABBInvadersGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	GetGameState<ABBInvadersGameStateBase>()->Refresh();
}

FVector ABBInvadersGameModeBase::CalcRandOutOfBoundsPos(double objectRadius) const
{
	return GetGameState<ABBInvadersGameStateBase>()->CalcRandOutOfBoundsPos(objectRadius);
}

AOrbit* ABBInvadersGameModeBase::SpawnNewOrbit(double additionalRadius)
{
	ABBInvadersGameStateBase* gameState{ GetGameState<ABBInvadersGameStateBase>() };
	check(gameState);
	const FPlayAreaInfo* mapInfo{ &gameState->mapInfo };

	auto* outermostOrbit{ ProcessCheckOrbits() };

	double newRadius{ outermostOrbit ?
		outermostOrbit->GetOuterRadius() : mapInfo->halfSize.Size2D() };

	FTransform newOrbitTransform{ 
		FRotationMatrix::MakeFromXZ(mapInfo->forward, mapInfo->up).ToQuat(),
		mapInfo->center };

	auto* newOrbit{ AOrbit::SpawnOrbit(*GetWorld(), newOrbitTransform, newRadius)};
	
	orbits.AddTail(TWeakObjectPtr<AOrbit>{newOrbit});
	return newOrbit;
}

AAdvancedInvader* ABBInvadersGameModeBase::SpawnNewAdvancedInvader() const
{	
	UWorld* world{ GetWorld() };
	const AActor* target{ GetGameState<ABBInvadersGameStateBase>()->GetCenterActor() };
	//ensure?
	check(world && target);

	auto* newInvader{ AAdvancedInvader::SpawnAdvancedInvaderDeferred(*world, *target) };

	FVector location{ CalcRandOutOfBoundsPos(newInvader->GetCollisionRadius()) };

	newInvader->FinishSpawning({ FRotator::ZeroRotator, location });
	return newInvader;
}

AAsteroid* ABBInvadersGameModeBase::SpawnNewAsteroid() const
{
	UWorld* world{ GetWorld() };
	//ensure?
	check(world);

	auto* newAsteroid{ AAsteroid::SpawnAsteroidDeferred(*world) };
	newAsteroid->FinishSpawningSetVelocity(
		CalcRandOutOfBoundsPos(newAsteroid->GetMeshRadius()), 
		CastChecked<ABBInvadersGameStateBase>(world->GetGameState())->GetCenter());

	return newAsteroid;
}

AOrbit* ABBInvadersGameModeBase::ProcessCheckOrbits()
{
	for (decltype(orbits)::TIterator it{orbits.GetHead()}; it; ) {
		if ((*it).IsValid()) {
			++it;
		}
		else {
			auto&& node{ it.GetNode() };
			++it;
			orbits.RemoveNode(node);
		}
	}
	return orbits.Num() ?
		orbits.GetTail()->GetValue().Get() :
		nullptr;
}

AOrbit* ABBInvadersGameModeBase::ProcessCheckOrbits(TFunction<void(AOrbit&)>&& func)
{
	for (decltype(orbits)::TIterator it{orbits.GetHead()}; it; ) {
		if (TWeakObjectPtr<AOrbit> weakP{ (*it) }; weakP.IsValid() /*&& (*it)->GetInvadersNum()*/) {

			func(*weakP.Get());

			++it;
		}
		else {
			auto* node{ it.GetNode() };
			++it;
			orbits.RemoveNode(node);
		}
	}
	return orbits.Num() ?
		orbits.GetTail()->GetValue().Get() :
		nullptr;
}

