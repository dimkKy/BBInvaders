// by Dmitry Kolontay


#include "CoreSystems/BBInvadersGameInstance.h"
#include "CoreSystems/AssetProvider.h"

EDataValidationResult UBBInvadersGameInstance::IsDataValid(TArray<FText>& ValidationErrors)
{
	Super::IsDataValid(ValidationErrors);
	//
	if (!assetProviderClass) {
		ValidationErrors.Add(FText::FromString("Invalid assetProviderClass"));
	}
	//
	if (ValidationErrors.Num() > 0) {
		return EDataValidationResult::Invalid;
	}
	else {
		return EDataValidationResult::Valid;
	}
}
