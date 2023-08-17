// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class BBINVADERS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* newGameButton;
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* exitButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* settingsButton;
};
