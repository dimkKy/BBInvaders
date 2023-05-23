// by Dmitry Kolontay

#pragma once

#include "BBInvadersUtils.h"
#include "UObject/Object.h"
#include "Components/Button.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"

template<class S, class O>
bool BBInvadersUtils::IsIn(const S& subject, const O* other)
{
	return &subject == other;
}

template<class S, class O, class ...Os>
bool BBInvadersUtils::IsIn(const S& subject, const O* other, const Os* ...others)
{
	return &subject == other ? true : IsIn(subject, others...);
}

/*template<BBInvadersUtils::ChildOf<UObject> TObject>
void BBInvadersUtils::SetupButtons(TObject* object, UButton* button, void(TObject::* onClickedFunc)(void))
{
	check(button);
	button->SetClickMethod(EButtonClickMethod::PreciseClick);
	button->OnClicked.AddDynamic(object, onClickedFunc);
}*/

template<BBInvadersUtils::ChildOf<AActor> TActor>
auto* BBInvadersUtils::GetFirstActor(UWorld* world)
{
	check(world);
	return *TActorIterator<TActor>(world);
}

/*template<BBInvadersUtils::ChildOf<AActor> TActor>
void BBInvadersUtils::ForActorsOfClass(UWorld* world, std::invocable<AActor*> auto&& func)
{
	//UGameplayStatics::GetAllActorsOfClass
	check(world);
	for (TActorIterator<TActor> it(world); it; ++it) {
		func(*it);
	}
}

template<BBInvadersUtils::ChildOf<AActor> TActor, BBInvadersUtils::ChildOf<AActor>...TOthers>
	requires (sizeof...(TOthers) > 0)
void BBInvadersUtils::ForActorsOfClass(UWorld* world, std::invocable<AActor*> auto&& func)
{
	ForActorsOfClass<TActor>(world, func);
	ForActorsOfClass<TOthers...>(world, func);
}*/
