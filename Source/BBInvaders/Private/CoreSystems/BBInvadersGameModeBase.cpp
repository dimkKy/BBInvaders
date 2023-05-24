// by Dmitry Kolontay


#include "CoreSystems/BBInvadersGameModeBase.h"
#include "UI/BBInvadersHUD.h"
#include "Player/BBInvadersPlayerController.h"
#include "Player/PlayerPawn.h"
#include "BBInvadersUtils.h"
#include "Player/MainMenuPawn.h"
#include "CoreSystems/AsteroidTracker.h"

#include "Environment/Invader.h"
#include "Environment/AdvancedInvader.h"
#include "Environment/Asteroid.h"
#include "Environment/Orbit.h"
#include "CoreSystems/AssetProvider.h"
#include "Containers/List.h"

ABBInvadersGameModeBase::ABBInvadersGameModeBase() :
	localController{ nullptr }, mapHalfSize{0.f}
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	SetActorTickInterval(2.f);
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

	//BBInvadersUtils::ForActorsOfClass<AInvader, AOrbit, AAsteroid>(world, [](AActor* actor) {actor->Destroy(); });
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
	RefreshGameState();
	Super::BeginPlay();
	localController->GetBBInvadersHUD()->RequestBindings(*this);
}

APawn* ABBInvadersGameModeBase::RefreshGameState()
{
	UWorld* world{ GetWorld() };
	APlayerPawn* pawn{ BBInvadersUtils::GetFirstActor<APlayerPawn>(world) };
	check(world && pawn);
	mapCenter = pawn->GetActorLocation();
	mapForward = pawn->GetActorForwardVector();
	mapUp = pawn->GetActorUpVector();
	mapHalfSize = pawn->CalcMapHalfSize();
	
	if (AAsteroidTracker* tracker{ BBInvadersUtils::GetFirstActor<AAsteroidTracker>(world) }) {
		tracker->SetTrackArea(pawn->GetActorTransform(), mapHalfSize);
	}
	else {
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		world->SpawnActor<AAsteroidTracker>(
			AAsteroidTracker::StaticClass(), pawn->GetActorTransform(), params);
	}
	return pawn;
}

FVector ABBInvadersGameModeBase::CalcRandOutOfBoundsPos(float objectRadius)
{
	float angle{ FMath::RandRange(0.f, 360.f) };
	return mapForward.RotateAngleAxis(angle, mapUp) * mapHalfSize.Size2D() + objectRadius;
}

AOrbit* ABBInvadersGameModeBase::SpawnNewOrbit(float additionalRadius)
{
	return nullptr;
}

AAdvancedInvader* ABBInvadersGameModeBase::SpawnNewAdvancedInvader()
{
	auto* newInvader{ GetWorld()->SpawnActorDeferred<AAdvancedInvader>(
		AAdvancedInvader::StaticClass(), FTransform::Identity,
		nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn) };
	
	UStaticMesh* mesh{ GetWorld()->GetSubsystem<UAssetProvider>()->invaderMesh };
	newInvader->SetMesh(*mesh);
	FVector location{ CalcRandOutOfBoundsPos(mesh->GetBounds().GetSphere().W)};

	newInvader->FinishSpawning({ 
		FRotationMatrix::MakeFromXZ(mapCenter - location, mapUp).Rotator(),
		location, FVector::OneVector }, false);

	return newInvader;
}

AAsteroid* ABBInvadersGameModeBase::SpawnNewAsteroid()
{
	auto* newAsteroid{ GetWorld()->SpawnActorDeferred<AAsteroid>(
		AAsteroid::StaticClass(), FTransform::Identity,
		nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn) };

	newAsteroid->SetSizeAssignMesh(static_cast<EAsteroidSize>(FMath::RandRange(0, static_cast<int32>(EAsteroidSize::EAS_MAX) - 1)));

	FVector location{ CalcRandOutOfBoundsPos(newAsteroid->GetMeshRadius()) };

	newAsteroid->FinishSpawning({
		FRotationMatrix::MakeFromXZ(mapCenter - location, mapUp).Rotator(),
		location, FVector::OneVector }, false);

	return newAsteroid;
}

