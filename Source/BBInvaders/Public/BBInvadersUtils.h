// by Dmitry Kolontay

#pragma once

#include <type_traits>
//#include <concepts>

class UCameraComponent;
class UButton;
class UObject;
class AActor;

//TIsDerivedFrom

namespace BBInvadersUtils {
	//if constexpr?
	template <class S, class O>
	inline bool IsIn(const S& subject, const O* other);

	template <class S, class O, class...Os>
	inline bool IsIn(const S& subject, const O* other, const Os*... others);
	
	template <class TDerived, class TBase, bool allowSame = false>
	concept ChildOf = std::is_base_of<TBase, TDerived>::value &&
		(allowSame || std::negation<std::is_same<TBase, TDerived>>::value);

	template <typename TDerived, template<typename> typename TBase>
	struct is_derived_from_any
	{
		template<typename TParam>
		static constexpr std::true_type is_derived(const volatile TBase<TParam>&);
		static constexpr std::false_type is_derived(...);
		using type = decltype(is_derived(std::declval<TDerived&>()));
	};

	template <class TDerived, template<typename> typename TBase>
	concept ChildOfAny = is_derived_from_any<TDerived, TBase>::type::value;

	constexpr float defaultNearClipPlane = 10.f;
	float GetCameraNearPlane(const UCameraComponent& camera);


	/*#define BindWrapper( object, button, onClickedFunc ) { \
		button->SetClickMethod(EButtonClickMethod::PreciseClick); \
		button->OnClicked.AddDynamic(object, onClickedFunc); \
	}*/
	//template <ChildOf<UObject> TObject>
	//void SetupButtons(TObject* object, UButton* button, void (TObject::* onClickedFunc)(void));

	//void SetupButtons(ChildOf<UObject> auto* object, UButton* button, TFunction<void(void)> onClickedFunc);

	template <ChildOf<AActor> TActor>
	auto* GetFirstActor(UWorld* world);

	/*template <ChildOf<AActor> TActor>
	void ForActorsOfClass(UWorld* world, std::invocable<AActor*> auto&& func);
	
	//concept non empty
	template <ChildOf<AActor> TActor, ChildOf<AActor>...TOthers>
		requires (sizeof...(TOthers) > 0)
	void ForActorsOfClass(UWorld* world, std::invocable<AActor*> auto&& func);*/

	constexpr ECollisionChannel ECC_Projectile = ECollisionChannel::ECC_GameTraceChannel1;
	constexpr ECollisionChannel ECC_Asteroid = ECollisionChannel::ECC_GameTraceChannel2;
	constexpr ECollisionChannel ECC_Invader = ECollisionChannel::ECC_GameTraceChannel3;

	const FRotator UpRotator = { -90.f, 0.f, 0.f };
	const FRotator BehindRotator = { 0.f, 180.f, 0.f };
	const FRotator unitRotator = { 0.f, 1.f, 0.f };

	const FName muzzleSocket = "Muzzle";
}

/*namespace WHTS87Utils::NamingRules {


	const FName toNextSWSocket{ "NextFloorSocket" };
	const FString doorSocket{ "DoorSocket" };
	const FString doorFrameSocket{ "DoorFrameSocket" };
	const FString specialRoomSocket{ "SpecialSocket" };
	const FString subroomSocket{ "SubroomSocket" };
	const FString volumePoint{ "volumePoint" };

}*/

#include "BBInvadersUtils.inl"

