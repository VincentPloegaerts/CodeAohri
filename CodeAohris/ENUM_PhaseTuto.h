#pragma once

UENUM()
enum EPhaseTuto
{
	// NotStarted,
	// CinematicEnter,
	// WaitDelayFirstShoot,
	// BossShootingDodge,
	// DashDodge,
	// PlayerFirstShoot,
	// ShowingSlowMow,
	// BossShootingSlowMow,
	// SlowMoDodge,
	// PlayerShootLoadHarpoon,
	// HarpoonOnGround,
	// ShootUntilHarpoonStun,
	// PlayerShootLoadSelfGuided,
	// SelfGuidedOnGround,
	// PlayerShootSelfGuided,
	// Ended
	
	NotStarted,
	CinematicEnter,
	PlayerThreeShoot,
	HarpoonOnGround,
	ShootUntilHarpoonStun,
	DashCancelShoot,
	SelfGuidedOnGround,
	PlayerShootSelfGuided,
	DashThroughProjectileWall,
	SlowMoDodgePattern,
	BossFight,
	Ended
	
};