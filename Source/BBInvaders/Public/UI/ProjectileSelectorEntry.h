// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ProjectileSelectorEntry.generated.h"

class UTextBlock;
class UImage;
class UProjectileDataAsset;

/**
 * 
 */
UCLASS()
class BBINVADERS_API UProjectileSelectorEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	void UpdateCost(float currentInflation);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* outline;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* currentCost;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* visibleName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* icon;

#if WITH_EDITOR
		virtual EDataValidationResult IsDataValid(FDataValidationContext& context) const override;
#endif
protected:
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	void SetCost(float cost);

	static const FNumberFormattingOptions formattingOptions;
	
	//for faster access?
	const UProjectileDataAsset* cachedInfo;
};
