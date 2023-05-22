// by Dmitry Kolontay


#include "UI/AdvancedButton.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/ButtonSlot.h"

//#include "BaseWidgetBlueprint.h"

UAdvancedButton::UAdvancedButton() :
	text{ /*CreateDefaultSubobject<UTextBlock>("textBlock")*/ }
{
	//AddChild(text);
	//CreateDefaultSubobject<UButtonSlot>("buttonSlot")
	//SetContent(text);
}

void UAdvancedButton::OnWidgetRebuilt()
{
	//UWidgetBlueprint
	/*if (auto* userWidget{ Cast<UBaseWidgetBlueprint>(WidgetGeneratedBy.Get()) }) {
		//auto* textWidget{ userWidget->WidgetTree->ConstructWidget<UTextBlock>() };
		text = userWidget->WidgetTree->ConstructWidget<UTextBlock>();

		EObjectFlags NewObjectFlags = RF_Transactional;
		if (HasAnyFlags(RF_Transient))
		{
			NewObjectFlags |= RF_Transient;
		}
		UPanelSlot* PanelSlot = NewObject<UPanelSlot>(this, GetSlotClass(), NAME_None, NewObjectFlags);

		PanelSlot->Content = text;
		PanelSlot->Parent = this;
		text->Slot = PanelSlot;
		Slots.Add(PanelSlot);
		OnSlotAdded(PanelSlot);
		InvalidateLayoutAndVolatility();
	}*/
	Super::OnWidgetRebuilt();
	//AddChild(text);
}
