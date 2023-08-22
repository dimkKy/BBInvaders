// by Dmitry Kolontay


#include "UI/ProjectileSelectorEntry.h"
#include "CoreSystems/ProjectileDataAsset.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Blueprint/WidgetTree.h"

const FNumberFormattingOptions UProjectileSelectorEntry::formattingOptions{
	FNumberFormattingOptions().SetMaximumFractionalDigits(2) };

void UProjectileSelectorEntry::NativeOnInitialized()
{
	outline->Brush.DrawAs = ESlateBrushDrawType::Border;
	outline->SetRenderOpacity(0.f);

	icon->Brush.DrawAs = ESlateBrushDrawType::Image;
}

#if WITH_EDITOR
EDataValidationResult UProjectileSelectorEntry::IsDataValid(TArray<FText>& ValidationErrors)
{
	Super::IsDataValid(ValidationErrors);

	if (WidgetTree->RootWidget != outline)
		ValidationErrors.Add(FText::FromString("outline is supposed to be the root"));

	return ValidationErrors.Num() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif

void UProjectileSelectorEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	outline->SetRenderOpacity(bIsSelected ? 1.f : 0.f);
}

void UProjectileSelectorEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UProjectileDataAsset* projectileInfo{ CastChecked<UProjectileDataAsset>(ListItemObject) };

	currentCost->SetText(FText::AsNumber(projectileInfo->baseCost, &formattingOptions));
	visibleName->SetText(projectileInfo->visibleName);
	icon->SetBrushFromSoftTexture(projectileInfo->icon, true);
}
