// by Dmitry Kolontay


#include "CoreSystems/AssetProvider.h"

bool UAssetProvider::ShouldCreateSubsystem(UObject* Outer) const
{
    return this->GetClass()->IsInBlueprint();
}

UStaticMesh* UAssetProvider::GetAsteroidMesh(EAsteroidSize size) const
{
    check(asteroidMeshes[static_cast<int32>(size)]);
    return asteroidMeshes[static_cast<int32>(size)];
}

UStaticMesh* UAssetProvider::GetInvaderMesh() const
{
    if (invaderMesh) {
        return invaderMesh.Get();
    }
    else {
        return invaderMesh.LoadSynchronous();
    }
    
}

EDataValidationResult UAssetProvider::IsDataValid(TArray<FText>& ValidationErrors)
{
    return EDataValidationResult::NotValidated;
}
