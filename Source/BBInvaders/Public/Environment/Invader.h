// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoreSystems/Shooter.h"
#include "Environment/PlanetaryThreatable.h"
#include "Invader.generated.h"

class UStaticMeshComponent;
class UStaticMesh;
class APlayerPawn;
class UProjectileData;

DECLARE_DELEGATE_OneParam(FNotifyInvaderDestroyed, AInvader*);

//Get rid of _MAX?
UENUM(BlueprintType)
enum class EInvaderType : uint8
{
	EIT_Default UMETA(DisplayName = "Default"),
	//EAS_Medium UMETA(DisplayName = "Medium"),
	EIT_Advanced UMETA(DisplayName = "Advanced"),
	EIT_MAX UMETA(Hidden),
};

UCLASS()
class BBINVADERS_API AInvader : public AActor, public IShooter, public IPlanetaryThreatable
{
	GENERATED_BODY()
	
public:	
	AInvader();

	void SetMesh(UStaticMesh& newMesh);
	double GetCollisionRadius() const;

	virtual void Shoot(UProjectileData* data) override;
	virtual EShooterType GetShooterType() const override;

	//void SetLookAt(const FVector& worldPos);

	virtual float GetOnPlanetCollisionDamage() const override;

	virtual int32 GetOnKillBounty() const;

	FNotifyInvaderDestroyed onDestroyedDelegate;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& context) const override;
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
