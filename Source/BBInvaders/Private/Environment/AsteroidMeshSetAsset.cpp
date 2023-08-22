// by Dmitry Kolontay


#include "Environment/AsteroidMeshSetAsset.h"

const FPrimaryAssetType UAsteroidMeshSetAsset::assetType{ "AsteroidMeshSet" };

FPrimaryAssetId UAsteroidMeshSetAsset::GetPrimaryAssetId() const
{
	return {assetType, GetFName()};
}

UStaticMesh* UAsteroidMeshSetAsset::GetStaticMesh(EAsteroidSize size) const
{
	if (size != EAsteroidSize::EAS_MAX) {
		return asteroidMeshes[static_cast<uint8>(size)].LoadSynchronous();
	}
	else {
		return nullptr;
	}
}

EDataValidationResult UAsteroidMeshSetAsset::IsDataValid(TArray<FText>& ValidationErrors)
{
	Super::IsDataValid(ValidationErrors);
	
	for (auto& mesh : asteroidMeshes) {
		if (mesh.IsNull()) {
			ValidationErrors.Add(FText::FromString("all meshes should be valid"));
			break;
		}
	}

	return ValidationErrors.Num() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
