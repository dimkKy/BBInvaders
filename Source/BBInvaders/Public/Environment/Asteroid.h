// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"
#include "Asteroid.generated.h"


UENUM(BlueprintType)
enum EAsteroidSize
{
	EAS_Big,
	EAS_Medium,
	EAS_Small,
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
	

	UPROPERTY(EditDefaultsOnly)
		UStaticMesh* meshes[EAS_MAX];

	static void SpawnNewAsteroid(const FVector2D& locationNormalized);


protected:
	virtual void BeginPlay() override;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* body;

	constexpr static float maxRotationDefiationHalfAngle = 40.f /** PI / 180.f*/;

};
