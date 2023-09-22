// by Dmitry Kolontay


#include "CoreSystems/BBInvadersGameInstance.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#if WITH_EDITOR
EDataValidationResult UBBInvadersGameInstance::IsDataValid(FDataValidationContext& context) const
{
	Super::IsDataValid(context);
	
	
	if (context.GetNumErrors() > 0) {
		return EDataValidationResult::Invalid;
	}
	else {
		return EDataValidationResult::Valid;
	}
}
#endif

