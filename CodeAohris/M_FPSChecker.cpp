#include "M_FPSChecker.h"

AM_FPSChecker::AM_FPSChecker()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AM_FPSChecker::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle _timer;
	GetWorld()->GetTimerManager().SetTimer(_timer, this, &AM_FPSChecker::ActivateTimer, fStartDelay);
}

void AM_FPSChecker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bActivated)return;

	++iFrameCount;
	fTotalTime += DeltaTime;
	fCurrentTimeInSecond += DeltaTime;
	
	fAverageFrameInSecond = (float)iFrameCount / fTotalTime;
	
	if (fMaximalFrameInSecond < fAverageFrameInSecond)
		fMaximalFrameInSecond = fAverageFrameInSecond;
		
	if (fMinimalFrameInSecond > fAverageFrameInSecond)
		fMinimalFrameInSecond = fAverageFrameInSecond;
	
	if (fCurrentTimeInSecond > 1.0f)
	{
		fCurrentTimeInSecond = 0.0f;

		fTotalAverageFrame += fAverageFrameInSecond;
		++iSecond;
		
		fTotalFPS = fTotalAverageFrame / (float)iSecond;
	}

	if (bActivateGEngineLOG)
	{
		ON_SCREEN_LOG("Total Frame : " + FString::FromInt(iFrameCount));
		ON_SCREEN_LOG("Total Time : " + FString::SanitizeFloat(fTotalTime) + "s");
		ON_SCREEN_LOG("Current FPS : " + FString::SanitizeFloat(fAverageFrameInSecond));

		ON_SCREEN_LOG(" ");
		
		ON_SCREEN_LOG("Maximal FPS : " + FString::SanitizeFloat(fMaximalFrameInSecond));
		ON_SCREEN_LOG("Average FPS : " + FString::SanitizeFloat(fTotalFPS));
		ON_SCREEN_LOG("Minimal FPS : " + FString::SanitizeFloat(fMinimalFrameInSecond));
	}
}

void AM_FPSChecker::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	LOG("----------------------------------FPS Checker-----------------------------------------");
	LOG("Total Frame : %s", *FString::FromInt(iFrameCount));
	LOG("Total Time : %ss", *FString::SanitizeFloat(fTotalTime));
	LOG("Current FPS : %ss", *FString::SanitizeFloat(fAverageFrameInSecond));
	LOG(" ");
	LOG("Maximal FPS : %ss", *FString::SanitizeFloat(fMaximalFrameInSecond));
	LOG("Average FPS : %ss", *FString::SanitizeFloat(fTotalFPS));
	LOG("Minimal FPS : %ss", *FString::SanitizeFloat(fMinimalFrameInSecond));
	LOG("--------------------------------------------------------------------------------------");
}

void AM_FPSChecker::ActivateTimer()
{
	bActivated = true;
}