// by Dmitry Kolontay


#include "Projectiles/RocketProjectileData.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

void URocketProjectileData::OnShooting(const FVector& from, const FVector& to, AActor* owner, AActor* target) const
{
	if (bIsHoming) {

	}
	else {
		//as kinetic
	}
}

EDataValidationResult URocketProjectileData::IsDataValid(FDataValidationContext& context) const
{
	Super::IsDataValid(context);

	if (maxSpeed <= 0.f)
		context.AddError(FText::FromString("maxSpeed have to be positive"));

	if (maxSpeed < initialSpeed)
		context.AddError(FText::FromString("maxSpeed can not be lower then initialSpeed"));

	/*if (userType == EShooterType::EST_MAX) {
		context.AddError(FText::FromString("invalid userType"));
	}*/

	return context.GetNumErrors() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
