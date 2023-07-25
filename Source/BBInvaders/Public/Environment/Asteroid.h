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
	EAS_Small,
	EAS_Medium,
	EAS_Big,
	EAS_MAX,
};

//class UInstancedStaticMeshComponent;
class UStaticMeshComponent;
class UAssetProvider;
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

	static EAsteroidSize GetSmaller(EAsteroidSize _size);
	static EAsteroidSize RandomSize();

	void SetSizeAssignMesh(EAsteroidSize newSize, const UAssetProvider& provider);

	void SetVelocity(const AActor& target);
	void SetVelocity(const FVector& newVel);

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

	void Split();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* body;

	EAsteroidSize size;
	FVector velocity;

	constexpr static float aimAngleAmplitude{ 40.f } /** PI / 180.f*/;
	constexpr static float splitAngleAmplitude{ 30.f } /** PI / 180.f*/;
	constexpr static float onHitVelocityMultiplier{ 0.9f };
	constexpr static std::pair<float, float> velocityRange{ 100.f, 500.f };
	constexpr static std::pair<float, float> rotationSpeedRange{ 0.1f, 2.f };
};
