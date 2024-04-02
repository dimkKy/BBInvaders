// by Dmitry Kolontay


#include "Environment/AsteroidMeshSet.h"
#include "CoreSystems/BBIAssetManager.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

const FPrimaryAssetType UAsteroidMeshSet::assetType{ "AsteroidMeshSet" };

FPrimaryAssetId UAsteroidMeshSet::GetPrimaryAssetId() const
{
	return {assetType, GetFName()};
}

UStaticMesh* UAsteroidMeshSet::GetStaticMesh(EAsteroidSize size) const
{
	switch (size) {
	default:
		//auto&& assetManager{ UBBIAssetManager::Get() };
		//assetManager.LoadAssetList(asteroidMeshes)
		[[fallthrough]];
	case EAsteroidSize::EAS_Small:
		return asteroidMeshes[static_cast<uint8>(EAsteroidSize::EAS_Small)].LoadSynchronous();
	case EAsteroidSize::EAS_MAX:
		check(false);
		return nullptr;
	}
}

EDataValidationResult UAsteroidMeshSet::IsDataValid(FDataValidationContext& context) const
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
