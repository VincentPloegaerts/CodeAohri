#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "STRUCT_AccessibilityConfig.h"
#include "STRUCT_BossProgress.h"
#include "STRUCT_PlayerConfig.h"
#include "STRUCT_CameraConfig.h"
#include "STRUCT_GraphicsConfig.h"
#include "STRUCT_SoundConfig.h"

#include "S_PlayerSave.generated.h"

UCLASS()
class INSIDEUS_API US_PlayerSave : public USaveGame
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerSaveEdited, const US_PlayerSave*, _playerSave);

	UPROPERTY()
	FOnPlayerSaveEdited onPlayerSaveEdited;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Save | Player")
	FPlayerConfig playerConfig = FPlayerConfig();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Save | Camera")
	FCameraConfig cameraConfig = FCameraConfig();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Save | Graphics")
	FGraphicsConfig graphicsConfig = FGraphicsConfig();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Save | Sound")
	FSoundConfig soundConfig = FSoundConfig(); 
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Save | Player Accessibility")
	FAccessibilityConfig accessibilityConfig = FAccessibilityConfig();

public:
	FORCEINLINE FOnPlayerSaveEdited& OnPlayerSaveEdited() { return onPlayerSaveEdited; }

	//Player 
	
	FORCEINLINE const FPlayerConfig& GetPlayerConfig() const { return playerConfig; }
	//Return a reference on PlayerConfig struct (allow to edit directly the struct)
	FORCEINLINE FPlayerConfig& EditPlayerConfig() { return playerConfig; }

	//Camera 
	
	FORCEINLINE const FCameraConfig& GetCameraConfig() const { return cameraConfig; }
	//Return a reference on CameraConfig struct (allow to edit directly the struct)
	FORCEINLINE FCameraConfig& EditCameraConfig() { return cameraConfig; }
	
	//Graphics
	
	FORCEINLINE const int32& GetDefaultScalability() const { return graphicsConfig.GetDefaultScalability(); }

	FORCEINLINE const FGraphicsConfig& GetGraphicConfig() const { return graphicsConfig; }
	//Return a reference on GraphicConfig struct (allow to edit directly the struct)
	FORCEINLINE FGraphicsConfig& EditGraphicConfig() { return graphicsConfig; }

	//Sound

	FORCEINLINE const FSoundConfig& GetSoundConfig() const { return soundConfig; }
	//Return a reference on SoundConfig struct (allow to edit directly the struct)
	FORCEINLINE FSoundConfig& EditSoundConfig() { return soundConfig; }
	
	//Accessibility

	FORCEINLINE const FAccessibilityConfig& GetAccessibilityConfig() const { return accessibilityConfig; }
	//Return a reference on AccessibilityConfig struct (allow to edit directly the struct)
	FORCEINLINE FAccessibilityConfig& EditAccessibilityConfig() { return accessibilityConfig; }
	
	//Can be call to Broadcast OnPlayerSaveEdited
	void PlayerSaveEdited() const;
};