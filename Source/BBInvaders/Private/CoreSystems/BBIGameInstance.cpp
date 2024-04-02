// by Dmitry Kolontay


#include "CoreSystems/BBIGameInstance.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#if WITH_EDITOR
EDataValidationResult UBBIGameInstance::IsDataValid(FDataValidationContext& context) const
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

