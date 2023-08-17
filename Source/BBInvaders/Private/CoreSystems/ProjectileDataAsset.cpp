// by Dmitry Kolontay


#include "CoreSystems/ProjectileDataAsset.h"
//#include "CoreSystems/Shooter.h"

const FPrimaryAssetType UProjectileDataAsset::assetType{ "ProjectileDataAsset" };

UProjectileDataAsset::UProjectileDataAsset() :
	visibleName{}, initialSpeed{ 0.f }, maxSpeed{ 100.f },
	baseCost{ 0.f }
{

}

FPrimaryAssetId UProjectileDataAsset::GetPrimaryAssetId() const
{
	return {assetType, GetFName()};
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
		/*if (userType == EShooterType::EST_PlayerOnly && baseCost == 0.f) {
			ValidationErrors.Add(FText::FromString("baseCost should be positive for player to use this"));
		}*/
	}		

	//if (bEverAvailableToPlayer && baseCost <= 0.f)
		//ValidationErrors.Add(FText::FromString("available to player can not be free"));

	return ValidationErrors.Num() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif
