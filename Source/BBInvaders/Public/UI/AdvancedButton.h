// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
//#include "Blueprint/UserWidget.h"
#include "AdvancedButton.generated.h"

//class UButton;
class UTextBlock;

/**
 * UNFINISHED
 */
UCLASS()
class BBINVADERS_API UAdvancedButton : public UButton
{
	GENERATED_BODY()
		
public:
	UAdvancedButton();

	//UPROPERTY(meta = (BindWidget))
		//UButton* button;
	UPROPERTY(/*Instancedmeta = (BindWidget)*/)
		UTextBlock* text;
protected:
	virtual void OnWidgetRebuilt() override;
};
