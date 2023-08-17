// by Dmitry Kolontay


#include "UI/ProjectileSelector.h"
#include "Components/ListView.h"
#include "CoreSystems/ProjectileDataAsset.h"

void UProjectileSelector::NativeOnInitialized()
{
    listView->SetSelectionMode(ESelectionMode::Single);
    selectedIndex = 0;
}

UProjectileDataAsset* UProjectileSelector::GetSelectedProjectile() const
{
    return ExactCast<UProjectileDataAsset>(listView->GetSelectedItem());
}

void UProjectileSelector::SetAvailableProjectiles(const TArray<UProjectileDataAsset*>& projectiles)
{
    listView->SetListItems(projectiles);
    selectedIndex = 0;
}

void UProjectileSelector::SelectNext(bool bReverse)
{
    if (bReverse) {
        selectedIndex--;
        if (selectedIndex < 0) {
            selectedIndex = 0;
        }
    }
    else {
        selectedIndex++;
        int32 numLimit{ listView->GetNumItems() - 1 };
        if (selectedIndex > numLimit) {
            selectedIndex = numLimit;
        }
    }
    listView->SetSelectedIndex(selectedIndex);
}
