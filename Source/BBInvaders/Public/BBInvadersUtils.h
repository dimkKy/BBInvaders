// by Dmitry Kolontay

#pragma once

#include <type_traits>
#include <concepts>

class UCameraComponent;
class UButton;
class UObject;
class AActor;

//TIsDerivedFrom

namespace BBInvadersUtils {
	//if constexpr? !POINTERS
	template <class S, class O>
	inline bool IsIn(const S& subject, const O* other);

	template <class S, class O, class...Os>
	inline bool IsIn(const S& subject, const O* other, const Os*... others);
	
	template <class TDerived, class TBase, bool allowSame = false>
	concept ChildOf = std::is_base_of_v<TBase, TDerived> &&
		(allowSame || !std::is_same_v<TBase, TDerived>);

	template <typename TDerived, template<typename> typename TBase>
	struct is_derived_from_any
	{
		template<typename TParam>
		static constexpr std::true_type is_derived(const TBase<TParam>&);
		static constexpr std::false_type is_derived(...);
		using type = decltype(is_derived(std::declval<TDerived&>()));
	};

	template <class TDerived, template<typename> typename TBase>
	using is_derived_from_any_t = is_derived_from_any<TDerived, TBase>::type;

	template <class TDerived, template<typename> typename TBase>
	inline constexpr bool is_derived_from_any_v{ is_derived_from_any_t<TDerived, TBase>::value };

	template <class TDerived, template<typename> typename TBase>
	concept ChildOfAny = is_derived_from_any_v<TDerived, TBase>;

	constexpr float defaultNearClipPlane{ 10.f };
	float GetCameraNearPlane(const UCameraComponent& camera);

	template <typename... Args>
	concept NonEmpty = sizeof...(Args) > 0;

	template<class T, class... Classes>
	concept AllSame = (std::is_same_v<T, Classes> && ...);

	template <ChildOf<AActor> TActor>
	TActor* GetFirstActor(UWorld* world);

	template <ChildOf<AActor> TActor>
	void ForActorsOfClass(UWorld* world, std::invocable<AActor*> auto&& func);
	
	template <ChildOf<AActor> TActor, ChildOf<AActor>...TOthers>
	void ForActorsOfClass(UWorld* world, std::invocable<AActor*> auto&& func) 
		requires NonEmpty<TOthers...>;

	template<bool generateOverlapEvents, class...TChannels>
	void ConfigureDefaultCollision(UPrimitiveComponent* comp, ECollisionChannel compType, 
		TChannels...overlapChannels) requires NonEmpty<TChannels...>;

	template <typename TBase, ChildOf<TBase> TChild, bool bDeleteNulls>
	TArray<TChild*> DowncastArray(const TArray<TBase*>& array);

	constexpr ECollisionChannel ECC_Projectile{ ECollisionChannel::ECC_GameTraceChannel1 };
	constexpr ECollisionChannel ECC_Asteroid{ ECollisionChannel::ECC_GameTraceChannel2 };
	constexpr ECollisionChannel ECC_Invader{ ECollisionChannel::ECC_GameTraceChannel3 };

	float RandomAngle();
	double RandomAngle_Double();
	FRotator RandomRotator();
	//Util to generate a random number in a range between -max and max
	float RandAbsRange(float absMax);
	double RandAbsRange(double absMax);

	const FRotator UpRotation{ -90., 0., 0. };
	const FRotator BehindRotation{ 0., 180., 0. };
	const FRotator unitRotation{ 0., 1., 0. };

	const FName muzzleSocket{ "Muzzle" };
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

