// by Dmitry Kolontay


#include "CoreSystems/BBInvadersGameInstance.h"

#if WITH_EDITOR
EDataValidationResult UBBInvadersGameInstance::IsDataValid(TArray<FText>& ValidationErrors)
{
	Super::IsDataValid(ValidationErrors);
	
	
	if (ValidationErrors.Num() > 0) {
		return EDataValidationResult::Invalid;
	}
	else {
		return EDataValidationResult::Valid;
	}
}
#endif

