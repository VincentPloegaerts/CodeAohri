#pragma once
#include "ENUM_BossMovementMode.generated.h"

UENUM()
enum EBossMovementMode
{
	/* Move on the ground on the selected ArenaPoint.
	 * If he is already on this point, he will just wait some second.
	 * If ArenaPoint is random and he is already on this point, will swap to another point.
	 */
	MOVE_ON_GROUND,

	/* Same behavior than move on ground.
	 * But will do an animation of going in ground, and going out.
	 */
	MOVE_IN_GROUND,

	/* Move behind the player but check only one time the player location.
	 * If he can't, will go in front, right or left of him.
	 */
	MOVE_IN_GROUND_BEHIND_PLAYER,

	/* Move toward the player and check continuously the player location.
	 * When close to it, he will start to search if he can go behind him.
	 * If the location where he want to go is ArenaLayer, he will continue his road.
	 * If the location isn't good, he will swap to go in front, right or left of the player.
	 */
	MOVE_IN_GROUND_BEHIND_PLAYER_DYNAMIC,

	/* Dash in the direction of the player.
	 * But check his location only one time.
	 */
	DASH_IN_PLAYER_DIRECTION,

	/* Dash on the nearest ArenaPoint of the boss.
	 * If already on this point, will swap to another.
	 */
	DASH_NEAREST_FROM_BOSS,

	/* Dash on the farest ArenaPoint of the player.
	 * If already on this point, will swap to another.
	 */
	DASH_FAREST_FROM_PLAYER,

	/* Same Behavior than Move on ground.
	 * But will do dashing animation.
	 */
	DASH_ON_GROUND
};