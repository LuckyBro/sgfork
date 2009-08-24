/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.
Copyright (C) 2000-2003 Iron Claw Interactive
Copyright (C) 2005-2009 Smokin' Guns

This file is part of Smokin' Guns.

Smokin' Guns is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Smokin' Guns is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Smokin' Guns; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
//
// bg_misc.c -- both games misc functions, all completely stateless

#include "../qcommon/q_shared.h"
#include "bg_public.h"

vec3_t playerMins = {PLAYER_MIN_PITCH, PLAYER_MIN_YAW, MINS_Z};
vec3_t playerMaxs = {PLAYER_MAX_PITCH, PLAYER_MAX_YAW, MAXS_Z};

vec3_t playerMins_hit = {PLAYER_MIN_HIT_PITCH, PLAYER_MIN_HIT_YAW, MINS_Z};
vec3_t playerMaxs_hit = {PLAYER_MAX_HIT_PITCH, PLAYER_MAX_HIT_YAW, MAXS_Z_HIT};

vec3_t gatling_mins = {GATLING_MINS_PITCH, GATLING_MINS_YAW, GATLING_MINS_ROLL};
vec3_t gatling_maxs = {GATLING_MAXS_PITCH, GATLING_MAXS_YAW, GATLING_MAXS_ROLL};

// Tequila comment: mins2/max2 are now used to check player is not trying to
// build the gatling too near a solid or another entity (like gatlings ;))
vec3_t gatling_mins2 = {GATLING_MINS2_PITCH, GATLING_MINS2_YAW, GATLING_MINS2_ROLL};
vec3_t gatling_maxs2 = {GATLING_MAXS2_PITCH, GATLING_MAXS2_YAW, GATLING_MAXS2_ROLL};

