// by Dmitry Kolontay


#include "CoreSystems/BBInvadersGameModeBase.h"
#include "UI/BBInvadersHUD.h"
#include "Player/BBInvadersPlayerController.h"
#include "Player/PlayerPawn.h"
#include "Player/BBInvadersPlayerState.h"
#include "BBInvadersUtils.h"
#include "Player/MainMenuPawn.h"
#include "CoreSystems/AsteroidTracker.h"
#include "CoreSystems/BBInvadersProjectile.h"
#include "CoreSystems/BBInvadersGameStateBase.h"
#include "Environment/Invader.h"
#include "Environment/AdvancedInvader.h"
#include "Environment/Asteroid.h"
#include "Environment/Orbit.h"
#include "CoreSystems/AssetProvider.h"
#include "Containers/List.h"

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

	/*check(localController.IsValid());
	APawn* playerPawn{ localController->GetPawn() };
	check(playerPawn);

	for (decltype(orbits)::TIterator it{orbits.GetHead()}; it; ) {
		if ((*it).IsValid()) {

			//operate orbit
			if (true) {
				(*it)->SetRotationSpeed(true);
			}

			++it;
		}
		else {
			auto* node{ it.GetNode()};
			++it;
			orbits.RemoveNode(node);
		}
	}
	//shrink orbits
	if (true) {
		for (auto& orbit : orbits) {
			orbit->Shrink(1.f);
		}
	}
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

	BBInvadersUtils::ForActorsOfClass
		<AInvader, AOrbit, AAsteroid, ABBInvadersProjectile>
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

	if (PlayerStateClass &&
		PlayerStateClass->IsChildOf<ABBInvadersPlayerState>()) {

	}
	else {
		ValidationErrors.Add(FText::FromString("Invalid PlayerStateClass"));
	}

	return ValidationErrors.Num() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif

void ABBInvadersGameModeBase::BeginPlay()
{
	RefreshGameState();
	Super::BeginPlay();
	localController->GetBBInvadersHUD()->RequestBindings(*this);
}

APawn* ABBInvadersGameModeBase::RefreshGameState()
{
	UWorld* world{ GetWorld() };
	APlayerPawn* pawn{ BBInvadersUtils::GetFirstActor<APlayerPawn>(world) };
	check(world && pawn);

	ABBInvadersGameStateBase* gameState{ GetGameState<ABBInvadersGameStateBase>() };
	gameState->SetMapInfo(*pawn, pawn->CalcMapHalfSize());
	
	if (AAsteroidTracker* tracker{ BBInvadersUtils::GetFirstActor<AAsteroidTracker>(world) }) {
		tracker->SetTrackArea(pawn->GetActorTransform(), gameState->mapInfo.halfSize);
	}
	else {
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = 
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		world->SpawnActor<AAsteroidTracker>(
			AAsteroidTracker::StaticClass(), pawn->GetActorTransform(), params);
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

	float newRadius{ orbits.Num() ?
		orbits.GetTail()->GetValue()->GetOuterRadius() :
		gameState->mapInfo.halfSize.Size2D()
	};

	FActorSpawnParameters params;
	params.Owner = this;
	params.SpawnCollisionHandlingOverride = 
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//TODO
	auto* newOrbit{ GetWorld()->SpawnActor<AOrbit>(gameState->mapInfo.center,
		FRotationMatrix::MakeFromXZ(gameState->mapInfo.forward, gameState->mapInfo.up).Rotator(), 
		params) };

	return nullptr;
}

AAdvancedInvader* ABBInvadersGameModeBase::SpawnNewAdvancedInvader() const
{	
	UStaticMesh* mesh{ GetWorld()->GetSubsystem<UAssetProvider>()->invaderMesh };
	FVector location{ CalcRandOutOfBoundsPos(mesh->GetBounds().GetSphere().W)};

	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto* newInvader{ GetWorld()->SpawnActor<AAdvancedInvader>(
		location,
		FRotator::ZeroRotator, params) };

	newInvader->SetMesh(*mesh);
	return newInvader;
}

AAsteroid* ABBInvadersGameModeBase::SpawnNewAsteroid() const
{
	auto* newAsteroid{ GetWorld()->SpawnActorDeferred<AAsteroid>(
		AAsteroid::StaticClass(), FTransform::Identity,
		nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn) };

	newAsteroid->SetSizeAssignMesh(static_cast<EAsteroidSize>(
		FMath::RandRange(0, static_cast<int32>(EAsteroidSize::EAS_MAX) - 1)));

	FVector location{ CalcRandOutOfBoundsPos(newAsteroid->GetMeshRadius()) };
	FRotator randomRotator{
		BBInvadersUtils::RandomAngle(),
		BBInvadersUtils::RandomAngle(),
		BBInvadersUtils::RandomAngle()
	};

	newAsteroid->FinishSpawning(
		{ randomRotator, location, FVector::OneVector }
		, false);

	return newAsteroid;
}

