// by Dmitry Kolontay


#include "UI/ProjectileSelector.h"
#include "UI/ProjectileSelectorEntry.h"
#include "CoreSystems/BBInvadersGameStateBase.h"
#include "Components/ListView.h"
#include "CoreSystems/ProjectileDataAsset.h"

void UProjectileSelector::NativeOnInitialized()
{
    listView->SetSelectionMode(ESelectionMode::Single);
}

UProjectileDataAsset* UProjectileSelector::GetSelectedProjectile() const
{
    //return static_cast<decltype(GetSelectedProjectile())>(listView->GetSelectedItem());
    return static_cast<UProjectileDataAsset*>(listView->GetSelectedItem());
}

void UProjectileSelector::SetAvailableProjectiles(const TArray<UProjectileDataAsset*>& projectiles)
{
    if (ensureAlways(projectiles.Num())) {
        listView->SetListItems(projectiles);
        selectedIndex = 0;
        listView->SetSelectedIndex(selectedIndex);
    }
    else {
        //try reload
    }
    
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

void UProjectileSelector::UpdatePrices()
{
    float currentInflation{ CastChecked<ABBIGameStateBase>(GetWorld()->GetGameState())->GetCurrentInflation() };

    for (auto&& object : listView->GetDisplayedEntryWidgets()) {
        check(IsValid(object));
        ExactCast<UProjectileSelectorEntry>(object)->UpdateCost(currentInflation);
    }
}
