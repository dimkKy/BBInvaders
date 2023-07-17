// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FactoryInfoBase.generated.h"

UENUM(BlueprintType)
enum class EIndustryType : uint8
{
	EIT_Arms,
	EIT_Medical, 
	EIT_Food,
	EIT_Light,
	EIT_Heavy,
	EIT_Semiconductor,
	EIT_Power,

	EIT_MAX
};

/**
 * 
 */
UCLASS()
class BBINVADERS_API UFactoryInfoBase : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UFactoryInfoBase();

	FText GetDisplayName() const;
	FText GetDescription() const;

	virtual int32 GetConstructionCost(int32 tier = 0) const;
	virtual float GetConstructionTime(int32 tier = 0) const;
	virtual int32 GetIncome(int32 tier = 0) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EIndustryType type;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText displayName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine = true))
		FText description;
};
