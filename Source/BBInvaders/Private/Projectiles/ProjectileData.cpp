// by Dmitry Kolontay


#include "Projectiles/ProjectileData.h"
#include "Engine/Texture2D.h"
#include "CoreSystems/BBIAssetManager.h"
//#include "CoreSystems/Shooter.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

UProjectileData::UProjectileData()// :
	//*userType{ EShooterType::EST_MAX },*/ visibleName {}, initialSpeed{ 0.f }, 
	//maxSpeed{ 100.f }, bIsHoming{ false }, baseCost { 0.f }
{

}

FPrimaryAssetId UProjectileData::GetPrimaryAssetId() const
{
	return {assetType, GetFName()};
}

void UProjectileData::PreLoadAsync(bool bLoadMesh, FStreamableDelegate onIconLoaded)
{
	if (icon.IsPending()) {
		UBBIAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
			icon.ToSoftObjectPath(), MoveTemp(onIconLoaded), FStreamableManager::AsyncLoadHighPriority);
	}
	/*if (bLoadMesh && bodyMesh.IsPending()) {
		UBBIAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
			bodyMesh.ToSoftObjectPath(), FStreamableDelegate{}, FStreamableManager::AsyncLoadHighPriority);
	}*/
	
}

bool UProjectileData::IsToShooter(EShooterType type) const
{
	return static_cast<uint8>(type) & shooterTypes;
}

bool UProjectileData::IsAvailableToPlayer(const ABBIPlayerState* state) const
{
	return IsToShooter(EShooterType::EST_Player);
}

#if WITH_EDITOR

EDataValidationResult UProjectileData::IsDataValid(FDataValidationContext& context) const
{
	Super::IsDataValid(context);

	if (baseCost < 0.f) {
		context.AddError(FText::FromString("baseCost can not be negative"));
	}
	else {
		/*if (userType == EShooterType::EST_Player && baseCost == 0.f) {
			context.AddError(FText::FromString("baseCost should be positive for player to use this"));
		}*/
	}		

	/*if (userType == EShooterType::EST_MAX) {
		context.AddError(FText::FromString("invalid userType"));
	}*/

	return context.GetNumErrors() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif
