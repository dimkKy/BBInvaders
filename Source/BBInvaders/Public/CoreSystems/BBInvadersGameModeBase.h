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
#endif
protected:
	virtual void BeginPlay() override;
	APawn* RefreshGameState();

	

	FVector CalcRandOutOfBoundsPos(float objectRadius);

	AOrbit* SpawnNewOrbit(float additionalRadius = 0.f);
	AAdvancedInvader* SpawnNewAdvancedInvader();
	AAsteroid* SpawnNewAsteroid();

	TWeakObjectPtr<ABBInvadersPlayerController> localController;
	TDoubleLinkedList<TWeakObjectPtr<AOrbit>> orbits;

	//to GameState
	FVector mapHalfSize;
	FVector mapCenter;
	FVector mapForward;
	FVector mapUp;

	static constexpr int32 maxOrbits = 5;
};