//weapon informations
wpinfo_t bg_weaponlist[] =
{
	/*
	=============
	WP_NONE
	=============
	*/
	{
		// animations
		{
			// WP_ANIM_CHANGE
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_DROP
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_IDLE
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_FIRE
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_ALT_FIRE
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_RELOAD
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL2
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			}
		},
		0, //spread
		0, //damage
		0, //range
		0, //addTime
		0, //count
		0, //clipAmmo
		0, //clip
		0, //maxAmmo

		0, //v_model
		0, //v_barrel
		0, //snd_fire
		0, //snd_reload

		0, //name
		0, //path
		0  //wp_sort
	},

	/*
	=============
	WP_KNIFE
	=============
	*/
	{
		// animations
		{
			// WP_ANIM_CHANGE
			{
				30, //firstframe
				(40-30), //numFrames
				0, //loopFrames
				40, //frameLerp
				0, //initialLerp
				1, //reversed
				0 //flipflop
			},
			// WP_ANIM_DROP
			{
				30, //firstframe
				(40-30), //numFrames
				0, //loopFrames
				40, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_IDLE
			{
				20, //firstframe
				10, //numFrames
				0, //loopFrames
				80, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_FIRE
			{
				0, //firstframe
				20, //numFrames
				0, //loopFrames
				20, //frameLerp
				0, //initialLerp
				0, //reversed
				1 //flipflop
			},
			// WP_ANIM_ALT_FIRE
			{
				42, //firstframe
				24, //numFrames
				0, //loopFrames
				40, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_RELOAD
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL //change : throw->normal
			{
				66, //firstframe
				13, //numFrames
				0, //loopFrames
				60, //frameLerp
				60, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL2 //change : normal->throw
			{
				66, //firstframe
				13, //numFrames
				0, //loopFrames
				60, //frameLerp
				60, //initialLerp
				1, //reversed
				0 //flipflop
			}
		},
    KNIFE_SPREAD, //spread
    KNIFE_DAMAGE, //damage
    KNIFE_RANGE, //range
    KNIFE_ADDTIME, //addTime
    KNIFE_COUNT, //count
    KNIFE_CLIPAMMO, //clipAmmo
    KNIFE_CLIP, //clip
    KNIFE_MAXAMMO, //maxAmmo

		"models/weapons2/knife/v_knife.md3", //v_model
		0, //v_barrel
		"sound/weapons/knife_attack.wav",  //snd_fire
		0, //snd_reload

		"Bowie Knife", //name
		"models/weapons2/knife/", //path
		WPS_MELEE
	},
	/*
	=============
	WP_REM58
	=============
	*/
	{
		// animations
		{
			// WP_ANIM_CHANGE
			{
				0, //firstframe
				37, //numFrames
				0, //loopFrames
				20, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_DROP
			{
				251, //firstframe
				10, //numFrames
				0, //loopFrames
				40, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_IDLE
			{
				189, //firstframe
				42, //numFrames
				0, //loopFrames
				35, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_FIRE
			{
				43,//38, //firstframe
				26, //numFrames
				0, //loopFrames
				30, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_ALT_FIRE
			{
				231, //firstframe
				20, //numFrames
				0, //loopFrames
				14, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_RELOAD
			{
				69, //firstframe
				120, //numFrames
				0, //loopFrames
				30, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL2
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			}
		},
		REM58_SPREAD, //spread
		REM58_DAMAGE, //damage
		REM58_RANGE, //range
		REM58_ADDTIME, //addTime
		REM58_COUNT, //count
		REM58_CLIPAMMO, //clipAmmo
		REM58_CLIP, //clip
		REM58_MAXAMMO, //maxAmmo

		"models/weapons2/remington58/v_remington58.md3", //v_model
		0, //v_barrel
		"sound/weapons/rem58_",  //snd_fire
		"sound/weapons/rem58_reload.wav", //snd_reload

		"Remington58",
		"models/weapons2/remington58/", //path
		WPS_PISTOL
	},
	/*
	=============
	WP_SCHOFIELD
	=============
	*/
	{
		// animations
		{
			// WP_ANIM_CHANGE
			{
				0, //firstframe
				37, //numFrames
				0, //loopFrames
				20, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_DROP
			{
				207, //firstframe
				10, //numFrames
				0, //loopFrames
				40, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_IDLE
			{
				144, //firstframe
				42, //numFrames
				0, //loopFrames
				35, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_FIRE
			{
				43,//38, //firstframe
				26, //numFrames
				0, //loopFrames
				30, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_ALT_FIRE
			{
				187, //firstframe
				20, //numFrames
				0, //loopFrames
				14, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_RELOAD
			{
				69, //firstframe
				74, //numFrames
				0, //loopFrames
				50, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL2
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			}
		},
		SCHOFIELD_SPREAD, //spread
		SCHOFIELD_DAMAGE, //damage
		SCHOFIELD_RANGE, //range
		SCHOFIELD_ADDTIME, //addTime
		SCHOFIELD_COUNT, //count
		SCHOFIELD_CLIPAMMO, //clipAmmo
		SCHOFIELD_CLIP, //clip
		SCHOFIELD_MAXAMMO, //maxAmmo

		"models/weapons2/schofield/v_schofield.md3", //v_model
		0, //v_barrel
		"sound/weapons/schof_",  //snd_fire
		"sound/weapons/schof_reload.wav", //snd_reload

		"Smith&Wesson Schofield",
		"models/weapons2/schofield/", //path
		WPS_PISTOL
	},
	/*
	=============
	WP_PEACEMAKER
	=============
	*/
	{
		// animations
		{
			// WP_ANIM_CHANGE
			{
				0, //firstframe
				37, //numFrames
				0, //loopFrames
				20, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_DROP
			{
				177, //firstframe
				(187-177), //numFrames
				0, //loopFrames
				40, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_IDLE
			{
				118, //firstframe
				(156-118), //numFrames
				0, //loopFrames
				35, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_FIRE
			{
				43,//38, //firstframe
				26, //numFrames
				0, //loopFrames
				30, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_ALT_FIRE
			{
				156, //firstframe
				(177-156), //numFrames
				0, //loopFrames
				14, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_RELOAD
			{
				69, //firstframe
				(118-68),//numFrames
				0, //loopFrames
				49, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL2
			{
				83, //firstframe
				(118-82), //numFrames
				0, //loopFrames
				49, //frameLerp
				49, //initialLerp
				0, //reversed
				0 //flipflop
			}
		},
		PEACEMAKER_SPREAD, //spread
		PEACEMAKER_DAMAGE, //damage
		PEACEMAKER_RANGE, //range
		PEACEMAKER_ADDTIME, //addTime
		PEACEMAKER_COUNT, //count
		PEACEMAKER_CLIPAMMO, //clipAmmo
		PEACEMAKER_CLIP, //clip
		PEACEMAKER_MAXAMMO, //maxAmmo

		"models/weapons2/peacemaker/v_peacemaker.md3", //v_model
		0, //v_barrel
		"sound/weapons/peacem_",  //snd_fire
		"sound/weapons/peacem_reload1.wav", //snd_reload

		"Colt Peacemaker",
		"models/weapons2/peacemaker/", //path
		WPS_PISTOL
	},
	/*
	=============
	WP_WINCHESTER66
	=============
	*/
	{
		// animations
		{
			// WP_ANIM_CHANGE
			{
				0, //firstframe
				37, //numFrames
				0, //loopFrames
				20, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_DROP
			{
				106, //firstframe
				(115-105), //numFrames
				0, //loopFrames
				30, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_IDLE
			{
				94, //firstframe
				(103-93), //numFrames
				0, //loopFrames
				70, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_FIRE
			{
				43, //firstframe
				(65-43), //numFrames
				0, //loopFrames
				40, //frameLerp
				40, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_ALT_FIRE
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_RELOAD
			{
				65, //firstframe
				(96-65), //numFrames
				0, //loopFrames
				40, //frameLerp
				40, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL2
			{
				69, //firstframe
				(96-69), //numFrames
				0, //loopFrames
				40, //frameLerp
				40, //initialLerp
				0, //reversed
				0 //flipflop
			}
		},
		WINCHESTER66_SPREAD, //spread
		WINCHESTER66_DAMAGE, //damage
		WINCHESTER66_RANGE, //range
		WINCHESTER66_ADDTIME, //addTime
		WINCHESTER66_COUNT, //count
		WINCHESTER66_CLIPAMMO, //clipAmmo
		WINCHESTER66_CLIP, //clip
		WINCHESTER66_MAXAMMO, //maxAmmo

		"models/weapons2/winch66/v_winchester66.md3", //v_model
		0, //v_barrel
		"sound/weapons/winch66_",  //snd_fire
		"sound/weapons/rifle_reload1.wav", //snd_reload

		"Winchester 1866", //name
		"models/weapons2/winch66/", //path
		WPS_GUN
	},
	/*
	=============
	WP_LIGHTNING
	=============
	*/
	{
		// animations
		{
			// WP_ANIM_CHANGE
			{
				0, //firstframe
				37, //numFrames
				0, //loopFrames
				20, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_DROP
			{
				104, //firstframe
				(113-103), //numFrames
				0, //loopFrames
				30, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_IDLE
			{
				94, //firstframe
				(103-93), //numFrames
				0, //loopFrames
				70, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_FIRE
			{
				43, //firstframe
				(63-43), //numFrames
				0, //loopFrames
				35, //frameLerp
				10, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_ALT_FIRE
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_RELOAD
			{
				63, //firstframe
				(94-63), //numFrames
				0, //loopFrames
				40, //frameLerp
				40, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL2
			{
				67, //firstframe
				(94-67), //numFrames
				0, //loopFrames
				40, //frameLerp
				40, //initialLerp
				0, //reversed
				0 //flipflop
			}
		},
		LIGHTNING_SPREAD, //spread
		LIGHTNING_DAMAGE, //damage
		_LIGHTNING_RANGE, //range
		LIGHTNING_ADDTIME, //addTime
		LIGHTNING_COUNT, //count
		LIGHTNING_CLIPAMMO, //clipAmmo
		LIGHTNING_CLIP, //clip
		LIGHTNING_MAXAMMO, //maxAmmo

		"models/weapons2/colt_light/v_lightning.md3", //v_model
		0, //v_barrel
		"sound/weapons/lightning_",  //snd_fire
		0, //snd_reload

		"Colt Lightning", //name
		"models/weapons2/colt_light/", //path
		WPS_GUN
	},

	/*
	=============
	WP_SHARPS
	=============
	*/
	{
		// animations
		{
			// WP_ANIM_CHANGE
			{
				0, //firstframe
				37, //numFrames
				0, //loopFrames
				20, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_DROP
			{
				108, //firstframe
				(118-108), //numFrames
				0, //loopFrames
				30, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_IDLE
			{
				37, //firstframe
				1, //numFrames
				0, //loopFrames
				25, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_FIRE
			{
				43, //firstframe
				(54-43), //numFrames
				0, //loopFrames
				40, //frameLerp
				40, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_ALT_FIRE
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_RELOAD
			{
				53, //firstframe
				(107-53), //numFrames
				0, //loopFrames
				40, //frameLerp
				40, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL
			{
				108, //firstframe
				(118-108), //numFrames
				0, //loopFrames
				60, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL2
			{
				108, //firstframe
				(118-108), //numFrames
				0, //loopFrames
				60, //frameLerp
				0, //initialLerp
				1, //reversed
				0 //flipflop
			},
		},
		SHARP_SPREAD, //spread
		SHARP_DAMAGE, //damage
		SHARP_RANGE, //range
		SHARP_ADDTIME, //addTime
		SHARP_COUNT, //count
		SHARP_CLIPAMMO, //clipAmmo
		SHARP_CLIP, //clip
		SHARP_MAXAMMO, //maxAmmo

		"models/weapons2/sharps/v_sharps.md3", //v_model
		0, //v_barrel
		"sound/weapons/sharps_",  //snd_fire
		"sound/weapons/sharps_reload.wav", //snd_reload

		"Sharps Rifle", //name
		"models/weapons2/sharps/", //path
		WPS_GUN
	},
	/*
	=============
	WP_REMINGTON_GAUGE
	=============
	*/
	{
		// animations
		{
			// WP_ANIM_CHANGE
			{
				0, //firstframe
				48, //numFrames
				0, //loopFrames
				35, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_DROP
			{
				261, //firstframe
				4, //numFrames
				0, //loopFrames
				70, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_IDLE , not really used for shotgun
			{
				47, //firstframe
				1, //numFrames
				0, //loopFrames
				50, //frameLerp
				50, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_FIRE
			{
				49, //firstframe
				(60-48), //numFrames
				0, //loopFrames
				35, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_ALT_FIRE
			{
				73, //firstframe
				(60-48), //numFrames
				0, //loopFrames
				35, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_RELOAD
			{
				86, //firstframe
				(178-85), //numFrames
				0, //loopFrames
				35, //frameLerp
				35, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL here: fire if it is shot 2
			{
				61, //firstframe
				(60-48), //numFrames
				0, //loopFrames
				35, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL2 here: reload: if there is only one shell to reload
			{
				179, //firstframe
				(260-178), //numFrames
				0, //loopFrames
				35, //frameLerp
				35, //initialLerp
				0, //reversed
				0 //flipflop
			}
		},
		REMINGTON_GAUGE_SPREAD, //spread
		REMINGTON_GAUGE_DAMAGE, //damage
		REMINGTON_GAUGE_RANGE, //range
		REMINGTON_GAUGE_ADDTIME, //addTime
		REMINGTON_GAUGE_COUNT, //count
		REMINGTON_GAUGE_CLIPAMMO, //clipAmmo
		REMINGTON_GAUGE_CLIP, //clip
		REMINGTON_GAUGE_MAXAMMO, //maxAmmo

		"models/weapons2/remgauge/v_remgauge.md3", //v_model
		0, //v_barrel
		"sound/weapons/shotgun_",  //snd_fire
		0, //snd_reload

		"Remington 12 Gauge", //name
		"models/weapons2/remgauge/", //path
		WPS_GUN
	},
	/*
	=============
	WP_SAWEDOFF
	=============
	*/
	{
		// animations
		{
			// WP_ANIM_CHANGE
			{
				0, //firstframe
				20, //numFrames
				0, //loopFrames
				35, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_DROP
			{
				161, //firstframe
				6, //numFrames
				0, //loopFrames
				50, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_IDLE , not really used for shotgun
			{
				19, //firstframe
				1, //numFrames
				0, //loopFrames
				50, //frameLerp
				50, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_FIRE
			{
				20, //firstframe
				(34-20), //numFrames
				0, //loopFrames
				35, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_ALT_FIRE
			{
				20, //firstframe
				(34-20), //numFrames
				0, //loopFrames
				35, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_RELOAD
			{
				34, //firstframe
				(99-33), //numFrames
				0, //loopFrames
				35, //frameLerp
				35, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL2 here: reload: if there is only one shell to reload
			{
				100, //firstframe
				(162-100), //numFrames
				0, //loopFrames
				35, //frameLerp
				35, //initialLerp
				0, //reversed
				0 //flipflop
			}
		},
		SAWEDOFF_SPREAD, //spread
		SAWEDOFF_DAMAGE, //damage
		SAWEDOFF_RANGE, //range
		SAWEDOFF_ADDTIME, //addTime
		SAWEDOFF_COUNT, //count
		SAWEDOFF_CLIPAMMO, //clipAmmo
		SAWEDOFF_CLIP, //clip
		SAWEDOFF_MAXAMMO, //maxAmmo

		"models/weapons2/sawedoff/v_sawedoff.md3", //v_model
		0, //v_barrel
		"sound/weapons/shotgun_",  //snd_fire
		0, //snd_reload

		"Sawed Off Shotgun", //name
		"models/weapons2/sawedoff/", //path
		WPS_GUN
	},

	/*
	=============
	WP_WINCH97
	=============
	*/
	{
		// animations
		{
			// WP_ANIM_CHANGE
			{
				0, //firstframe
				37, //numFrames
				0, //loopFrames
				27, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_DROP
			{
				104, //firstframe
				(113-103), //numFrames
				0, //loopFrames
				30, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_IDLE
			{
				94, //firstframe
				(103-93), //numFrames
				0, //loopFrames
				100, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_FIRE
			{
				39, //firstframe
				(63-39), //numFrames
				0, //loopFrames
				32, //frameLerp
				10, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_ALT_FIRE
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_RELOAD
			{
				63, //firstframe
				(94-63), //numFrames
				0, //loopFrames
				50, //frameLerp
				50, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL2
			{
				70, //firstframe
				(94-70), //numFrames
				0, //loopFrames
				50, //frameLerp
				50, //initialLerp
				0, //reversed
				0 //flipflop
			}
		},
		WINCH97_SPREAD, //spread
		WINCH97_DAMAGE, //damage
		WINCH97_RANGE, //range
		WINCH97_ADDTIME, //addTime
		WINCH97_COUNT, //count
		WINCH97_CLIPAMMO, //clipAmmo
		WINCH97_CLIP, //clip
		WINCH97_MAXAMMO, //maxAmmo

		"models/weapons2/winch97/v_winch97.md3", //v_model
		0, //v_barrel
		"sound/weapons/winch97_",  //snd_fire
		"sound/weapons/winch97_reload1.wav", //snd_reload

		"Winchester 1897", //name
		"models/weapons2/winch97/", //path
		WPS_GUN
	},

		/*
	=============
	WP_GATLING
	=============
	*/
	{
		// animations
		{
			// WP_ANIM_CHANGE
			{
				0, //firstframe
				26, //numFrames
				0, //loopFrames
				20, //frameLerp
				0, //initialLerp
				1, //reversed
				0 //flipflop
			},
			// WP_ANIM_DROP
			{
				0, //firstframe
				26, //numFrames
				0, //loopFrames
				20, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_IDLE
			{
				0, //firstframe // idle animation
				1, //numFrames
				0, //loopFrames
				10, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_FIRE
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_ALT_FIRE
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_RELOAD
			{
				0, //firstframe
				43, //numFrames
				0, //loopFrames
				32, //frameLerp
				32, //initialLerp
				0, //reversed
				1 //flipflop
			},
			// WP_ANIM_SPECIAL // idle animation of the gatling holding in the hand
			{
				0, //firstframe
				1, //numFrames
				0, //loopFrames
				10, //frameLerp
				10, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL2
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			}
		},
		GATLING_SPREAD, //spread
		GATLING_DAMAGE, //damage
		_GATLING_RANGE, //range
		GATLING_ADDTIME, //addTime
		GATLING_COUNT, //count
		GATLING_CLIPAMMO, //clipAmmo
		GATLING_CLIP, //clip
		GATLING_MAXAMMO, //maxAmmo

		"models/weapons2/gatling/v_gatling.md3", //v_model
		0, //v_barrel
		"sound/weapons/gatling_",  //snd_fire
		"sound/weapons/gatling_reload.wav", //snd_reload

		"Gatling Gun", //name
		"models/weapons2/gatling/", //path
		WPS_GUN
	},

	/*
	=============
	WP_DYNAMITE
	=============
	*/
	{
		// animations
		{
			// WP_ANIM_CHANGE
			{
				5, //firstframe
				20, //numFrames
				0, //loopFrames
				30, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_DROP
			{
				5, //firstframe
				20, //numFrames
				0, //loopFrames
				30, //frameLerp
				0, //initialLerp
				1, //reversed
				0 //flipflop
			},
			// WP_ANIM_IDLE
			{
				135, //firstframe
				(154-134), //numFrames
				0, //loopFrames
				50, //frameLerp
				40, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_FIRE
			{
				25, //firstframe
				(79-24), //numFrames
				0, //loopFrames
				20, //frameLerp
				20, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_ALT_FIRE
			//burn the dynamite with the match
			{
				80, //firstframe
				(104-79), //numFrames
				0, //loopFrames
				55, //frameLerp
				55, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_RELOAD
			//here it is fireend
			{
				154, //firstframe
				(196-153), //numFrames
				0, //loopFrames
				20, //frameLerp
				20, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL
			// burning down the dynamite
			{
				104, //firstframe
				(134-103), //numFrames
				0, //loopFrames
				161, //frameLerp
				54, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL2
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			}
		},
		0, //spread
		0, //damage
		250, //range
		0, //addTime
		460, //count
		-1, //clipAmmo
		0, //clip
		10, //maxAmmo

		"models/weapons2/dyn/v_dynamite.md3", //v_model
		0, //v_barrel
		"sound/weapons/dyn_throw.wav",  //snd_fire
		0, //snd_reload

		"Dynamite", //name
		"models/weapons2/dyn/", //path
		WPS_OTHER
	},
	/*
	=============
	WP_MOLOTOV
	=============
	*/
	{
		// animations
		{
			// WP_ANIM_CHANGE
			{
				5, //firstframe
				15, //numFrames
				0, //loopFrames
				40, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_DROP
			{
				5, //firstframe
				15, //numFrames
				0, //loopFrames
				40, //frameLerp
				0, //initialLerp
				1, //reversed
				0 //flipflop
			},
			// WP_ANIM_IDLE
			{
				21, //firstframe
				(45-20), //numFrames
				0, //loopFrames
				90, //frameLerp
				90, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_FIRE
			{
				72, //firstframe
				(95-71), //numFrames
				0, //loopFrames
				45, //frameLerp
				45, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_ALT_FIRE
			//burn the dynamite with the match
			{
				45, //firstframe
				(67-44), //numFrames
				0, //loopFrames
				60, //frameLerp
				60, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_RELOAD
			//here it is fireend
			{
				72, //firstframe
				(90-71), //numFrames
				0, //loopFrames
				45, //frameLerp
				45, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL
			// special idle animation for burning bottle
			{
				96, //firstframe
				(117-95), //numFrames
				0, //loopFrames
				85, //frameLerp
				85, //initialLerp
				0, //reversed
				0 //flipflop
			},
			// WP_ANIM_SPECIAL2
			{
				0, //firstframe
				0, //numFrames
				0, //loopFrames
				0, //frameLerp
				0, //initialLerp
				0, //reversed
				0 //flipflop
			}
		},
		0, //spread
		0, //damage
		250, //range
		0, //addTime
		405, //count
		-1, //clipAmmo
		0, //clip
		10, //maxAmmo

		"models/weapons2/molotov/v_molotov.md3", //v_model
		0, //v_barrel
		"sound/weapons/dyn_throw.wav",  //snd_fire
		0, //snd_reload

		"Molotov Cocktail", //name
		"models/weapons2/molotov/", //path
		WPS_OTHER
	},
};

/*QUAKED item_***** ( 0 0 0 ) (-16 -16 -16) (16 16 16) suspended
DO NOT USE THIS CLASS, IT JUST HOLDS GENERAL INFORMATION.
The suspended flag will allow items to hang in the air, otherwise they are dropped to the next surface.

If an item is the target of another entity, it will not spawn in until fired.

An item fires all of its targets when it is picked up.  If the toucher can't carry it, the targets won't be fired.

"notfree" if set to 1, don't spawn in free for all games
"notteam" if set to 1, don't spawn in team games
"notsingle" if set to 1, don't spawn in single player games
"wait"	override the default wait before respawning.  -1 = never respawn automatically, which can be used with targeted spawning.
"random" random number of plus or minus seconds varied from the respawn time
"count" override quantity or duration on most items.
*/
gitem_t	bg_itemlist[] =
{
	{
		NULL,
		NULL,
		{ NULL,
		NULL, NULL, NULL } ,
/* icon */		NULL,
		0.0f,
/* pickup */	NULL,
		0,
		0,
		0,
		0,// $
		0,
/* precache */ "",
/* sounds */ ""
	},	// leave index 0 alone

	//
	// ARMOR
	//

/*QUAKED item_boiler_plate (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"item_boiler_plate",
		"sound/misc/it_pickup.wav",
		{ "models/powerups/holdable/boiler.md3",
		NULL, NULL, NULL} ,
/* icon */		"hud/weapons/boiler",
		1,
/* pickup */	"Boiler Plate",
		BOILER_PLATE,
		IT_ARMOR,
		0,
		15,
		WS_MISC,
/* precache */ "",
/* sounds */ ""
	},

	//
	// POWERUP ITEMS
	//

// has to be in front of item money because of many reasons ;)
/*QUAKED pickup_money (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"pickup_money",
		"sound/misc/m_pickup.wav",
        { "models/items/moneybag.md3",
        NULL, NULL, NULL },
/* icon */		"hud/weapons/moneybag",
		1,
/* pickup */	"Money",
		0,
		IT_POWERUP,
		PW_GOLD,
		0,
		0,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED item_money (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"item_money",
		"sound/misc/m_pickup.wav",
        { "models/items/moneybag.md3",
        NULL, NULL, NULL },
/* icon */		"hud/weapons/moneybag",
		1,
/* pickup */	"Moneybag",
		0,
		IT_POWERUP,
		PW_GOLD,
		0,
		0,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED item_scope (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"item_scope",
		"sound/misc/it_pickup.wav",
        { "models/weapons2/scope/scope.md3",
        NULL, NULL, NULL },
/* icon */		"hud/weapons/scope",
		3,
/* pickup */	"Scope",
		0,
		IT_POWERUP,
		PW_SCOPE,
		15,
		WS_MISC,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED item_belt (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"item_belt",
		"sound/misc/it_pickup.wav",
        { "models/powerups/holdable/belt.md3",
        NULL, NULL, NULL },
/* icon */		"hud/weapons/belt",
		1,
/* pickup */	"Ammunition Belt",
		0,
		IT_POWERUP,
		PW_BELT,
		10,
		WS_MISC,
/* precache */ "",
/* sounds */ ""
	},


	//
	// WEAPONS
	//

/*QUAKED weapon_winch66 (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_winch66",
		"sound/misc/w_pickup.wav",
        { "models/weapons2/winch66/e_winchester66.md3",
		NULL, NULL, NULL },
/* icon */		"hud/weapons/winch66",
		3,
/* pickup */	"Winchester 1866",
		12,
		IT_WEAPON,
		WP_WINCHESTER66,
		25,
		WS_RIFLE,
/* precache */ "",
/* sounds */ ""
	},

	/*QUAKED weapon_lightning (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_lightning",
		"sound/misc/w_pickup.wav",
        { "models/weapons2/colt_light/e_lightning.md3",
		NULL, NULL, NULL },
/* icon */		"hud/weapons/lightning",
		3,
/* pickup */	"Colt Lightning",
		12,
		IT_WEAPON,
		WP_LIGHTNING,
		30,
		WS_RIFLE,
/* precache */ "",
/* sounds */ ""
	},


	/*QUAKED weapon_sharps (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_sharps",
		"sound/misc/w_pickup.wav",
        { "models/weapons2/sharps/e_sharps.md3",
		NULL, NULL, NULL },
/* icon */		"hud/weapons/sharps",
		3,
/* pickup */	"Sharps Rifle",
		1,
		IT_WEAPON,
		WP_SHARPS,
		40,
		WS_RIFLE,
/* precache */ "",
/* sounds */ ""
	},

		/*QUAKED weapon_gatling (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_gatling",
		"sound/misc/w_pickup.wav",
        { "models/weapons2/gatling/e_gatling.md3",
		NULL, NULL, NULL },
/* icon */		"hud/weapons/gatling",
		1,
/* pickup */	"Gatling Gun",
		25,
		IT_WEAPON,
		WP_GATLING,
		53,
		WS_MGUN,
/* precache */ "",
/* sounds */ ""
	},

//
// PISTOLS
//

/*QUAKED weapon_remington58 (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_remington58",
		"sound/misc/w_pickup.wav",
        { "models/weapons2/remington58/e_remington58.md3",
		NULL, NULL, NULL },
/* icon */		"hud/weapons/rem58",
		3,
/* pickup */	"Remington58",
		6,
		IT_WEAPON,
		WP_REM58,
		15,
		WS_PISTOL,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_schofield (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_schofield",
		"sound/misc/w_pickup.wav",
        { "models/weapons2/schofield/e_schofield.md3",
		NULL, NULL, NULL } ,
/* icon */		"hud/weapons/schofield",
		3,
/* pickup */	"S&W Schofield",
		6,
		IT_WEAPON,
		WP_SCHOFIELD,
		18,
		WS_PISTOL,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_peacemaker (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_peacemaker",
		"sound/misc/w_pickup.wav",
        { "models/weapons2/peacemaker/e_peacemaker.md3",
		NULL, NULL, NULL } ,
/* icon */		"hud/weapons/peacemaker",
		3,
/* pickup */	"Colt Peacemaker",
		6,
		IT_WEAPON,
		WP_PEACEMAKER,
		22,
		WS_PISTOL,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_dynamite (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_dynamite",
		"sound/misc/w_pickup.wav",
        { "models/weapons2/dyn/e_dynamite.md3",
		NULL, NULL, NULL } ,
/* icon */		"hud/weapons/dynamite",
		3,
/* pickup */	"Dynamite",
		1,
		IT_WEAPON,
		WP_DYNAMITE,
		10,
		WS_MISC,
/* precache */ "",
/* sounds */ ""//sound/weapons/grenade/hgrenb1a.wav sound/weapons/grenade/hgrenb2a.wav"
	},

	/*QUAKED weapon_molotov (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_molotov",
		"sound/misc/w_pickup.wav",
        { "models/weapons2/molotov/e_molotov.md3",
		NULL, NULL, NULL } ,
/* icon */		"hud/weapons/molotov",
		3,
/* pickup */	"Molotov Cocktail",
		1,
		IT_WEAPON,
		WP_MOLOTOV,
		10,
		WS_MISC,
/* precache */ "",
/* sounds */ ""//sound/weapons/grenade/hgrenb1a.wav sound/weapons/grenade/hgrenb2a.wav"
	},


/*QUAKED weapon_knife (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_knife",
		"sound/misc/w_pickup.wav",
        { "models/weapons2/knife/e_knife.md3",
		NULL, NULL, NULL } ,
/* icon */		"hud/weapons/knife",
		3,
/* pickup */	"Bowie Knife",
		1,
		IT_WEAPON,
		WP_KNIFE,
		8,
		WS_MISC,
/* precache */ "",
/* sounds */ ""
	},


/*QUAKED weapon_shotgun (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_shotgun",
		"sound/misc/w_pickup.wav",
        { "models/weapons2/remgauge/e_remington.md3",
		NULL, NULL, NULL } ,
/* icon */		"hud/weapons/remington",
		3,
/* pickup */	"Remington 12 Gauge",
		2,
		IT_WEAPON,
		WP_REMINGTON_GAUGE,
		24,
		WS_SHOTGUN,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED weapon_sawedoff (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_sawedoff",
		"sound/misc/w_pickup.wav",
        { "models/weapons2/sawedoff/e_sawedoff.md3",
		NULL, NULL, NULL } ,
/* icon */		"hud/weapons/sawedoff",
		3,
/* pickup */	"Sawed Off Shotgun",
		2,
		IT_WEAPON,
		WP_SAWEDOFF,
		27,
		WS_SHOTGUN,
/* precache */ "",
/* sounds */ ""
	},

			/*QUAKED weapon_winch97 (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"weapon_winch97",
		"sound/misc/w_pickup.wav",
        { "models/weapons2/winch97/e_winch97.md3",
		NULL, NULL, NULL } ,
/* icon */		"hud/weapons/winch97",
		3,
/* pickup */	"Winchester 1897",
		5,
		IT_WEAPON,
		WP_WINCH97,
		33,
		WS_SHOTGUN,
/* precache */ "",
/* sounds */ ""
	},

	//
	// AMMO ITEMS
	//

/*QUAKED ammo_shells (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"ammo_shells",
		"sound/misc/am_pickup.wav",
        { "models/powerups/ammo/shotgammo.md3",
		NULL, NULL, NULL } ,
/* icon */		"hud/ammo_shell",
		3.2f,
/* pickup */	"Shells",
		12,
		IT_AMMO,
		WP_SHELLS_CLIP,
		5,
		0,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED ammo_bullets (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"ammo_bullets",
		"sound/misc/am_pickup.wav",
        { "models/powerups/ammo/pistolammo.md3",
		NULL, NULL, NULL } ,
/* icon */		"hud/ammo_bullet",
		3.3f,
/* pickup */	"Bullets",
		12,
		IT_AMMO,
		WP_BULLETS_CLIP,
		5,
		0,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED ammo_dynamite (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"ammo_dynamite",
		"sound/misc/am_pickup.wav",
        { "models/powerups/ammo/dynammo.md3",
		NULL, NULL, NULL } ,
/* icon */		"hud/weapons/dynamite",
		3,
/* pickup */	"Dynamite",
		5,
		IT_AMMO,
		WP_DYNAMITE,
		16,
		0,
/* precache */ "",
/* sounds */ ""
	},

	/*QUAKED ammo_molotov (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"ammo_molotov",
		"sound/misc/am_pickup.wav",
        { "models/weapons2/molotov/e_molotov.md3",
		NULL, NULL, NULL } ,
/* icon */		"hud/weapons/molotov",
		3,
/* pickup */	"Mol. Cocktails",
		5,
		IT_AMMO,
		WP_MOLOTOV,
		14,
		0,
/* precache */ "",
/* sounds */ ""
	},

	/*QUAKED ammo_knives (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"ammo_knives",
		"sound/misc/am_pickup.wav",
        { "models/weapons2/knife/e_knife.md3",
		NULL, NULL, NULL } ,
/* icon */		"hud/weapons/knife",
		3,
/* pickup */	"Knives",
		5,
		IT_AMMO,
		WP_KNIFE,
		17,
		0,
/* precache */ "",
/* sounds */ ""
	},

/*QUAKED ammo_cartridges (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"ammo_cartridges",
		"sound/misc/am_pickup.wav",
        { "models/powerups/ammo/rifleammo.md3",
		NULL, NULL, NULL } ,
/* icon */		"hud/ammo_cart",
		4.1f,
/* pickup */	"Cartridges",
		12,
		IT_AMMO,
		WP_CART_CLIP,
		5,
		0,
/* precache */ "",
/* sounds */ ""
	},

	/*QUAKED ammo_gatling (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"ammo_gatling",
		"sound/misc/am_pickup.wav",
        { "models/powerups/ammo/rifleammo.md3",
		NULL, NULL, NULL } ,
/* icon */		"hud/ammo_sharps",
		5.3f,
/* pickup */	"Gatling Gun Ammo",
		1,
		IT_AMMO,
		WP_GATLING_CLIP,
		5,
		0,
/* precache */ "",
/* sounds */ ""
	},

		/*QUAKED ammo_sharps (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
*/
	{
		"ammo_sharps",
		"sound/misc/am_pickup.wav",
        { "models/powerups/ammo/rifleammo.md3",
		NULL, NULL, NULL } ,
/* icon */		"hud/ammo_sharps",
		5.3f,
/* pickup */	"Sharps Ammo",
		12,
		IT_AMMO,
		WP_SHARPS_CLIP,
		5,
		0,
/* precache */ "",
/* sounds */ ""
	},

	// end of list marker
	{NULL}
};

int		bg_numItems = sizeof(bg_itemlist) / sizeof(bg_itemlist[0]) - 1;

/*
===============
BG_FindItemForWeapon

===============
*/
gitem_t	*BG_FindItemForWeapon( weapon_t weapon ) {
	gitem_t	*it;

	for ( it = bg_itemlist + 1 ; it->classname ; it++) {
		if ( it->giType == IT_WEAPON && it->giTag == weapon ) {
			return it;
		}
	}

	Com_Printf( "Couldn't find item for weapon %i\nPlease report on http://www.smokin-guns.net forum or http://sourceforge.net/projects/smokinguns\n", weapon);
	return NULL;
}

/*
===============
BG_FindItemForAmmo

===============
*/
gitem_t	*BG_FindItemForAmmo( weapon_t ammo ) {
	gitem_t	*it;

	for ( it = bg_itemlist + 1 ; it->classname ; it++) {
		if ( it->giType == IT_AMMO && it->giTag == ammo ) {
			return it;
		}
	}

	Com_Printf( "Couldn't find item for ammo %i\nPlease report on http://www.smokin-guns.net forum or http://sourceforge.net/projects/smokinguns\n", ammo);
	return NULL;
}

/*
===========================
BG_FindPlayerWeapon
===========================
*/
int	BG_FindPlayerWeapon( int firstweapon, int lastweapon, playerState_t	*ps){
	int i;

	for(i=firstweapon;i<lastweapon;i++){
		if(ps->stats[STAT_WEAPONS] & (1<<i)){
			return i;
		}
	}
	return 0;
}

/*
===============
BG_FindItemForClassname
by Spoon
===============
*/
gitem_t	*BG_FindItemForClassname( const char *classname ) {
	gitem_t	*it;

	for ( it = bg_itemlist + 1 ; it->classname ; it++ ) {
		if ( !Q_stricmp( it->classname, classname ) )
			return it;
	}

	return NULL;
}

/*
===============
BG_FindItem

===============
*/
gitem_t	*BG_FindItem( const char *pickupName ) {
	gitem_t	*it;

	for ( it = bg_itemlist + 1 ; it->classname ; it++ ) {
		if ( !Q_stricmp( it->pickup_name, pickupName ) )
			return it;
	}

	return NULL;
}

/*
============
BG_PlayerTouchesItem

Items can be picked up without actually touching their physical bounds to make
grabbing them easier
============
*/
qboolean	BG_PlayerTouchesItem( playerState_t *ps, entityState_t *item, int atTime ) {
	vec3_t		origin;

	BG_EvaluateTrajectory( &item->pos, atTime, origin );

	// we are ignoring ducked differences here
	if ( ps->origin[0] - origin[0] > 44
		|| ps->origin[0] - origin[0] < -50
		|| ps->origin[1] - origin[1] > 36
		|| ps->origin[1] - origin[1] < -36
		|| ps->origin[2] - origin[2] > 36
		|| ps->origin[2] - origin[2] < -36 ) {
		return qfalse;
	}

	return qtrue;
}


/*
================
BG_CanItemBeGrabbed

Returns false if the item should not be picked up.
This needs to be the same for client side prediction and server use.
================
*/
qboolean BG_CanItemBeGrabbed( int gametype, const entityState_t *ent, const playerState_t *ps ) {
	gitem_t	*item;
	int		belt = 1, i;

// hika additional comments:
// Molotovs, dynamites and knives are no longer affected by belt double ammo effect.
// See ./code/g_cmds.c : Cmd_BuyItem_f() and
//     ./code/g_items.c : Add_Ammo()

	if(ps->powerups[PW_BELT])
		belt = 2;

	if(ent->eType == ET_TURRET){
		return qfalse;
	}

	if ( ent->modelindex < 1 || ent->modelindex >= bg_numItems ) {
		return qfalse;
	}

	if(ps->persistant[PERS_TEAM] >= TEAM_SPECTATOR)
		return qfalse;

	item = &bg_itemlist[ent->modelindex];

	switch( item->giType ) {
	case IT_WEAPON:
		//can't pickup the same weapon twice
		switch(item->giTag){
		case WP_KNIFE:
		case WP_DYNAMITE:
		case WP_MOLOTOV:
		// Molotovs, dynamites and knives can no longer be affected by belt double ammo effect.
			if ( ps->ammo[ item->giTag ] >= bg_weaponlist[item->giTag].maxAmmo ){
				return qfalse;
			}
			break;
		}

		// can't have more than two pistols
		if(bg_weaponlist[item->giTag].wp_sort == WPS_PISTOL){
			int count = 0;

			if(ps->stats[STAT_FLAGS] & SF_SEC_PISTOL)
				count = 1;

			for(i=WP_REM58; i < WP_NUM_WEAPONS; i++){

				if(bg_weaponlist[i].wp_sort == WPS_PISTOL &&
					(ps->stats[STAT_WEAPONS] & (1<< i)))
					count++;

				if(count >= 2)
					return qfalse;
			}
		}

		// cant't have two special weapons
		if(item->giTag >= WP_WINCHESTER66 && item->giTag < WP_DYNAMITE){
			for(i=WP_WINCHESTER66; i < WP_DYNAMITE; i++){
				if(ps->stats[STAT_WEAPONS] & (1 << i)){
					return qfalse;
				}
			}
		}
		return qtrue;	// weapons are always picked up

	case IT_AMMO:
		switch(item->giTag){

		case WP_BULLETS_CLIP:
			if ( ps->ammo[ item->giTag ] >= bg_weaponlist[WP_PEACEMAKER].maxAmmo*belt )
				return qfalse;
			break;
		case WP_SHELLS_CLIP:
			if ( ps->ammo[ item->giTag ] >= bg_weaponlist[WP_REMINGTON_GAUGE].maxAmmo*belt )
				return qfalse;
			break;
		case WP_CART_CLIP:
			if ( ps->ammo[ item->giTag ] >= bg_weaponlist[WP_WINCHESTER66].maxAmmo*belt )
				return qfalse;
			break;
		case WP_SHARPS_CLIP:
			if ( ps->ammo[ item->giTag ] >= bg_weaponlist[WP_SHARPS].maxAmmo*belt )
				return qfalse;
			break;
		case WP_GATLING_CLIP:
			if ( ps->ammo[ item->giTag ] >= bg_weaponlist[WP_GATLING].maxAmmo*belt )
				return qfalse;
			break;
		case WP_DYNAMITE:
		// Dynamites can no longer be affected by belt double ammo effect.
			if ( ps->ammo[ item->giTag ] >= bg_weaponlist[item->giTag].maxAmmo )
				return qfalse;
			break;
		default:
			if ( ps->ammo[ item->giTag ] >= bg_weaponlist[item->giTag].maxAmmo*belt )
				return qfalse;
			break;
		}
		return qtrue;

	case IT_ARMOR:
		if ( ps->stats[STAT_ARMOR] >= BOILER_PLATE ) {
			return qfalse;
		}
		return qtrue;

	case IT_HEALTH:
		// small and mega healths will go over the max, otherwise
		// don't pick up if already at max
		if ( item->quantity == 5 || item->quantity == 100 ) {
			if ( ps->stats[STAT_HEALTH] >= ps->stats[STAT_MAX_HEALTH] * 2 ) {
				return qfalse;
			}
			return qtrue;
		}

		if ( ps->stats[STAT_HEALTH] >= ps->stats[STAT_MAX_HEALTH] ) {
			return qfalse;
		}
		return qtrue;

	case IT_POWERUP:
		//if he already has this powerup
		if(ps->powerups[item->giTag])
			return qfalse;

		if(item->giTag == PW_GOLD){

			if(gametype == GT_BR){
				if(!ps->persistant[PERS_ROBBER])
					return qfalse;
			} else {
				if(ps->stats[STAT_MONEY] >= MAX_MONEY)
					return qfalse;
			}
		}
		return qtrue;	// powerups are always picked up

	case IT_TEAM: // team items, such as flags
		return qfalse;

	case IT_HOLDABLE:
		// can only hold one item at a time
		if ( ps->stats[STAT_HOLDABLE_ITEM] ) {
			return qfalse;
		}
		return qtrue;

	case IT_BAD:
		Com_Error( ERR_DROP, "BG_CanItemBeGrabbed: IT_BAD" );
	default:
#ifndef Q3_VM
#ifndef NDEBUG
        Com_Printf("BG_CanItemBeGrabbed: unknown enum %d\n", item->giType );
#endif
#endif
         break;
	}

	return qfalse;
}

//======================================================================

/*
================
BG_EvaluateTrajectory

================
*/
void BG_EvaluateTrajectory( const trajectory_t *tr, int atTime, vec3_t result ) {
	float		deltaTime;
	float		phase;

	switch( tr->trType ) {
	case TR_STATIONARY:
	case TR_INTERPOLATE:
		VectorCopy( tr->trBase, result );
		break;
	case TR_LINEAR:
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
		break;
	case TR_SINE:
		deltaTime = ( atTime - tr->trTime ) / (float) tr->trDuration;
		phase = sin( deltaTime * M_PI * 2 );
		VectorMA( tr->trBase, phase, tr->trDelta, result );
		break;
	case TR_LINEAR_STOP:
		if ( atTime > tr->trTime + tr->trDuration ) {
			atTime = tr->trTime + tr->trDuration;
		}
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		if ( deltaTime < 0 ) {
			deltaTime = 0;
		}
		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
		break;
	case TR_GRAVITY:
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
		result[2] -= 0.5 * DEFAULT_GRAVITY * deltaTime * deltaTime;		// FIXME: local gravity...
		break;
	case TR_GRAVITY_LOW:
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
		result[2] -= 0.3 * DEFAULT_GRAVITY * deltaTime * deltaTime;		// FIXME: local gravity...
		break;
	case TR_GRAVITY_LOW2:
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
		result[2] -= 0.4 * DEFAULT_GRAVITY * deltaTime * deltaTime;		// FIXME: local gravity...
		break;
	default:
		Com_Error( ERR_DROP, "BG_EvaluateTrajectory: unknown trType: %i", tr->trType );
		break;
	}
}

/*
================
BG_EvaluateTrajectoryDelta

For determining velocity at a given time
================
*/
void BG_EvaluateTrajectoryDelta( const trajectory_t *tr, int atTime, vec3_t result ) {
	float	deltaTime;
	float	phase;

	switch( tr->trType ) {
	case TR_STATIONARY:
	case TR_INTERPOLATE:
		VectorClear( result );
		break;
	case TR_LINEAR:
		VectorCopy( tr->trDelta, result );
		break;
	case TR_SINE:
		deltaTime = ( atTime - tr->trTime ) / (float) tr->trDuration;
		phase = cos( deltaTime * M_PI * 2 );	// derivative of sin = cos
		phase *= 0.5;
		VectorScale( tr->trDelta, phase, result );
		break;
	case TR_LINEAR_STOP:
		if ( atTime > tr->trTime + tr->trDuration ) {
			VectorClear( result );
			return;
		}
		VectorCopy( tr->trDelta, result );
		break;
	case TR_GRAVITY:
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		VectorCopy( tr->trDelta, result );
		result[2] -= DEFAULT_GRAVITY * deltaTime;		// FIXME: local gravity...
		break;
	case TR_GRAVITY_LOW:
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		VectorCopy( tr->trDelta, result );
		result[2] -= DEFAULT_GRAVITY/2 * deltaTime;		// FIXME: local gravity...
		break;
	case TR_GRAVITY_LOW2:
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		VectorCopy( tr->trDelta, result );
		result[2] -= DEFAULT_GRAVITY/1.5 * deltaTime;		// FIXME: local gravity...
		break;
	default:
		Com_Error( ERR_DROP, "BG_EvaluateTrajectoryDelta: unknown trType: %i", tr->trType );
		break;
	}
}

#ifdef _DEBUG
// FIXME: Move that array close to the corresponding enumeration in q_shared.h
char *eventnames[] = {
	"EV_NONE",

	"EV_FOOTSTEP",
	"EV_FOOTSTEP_METAL",
	"EV_FOOTSPLASH",
	"EV_FOOTWADE",
	"EV_SWIM",

	"EV_STEP_4",
	"EV_STEP_8",
	"EV_STEP_12",
	"EV_STEP_16",

	"EV_FALL_SHORT",
	"EV_FALL_MEDIUM",
	"EV_FALL_FAR",

	"EV_JUMP_PAD",			// boing sound at origin", jump sound on player

	"EV_JUMP",
	"EV_WATER_TOUCH",	// foot touches
	"EV_WATER_LEAVE",	// foot leaves
	"EV_WATER_UNDER",	// head touches
	"EV_WATER_CLEAR",	// head leaves

	"EV_ITEM_PICKUP",			// normal item pickups are predictable

	"EV_NOAMMO",
	"EV_CHANGE_WEAPON",
	"EV_FIRE_WEAPON",

	"EV_USE_ITEM0",
	"EV_USE_ITEM1",
	"EV_USE_ITEM2",
	"EV_USE_ITEM3",
	"EV_USE_ITEM4",
	"EV_USE_ITEM5",
	"EV_USE_ITEM6",
	"EV_USE_ITEM7",
	"EV_USE_ITEM8",
	"EV_USE_ITEM9",
	"EV_USE_ITEM10",
	"EV_USE_ITEM11",
	"EV_USE_ITEM12",
	"EV_USE_ITEM13",
	"EV_USE_ITEM14",
	"EV_USE_ITEM15",

	"EV_ITEM_RESPAWN",
	"EV_ITEM_POP",
	"EV_PLAYER_TELEPORT_IN",
	"EV_PLAYER_TELEPORT_OUT",

	"EV_GRENADE_BOUNCE",		// eventParm will be the soundindex

	"EV_GENERAL_SOUND",
	"EV_GLOBAL_SOUND",		// no attenuation
	"EV_GLOBAL_TEAM_SOUND",

	"EV_BULLET_HIT_FLESH",
	"EV_BULLET_HIT_WALL",

	"EV_MISSILE_HIT",
	"EV_MISSILE_MISS",
	"EV_MISSILE_MISS_METAL",
	"EV_RAILTRAIL",
	"EV_SHOTGUN",
	"EV_BULLET",				// otherEntity is the shooter

	"EV_PAIN",
	"EV_DEATH1",
	"EV_DEATH2",
	"EV_DEATH3",
	"EV_OBITUARY",

	"EV_POWERUP_QUAD",
	"EV_POWERUP_BATTLESUIT",
	"EV_POWERUP_REGEN",

	"EV_GIB_PLAYER",			// gib a previously living player
	"EV_SCOREPLUM",			// score plum

	"EV_DEBUG_LINE",
	"EV_STOPLOOPINGSOUND",
	"EV_TAUNT"

};
#endif

/*
===============
BG_AddPredictableEventToPlayerstate

Handles the sequence numbers
===============
*/

void	trap_Cvar_VariableStringBuffer( const char *var_name, char *buffer, int bufsize );

void BG_AddPredictableEventToPlayerstate( int newEvent, int eventParm, playerState_t *ps ) {

#ifdef _DEBUG
	{
		char buf[256];
		trap_Cvar_VariableStringBuffer("showevents", buf, sizeof(buf));
		if ( atof(buf) != 0 ) {
#ifdef QAGAME
			Com_Printf(" game event svt %5d -> %5d: num = %20s parm %d\n", ps->pmove_framecount/*ps->commandTime*/, ps->eventSequence, eventnames[newEvent], eventParm);
#else
			Com_Printf("Cgame event svt %5d -> %5d: num = %20s parm %d\n", ps->pmove_framecount/*ps->commandTime*/, ps->eventSequence, eventnames[newEvent], eventParm);
#endif
		}
	}
#endif
	ps->events[ps->eventSequence & (MAX_PS_EVENTS-1)] = newEvent;
	ps->eventParms[ps->eventSequence & (MAX_PS_EVENTS-1)] = eventParm;
	ps->eventSequence++;
}

/*
========================
BG_PlayerStateToEntityState

This is done after each set of usercmd_t on the server,
and after local prediction on the client
========================
*/
void BG_PlayerStateToEntityState( playerState_t *ps, entityState_t *s, qboolean snap ) {
	int		i;

	if(ps->pm_type == PM_SPECTATOR && !(ps->stats[STAT_FLAGS] & SF_BOT)){
		s->eType = ET_FLY;
	} else if ( ps->pm_type == PM_INTERMISSION || ps->pm_type == PM_SPECTATOR ) {
		s->eType = ET_INVISIBLE;
	} else if ( ps->stats[STAT_HEALTH] <= GIB_HEALTH ) {
		s->eType = ET_INVISIBLE;
	} else {
		s->eType = ET_PLAYER;
	}

	s->number = ps->clientNum;

	s->pos.trType = TR_INTERPOLATE;
	VectorCopy( ps->origin, s->pos.trBase );
	if ( snap ) {
		SnapVector( s->pos.trBase );
	}
	// set the trDelta for flag direction
	VectorCopy( ps->velocity, s->pos.trDelta );

	s->apos.trType = TR_INTERPOLATE;
	VectorCopy( ps->viewangles, s->apos.trBase );
	if ( snap ) {
		SnapVector( s->apos.trBase );
	}

	s->angles2[YAW] = ps->movementDir;
	s->legsAnim = ps->legsAnim;
	s->torsoAnim = ps->torsoAnim;
	s->clientNum = ps->clientNum;		// ET_PLAYER looks here instead of at number
										// so corpses can also reference the proper config
	s->eFlags = ps->eFlags;
	if ( ps->stats[STAT_HEALTH] <= 0 ) {
		s->eFlags |= EF_DEAD;
	} else {
		s->eFlags &= ~EF_DEAD;
	}

	if ( ps->externalEvent ) {
		s->event = ps->externalEvent;
		s->eventParm = ps->externalEventParm;
	} else if ( ps->entityEventSequence < ps->eventSequence ) {
		int		seq;

		if ( ps->entityEventSequence < ps->eventSequence - MAX_PS_EVENTS) {
			ps->entityEventSequence = ps->eventSequence - MAX_PS_EVENTS;
		}
		seq = ps->entityEventSequence & (MAX_PS_EVENTS-1);
		s->event = ps->events[ seq ] | ( ( ps->entityEventSequence & 3 ) << 8 );
		s->eventParm = ps->eventParms[ seq ];
		ps->entityEventSequence++;
	}

	s->weapon = ps->weapon;
	s->groundEntityNum = ps->groundEntityNum;

	s->powerups = 0;
	for ( i = 0 ; i < MAX_POWERUPS ; i++ ) {
		if ( ps->powerups[ i ] ) {
			if(i == PW_SCOPE && ps->powerups[i] != 2)
				continue;

			if(i == DM_TORSO_1 || i == DM_LEGS_1|| i == DM_HEAD_1){
				if(ps->powerups[i] >= 2)
					s->powerups |= 1 << (i+1);
				else
					s->powerups |= 1 << i;
				i++;
				continue;
			}
			s->powerups |= 1 << i;
		}
	}
	s->time = ps->powerups[PW_BURNBIT];
	s->time2 = ps->stats[STAT_WEAPONS];
	s->frame = ps->weapon2;

	s->loopSound = ps->loopSound;
	s->generic1 = ps->generic1;

	if((ps->weapon == WP_MOLOTOV || ps->weapon == WP_DYNAMITE) &&
		ps->stats[STAT_WP_MODE] < 0){
		s->powerups |= (1 << PW_BURN);
	}

	// Keep the SEC_PISTOL info, in a "special" bit.
	// Use the special WP_AKIMBO as it is just after WP_NUM_WEAPONS
	// (see weapon_t enum in bg_public.h)
	// Of course, WP_AKIMBO should be less than the max int capacity
	if (ps->stats[STAT_FLAGS] & SF_SEC_PISTOL)
		s->time2 |= (1 << WP_AKIMBO);
}

/*
========================
BG_PlayerStateToEntityStateExtraPolate

This is done after each set of usercmd_t on the server,
and after local prediction on the client
========================
*/
void BG_PlayerStateToEntityStateExtraPolate( playerState_t *ps, entityState_t *s, int time, qboolean snap ) {
	int		i;

	if(ps->pm_type == PM_SPECTATOR && !(ps->stats[STAT_FLAGS] & SF_BOT)){
		s->eType = ET_FLY;
	} else if ( ps->pm_type == PM_INTERMISSION || ps->pm_type == PM_SPECTATOR ) {
		s->eType = ET_INVISIBLE;
	} else if ( ps->stats[STAT_HEALTH] <= GIB_HEALTH ) {
		s->eType = ET_INVISIBLE;
	} else {
		s->eType = ET_PLAYER;
	}

	s->number = ps->clientNum;

	s->pos.trType = TR_LINEAR_STOP;
	VectorCopy( ps->origin, s->pos.trBase );
	if ( snap ) {
		SnapVector( s->pos.trBase );
	}
	// set the trDelta for flag direction and linear prediction
	VectorCopy( ps->velocity, s->pos.trDelta );
	// set the time for linear prediction
	s->pos.trTime = time;
	// set maximum extra polation time
	s->pos.trDuration = 50; // 1000 / sv_fps (default = 20)

	s->apos.trType = TR_INTERPOLATE;
	VectorCopy( ps->viewangles, s->apos.trBase );
	if ( snap ) {
		SnapVector( s->apos.trBase );
	}

	s->angles2[YAW] = ps->movementDir;
	s->legsAnim = ps->legsAnim;
	s->torsoAnim = ps->torsoAnim;
	s->clientNum = ps->clientNum;		// ET_PLAYER looks here instead of at number
										// so corpses can also reference the proper config
	s->eFlags = ps->eFlags;
	if ( ps->stats[STAT_HEALTH] <= 0 ) {
		s->eFlags |= EF_DEAD;
	} else {
		s->eFlags &= ~EF_DEAD;
	}

	if ( ps->externalEvent ) {
		s->event = ps->externalEvent;
		s->eventParm = ps->externalEventParm;
	} else if ( ps->entityEventSequence < ps->eventSequence ) {
		int		seq;

		if ( ps->entityEventSequence < ps->eventSequence - MAX_PS_EVENTS) {
			ps->entityEventSequence = ps->eventSequence - MAX_PS_EVENTS;
		}
		seq = ps->entityEventSequence & (MAX_PS_EVENTS-1);
		s->event = ps->events[ seq ] | ( ( ps->entityEventSequence & 3 ) << 8 );
		s->eventParm = ps->eventParms[ seq ];
		ps->entityEventSequence++;
	}

	s->weapon = ps->weapon;
	s->groundEntityNum = ps->groundEntityNum;

	s->powerups = 0;
	for ( i = 0 ; i < MAX_POWERUPS ; i++ ) {
		if ( ps->powerups[ i ] ) {
			if(i == PW_SCOPE && ps->powerups[i] != 2)
				continue;

			if(i == DM_TORSO_1 || i == DM_LEGS_1|| i == DM_HEAD_1){
				if(ps->powerups[i] >= 2)
					s->powerups |= 1 << (i+1);
				else
					s->powerups |= 1 << i;

				i++;
				continue;
			}
			s->powerups |= 1 << i;
		}
	}
	s->time = ps->powerups[PW_BURNBIT];
	s->time2 = ps->stats[STAT_WEAPONS];
	s->frame = ps->weapon2;

	s->loopSound = ps->loopSound;
	s->generic1 = ps->generic1;

	if((ps->weapon == WP_MOLOTOV || ps->weapon == WP_DYNAMITE) &&
		ps->stats[STAT_WP_MODE] < 0){
		s->powerups |= (1 << PW_BURN);
	}

	// Keep the SEC_PISTOL info, in a "special" bit.
	// Use the special WP_AKIMBO as it is just after WP_NUM_WEAPONS
	// (see weapon_t enum in bg_public.h).
	// Of course, WP_AKIMBO should be less than the max int capacity
	if (ps->stats[STAT_FLAGS] & SF_SEC_PISTOL)
		s->time2 |= (1 << WP_AKIMBO);
}

/*
==========================
BG_AnimLength
by Spoon
==========================
*/
int BG_AnimLength( int anim, int weapon) {
	int length;

	length = (bg_weaponlist[weapon].animations[anim].numFrames-1)*
		bg_weaponlist[weapon].animations[anim].frameLerp+
		bg_weaponlist[weapon].animations[anim].initialLerp;

	if(bg_weaponlist[weapon].animations[anim].flipflop){
		length *= 2;
		length -= bg_weaponlist[weapon].animations[anim].initialLerp;
	}

	return length;
}

//infoname, surfaceFlag, radius, weight, num, fallingfactor, thickness
prefixInfo_t prefixInfo[NUM_PREFIXINFO] = {
	{ "metal", SURF_METAL,		1.9f,	1.2f,	7,	1,	0.1f},
	{ "wood", SURF_WOOD,		1.6f,	1.0f,	10,	1,	1.0f},
	{ "cloth", SURF_CLOTH,		1.0f,	0.75f ,	7,	1,	0.2f},
	{ "dirt", SURF_DIRT,		4.0f,	1.0f,	15, 1,	0.7f},
	{ "glass", SURF_GLASS,		0.6f,	1.2f,	5,	1,	0.5f},
	{ "plant", SURF_PLANT,		4.0f,	1.0f,	7,	1,	2.0f},
	{ "sand", SURF_SAND,		0.6f,	0.85f,	25, 1,	0.3f},
	{ "snow", SURF_SNOW,		4.0f,	0.85f,	15, 1,	0.4f},
	{ "stone", SURF_STONE,		4.0f,	1.0f,	15, 1,	0.3f},
	{ "water", SURF_WATER,		0.6f,	1.0f,	15, 1,	1.0f},
	{ "grass", SURF_GRASS,		7.0f,	0.9f,	7,	1,	2.0f},
	{ "other", 0,				0.8f,	1.0f,	10, 1,	0.5f}
};

// shoot-thru-walls code( STW wuahahahaha, damn shit)
qboolean BG_ShootThruWall( float *damage,
						  vec3_t start, vec3_t muzzle, int surfaceFlags, vec3_t end,
						void (*trace)( trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentMask )){
	trace_t			tr;
	int	i;
	float	factor = 0.0f, distance;
	vec3_t dir;

	// find surface
	for(i = 0; i < NUM_PREFIXINFO; i++){
		if(surfaceFlags & prefixInfo[i].surfaceFlags){
			factor = prefixInfo[i].thickness;
			break;
		}
	}

	// no surface found
	if(factor == 0.0f){
		return qfalse;
	}

	// calculate direction
	VectorSubtract(start, muzzle, dir);
	distance = VectorNormalize(dir);

	// set strength of bullet
	factor *= 2.5f;//(1.1f + (*damage/25.0f));

#define CHECK 2.0f
#define MAX_DISTANCE 300
	for(i = 1; i < 10; i++){
		vec3_t temp, tempend;
		vec3_t	mins, maxs;

		VectorSet(mins, -CHECK, -CHECK, -CHECK);
		VectorSet(maxs, CHECK, CHECK, CHECK);

		VectorMA(start, i*factor, dir, temp);
		VectorMA(start, (i+1)*factor, dir, tempend);

		trace( &tr, temp, NULL, NULL, tempend, -1, MASK_SHOT);

		// shoot thru!
		if(!tr.allsolid && !tr.startsolid){
			vec3_t dist, trend;
			float distance;

			// trace into the direction and look if the level has ended after that wall
			VectorMA(start, 10000, dir, trend);
			trace( &tr, tempend, mins, maxs, trend, -1, MASK_SOLID);
			if(tr.fraction == 1.0 || tr.allsolid || tr.startsolid)
				return qfalse;

			// get exact endpoint (shoot into the other direction)
			VectorMA(start, -100, dir, trend);
			trace( &tr, tempend, mins, maxs, trend, -1, MASK_SOLID);
			VectorCopy(tr.endpos, end);

			// if the surface doesn't have a right normal vector don't shoouthru(curves)
			/*if(tr.plane.normal[0] == 0 &&
				tr.plane.normal[0] == 0 &&
				tr.plane.normal[0] == 0)
				return qfalse;*/

			// now see if we could trace a direct line between the two sides
			//trace( &tr, tempend, NULL, NULL, start, -1, MASK_SOLID);

			//Com_Printf("%f %i %i\n", tr.fraction, tr.startsolid, tr.allsolid, tr.entityNum);

			// nothing solid in between
			//if(tr.fraction == 1.0 && !tr.allsolid){
			//	return qfalse;
			//}

			// calculate distance
			VectorSubtract(end, start, dist);
			distance = VectorNormalize(dist);

			if(distance > *damage*6.0f)
				break;

			// decrease damage
			if(distance > MAX_DISTANCE)
				break;

			// change the endpos a bit
			//VectorMA(end, -1, dist, end);
			*damage *= 0.5f*sqrt((MAX_DISTANCE-distance)/MAX_DISTANCE);
			return qtrue;
		}
	}
	return qfalse;
}

void BG_SurfaceFlags2Prefix(int surfaceFlags, char	*prefix){
	int i;

	for(i=0;i<NUM_PREFIXINFO;i++){
		if(surfaceFlags & prefixInfo[i].surfaceFlags){
			strcpy(prefix, prefixInfo[i].name);
			break;
		}
	}
}

void BG_StringRead(char *destination, char *source, int size) {
	int i;

	for( i = 0; i < size; i++ ) {
		destination[i] = source[i];
	}
}

void BG_ModifyEyeAngles( vec3_t origin, vec3_t viewangles,
						void (*trace)( trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentMask ),
						vec3_t legOffset, qboolean print){
	vec3_t			forward, angles, start, temp;
	float			pitch;
	trace_t			tr;
	vec3_t			mins, maxs;

	VectorCopy(origin, start);
	VectorSet(mins, -5.0f, -5.0f, -5.0f);
	VectorSet(maxs, 5.0f, 5.0f, 5.0f);

	origin[2] += 3.0f;
	AngleVectors( viewangles, forward, NULL, NULL );

	VectorCopy(viewangles, angles);
	AnglesNormalize180(angles);

	pitch = angles[PITCH];
	angles[PITCH] = 0;
	angles[ROLL] = 0;

	AngleVectors(angles, forward, NULL, NULL);

	if(print) {
	if(legOffset)
		Com_Printf("cg ");
	else if(!legOffset)
		Com_Printf("g  ");
	}

	VectorScale(forward, (pitch/3.5f), temp);
	VectorAdd( origin, temp, origin);

	if(print)
		Com_Printf("%f, %f %f %f\n", pitch/3.5f, origin[0], origin[1], origin[2]);

	// check if point is in solid
	VectorScale(mins, 1.3f, mins);
	VectorScale(maxs, 1.3f, maxs);
	trace( &tr, start, mins, maxs, origin, -1, CONTENTS_SOLID);

	if(legOffset)
		VectorClear(legOffset);

	if(tr.fraction != 1.0){

		if(legOffset){
			VectorSubtract(origin, tr.endpos, legOffset);
		}

		VectorCopy(tr.endpos, origin);
	}
}

int BG_CountTypeWeapons(int type, int weapons){
	int i;
	int count = 0;

	for(i=1; i < WP_NUM_WEAPONS; i++){

		if(weapons & (1 << i)){

			if(bg_weaponlist[i].wp_sort == type)
				count++;
		}
	}

	return count;
}

int BG_SearchTypeWeapon(int type, int weapons, int wp_ignore){
	int i;

	for(i=1; i<WP_NUM_WEAPONS; i++){

		if(i == wp_ignore)
			continue;

		if(weapons & (1 << i)){

			if(bg_weaponlist[i].wp_sort == type)
				return i;
		}
	}
	return 0;
}

qboolean CheckPistols(playerState_t *ps, int *weapon){

	// can't have more than two pistols -> drop bad ones
	int count = 0;
	int i;
	*weapon = 999;

	if(ps->stats[STAT_FLAGS] & SF_SEC_PISTOL)
		count = 1;

	for(i=WP_REM58; i < WP_NUM_WEAPONS; i++){

		if(bg_weaponlist[i].wp_sort == WPS_PISTOL &&
			(ps->stats[STAT_WEAPONS] & (1 << i))){
			count++;

			if(i < *weapon)
				*weapon = i;
		}

		if(count >= 2){
			return qtrue;
		}
	}
	return qfalse;
}

// I often want to check the vectors ingame, so i need this, works faster
void Com_PrintfVector(vec3_t vec){
	Com_Printf("%f, %f, %f\n", vec[0], vec[1], vec[2]);
}

void Com_PrintfVectorInt(int vec[3]){
	Com_Printf("%i, %i, %i\n", vec[0], vec[1], vec[2]);
}

void BG_SetWhiskeyDrop(trajectory_t *tr, vec3_t org, vec3_t normal, vec3_t dir){
	vec3_t origin;

	VectorMA(org, 8, normal, origin);

	if(tr){
		VectorCopy(origin, tr->trBase);
		VectorScale(dir, 180, tr->trDelta);
		tr->trType = TR_GRAVITY_LOW2;
	} else {
		VectorCopy(origin, org);
	}
}

void BG_DirsToEntityState(entityState_t *es, vec3_t bottledirs[ALC_COUNT]){

	es->frame = DirToByte(bottledirs[0]);
	es->legsAnim = DirToByte(bottledirs[1]);
	es->otherEntityNum = DirToByte(bottledirs[2]);
	es->powerups = DirToByte(bottledirs[3]);
	es->time = DirToByte(bottledirs[4]);
	es->time2 = DirToByte(bottledirs[5]);
	es->torsoAnim = DirToByte(bottledirs[6]);
	es->otherEntityNum2 = DirToByte(bottledirs[7]);
}

void BG_EntityStateToDirs(entityState_t *es, vec3_t bottledirs[ALC_COUNT]){

	ByteToDir(es->frame, bottledirs[0]);
	ByteToDir(es->legsAnim, bottledirs[1]);
	ByteToDir(es->otherEntityNum, bottledirs[2]);
	ByteToDir(es->powerups, bottledirs[3]);
	ByteToDir(es->time, bottledirs[4]);
	ByteToDir(es->time2, bottledirs[5]);
	ByteToDir(es->torsoAnim, bottledirs[6]);
	ByteToDir(es->otherEntityNum2, bottledirs[7]);
}

int BG_MapPrefix(char *map, int gametype){

	if(map && map[0] && map[1]){
		if(map[2] == '_'){
			char gt[4];

			gt[0] = map[0];
			gt[1] = map[1];
			gt[2] = map[2];
			gt[3] = '\0';

			if(!Q_stricmp(gt,"br_")){
				return GT_BR;
			} else if(!Q_stricmp(gt, "du_")){
				return GT_DUEL;
			}
		}
	}

	if(gametype == GT_SINGLE_PLAYER ||
		    gametype == GT_DUEL ||
		    gametype >= GT_BR)
		return GT_FFA;

	return gametype;
}

hit_info_t	hit_info[NUM_HIT_LOCATIONS] = {
	{"hit_h_head",			"head",		"head",		HIT_HEAD,			PART_HEAD	},

	{"hit_u_neck",			"neck",		"neck",		HIT_NECK,			PART_UPPER	},
	{"hit_u_shoulder_r",	"shoulder",	"shoulder",	HIT_SHOULDER_R,		PART_UPPER	},
	{"hit_u_upper_arm_r",	"arm",		"arm",		HIT_UPPER_ARM_R,	PART_UPPER	},
	{"hit_u_lower_arm_r",	"arm",		"arm",		HIT_LOWER_ARM_R,	PART_UPPER	},
	{"hit_u_hand_r",		"hand",		"hand",		HIT_HAND_R,			PART_UPPER	},
	{"hit_u_shoulder_l",	"shoulder",	"shoulder",	HIT_SHOULDER_L,		PART_UPPER	},
	{"hit_u_upper_arm_l",	"arm",		"arm",		HIT_UPPER_ARM_L,	PART_UPPER	},
	{"hit_u_lower_arm_l",	"arm",		"arm",		HIT_LOWER_ARM_L,	PART_UPPER	},
	{"hit_u_hand_l",		"hand",		"hand",		HIT_HAND_L,			PART_UPPER	},
	{"hit_u_chest",			"chest",	"back",		HIT_CHEST,			PART_UPPER	},
	{"hit_u_stomach",		"stomach",	"back",		HIT_STOMACH,		PART_UPPER	},

	{"hit_l_upper_leg_r",	"leg",		"leg",		HIT_UPPER_LEG_R,	PART_LOWER	},
	{"hit_l_lower_leg_r",	"leg",		"leg",		HIT_LOWER_LEG_R,	PART_LOWER	},
	{"hit_l_foot_r",		"foot",		"foot",		HIT_FOOT_R,			PART_LOWER	},
	{"hit_l_upper_leg_l",	"leg",		"leg",		HIT_UPPER_LEG_L,	PART_LOWER	},
	{"hit_l_lower_leg_l",	"leg",		"leg",		HIT_LOWER_LEG_L,	PART_LOWER	},
	{"hit_l_foot_l",		"foot",		"foot",		HIT_FOOT_L,			PART_LOWER	},
	{"hit_l_pelvis",		"groin",	"butt",		HIT_PELVIS,			PART_LOWER	}
};
