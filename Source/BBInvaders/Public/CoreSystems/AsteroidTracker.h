// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AsteroidTracker.generated.h"

class UBoxComponent;

UCLASS()
class BBINVADERS_API AAsteroidTracker : public AActor
{
	GENERATED_BODY()
	
public:	
	AAsteroidTracker();
	virtual void Tick(float DeltaTime) override;
	void SetTrackArea(const FTransform& transform, const FVector& halfSize);
	void SetTrackArea(const FVector& halfSize);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* comp, AActor* other, UPrimitiveComponent* otherComp, int32 otherBodyIndex);
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* trackBox;
};
