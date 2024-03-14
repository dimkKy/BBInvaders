// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Environment/Invader.h"
#include "InvaderVisualsAsset.generated.h"

//class AInvader

/**
 * 
 */
UCLASS()
class BBINVADERS_API UInvaderVisualsAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	static inline const FPrimaryAssetType assetType{ "InvaderVisuals" };

	UStaticMesh* GetStaticMesh() const;

	EInvaderType GetInvaderType() const;
	int32 GetInvaderTypeI() const;

protected:
	UPROPERTY(EditInstanceOnly)
		EInvaderType type;

	UPROPERTY(EditInstanceOnly)
		TSoftObjectPtr<UStaticMesh> bodyMesh;
};
