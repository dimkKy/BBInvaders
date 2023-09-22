// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BBInvadersGameModeBase.generated.h"

class ABBInvadersPlayerController;
class AAdvancedInvader;
class AOrbit;
class AAsteroid;
class APawn;

struct FStreamableHandle;

/**
 * TO BE REFACTORED
 */
UCLASS()
class BBINVADERS_API ABBInvadersGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABBInvadersGameModeBase();
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual void Tick(float DeltaTime) override;

	void StartGameplay();
	void GoToMainMenu();
	void TogglePause();

	void OnGameOver();

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& context) const override;

	UFUNCTION(CallInEditor, Category = "Debug", meta = (DevelopmentOnly))
		void _SpawnNewAdvancedInvader() const;
	UFUNCTION(CallInEditor, Category = "Debug", meta = (DevelopmentOnly))
		void _SpawnNewAsteroid() const;
	UFUNCTION(CallInEditor, Category = "Debug", meta = (DevelopmentOnly))
		void _SpawnNewOrbit();

#endif // WITH_EDITOR
protected:
	virtual void BeginPlay() override;

	FVector CalcRandOutOfBoundsPos(double objectRadius) const;

	UE_NODISCARD AOrbit* SpawnNewOrbit(double additionalRadius = 0.);
	UE_NODISCARD AAdvancedInvader* SpawnNewAdvancedInvader() const;
	UE_NODISCARD AAsteroid* SpawnNewAsteroid() const;

	TWeakObjectPtr<ABBInvadersPlayerController> localController;
	TDoubleLinkedList<TWeakObjectPtr<AOrbit>> orbits;

	AOrbit* ProcessCheckOrbits();
	AOrbit* ProcessCheckOrbits(TFunction<void(AOrbit&)>&& func);

	static constexpr int32 maxOrbits{ 5 };
};

