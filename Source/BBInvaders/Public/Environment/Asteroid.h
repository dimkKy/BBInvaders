// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Environment/PlanetaryThreatable.h"
#include "Asteroid.generated.h"


//Get rid of _MAX?
UENUM(BlueprintType)
enum class EAsteroidSize : uint8
{
	EAS_Small UMETA(DisplayName = "Small"),
	EAS_Medium UMETA(DisplayName = "Medium"),
	EAS_Big UMETA(DisplayName = "Big"),
	EAS_MAX UMETA(Hidden),
};

//class UInstancedStaticMeshComponent;
class UStaticMeshComponent;
class UStaticMesh;
class UAsteroidMeshSet;

UCLASS()
class BBINVADERS_API AAsteroid : public AActor, public IPlanetaryThreatable
{
	GENERATED_BODY()
	
public:	
	AAsteroid();
	virtual void Tick(float DeltaTime) override;

	static ThisClass* SpawnAsteroid(UWorld& w, const FVector& location,
		const FVector& targetLoc, EAsteroidSize size = AAsteroid::RandomSize());
	UE_NODISCARD static ThisClass* SpawnAsteroidDeferred(UWorld& w, EAsteroidSize size = AAsteroid::RandomSize());
	void FinishSpawningSetVelocity(const FVector& location, const FVector& targetLoc);

	static EAsteroidSize GetSmaller(EAsteroidSize _size);
	static EAsteroidSize RandomSize();

	void SetSizeAssignMesh(EAsteroidSize newSize);

	void CalculateRandomVelocity(const FVector& targetLocation);
	//void CalculateRandomVelocity(const FVector& newVel);

	void SetRotation();
	void SetRotation(const FVector& rotAxis, float rotSpeed);

	float GetMeshRadius() const;
	
	virtual float GetOnPlanetCollisionDamage() const override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* component,
			AActor* otherActor, UPrimitiveComponent* otherComp,
			int32 otherIndex, bool bFromSweep, const FHitResult& result);

	static AAsteroid* SpawnAsteroid(UWorld& w, const FVector& location, EAsteroidSize size);

	AAsteroid* Split();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* body;

	UPROPERTY()
		const UAsteroidMeshSet* projectileData;

	EAsteroidSize size;
	FVector velocity;

	constexpr static float aimAngleAmplitude{ 40.f } /** PI / 180.f*/;
	constexpr static float splitAngleAmplitude{ 30.f } /** PI / 180.f*/;
	constexpr static float onHitVelocityMultiplier{ 0.9f };
	constexpr static std::pair<float, float> velocityRange{ 100.f, 500.f };
	constexpr static std::pair<float, float> rotationSpeedRange{ 0.1f, 2.f };
};
