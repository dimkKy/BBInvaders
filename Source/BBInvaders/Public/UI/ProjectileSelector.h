// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectileSelector.generated.h"

class UListView;
class UProjectileDataAsset;

/**
 * 
 */
UCLASS()
class BBINVADERS_API UProjectileSelector : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	UProjectileDataAsset* GetSelectedProjectile() const;
	void SetAvailableProjectiles(const TArray<UProjectileDataAsset*>& projectiles);
	void SelectNext(bool bReverse = false);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UListView* listView;

	int32 selectedIndex;
};
