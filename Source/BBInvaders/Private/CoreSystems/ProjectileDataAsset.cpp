// by Dmitry Kolontay


#include "CoreSystems/ProjectileDataAsset.h"
#include "Engine/Texture2D.h"
#include "CoreSystems/BBInvadersAssetManager.h"
//#include "CoreSystems/Shooter.h"

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
EDataValidationResult UProjectileDataAsset::IsDataValid(TArray<FText>& ValidationErrors)
{
	Super::IsDataValid(ValidationErrors);

	if (maxSpeed <= 0.f)
		ValidationErrors.Add(FText::FromString("maxSpeed have to be positive"));

	if (maxSpeed < initialSpeed)
		ValidationErrors.Add(FText::FromString("maxSpeed can not be lower then initialSpeed"));

	if (baseCost < 0.f) {
		ValidationErrors.Add(FText::FromString("baseCost can not be negative"));
	}
	else {
		if (userType == EShooterType::EST_PlayerOnly && baseCost == 0.f) {
			ValidationErrors.Add(FText::FromString("baseCost should be positive for player to use this"));
		}
	}		

	if (userType == EShooterType::EST_MAX) {
		ValidationErrors.Add(FText::FromString("invalid userType"));
	}

	return ValidationErrors.Num() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif
