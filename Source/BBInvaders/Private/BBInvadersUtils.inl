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
TActor* BBInvadersUtils::GetFirstActor(UWorld* world)
{
	check(world);
	auto it{ TActorIterator<TActor>(world) };
	return it ? *it : nullptr;
}

template<bool generateOverlapEvents, class ...TChannels>
void BBInvadersUtils::ConfigureOverlapCollision(UPrimitiveComponent* comp, ECollisionChannel compType, 
	TChannels ...overlapChannels) requires BBInvadersUtils::NonEmpty<TChannels...> 
{
	comp->SetCanEverAffectNavigation(false);
	comp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	comp->SetCollisionObjectType(compType);
	//comp->SetCollisionResponseToAllChannels(ECR_Ignore);
	comp->SetGenerateOverlapEvents(generateOverlapEvents);

	auto setter = [=](ECollisionChannel channel) constexpr {
		comp->SetCollisionResponseToChannel(channel, ECR_Overlap);
	};
	auto getter = [=](auto...channels) constexpr {
		(setter(channels), ...);
	};
	getter(overlapChannels...);
}

template<class ...TChannels>
void BBInvadersUtils::ConfigureBlockCollision(UPrimitiveComponent* comp, ECollisionChannel compType,
	TChannels ...overlapChannels) requires BBInvadersUtils::NonEmpty<TChannels...>
{
	comp->SetCanEverAffectNavigation(false);
	comp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	comp->SetCollisionObjectType(compType);
	//comp->SetCollisionResponseToAllChannels(ECR_Ignore);

	auto setter = [=](ECollisionChannel channel) constexpr {
		comp->SetCollisionResponseToChannel(channel, ECR_Block);
		};
	auto getter = [=](auto...channels) constexpr {
		(setter(channels), ...);
		};
	getter(overlapChannels...);
}

template<typename TBase, BBInvadersUtils::ChildOf<TBase> TChild, bool bDeleteNulls>
TArray<TChild*> BBInvadersUtils::DowncastArray(const TArray<TBase*>& array)
{
	if constexpr (bDeleteNulls) {
		return {};
	}
	else {
		return {};
	}
}

template<BBInvadersUtils::ChildOf<AActor> TActor>
void BBInvadersUtils::ForActorsOfClass(UWorld* world, std::invocable<AActor*> auto&& func)
{
	check(world);
	for (TActorIterator<TActor> it(world); it; ++it) {
		func(*it);
	}
}

template<BBInvadersUtils::ChildOf<AActor> TActor, BBInvadersUtils::ChildOf<AActor>...TOthers>
void BBInvadersUtils::ForActorsOfClass(UWorld* world, std::invocable<AActor*> auto&& func)
	requires BBInvadersUtils::NonEmpty<TOthers...>
{
	ForActorsOfClass<TActor>(world, func);
	ForActorsOfClass<TOthers...>(world, func);
}
