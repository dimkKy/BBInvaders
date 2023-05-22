// by Dmitry Kolontay


#include "Player/MainMenuCameraActor.h"
#include "Camera/CameraComponent.h"
#include "Player/PlayerPawn.h"
//#include "BBInvadersUtils.h"
#include "Math/UnrealMathUtility.h"
//#include "Kismet/GameplayStatics.h"

AMainMenuCameraActor::AMainMenuCameraActor() :
	camera{ CreateDefaultSubobject<UCameraComponent>("camera") }
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(camera);
}

void AMainMenuCameraActor::BeginPlay()
{

	// If we find a matching PC, bind to it immediately.
	/*APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		PC->SetViewTarget(this);
	}*/
	Super::BeginPlay();
	/*for (TActorIterator<AActor> it(GetWorld(), ActorClass); it; ++it)
	{
		AActor* Actor = *It;
		return Actor;
	}*/
	//GetActorUpVector();
	//GetActorUpVector();
	/*if (APlayerPawn* player{ nullptr/*BBInvadersUtils::GetFirstActor<APlayerPawn>(GetWorld())* }) {
		
		auto playerUP{ player->GetActorUpVector() };

		if (FVector::Parallel(playerUP, GetActorForwardVector())) {
			//TODO
			check(false);
		}
		else {
			//FMath::RayPlaneIntersection();
		}
		
	}*/
}

void AMainMenuCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainMenuCameraActor::ThrowAsteroid()
{
}

