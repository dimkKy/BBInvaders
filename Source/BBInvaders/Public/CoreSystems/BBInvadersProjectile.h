// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BBInvadersProjectile.generated.h"

class UProjectileMovementComponent;

UCLASS(Abstract)
class BBINVADERS_API ABBInvadersProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABBInvadersProjectile();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* component, 
			AActor* otherActor, UPrimitiveComponent* otherComp, 
			int32 otherIndex, bool bFromSweep, const FHitResult& result);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UProjectileMovementComponent* movement;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* body;

	static constexpr float lifespan{ 40.f };
	static constexpr int32 simulationInteractions{ 3 };

};
