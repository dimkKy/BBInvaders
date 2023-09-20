// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CoreSystems/Shooter.h"
#include "MainMenuPawn.generated.h"

class UCameraComponent;

UCLASS()
class BBINVADERS_API AMainMenuPawn : public APawn, public IShooter
{
	GENERATED_BODY()

public:
	AMainMenuPawn();

	virtual void Shoot() override;
	virtual EShooterType GetShooterType() const override;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& context) const override;
#endif
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UCameraComponent* camera;
};
