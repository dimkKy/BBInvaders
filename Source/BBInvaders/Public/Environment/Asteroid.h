// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Environment/PlanetaryThreatable.h"
#include "Asteroid.generated.h"

UENUM(BlueprintType)
enum EAsteroidSize
{
	EAS_Small,
	EAS_Medium,
	EAS_Big,
	EAS_MAX,
};

//class UInstancedStaticMeshComponent;
class UStaticMeshComponent;
class UStaticMesh;

UCLASS()
class BBINVADERS_API AAsteroid : public AActor, public IPlanetaryThreatable
{
	GENERATED_BODY()
	
public:	
	AAsteroid();
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		//UInstancedStaticMeshComponent* asteroids;

	void SetSizeAssignMesh(EAsteroidSize newSize);
	void SetVelocity(float newVelocity);
	float GetMeshRadius() const;
	
	virtual float GetOnPlanetCollisionDamage() const override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* component,
			AActor* otherActor, UPrimitiveComponent* otherComp,
			int32 otherIndex, bool bFromSweep, const FHitResult& result);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* body;

	EAsteroidSize size;
	float currentVelocity;

	constexpr static float maxRotationDeviationHalfAngle = 40.f /** PI / 180.f*/;
	constexpr static float maxSplitDeviationHalfAngle = 30.f /** PI / 180.f*/;
	constexpr static float onHitVelocityMultiplier = 0.9f;
};
