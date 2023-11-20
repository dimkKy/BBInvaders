// by Dmitry Kolontay


#include "Environment/AsteroidMeshSetAsset.h"
#include "CoreSystems/BBInvadersAssetManager.h"
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
	switch (size) {
	default:
		//auto&& assetManager{ UBBInvadersAssetManager::Get() };
		//assetManager.LoadAssetList(asteroidMeshes)
		[[fallthrough]];
	case EAsteroidSize::EAS_Small:
		return asteroidMeshes[static_cast<uint8>(EAsteroidSize::EAS_Small)].LoadSynchronous();
	case EAsteroidSize::EAS_MAX:
		check(false);
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
