// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KineticMoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BBINVADERS_API UKineticMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UKineticMoveComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void RegisterComponentTickFunctions(bool bRegister) override;
	void SetParams(float startSpeed, float acceler);
	
protected:
	virtual void BeginPlay() override;
	
	float currentSpeed{ 0.f };
	float acceleration{ 0.f };
	UPROPERTY(BlueprintReadOnly/*, Transient, DuplicateTransient*/)
		TObjectPtr<USceneComponent> updatedComponent;
};
