// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class BBINVADERS_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* resumeButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* toMainMenuButton;
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* exitButton;
};
