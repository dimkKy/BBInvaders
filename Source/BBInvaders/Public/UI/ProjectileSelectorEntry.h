// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ProjectileSelectorEntry.generated.h"

class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class BBINVADERS_API UProjectileSelectorEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* outline;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* currentCost;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* visibleName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* icon;

#if WITH_EDITOR
		virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
protected:
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	static const FNumberFormattingOptions formattingOptions;
};
