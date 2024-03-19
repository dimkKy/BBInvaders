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

ABBIGameModeBase::ABBIGameModeBase() :
	localController{ nullptr }
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	//SetActorTickInterval(2.f);
}

void ABBIGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* player)
{
	Super::HandleStartingNewPlayer_Implementation(player);
	localController = CastChecked<ABBIPlayerController>(player);
}

void ABBIGameModeBase::Tick(float DeltaTime)
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

void ABBIGameModeBase::StartGameplay()
{
	localController->Possess(GetGameState<ABBIGameStateBase>()->Refresh());
	SetActorTickEnabled(true);
}

void ABBIGameModeBase::GoToMainMenu()
{
	ClearPause();
	//load level
	UWorld* world{ GetWorld() };
	using namespace BBInvadersUtils;
	AMainMenuPawn* menuPawn{ GetFirstActor<AMainMenuPawn>(world) };

	localController->Possess(menuPawn);
	SetActorTickEnabled(false);

	ForActorsOfClass<AInvader, AOrbit, AAsteroid, ABBIProjectile>
		(world, [](AActor* actor) {actor->Destroy(); });
}

void ABBIGameModeBase::TogglePause()
{
	//since we have only one player I decided not to bother doing it via PC
	if (IsPaused()) {
		ClearPause();
	}
	else {
		SetPause(localController.Get());
	}
}

void ABBIGameModeBase::OnGameOver()
{
	GoToMainMenu();
}

#if WITH_EDITOR
EDataValidationResult ABBIGameModeBase::IsDataValid(FDataValidationContext& context) const
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

	if (HUDClass && HUDClass->IsChildOf<ABBIHUD>()) {
		if (!HUDClass->IsInBlueprint()) {
			context.AddError(FText::FromString("Use blueprinted ABBInvadersHUD"));
		}
	}
	else {
		context.AddError(FText::FromString("Invalid HUDClass"));
	}

	if (PlayerControllerClass && 
		PlayerControllerClass->IsChildOf<ABBIPlayerController>()) {

	}
	else {
		context.AddError(FText::FromString("Invalid PlayerControllerClass"));
	}

	if (GameStateClass &&
		GameStateClass->IsChildOf<ABBIGameStateBase>()) {
		if (!GameStateClass->IsInBlueprint()) {
			context.AddWarning(FText::FromString("Consider blueprint GameStateClass"));
		}
	}
	else {
		context.AddError(FText::FromString("Invalid GameStateClass"));
	}

	if (PlayerStateClass &&
		PlayerStateClass->IsChildOf<ABBIPlayerState>()) {

	}
	else {
		context.AddError(FText::FromString("Invalid PlayerStateClass"));
	}

	return context.GetNumErrors() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}


void ABBIGameModeBase::_SpawnNewAdvancedInvader() const
{
	verify(SpawnNewAdvancedInvader());
}

void ABBIGameModeBase::_SpawnNewAsteroid() const
{
	verify(SpawnNewAsteroid());
}

void ABBIGameModeBase::_SpawnNewOrbit()
{
	verify(SpawnNewOrbit());
}
#endif

void ABBIGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	GetGameState<ABBIGameStateBase>()->Refresh();
}

FVector ABBIGameModeBase::CalcRandOutOfBoundsPos(double objectRadius) const
{
	return GetGameState<ABBIGameStateBase>()->CalcRandOutOfBoundsPos(objectRadius);
}

AOrbit* ABBIGameModeBase::SpawnNewOrbit(double additionalRadius)
{
	ABBIGameStateBase* gameState{ GetGameState<ABBIGameStateBase>() };
	check(gameState);
	const FPlayAreaInfo& mapInfo{ gameState->GetMapInfo() };

	//from tail?
	AOrbit* outermostOrbit{ ProcessCheckOrbits() };

	//out of sight always?
	/*double newRadius{ outermostOrbit ?
		outermostOrbit->GetOuterRadius() : mapInfo->halfSize.Size2D() };*/

	double newRadius{ outermostOrbit ?
		FMath::Max(outermostOrbit->GetOuterRadius(), mapInfo.Radius()) :
		mapInfo.Radius() };

	AOrbit* newOrbit{ AOrbit::SpawnOrbit(*GetWorld(), mapInfo.DefaultTransform(), newRadius)};
	
	orbits.AddTail(TWeakObjectPtr<AOrbit>{newOrbit});
	return newOrbit;
}

AAdvancedInvader* ABBIGameModeBase::SpawnNewAdvancedInvader() const
{	
	UWorld* world{ GetWorld() };
	const AActor* target{ GetGameState<ABBIGameStateBase>()->GetCenterActor() };
	//ensure?
	check(world && target);

	auto* newInvader{ AAdvancedInvader::SpawnAdvancedInvaderDeferred(*world, *target) };

	FVector location{ CalcRandOutOfBoundsPos(newInvader->GetCollisionRadius()) };

	newInvader->FinishSpawning({ FRotator::ZeroRotator, location });
	return newInvader;
}

AAsteroid* ABBIGameModeBase::SpawnNewAsteroid() const
{
	UWorld* world{ GetWorld() };
	//ensure?
	check(world);

	auto* newAsteroid{ AAsteroid::SpawnAsteroidDeferred(*world) };
	newAsteroid->FinishSpawningSetVelocity(
		CalcRandOutOfBoundsPos(newAsteroid->GetMeshRadius()), 
		CastChecked<ABBIGameStateBase>(world->GetGameState())->GetMapInfo().Center());

	return newAsteroid;
}

void ABBIGameModeBase::OnOrbitCleared(AOrbit* orbit)
{
	ProcessDeleteOrbit(orbit);
}

AOrbit* ABBIGameModeBase::ProcessCheckOrbits()
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

AOrbit* ABBIGameModeBase::ProcessCheckOrbits(TFunction<void(AOrbit&)>&& func)
{
	for (decltype(orbits)::TIterator it{orbits.GetHead()}; it; ) {
		if ((*it).IsValid()/*&& (*it)->GetInvadersNum()*/) {
			func(*(*it).Get());
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

AOrbit* ABBIGameModeBase::ProcessDeleteOrbit(AOrbit* orbit)
{
	check(orbit);

	for (decltype(orbits)::TIterator it{ orbits.GetHead() }; it; ) {
		if ((*it).IsValid() && (*it).Get() != orbit) {
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

