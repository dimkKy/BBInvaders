// by Dmitry Kolontay


#include "CoreSystems/BBInvadersGameInstance.h"

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
