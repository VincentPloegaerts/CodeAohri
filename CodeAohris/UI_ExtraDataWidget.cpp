#include "UI_ExtraDataWidget.h"

#include "Steam/isteamfriends.h"
#include "Steam/isteamutils.h"

void UUI_ExtraDataWidget::InitializeExtraDataWidget(const LeaderboardEntry_t& _entry)
{
	userRankText->SetText(FText::FromString(FString("#") + FString::FromInt(_entry.m_nGlobalRank)));
	userImage->SetBrushFromTexture(GetSteamImageAsTexture(SteamFriends()->GetMediumFriendAvatar(_entry.m_steamIDUser)));
	userNameText->SetText(FText::FromString(SteamFriends()->GetFriendPersonaName(_entry.m_steamIDUser)));
	timeText->SetText(FText::FromString(FTimespan::FromSeconds(_entry.m_nScore).ToString()));
}

UTexture2D* UUI_ExtraDataWidget::GetSteamImageAsTexture(int _imageHandle)
{
	uint32 _avatarWith, _avatarHeight;
	bool _success = SteamUtils()->GetImageSize(_imageHandle, &_avatarWith, &_avatarHeight);

	if (!_success) return nullptr;

	uint8 *_avatarRGBA = new uint8[_avatarWith * _avatarHeight * 4];
	_success = SteamUtils()->GetImageRGBA(_imageHandle, _avatarRGBA, _avatarWith * _avatarHeight * 4 * sizeof(char));

	if (!_success) return nullptr;
		
	for (uint32 i = 0; i < _avatarWith * _avatarHeight * 4; i += 4)
	{
		const uint8 Temp = _avatarRGBA[i + 0];
		_avatarRGBA[i + 0] = _avatarRGBA[i + 2];
		_avatarRGBA[i + 2] = Temp;
	}

	UTexture2D* _avatar = UTexture2D::CreateTransient(_avatarWith, _avatarHeight, PF_B8G8R8A8);
	uint8* MipData = static_cast<uint8*>(_avatar->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
	FMemory::Memcpy(MipData, _avatarRGBA, _avatarHeight * _avatarWith * 4);
	_avatar->GetPlatformData()->Mips[0].BulkData.Unlock();
	_avatar->GetPlatformData()->SetNumSlices(1);
	_avatar->NeverStream = true;
	_avatar->UpdateResource();
	return _avatar;
}
