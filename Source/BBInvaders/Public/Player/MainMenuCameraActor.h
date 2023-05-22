// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainMenuCameraActor.generated.h"

class UCameraComponent;

UCLASS()
class BBINVADERS_API AMainMenuCameraActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMainMenuCameraActor();
	virtual void Tick(float DeltaTime) override;

	void ThrowAsteroid();
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UCameraComponent* camera;	

	FVector planetLocation;
	
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		//TSubclassOf<ABBInvadersProjectile> projectileClass;
};
