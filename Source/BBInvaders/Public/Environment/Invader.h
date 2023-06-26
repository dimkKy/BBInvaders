// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Environment/PlanetaryThreatable.h"
#include "Invader.generated.h"

class UStaticMeshComponent;
class UStaticMesh;
class ABBInvadersProjectile;
class APlayerPawn;

UCLASS()
class BBINVADERS_API AInvader : public AActor, public IPlanetaryThreatable
{
	GENERATED_BODY()
	
public:	
	AInvader();

	void SetMesh(UStaticMesh& newMesh);
	//interface?
	void Shoot();
	//void SetLookAt(const FVector& worldPos);

	virtual float GetOnPlanetCollisionDamage() const override;

	virtual int32 GetOnKillBounty() const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* component,
			AActor* otherActor, UPrimitiveComponent* otherComp,
			int32 otherIndex, bool bFromSweep, const FHitResult& result);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* body;

	void DestroyGiveReward(APlayerPawn* bountyReceiver = nullptr);
};
