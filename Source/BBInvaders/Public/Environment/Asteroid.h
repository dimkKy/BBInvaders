// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"
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
class BBINVADERS_API AAsteroid : public AActor
{
	GENERATED_BODY()
	
public:	
	AAsteroid();
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		//UInstancedStaticMeshComponent* asteroids;

	static void SpawnNewAsteroid(const FVector2D& locationNormalized);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* component,
			AActor* otherActor, UPrimitiveComponent* otherComp,
			int32 otherIndex, bool bFromSweep, const FHitResult& result);

protected:
	virtual void BeginPlay() override;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* body;

	EAsteroidSize size;

	constexpr static float maxRotationDefiationHalfAngle = 40.f /** PI / 180.f*/;

	FVector currentVelocity;

};
