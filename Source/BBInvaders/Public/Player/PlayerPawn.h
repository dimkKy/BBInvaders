// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CoreSystems/Shooter.h"
#include "PlayerPawn.generated.h"

class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class ABBInvadersProjectile;

/*
 *
 */
UCLASS(Abstract)
class BBINVADERS_API APlayerPawn : public APawn, public IShooter
{
	GENERATED_BODY()

public:
	APlayerPawn();
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComp) override;

	FVector CalcMapHalfSize() const;
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& context) const override;
#endif
protected:

	void MoveRight(float value);
	void MoveLeft(float value);
	void Accelerate(float value);

	void ZoomIn(float value);
	void ZoomOut(float value);

	void ZoomInAction();
	void ZoomOutAction();

	void ZoomCamera(float value);

	virtual void Shoot() override;
	virtual EShooterType GetShooterType() const override;

	UE_NODISCARD float CalcDamping() const;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* component,
			AActor* otherActor, UPrimitiveComponent* otherComp,
			int32 otherIndex, bool bFromSweep, const FHitResult& result);

	void OnGameOver();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* planet;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* platform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USpringArmComponent* cameraArm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UCameraComponent* camera;

	//UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)?
	float currentRotationSpeed;
	FVector2D cameraArmLengthRange;
	float health;

	static constexpr float maxHealth{ 100.f };
	static constexpr float healthRegenRate{ 1.f };

	static constexpr float maxSpeed{ 120.f };
	static constexpr float dampingMultiplier{ 0.95f };
	static constexpr float acceleration{ 100.f };

	static constexpr float zoomSpeed{ 375.f };
	static constexpr float zoomActionValue{ 10.f };
	static constexpr float minZoomMultiplier{ 1.15f };
	static constexpr float maxZoomMultiplier{ 4.5f };
};
