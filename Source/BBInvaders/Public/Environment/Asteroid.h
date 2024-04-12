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

namespace EAS {
	inline constexpr EAsteroidSize Small{ EAsteroidSize::EAS_Small };
	inline constexpr EAsteroidSize Medium{ EAsteroidSize::EAS_Medium };
	inline constexpr EAsteroidSize Big{ EAsteroidSize::EAS_Big };

	inline EAsteroidSize Smaller(EAsteroidSize size) {
		if (size != Small
			/*ensureAlways(!BBInvadersUtils::IsIn(_size, EAsteroidSize::EAS_MAX, EAsteroidSize::EAS_Small))*/) {
			return static_cast<EAsteroidSize>(static_cast<int32>(size) - 1);
		}
		else {
			return Small;
		}
	}
	inline EAsteroidSize Random() {
		return static_cast<EAsteroidSize>(
			FMath::RandRange(0, static_cast<int32>(EAsteroidSize::EAS_MAX) - 1));
	}
}

//class UInstancedStaticMeshComponent;
class UStaticMeshComponent;
class UStaticMesh;
class UAsteroidMeshSet;
struct FDamageEvent;

UCLASS()
class BBINVADERS_API AAsteroid : public AActor, public IPlanetaryThreatable
{
	GENERATED_BODY()
	
public:	
	AAsteroid();
	virtual void Tick(float DeltaTime) override;

	static AAsteroid* SpawnAsteroid(UWorld& w, const FVector& location,
		const FVector& targetLoc, EAsteroidSize size = EAS::Random());
	UE_NODISCARD static AAsteroid* SpawnAsteroidDeferred(UWorld& w, EAsteroidSize size = EAS::Random());
	void FinishSpawningSetVelocity(const FVector& location, const FVector& targetLoc);

	void SetSizeAssignMesh(EAsteroidSize newSize);

	void CalculateRandomVelocity(const FVector& targetLocation);
	//void CalculateRandomVelocity(const FVector& newVel);

	void SetRotation();
	void SetRotation(const FVector& rotAxis, float rotSpeed);

	float GetMeshRadius() const;
	
	virtual float GetOnPlanetCollisionDamage() const override;

	virtual float TakeDamage(float amount, FDamageEvent const& dEvent, AController* instigator, AActor* causer) override;

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
		const UAsteroidMeshSet* meshSet;

	EAsteroidSize size;
	FVector velocity;
	float health{ 100.f };
	constexpr static float aimAngleAmplitude{ 40.f } /** PI / 180.f*/;
	constexpr static float splitAngleAmplitude{ 30.f } /** PI / 180.f*/;
	constexpr static float onHitVelocityMultiplier{ 0.9f };
	constexpr static std::pair<float, float> velocityRange{ 100.f, 500.f };
	constexpr static std::pair<float, float> rotationSpeedRange{ 0.1f, 2.f };
};
