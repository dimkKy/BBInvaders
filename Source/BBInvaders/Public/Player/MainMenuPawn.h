// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainMenuPawn.generated.h"

class UCameraComponent;

UCLASS()
class BBINVADERS_API AMainMenuPawn : public APawn
{
	GENERATED_BODY()

public:
	AMainMenuPawn();

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UCameraComponent* camera;
};
