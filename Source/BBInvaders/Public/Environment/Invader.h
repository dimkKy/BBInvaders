// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoreSystems/Shooter.h"
#include "Environment/PlanetaryThreatable.h"
#include "Invader.generated.h"

class UStaticMeshComponent;
class UStaticMesh;
class ABBInvadersProjectile;
class APlayerPawn;

DECLARE_DELEGATE_OneParam(FNotifyInvaderDestroyed, AInvader*);

UCLASS()
class BBINVADERS_API AInvader : public AActor, public IShooter, public IPlanetaryThreatable
{
	GENERATED_BODY()
	
public:	
	AInvader();

	void SetMesh(UStaticMesh& newMesh);

	virtual void Shoot() override;
	virtual EShooterType GetShooterType() const override;

	//void SetLookAt(const FVector& worldPos);

	virtual float GetOnPlanetCollisionDamage() const override;

	virtual int32 GetOnKillBounty() const;

	FNotifyInvaderDestroyed onDestroyedDelegate;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* component,
			AActor* otherActor, UPrimitiveComponent* otherComp,
			int32 otherIndex, bool bFromSweep, const FHitResult& result);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* body;

	void DestroyGiveReward(APlayerPawn* bountyReceiver = nullptr);
};
