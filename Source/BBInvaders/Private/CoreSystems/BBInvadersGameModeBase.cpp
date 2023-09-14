// by Dmitry Kolontay


#include "CoreSystems/BBInvadersGameModeBase.h"
#include "UI/BBInvadersHUD.h"
#include "Player/BBInvadersPlayerController.h"
#include "Player/PlayerPawn.h"
#include "Player/BBInvadersPlayerState.h"
#include "BBInvadersUtils.h"
#include "Player/MainMenuPawn.h"
#include "CoreSystems/OutOfAreaActorTracker.h"
#include "CoreSystems/BBInvadersProjectile.h"
#include "CoreSystems/BBInvadersGameStateBase.h"
#include "Environment/Invader.h"
#include "Environment/AdvancedInvader.h"
#include "Environment/Asteroid.h"
#include "Environment/Orbit.h"
#include "Containers/List.h"

#include "CoreSystems/ProjectileDataAsset.h"

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
	localController->Possess(RefreshGameState());
	SetActorTickEnabled(true);
}

void ABBInvadersGameModeBase::GoToMainMenu()
{
	//god I wasted like 2 hours trying to get why didnt camera switch work properly
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

	if (PlayerControllerClass && 
		PlayerControllerClass->IsChildOf<ABBInvadersPlayerController>()) {

	}
	else {
		ValidationErrors.Add(FText::FromString("Invalid PlayerControllerClass"));
	}

	if (GameStateClass &&
		GameStateClass->IsChildOf<ABBInvadersGameStateBase>()) {

	}
	else {
		ValidationErrors.Add(FText::FromString("Invalid GameStateClass"));
	}

	if (PlayerStateClass &&
		PlayerStateClass->IsChildOf<ABBInvadersPlayerState>()) {

	}
	else {
		ValidationErrors.Add(FText::FromString("Invalid PlayerStateClass"));
	}

	return ValidationErrors.Num() > 0 ?
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
	RefreshGameState();
	localController->GetBBInvadersHUD()->RequestBindings(*this);
}

APawn* ABBInvadersGameModeBase::RefreshGameState()
{
	UWorld* world{ GetWorld() };
	APlayerPawn* pawn{ BBInvadersUtils::GetFirstActor<APlayerPawn>(world) };
	check(world && pawn);

	auto* gameState{ GetGameState<ABBInvadersGameStateBase>() };
	gameState->SetMapInfo(*pawn, pawn->CalcMapHalfSize());

	AOutOfAreaActorTracker* tracker{ 
		BBInvadersUtils::GetFirstActor<AOutOfAreaActorTracker>(world) };

	if (tracker) {
		tracker->SetTrackArea(pawn->GetActorTransform(), gameState->mapInfo.halfSize);
	}
	else {
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		tracker = world->SpawnActor<AOutOfAreaActorTracker>(
			AOutOfAreaActorTracker::StaticClass(), pawn->GetActorTransform(), params);
	}
	
	return pawn;
}

FVector ABBInvadersGameModeBase::CalcRandOutOfBoundsPos(float objectRadius) const
{
	return GetGameState<ABBInvadersGameStateBase>()->CalcRandOutOfBoundsPos(objectRadius);
}

AOrbit* ABBInvadersGameModeBase::SpawnNewOrbit(float additionalRadius)
{
	ABBInvadersGameStateBase* gameState{ GetGameState<ABBInvadersGameStateBase>() };
	check(gameState);
	const FPlayAreaInfo* mapInfo{ &gameState->mapInfo };

	auto* outermostOrbit{ ProcessCheckOrbits() };

	float newRadius{ outermostOrbit ?
		outermostOrbit->GetOuterRadius() : mapInfo->halfSize.Size2D() };

	FTransform newOrbitTransform{ 
		FRotationMatrix::MakeFromXZ(mapInfo->forward, mapInfo->up).ToQuat(),
		mapInfo->center };

	auto* newOrbit{ AOrbit::SpawnOrbit(*GetWorld(), newOrbitTransform, newRadius)};
	//cringe
	orbits.AddTail(TWeakObjectPtr<std::remove_pointer<decltype(newOrbit)>::type>{newOrbit});
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

