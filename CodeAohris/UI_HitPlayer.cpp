#include "UI_HitPlayer.h"

#include "Components/CanvasPanelSlot.h"

void UUI_HitPlayer::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UpdateAllSize(0.0f);
}

void UUI_HitPlayer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	Decay();
	Decaying();
}

void UUI_HitPlayer::UpdateAllSize(const float _size)
{
	UpdateImageSize(top, FVector2D(0.0f, _size));
	UpdateImageSize(bottom, FVector2D(0.0f, _size));
	UpdateImageSize(left, FVector2D(_size, 0.0f));
	UpdateImageSize(right, FVector2D(_size, 0.0f));
}

void UUI_HitPlayer::UpdateImageSize(const UImage* _image, FVector2D _size)
{
	UCanvasPanelSlot* _panel = Cast<UCanvasPanelSlot>(_image->Slot);
	_panel->SetSize(_size);
}

void UUI_HitPlayer::Decay()
{
	if(!bCanDecay)
		return;

	fInternalDecayTime += GetWorld()->DeltaTimeSeconds;

	if(fInternalDecayTime >= fDecayTime)
	{
		iLastNumberOfSection = iCurrentNumberOfSection;
		iCurrentNumberOfSection = 0;
		bIsDecaying = true;
		bCanDecay = false;
	}
}

void UUI_HitPlayer::Decaying()
{
	if(!bIsDecaying)
		return;

	fInternalDecayingTime += GetWorld()->DeltaTimeSeconds;
	UpdateAllSize(GetLastWidth() * (1.0f - fInternalDecayingTime / fDecayingTime));

	if(fInternalDecayingTime >= fDecayingTime)
		bIsDecaying = false;
}

void UUI_HitPlayer::Hit(float _null)
{	
	const int _current = iCurrentNumberOfSection + 1;
	iCurrentNumberOfSection = _current > iTotalMaxNumberOfSection ? iTotalMaxNumberOfSection : _current;
	fInternalDecayTime = 0.0f;
	bCanDecay = true;
	
	UpdateAllSize(GetCurrentWidth());
}

void UUI_HitPlayer::Reset()
{
	iCurrentNumberOfSection = 0;
	fInternalDecayTime = 0.0f;
	bCanDecay = false;
	
	UpdateAllSize(0.0f);
}
