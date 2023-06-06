#pragma once
#include "CoreMinimal.h"

#include "ENUM_DifficultyType.h"
#include "STRUCT_DifficultyData.h"

#include "M_DifficultyManager.generated.h"

USTRUCT(BlueprintType)
struct FDifficultyElement
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Diffculty")
	FString description = "NONE";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Diffculty")
	TArray<FString> extraDescription = { };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Diffculty")
	UTexture2D* difficultyTexture = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Diffculty")
	FDifficultyData difficultyData = FDifficultyData();


public:
	FString GetDifficultyDescription() const { return description; }
	TArray<FString> GetDifficultyExtraDescription() const { return extraDescription; }
	FDifficultyData GetDifficultyData() const { return difficultyData; }
	UTexture2D* GetDifficultyTexture() const { return difficultyTexture; }
};

UCLASS()
class INSIDEUS_API UM_DifficultyManager : public UObject
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDifficultyChanged, EDifficultyType, _difficulty, const FDifficultyData&, _difficultyData);
	
	UPROPERTY(EditAnywhere, Category = "Difficulty values", BlueprintReadWrite, meta = (AllowPrivateAccess = True))
		TMap<TEnumAsByte<EDifficultyType>, FDifficultyElement> difficulties = { };

	UPROPERTY(VisibleAnywhere, Category = "Difficulty values")
		TEnumAsByte<EDifficultyType> currentDifficulty = EDifficultyType::DIFFICULTY_NONE;
	
	UPROPERTY()
		FOnDifficultyChanged onDifficultyChanged = FOnDifficultyChanged();
	
public:
	FORCEINLINE FOnDifficultyChanged& OnDifficultyChanged() { return onDifficultyChanged; }
	
	// Setters
	void SetDifficulty(const EDifficultyType& _difficulty);

	// Getters
	FORCEINLINE const TMap<TEnumAsByte<EDifficultyType>, FDifficultyElement>& GetDifficulties() const { return difficulties; }
	
	FORCEINLINE EDifficultyType GetCurrentDifficulty() const { return currentDifficulty; }
	
	FORCEINLINE bool GetCurrentDifficultyData(FDifficultyData& _data) const			
	{
		if (GetDifficultyData(currentDifficulty, _data))
			return true;
		
		return GetDifficultyData(EDifficultyType::DIFFICULTY_BORN, _data);		//if Current Difficulty == None : Get a default difficulty data (Normal)
	}

	FORCEINLINE bool GetDifficultyData(const EDifficultyType& _difficulty, FDifficultyData& _data) const
	{
		if (!difficulties.Contains(_difficulty)) return false;
		_data = difficulties[_difficulty].GetDifficultyData();
		return true;
	}
};