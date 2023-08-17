// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GametimeUI.generated.h"

class UWidgetSwitcher;
class UButton;
class UPauseMenu;
class UCanvasPanel;
class UProjectileSelector;

/**
 * 
 */
UCLASS()
class BBINVADERS_API UGametimeUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	/*UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* newGameButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* exitButton;*/

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* pauseButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UPauseMenu* pauseMenu;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UProjectileSelector* projectileSelector;
	
protected:
	UFUNCTION()
		void OnPauseButtonClicked();
	UFUNCTION()
		void OnResumeButtonClicked();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UWidgetSwitcher* mainSwitcher;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* gametimeCanvas;

};
