// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GametimeUI.h"
#include "Components/WidgetSwitcher.h"
#include "UI/PauseMenu.h"
#include "Components/CanvasPanel.h"
#include "BBInvadersUtils.h"

void UGametimeUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	pauseButton->SetClickMethod(EButtonClickMethod::PreciseClick);
	pauseButton->OnClicked.AddDynamic(this, &UGametimeUI::OnPauseButtonClicked);

	pauseMenu->resumeButton->OnClicked.AddDynamic(this, &UGametimeUI::OnResumeButtonClicked);
	pauseMenu->toMainMenuButton->OnClicked.AddDynamic(this, &UGametimeUI::OnResumeButtonClicked);
	//BBInvadersUtils::SetupButtons(this, pauseButton, &UGametimeUI::OnPauseButtonClicked );

}

void UGametimeUI::OnPauseButtonClicked()
{
	//mainSwitcher->SetActiveWidgetIndex(1);
	mainSwitcher->SetActiveWidget(pauseMenu);
}

void UGametimeUI::OnResumeButtonClicked()
{
	mainSwitcher->SetActiveWidget(gametimeCanvas);
}
