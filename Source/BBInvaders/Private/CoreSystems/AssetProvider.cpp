// by Dmitry Kolontay


#include "CoreSystems/AssetProvider.h"

bool UAssetProvider::ShouldCreateSubsystem(UObject* Outer) const
{
    return this->GetClass()->IsInBlueprint();
}

EDataValidationResult UAssetProvider::IsDataValid(TArray<FText>& ValidationErrors)
{
    return EDataValidationResult::NotValidated;
}
