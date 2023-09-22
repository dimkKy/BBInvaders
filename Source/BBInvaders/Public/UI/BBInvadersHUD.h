// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BBInvadersHUD.generated.h"

class UGametimeUI;
class UMainMenu;
//class PauseMenu;

class ABBInvadersPlayerController;
class UProjectileSelector;
/**
 * 
 */
UCLASS(Abstract)
class BBINVADERS_API ABBInvadersHUD : public AHUD
{
	GENERATED_BODY()

public:
	ABBInvadersHUD();
	virtual void PostInitializeComponents() override;

	void OnViewTargetChange(const AActor& newTarget);

	void RequestBindings(ABBInvadersPlayerController& gameMode);

	UProjectileSelector* GetProjectileSelector() const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& context) const override;
#endif
protected:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UGametimeUI> gametimeUIClass;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UMainMenu> mainMenuClass;

	UPROPERTY()
		UGametimeUI* gametimeUI;
	UPROPERTY()
		UMainMenu* mainMenu;

};
