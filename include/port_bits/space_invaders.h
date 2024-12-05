#pragma once

namespace PortBits
{
	enum SpaceInvaders // Space Invaders Binding
	{
		SP_Credit = 1,
		SP_Start1P = 1 << 1,
		SP_Start2P = 1 << 2,
		SP_Shot1P = 1 << 4,
		SP_Left1P = 1 << 5,
		SP_Right1P = 1 << 6,

		SP_Tilt = 1 << 2,
		SP_Shot2P = 1 << 4,
		SP_Left2P = 1 << 5,
		SP_Right2P = 1 << 6,

		SP_Ufo = 1,
		SP_Shot = 1 << 1,
		SP_PlayerDie = 1 << 2,
		SP_InvaderDie = 1 << 3,
		SP_ExtraLife = 1 << 4,

		SP_FleetMove1 = 1,
		SP_FleetMove2 = 1 << 1,
		SP_FleetMove3 = 1 << 2,
		SP_FleetMove4 = 1 << 3,
		SP_UfoHit = 1 << 4
	};
}
