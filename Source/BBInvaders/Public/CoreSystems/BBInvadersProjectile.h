// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "BBInvadersProjectile.generated.h"

class UProjectileMovementComponent;
class UProjectileDataAsset;

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

	void SetProjectileData(const UProjectileDataAsset& data);
	
	/*static ABBInvadersProjectile* SpawnProjectile(UWorld& world, 
		const FTransform& transform, AActor* owner, const FProjectileData& data);*/

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UProjectileMovementComponent* movement;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* body;

	static constexpr float lifespan{ 40.f };
	static constexpr int32 simulationInteractions{ 3 };

};
