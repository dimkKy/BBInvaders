// by Dmitry Kolontay


#include "CoreSystems/ProjectileDataAsset.h"
#include "Engine/Texture2D.h"
#include "CoreSystems/BBInvadersAssetManager.h"
//#include "CoreSystems/Shooter.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

const FPrimaryAssetType UProjectileDataAsset::assetType{ "ProjectileData" };

UProjectileDataAsset::UProjectileDataAsset() :
	userType{ EShooterType::EST_MAX }, visibleName {}, initialSpeed{ 0.f }, 
	maxSpeed{ 100.f }, bIsHoming{ false }, baseCost { 0.f }
{

}

FPrimaryAssetId UProjectileDataAsset::GetPrimaryAssetId() const
{
	return {assetType, GetFName()};
}

void UProjectileDataAsset::PreLoadAsync(bool bLoadMesh, FStreamableDelegate onIconLoaded)
{
	if (icon.IsPending()) {
		UBBInvadersAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
			icon.ToSoftObjectPath(), MoveTemp(onIconLoaded), FStreamableManager::AsyncLoadHighPriority);
	}
	if (bLoadMesh && bodyMesh.IsPending()) {
		UBBInvadersAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
			bodyMesh.ToSoftObjectPath(), FStreamableDelegate{}, FStreamableManager::AsyncLoadHighPriority);
	}
	
}

#if WITH_EDITOR
EDataValidationResult UProjectileDataAsset::IsDataValid(FDataValidationContext& context) const
{
	Super::IsDataValid(context);

	if (maxSpeed <= 0.f)
		context.AddError(FText::FromString("maxSpeed have to be positive"));

	if (maxSpeed < initialSpeed)
		context.AddError(FText::FromString("maxSpeed can not be lower then initialSpeed"));

	if (baseCost < 0.f) {
		context.AddError(FText::FromString("baseCost can not be negative"));
	}
	else {
		if (userType == EShooterType::EST_PlayerOnly && baseCost == 0.f) {
			context.AddError(FText::FromString("baseCost should be positive for player to use this"));
		}
	}		

	if (userType == EShooterType::EST_MAX) {
		context.AddError(FText::FromString("invalid userType"));
	}

	return context.GetNumErrors() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif
