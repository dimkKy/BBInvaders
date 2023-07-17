// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BBInvadersGameModeBase.generated.h"

class ABBInvadersPlayerController;
class AOrbit;
class AAdvancedInvader;
class AAsteroid;
class APawn;
class ABBInvadersGameStateBase;

/**
 * TO BE REFACTORED (GAS?)
 */
UCLASS()
class BBINVADERS_API ABBInvadersGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABBInvadersGameModeBase();
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void StartGameplay();
	UFUNCTION()
		void GoToMainMenu();
	UFUNCTION()
		void TogglePause();

	void OnGameOver();

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		void _SpawnNewAdvancedInvader() const;
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		void _SpawnNewAsteroid() const;
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Debug", meta = (DevelopmentOnly))
		void _SpawnNewOrbit();

#endif // WITH_EDITOR
protected:
	virtual void BeginPlay() override;
	APawn* RefreshGameState();

	FVector CalcRandOutOfBoundsPos(float objectRadius) const;

	UE_NODISCARD AOrbit* SpawnNewOrbit(float additionalRadius = 0.f);
	UE_NODISCARD AAdvancedInvader* SpawnNewAdvancedInvader() const;
	UE_NODISCARD AAsteroid* SpawnNewAsteroid() const;
	
	void ShrinkOrbits(float deltaTime);

	TWeakObjectPtr<ABBInvadersPlayerController> localController;
	TDoubleLinkedList<TWeakObjectPtr<AOrbit>> orbits;

	static constexpr int32 maxOrbits = 5;
};

