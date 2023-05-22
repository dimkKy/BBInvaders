// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class ABBInvadersProjectile;

UCLASS(Abstract)
class BBINVADERS_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	APlayerPawn();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComp) override;

	FVector CalcMapHalfSize() const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* planet;
	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USpringArmComponent* platformArm;*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* platform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USpringArmComponent* cameraArm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UCameraComponent* camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<ABBInvadersProjectile> projectileClass;

	void MoveRight(float value);
	void MoveLeft(float value);
	//void Stop();
	void Accelerate(float value);
	
	void ZoomIn(float value);
	void ZoomOut(float value);
	void ZoomCamera(float value);


	void Shoot();

	UE_NODISCARD float CalcDamping() const;


	float currentRotationSpeed;
	//float targetRotationSpeed;


	static constexpr float maxSpeed = 120.f;
	static constexpr float dampingMultiplier = 0.95f;
	static constexpr float acceleration = 100.f;

	/*UPROPERTY(EditAnywhere, meta = (ClampMin = "1.0"))
		float maxSpeed;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.001", ClampMax = "100.1"))
		float dampingMultiplier;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.1"))
		float acceleration;*/

	//static constexpr float dampingMultiplier = -0.5f;
	static constexpr float zoomSpeed = 375.f;
	static constexpr float minZoomMultiplier = 1.15f;
	static constexpr float maxZoomMultiplier = 4.5f;

	FVector2D cameraArmLengthRange;
};
