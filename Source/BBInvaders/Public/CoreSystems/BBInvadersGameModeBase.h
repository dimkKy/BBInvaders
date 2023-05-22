// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BBInvadersGameModeBase.generated.h"

class ABBInvadersPlayerController;

/**
 * 
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


#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
	//void SetMapHalfSize(const FVector2D& size);
protected:
	virtual void BeginPlay() override;

	void ChangeGameMode(bool bToMenu, bool bChangeView);

	TWeakObjectPtr<ABBInvadersPlayerController> localController;
	FVector mapHalfSize;
};
