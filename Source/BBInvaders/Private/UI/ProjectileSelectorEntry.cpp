// by Dmitry Kolontay


#include "UI/ProjectileSelectorEntry.h"
#include "CoreSystems/ProjectileData.h"
#include "CoreSystems/BBIGameStateBase.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Blueprint/WidgetTree.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

const FNumberFormattingOptions UProjectileSelectorEntry::formattingOptions{
	FNumberFormattingOptions().SetMaximumFractionalDigits(2) };

void UProjectileSelectorEntry::NativeOnInitialized()
{
	auto tempBrush{ outline->GetBrush() };
	tempBrush.DrawAs = ESlateBrushDrawType::Image;
	outline->SetBrush(tempBrush);

	outline->SetRenderOpacity(0.f);
}

void UProjectileSelectorEntry::UpdateCost(float currentInflation)
{
	check(cachedInfo);
	SetCost(cachedInfo->baseCost * currentInflation);
}

#if WITH_EDITOR
EDataValidationResult UProjectileSelectorEntry::IsDataValid(FDataValidationContext& context) const
{
	Super::IsDataValid(context);

	if (WidgetTree->RootWidget != outline)
		context.AddError(FText::FromString("outline is supposed to be the root"));

	return context.GetNumErrors() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif

void UProjectileSelectorEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	outline->SetRenderOpacity(bIsSelected ? 1.f : 0.f);
}

void UProjectileSelectorEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UProjectileData* projectileInfo{ CastChecked<UProjectileData>(ListItemObject) };
	cachedInfo = CastChecked<UProjectileData>(ListItemObject);

	SetCost(projectileInfo->baseCost
		//* CastChecked<ABBIGameStateBase>(GetWorld()->GetGameState())->GetCurrentInflation()
	);

	visibleName->SetText(projectileInfo->visibleName);
	icon->SetBrushFromSoftTexture(projectileInfo->icon, true);
}

void UProjectileSelectorEntry::SetCost(float cost)
{
	//add currency later
	currentCost->SetText(FText::AsNumber(cost, &formattingOptions));
}
