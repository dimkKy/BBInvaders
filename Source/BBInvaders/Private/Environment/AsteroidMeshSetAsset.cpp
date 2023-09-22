// by Dmitry Kolontay


#include "Environment/AsteroidMeshSetAsset.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

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

EDataValidationResult UAsteroidMeshSetAsset::IsDataValid(FDataValidationContext& context) const
{
	Super::IsDataValid(context);
	
	for (auto& mesh : asteroidMeshes) {
		if (mesh.IsNull()) {
			context.AddError(FText::FromString("all meshes should be valid"));
			break;
		}
	}

	return context.GetNumErrors() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
