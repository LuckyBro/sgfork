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
// g_combat.c

#include "g_local.h"


/*
============
ScorePlum
============
*/
void ScorePlum( gentity_t *ent, vec3_t origin, int score ) {
	gentity_t *plum;

	plum = G_TempEntity( origin, EV_SCOREPLUM );
	// only send this temp entity to a single client
	plum->r.svFlags |= SVF_SINGLECLIENT;
	plum->r.singleClient = ent->s.number;
	//
	plum->s.otherEntityNum = ent->s.number;
	plum->s.time = score;
}

/*
============
AddScore

Adds score to both the client and his team
============
*/
void AddScore( gentity_t *ent, vec3_t origin, int score ) {
	if ( !ent->client ) {
		return;
	}
	// no scoring during pre-match warmup
	if ( level.warmupTime ) {
		return;
	}
	// show score plum
	ScorePlum(ent, origin, score);
	//
	ent->client->ps.persistant[PERS_SCORE] += score;
	if ( g_gametype.integer == GT_TEAM )
		level.teamScores[ ent->client->ps.persistant[PERS_TEAM] ] += score;
	CalculateRanks();
}

void AddScoreRTPTeam( int team, int score ) {
	// no scoring during pre-match warmup
	if ( level.warmupTime) {
		return;
	}

	level.teamScores[ team] += score;
	CalculateRanks();
}

void AddScoreRTP( gentity_t *ent, int score ) {
	if ( !ent->client ) {
		return;
	}
	// no scoring during pre-match warmup
	if ( level.warmupTime) {
		return;
	}
//	ent->client->ps.persistant[PERS_SCORE] += score;
//	if (g_gametype.integer == GT_TEAM)
	level.teamScores[ ent->client->ps.persistant[PERS_TEAM] ] += score;
	CalculateRanks();
}

/*
=================
TossClientItems

Toss the weapon and powerups for the killed player
=================
*/
void TossClientItems( gentity_t *self ) {
	gitem_t		*item;
	float		angle;
	int			i;
	gentity_t	*drop;

	gentity_t	*sec_pistol_drop;

	// in duel, no items will be  dropped
	if(g_gametype.integer == GT_DUEL)
		return;

	angle = 22.5f;
	for(i = WP_KNIFE; i < WP_NUM_WEAPONS; i++){
		if(!(self->client->ps.stats[STAT_WEAPONS] & ( 1 << i)))
			continue;

		if( i == WP_GATLING && self->client->ps.stats[STAT_GATLING_MODE] &&
			!(self->client->ps.stats[STAT_FLAGS] & SF_GAT_CARRY))
			continue;

		// find the item type for this weapon
		item = BG_FindItemForWeapon( i );

		angle += 66.6f;

		// spawn the item
		drop = Drop_Item( self, item, angle );

		// Check if the player carries 2 pistols of the same kind
		// Pistols _should_ be between knife and winchester66 (see bg_public.h)
		if (i > WP_KNIFE && i < WP_WINCHESTER66) {
			if((self->client->ps.stats[STAT_FLAGS] & SF_SEC_PISTOL)) {
				angle += 66.6f;

				// spawn the item
				sec_pistol_drop = Drop_Item( self, item, angle );

				// Since the second pistol is dropped (the one on the left holster),
				// the correct ammo must be set in the dropped weapon
				if (self->client->ps.ammo[WP_AKIMBO] > bg_weaponlist[i].clipAmmo)
					sec_pistol_drop->count = bg_weaponlist[i].clipAmmo + 1;
				else
					sec_pistol_drop->count = self->client->ps.ammo[WP_AKIMBO] + 1;

				self->client->ps.ammo[WP_AKIMBO] = 0;
				self->client->ps.stats[STAT_FLAGS] &= ~SF_SEC_PISTOL;
			}
		}

		if(item->giType == IT_WEAPON) {
			if (item->giTag == WP_DYNAMITE || item->giTag == WP_KNIFE ||
			                                  item->giTag == WP_MOLOTOV) {
				drop->count = self->client->ps.ammo[i] + 1;
			}
			else {
				if(self->client->ps.ammo[i] > bg_weaponlist[item->giTag].clipAmmo)
					drop->count = bg_weaponlist[item->giTag].clipAmmo + 1;
				else
					drop->count = self->client->ps.ammo[i] + 1;
			}
		}

		// Player no longer possessed the given weapon.
		// This prevent the player to drop the same weapon, several times.
		self->client->ps.ammo[i] = 0;
		self->client->ps.stats[STAT_WEAPONS] &= ~( 1 << i );
	}

	// drop all the powerups if not in teamplay
		angle = 45;
		for ( i = 1 ; i < PW_NUM_POWERUPS ; i++ ) {
			if(i >= DM_HEAD_1 && i <= DM_LEGS_2)
				continue;

			// Don't drop the belt, nobody wants to pick up an empty belt
			if ( i == PW_BELT )
				continue;

			if ( self->client->ps.powerups[ i ] ) {

				if(i == PW_GOLD)
					item = BG_FindItemForClassname("item_money");
				else
					continue;
				if ( !item ) {
					continue;
				}
				drop = Drop_Item( self, item, angle );
				// decide how many seconds it has left
				drop->count = 1;
				angle += 45;

				// remove the powerup
				self->client->ps.powerups[i] = 0;
			}
		}
	// drop armor
	if( self->client->ps.stats[STAT_ARMOR]){
		item = BG_FindItem("Boiler Plate");

		drop = Drop_Item(self, item, rand()%360);

		drop->count = self->client->ps.stats[STAT_ARMOR];

		// Player no longer possessed the boiler plate.
		// This prevent the player to drop another boiler plate, several times.
		self->client->ps.stats[STAT_ARMOR] = 0;
	}
}

/*
==================
LookAtKiller
==================
*/
void LookAtKiller( gentity_t *self, gentity_t *attacker ) {
	vec3_t		dir;
	vec3_t		angles;

	if ( attacker && attacker != self ) {
		VectorSubtract (attacker->s.pos.trBase, self->s.pos.trBase, dir);
    } else {
		return; //if client doesnt have killer
	}
	//small hack. grapplePoint isnt used when player dead, so we use it to tell the client
	//where to turn. A better solution would be if we could add a new variable to playerstate
	//which is communicated between the client and the server.
	vectoangles(dir, angles);
	self->client->ps.grapplePoint[YAW]   = angles[YAW];
	self->client->ps.grapplePoint[PITCH] = angles[PITCH];
	self->client->ps.grapplePoint[ROLL]  = 0;
}

/*
==================
GibEntity
==================
*/
void GibEntity( gentity_t *self, int killer ) {
	gentity_t *ent;
	int i;

	//if this entity still has kamikaze
	if (self->s.eFlags & EF_KAMIKAZE) {
		// check if there is a kamikaze timer around for this owner
		for (i = 0; i < MAX_GENTITIES; i++) {
			ent = &g_entities[i];
			if (!ent->inuse)
				continue;
			if (ent->activator != self)
				continue;
			if (strcmp(ent->classname, "kamikaze timer"))
				continue;
			G_FreeEntity(ent);
			break;
		}
	}
	G_AddEvent( self, EV_GIB_PLAYER, killer );
	self->takedamage = qfalse;
	self->s.eType = ET_INVISIBLE;
	self->r.contents = 0;
}

// these are just for logging, the client prints its own messages
// Tequila comment: enum should match meansOfDeath_t in bg_public.h
char	*modNames[] = {
	"MOD_UNKNOWN",
	//melee
	"MOD_KNIFE",

	//pistols
	"MOD_REM58",
	"MOD_SCHOFIELD",
	"MOD_PEACEMAKER",

	//rifles
	"MOD_WINCHESTER66",
	"MOD_LIGHTNING",
	"MOD_SHARPS",

	//shotguns
	"MOD_REMINGTON_GAUGE",
	"MOD_SAWEDOFF",
	"MOD_WINCH97",

	//automatics
	"MOD_GATLING",

	//explosives
	"MOD_DYNAMITE",
	"MOD_MOLOTOV",

	"MOD_WATER",
	"MOD_SLIME",
	"MOD_LAVA",
	"MOD_CRUSH",
	"MOD_TELEFRAG",
	"MOD_FALLING",
	"MOD_SUICIDE",
	"MOD_WORLD_DAMAGE",
	"MOD_TRIGGER_HURT",
	"MOD_BOILER"
};

/*
==================
player_die
==================
*/
void player_die( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath ) {
	gentity_t	*ent;
	int			anim;
	int			contents;
	int			killer;
	int			i;
	char		*killerName, *obit;
	int fallanim = 0;

	if ( self->client->ps.pm_type == PM_DEAD ) {
		return;
	}

	if ( level.intermissiontime ) {
		return;
	}

//unlagged - backward reconciliation #2
	// make sure the body shows up in the client's current position
	G_UnTimeShiftClient( self );
//unlagged - backward reconciliation #2

	// check for an almost capture
	self->client->ps.pm_type = PM_DEAD;

	if ( attacker ) {
		killer = attacker->s.number;
		if ( attacker->client ) {
			killerName = attacker->client->pers.netname;
		} else {
			killerName = "<non-client>";
		}
	} else {
		killer = ENTITYNUM_WORLD;
		killerName = "<world>";
	}

	if ( killer < 0 || killer >= MAX_CLIENTS ) {
		killer = ENTITYNUM_WORLD;
		killerName = "<world>";
	}

	if ( meansOfDeath < 0 || meansOfDeath >= sizeof( modNames ) / sizeof( modNames[0] ) ) {
		obit = "<bad obituary>";
	} else {
		obit = modNames[meansOfDeath];
	}

	G_LogPrintf("Kill: %i %i %i: %s killed %s by %s\n",
		killer, self->s.number, meansOfDeath, killerName,
		self->client->pers.netname, obit );

	// broadcast the death event to everyone
	ent = G_TempEntity( self->r.currentOrigin, EV_OBITUARY );
	ent->s.eventParm = meansOfDeath;
	ent->s.otherEntityNum = self->s.number;
	ent->s.otherEntityNum2 = killer;
	ent->r.svFlags = SVF_BROADCAST;	// send to everyone

	self->enemy = attacker;

	self->client->ps.persistant[PERS_KILLED]++;

	if (attacker && attacker->client) {
		attacker->client->lastkilled_client = self->s.number;

		if ( attacker == self || OnSameTeam (self, attacker ) ) {
			AddScore( attacker, self->r.currentOrigin, -1 );
		} else {
			AddScore( attacker, self->r.currentOrigin, 1 );
		}
	} else {
		AddScore( self, self->r.currentOrigin, -1 );
	}

	// if client is in a nodrop area, don't drop anything (but return CTF flags!)
	contents = trap_PointContents( self->r.currentOrigin, -1 );
	if ( !( contents & CONTENTS_NODROP )) {
		TossClientItems( self );
	}

	Cmd_Score_f( self );		// show scores
	// send updated scores to any clients that are following this one,
	// or they would get stale scoreboards
	for ( i = 0 ; i < level.maxclients ; i++ ) {
		gclient_t	*client;

		client = &level.clients[i];
		if ( client->pers.connected != CON_CONNECTED ) {
			continue;
		}
		if ( client->sess.sessionTeam < TEAM_SPECTATOR ) {
			continue;
		}
		if ( client->sess.spectatorClient == self->s.number ) {
			Cmd_Score_f( g_entities + i );
		}
	}

	self->takedamage = qfalse;	// cannot be gibbed

	self->s.weapon = WP_NONE;
	self->s.powerups = 0;
	self->r.contents = 0;
	// reset events (important for fly)
	self->s.event = 0;
	self->client->ps.externalEvent = 0;

	//decide player is killed himself or not
	if ((attacker && attacker == self) || (inflictor && inflictor == self)
		|| (meansOfDeath >= MOD_WATER && meansOfDeath <= MOD_TRIGGER_HURT )) {

		self->client->ps.pm_flags|=PMF_SUICIDE;
	}

	self->s.loopSound = 0;

	self->r.maxs[2] = -8;

	self->client->respawnTime = level.time + 3000;

	// check if we can play a special falloff animation
	if(self->client->ps.groundEntityNum != ENTITYNUM_NONE){
		vec3_t axis[3];
		trace_t trace;
		vec3_t mins, maxs, pos;
		vec3_t mins2, maxs2;
		vec3_t angles;

		VectorClear(angles);
		angles[YAW] = self->client->ps.viewangles[YAW];
		AnglesToAxis(angles, axis);
		VectorSet(mins, -20, -20, -180);
		VectorSet(maxs, 20, 20, 30);
		VectorSet(maxs2, 20, 20, 20);
		VectorSet(mins2, -20, -20, -19);

		// calculate position
		VectorMA(self->client->ps.origin, FALLOFF_RANGE, axis[0], pos);

		// see if there is space behind the player to fall down
		trap_Trace(&trace, pos, mins, maxs, pos, self->client->ps.clientNum, MASK_SHOT);

		if(!trace.allsolid && !trace.startsolid){
			fallanim = BOTH_FALL;
		} else {
			// do another trace, this time try to fall off backwards

			// calculate position
			VectorMA(self->client->ps.origin, -FALLOFF_RANGE, axis[0], pos);

			// see if there is space behind the player to fall down
			trap_Trace(&trace, pos, mins, maxs, pos, self->client->ps.clientNum, MASK_SHOT);

			if(!trace.allsolid && !trace.startsolid){
				fallanim = BOTH_FALL_BACK;
			}
		}
		// at last check if we can get a direct line to the falling-down-position
		trap_Trace(&trace, self->client->ps.origin, mins2, maxs2, pos, -1, MASK_SHOT);

		if(trace.fraction != 1.0)
			fallanim = 0;
	}
	if(fallanim)
		self->client->lasthurt_anim = fallanim;

	anim = self->client->lasthurt_anim;

	// for the no-blood option, we need to prevent the health
	// from going to gib level
	if ( self->health <= GIB_HEALTH ) {
		self->health = GIB_HEALTH+1;
	}

	self->client->ps.legsAnim =
		( ( self->client->ps.legsAnim & ANIM_TOGGLEBIT ) ^ ANIM_TOGGLEBIT ) | anim;
	self->client->ps.torsoAnim =
		( ( self->client->ps.torsoAnim & ANIM_TOGGLEBIT ) ^ ANIM_TOGGLEBIT ) | anim;

	G_AddEvent( self, EV_DEATH_DEFAULT + anim/2, killer );

	// the body can't be gibbed anymore
	self->die = 0;

	trap_LinkEntity (self);

	if(self->s.number != ENTITYNUM_NONE && self->s.number != ENTITYNUM_WORLD &&
		killer != ENTITYNUM_NONE && killer != ENTITYNUM_WORLD){
		int rank, amount;
		float	f_amount;
		gentity_t *assaulter = &g_entities[killer];
		gentity_t *victim = &g_entities[self->s.number];

		if(self->client->ps.clientNum == assaulter->client->ps.clientNum)
			return;

		//the attacker gets the reward ;) by Spoon
		rank = 1;

		//if he didn't kill a mate ;)
		if(victim->client->sess.sessionTeam == assaulter->client->sess.sessionTeam && g_gametype.integer >= GT_TEAM) {

			assaulter->client->ps.stats[STAT_MONEY] -= 2*LOSE_MONEY;

			if(assaulter->client->ps.stats[STAT_MONEY] < 0){
				if ( g_moneyRespawn.integer == 1 ) {
					// Joe Kari: in the new money system, the teamkiller don't waste its money, 
					// what he loses is a gift to his mate
					victim->client->ps.stats[STAT_MONEY] += 2*LOSE_MONEY + assaulter->client->ps.stats[STAT_MONEY] ;
				}
				assaulter->client->ps.stats[STAT_MONEY] = 0;
			} else if ( g_moneyRespawn.integer == 1 ) {
				// Joe Kari: in the new money system, the teamkiller don't waste its money, 
				// what he loses is a gift to his mate
				victim->client->ps.stats[STAT_MONEY] += 2*LOSE_MONEY ;
			}
			return;
		}

		//calculate rank of the inflictor
		for(i=0; i<level.maxclients; i++){

			if(level.clients[i].ps.clientNum == victim->client->ps.clientNum)
				continue;

			if(level.clients[i].ps.persistant[PERS_SCORE] > victim->client->ps.persistant[PERS_SCORE]){
				rank ++;
			}
		}

		//give the attacker the money
		if(assaulter->client->ps.clientNum != victim->client->ps.clientNum){
			const int	players = TeamCount(-1, TEAM_FREE) + TeamCount(-1, TEAM_RED) +
				TeamCount(-1, TEAM_BLUE) + TeamCount(-1, TEAM_RED_SPECTATOR) +
				TeamCount(-1, TEAM_BLUE_SPECTATOR) +
				(g_gametype.integer == GT_DUEL ? TeamCount( -1, TEAM_SPECTATOR) : 0);
			const float	factor = (float)players/4;
			float	factor2 = factor -1;

			if(factor2<0) {
				factor2 = 1;
			} else {
				factor2 = factor2/5;
				factor2 += 1;
			}
			//G_Printf("amount: %i %f", rank, factor2);


			f_amount = sqrt(sqrt(1.0/rank)); // take (1/rank)exp 1/4
			f_amount = (float)(f_amount*f_amount*f_amount) * (float)(m_maxreward.integer * factor2); // here exponent is 3

			if(g_gametype.integer == GT_DUEL)
				f_amount *= 1.75f;

			// overall exponent is 3/4
			amount = (int)f_amount;

			assaulter->client->ps.stats[STAT_MONEY] += amount;

			if(assaulter->client->ps.stats[STAT_MONEY] > MAX_MONEY)
				assaulter->client->ps.stats[STAT_MONEY] = MAX_MONEY;
		}

		victim->client->deaths++;

		if(victim->client->deaths >= SOCIAL_HELP_COUNT){
			victim ->client->deaths = 0;
			victim->client->ps.stats[STAT_MONEY] += SOCIAL_MONEY;
		}

		if(victim->client->ps.stats[STAT_MONEY] > MAX_MONEY)
			victim->client->ps.stats[STAT_MONEY] = MAX_MONEY;

		if(0)
			G_Printf("Rank: %i, Amount: %i\n", rank, amount);
	}

	//G_Printf("attacker: %i inflictor: %i\n", attacker->client->ps.persistant[PERS_SCORE],
	//	inflictor->client->ps.persistant[PERS_SCORE]);
}

#define	L_PRINT	0

/*
============
G_LocationDamage
============
*/
float G_LocationDamage(vec3_t point, gentity_t* targ, gentity_t* attacker, float take, int *mod) {
	vec3_t bulletPath;
	vec3_t bulletAngle;

	int clientRotation;
	int bulletRotation;	// Degrees rotation around client.
				// used to check Back of head vs. Face
	int impactRotation;

	//safe location from hit-detection
	int location = targ->client->lasthurt_location;
	int direction = 0;

	// sharps rifle can shoot thru boiler plate
	qboolean sharps = (attacker->client->ps.weapon == WP_SHARPS);
	// knife doesnt make the dynamite explode
	qboolean isknife  = (attacker->client->ps.weapon == WP_KNIFE);
	// be sure, the target has dynamit in his hand
	qboolean isdyn    = (targ->client->ps.weapon == WP_DYNAMITE);

	vec3_t dynorigin;


	gentity_t *tent;

	// First things first.  If we're not damaging them, why are we here?
	if (!take)
		return 0.0f;

	// Get a vector aiming from the client to the bullet hit
	VectorSubtract(targ->r.currentOrigin, point, bulletPath);
	// Convert it into PITCH, ROLL, YAW
	vectoangles(bulletPath, bulletAngle);

	clientRotation = targ->client->ps.viewangles[YAW];
	bulletRotation = bulletAngle[YAW];

	impactRotation = abs(clientRotation-bulletRotation);

	impactRotation = impactRotation % 360; // Keep it in the 0-359 range
	impactRotation = AngleNormalize180(impactRotation);

	if (fabs(impactRotation) > 70)
		direction = LOCATION_FRONT;
	else
		direction = LOCATION_BACK;

	attacker->client->lasthurt_direction = direction;

	// Check the location ignoring the rotation info
	switch ( location )
	{
	case HIT_HEAD:
		take *= 5.0f;
		break;
	case HIT_NECK:
		take *= 2.8f;
		break;
	case HIT_SHOULDER_R:
	case HIT_SHOULDER_L:
		take *= 2.0f;
		break;
	case HIT_HAND_R:
		//blow up the dynamite, the gun doesnt cause damage
		if (isdyn && !isknife){
			VectorCopy(point, dynorigin);
			G_InstantExplode(dynorigin, attacker);
		    return 0.0f;
		} else {
		  //don't blow up the dynamite, normal gun damage
		  take *= 1.0f;
		}
		break;
	case HIT_HAND_L:
	case HIT_LOWER_ARM_R:
	case HIT_LOWER_ARM_L:
	case HIT_UPPER_ARM_L:
	case HIT_UPPER_ARM_R:
		take *= 1.0f;
		break;
	case HIT_CHEST:
		if(targ->client->ps.stats[STAT_ARMOR] && !sharps){
			targ->client->ps.stats[STAT_ARMOR] -= take*4;

			if(targ->client->ps.stats[STAT_ARMOR] < 0)
				targ->client->ps.stats[STAT_ARMOR] = 0;

			// Tequila comment: Log protection got from boiler plate
			if ( g_debugDamage.integer ) {
				G_Printf( "%i: client:%i health:%i damage:%.1f where:chest_boiler_plate(%i) from:%s by:%i\n", level.time,
					targ->s.number,	targ->health, take, targ->client->ps.stats[STAT_ARMOR],
					modNames[*mod], attacker->s.number );
			}

			// Tequila comment: reset take after it could has been logged
			take = 0;

			*mod = MOD_BOILER;
		} else
			take *= 2.6f; // Belly or back
		break;
	case HIT_STOMACH:
		if(targ->client->ps.stats[STAT_ARMOR] && !sharps){
			targ->client->ps.stats[STAT_ARMOR] -= take*4;

			if(targ->client->ps.stats[STAT_ARMOR] < 0)
				targ->client->ps.stats[STAT_ARMOR] = 0;

			// Tequila comment: Log protection got from boiler plate
			if ( g_debugDamage.integer ) {
				G_Printf( "%i: client:%i health:%i damage:%.1f where:stomack_boiler_plate(%i) from:%s by:%i\n", level.time,
					targ->s.number,	targ->health, take, targ->client->ps.stats[STAT_ARMOR],
					modNames[*mod], attacker->s.number );
			}

			// Tequila comment: reset take after it could has been logged
			take = 0;

			*mod = MOD_BOILER;
		} else
			take *= 2.0f;
		break;
	case HIT_PELVIS:
		take *= 2.0f; // Groin shot
		break;
	case HIT_LOWER_LEG_L:
	case HIT_LOWER_LEG_R:
	case HIT_UPPER_LEG_L:
	case HIT_UPPER_LEG_R:
		take *= 1.8f;
		if(targ->client->ps.speed == g_speed.integer){
			targ->client->ps.speed *= 0.75f;
			//trap_SendServerCommand( targ-g_entities, va("print \"You were hit in the leg!\n\""));
		}
		break;
	case HIT_FOOT_L:
	case HIT_FOOT_R:
		take *= 1.2f;
		if(targ->client->ps.speed == g_speed.integer) {
			targ->client->ps.speed *= 0.75f;
			//trap_SendServerCommand( targ-g_entities, va("print \"You were hit in the leg!\n\""));
		}
		break;
	}

	// death animations
	switch ( location ){
	case HIT_HEAD:
		targ->client->lasthurt_anim = BOTH_DEATH_HEAD;
		break;
	case HIT_CHEST:
		if(direction != LOCATION_BACK)
			targ->client->lasthurt_anim = BOTH_DEATH_CHEST;
		else
			targ->client->lasthurt_anim = BOTH_DEATH_DEFAULT;
		break;
	case HIT_STOMACH:
		if(direction != LOCATION_BACK)
			targ->client->lasthurt_anim = BOTH_DEATH_STOMACH;
		else
			targ->client->lasthurt_anim = BOTH_DEATH_DEFAULT;
		break;
	case HIT_UPPER_ARM_L:
	case HIT_LOWER_ARM_L:
	case HIT_HAND_L:
		targ->client->lasthurt_anim = BOTH_DEATH_ARM_L;
		break;
	case HIT_UPPER_ARM_R:
	case HIT_LOWER_ARM_R:
	case HIT_HAND_R:
		targ->client->lasthurt_anim = BOTH_DEATH_ARM_R;
		break;
	case HIT_UPPER_LEG_L:
	case HIT_LOWER_LEG_L:
	case HIT_FOOT_L:
		targ->client->lasthurt_anim = BOTH_DEATH_LEG_L;
		break;
	case HIT_UPPER_LEG_R:
	case HIT_LOWER_LEG_R:
	case HIT_FOOT_R:
		targ->client->lasthurt_anim = BOTH_DEATH_LEG_R;
		break;
	default:
		targ->client->lasthurt_anim = BOTH_DEATH_DEFAULT;
		break;
	}

	//damage skins
	switch ( hit_info[location].hit_part ){
	case PART_HEAD:
		targ->client->ps.powerups[DM_HEAD_1]++;
		break;
	case PART_UPPER:
		targ->client->ps.powerups[DM_TORSO_1]++;
		break;
	default:
		targ->client->ps.powerups[DM_LEGS_1]++;
		break;
	}

	//reduce damage
	take *= 0.8f;

	// print a message
	// broadcast the death event to everyone
	if(!(attacker->s.eFlags & EF_HIT_MESSAGE)){
		tent = G_TempEntity( vec3_origin, EV_HIT_MESSAGE );

		if(direction == LOCATION_BACK)
			tent->s.weapon = 0;
		else
			tent->s.weapon = 1;

		tent->s.frame = location;
		tent->s.otherEntityNum = targ->s.number;
		tent->s.otherEntityNum2 = attacker->s.number;
		tent->r.svFlags |= SVF_BROADCAST;
		tent->s.angles2[0] = -1;
		tent->s.angles2[1] = -1;
		tent->s.angles2[2] = -1;
	}

	return take;
}

/*
============
CheckTeamKills
By Tequila, TeamKill management inspired by Conq patch
Return false when TK supported limit is not hit
============
*/
static qboolean CheckTeamKills( gentity_t *attacker, gentity_t *targ, int mod ) {
	if ( !g_maxteamkills.integer )
		return qfalse;

	// g_teamkillschecktime can be set to only activate the check at the round beginning
	if ( g_teamkillschecktime.integer && g_roundstarttime && level.time > g_roundstarttime + g_teamkillschecktime.integer * 1000 )
		return qfalse;

	// Some weapons should not have to be checked for TK
	switch(mod){
	case MOD_GATLING:
	case MOD_DYNAMITE:
	case MOD_MOLOTOV:
	case MOD_TELEFRAG:
		return qfalse;
	default:
		break;
	}

	if ( !attacker || !targ )
		return qfalse;

	if ( !attacker->client )
		return qfalse;

	if ( attacker == targ || !OnSameTeam (targ,attacker) )
		return qfalse;

	// Check if we should reset TK count before
	if ( attacker->client->pers.lastTeamKillTime && level.time > attacker->client->pers.lastTeamKillTime + g_teamkillsforgettime.integer * 1000 ) {
#ifndef NDEBUG
		G_Printf(S_COLOR_MAGENTA "CheckTeamKills: Forget TeamKillsCount (%i) from %s\n", attacker->client->pers.TeamKillsCount, attacker->client->pers.netname);
#endif
		attacker->client->pers.TeamKillsCount=0;
	}

	attacker->client->pers.TeamKillsCount ++;
#ifndef NDEBUG
	if (attacker->client->pers.lastTeamKillTime)
		G_Printf(S_COLOR_MAGENTA "CheckTeamKills: Last TeamKills for %s was %.2f seconds ago\n", attacker->client->pers.netname,(float)(level.time-attacker->client->pers.lastTeamKillTime)/1000.0f);
#endif
	attacker->client->pers.lastTeamKillTime = level.time;

	// Kick the attacker if TK limit is hit and return true
	if ( attacker->client->pers.TeamKillsCount >= g_maxteamkills.integer )
	{
		trap_DropClient( attacker-g_entities, "Reached the limit of supported teammate kills." );
		return qtrue;
	} else if ( attacker->client->pers.TeamKillsCount == g_maxteamkills.integer -1 )
		trap_SendServerCommand( attacker-g_entities,
			va("print \"%s" S_COLOR_YELLOW "... Be careful teammate !!! Next teammate kill could kick you from that server.\n\"",
			attacker->client->pers.netname));
#ifndef NDEBUG
	G_Printf(S_COLOR_MAGENTA "CheckTeamKills: TeamKillsCount is %i for %s\n", attacker->client->pers.TeamKillsCount, attacker->client->pers.netname);
#endif
	return qfalse;
}

/*
============
G_Damage

targ		entity that is being damaged
inflictor	entity that is causing the damage
attacker	entity that caused the inflictor to damage targ
	example: targ=monster, inflictor=rocket, attacker=player

dir			direction of the attack for knockback
point		point at which the damage is being inflicted, used for headshots
damage		amount of damage being inflicted
knockback	force to be applied against targ as a result of the damage

inflictor, attacker, dir, and point can be NULL for environmental effects

dflags		these flags are used to control how T_Damage works
	DAMAGE_RADIUS			damage was indirect (from a nearby explosion)
	DAMAGE_NO_ARMOR			armor does not protect from this damage
	DAMAGE_NO_KNOCKBACK		do not affect velocity, just view angles
	DAMAGE_NO_PROTECTION	kills godmode, armor, everything
============
*/

void G_Damage( gentity_t *targ, gentity_t *inflictor, gentity_t *attacker,
			   vec3_t dir, vec3_t point, float damage, int dflags, int mod) {
	gclient_t	*client;
	float		take;
	int			hsave=0;
	int			knockback;
	int			max;

	if (!targ->takedamage) {
		return;
	}

	// the intermission has allready been qualified for, so don't
	// allow any extra scoring
	if ( level.intermissionQueued ) {
		return;
	}
	if ( !inflictor ) {
		inflictor = &g_entities[ENTITYNUM_WORLD];
	}
	if ( !attacker ) {
		attacker = &g_entities[ENTITYNUM_WORLD];
	}

	// shootable doors / buttons don't actually have any health
	if ( targ->s.eType == ET_MOVER ) {
		if ( targ->use && targ->moverState == MOVER_POS1 ) {
			targ->use( targ, inflictor, attacker );
		}
		return;
	}
	//if the breakable only accepts damage of specific weapon(s)
	if( targ->s.eType == ET_BREAKABLE) {
		if(targ->s.weapon && mod != targ->s.weapon)
			return;
	}
	// reduce damage by the attacker's handicap value
	// unless they are rocket jumping
	if ( attacker->client && attacker != targ ) {
		max = attacker->client->ps.stats[STAT_MAX_HEALTH];
		damage = damage * max / 100;
	}

	client = targ->client;

	if ( client ) {
		if ( client->noclip ) {
			return;
		}
	}

	if ( !dir ) {
		dflags |= DAMAGE_NO_KNOCKBACK;
	} else {
		VectorNormalize(dir);
	}

	knockback = damage/1.5;
	if(attacker->client){
		if(attacker->client->ps.weapon == WP_SAWEDOFF ||
			attacker->client->ps.weapon == WP_REMINGTON_GAUGE ||
			attacker->client->ps.weapon == WP_WINCH97)
			knockback *= 6;
	}
	//allow full knockback if it's a dynamite
	if ( knockback > 200 && mod != MOD_DYNAMITE) {
		knockback = 200;
	}
	if ( targ->flags & FL_NO_KNOCKBACK ) {
		knockback = 0;
	}
	if ( dflags & DAMAGE_NO_KNOCKBACK ) {
		knockback = 0;
	}

	// check for completely getting out of the damage
	if ( !(dflags & DAMAGE_NO_PROTECTION) ) {

		// if TF_NO_FRIENDLY_FIRE is set, don't do damage to the target
		// if the attacker was on the same team
		if ( targ != attacker && OnSameTeam (targ, attacker)  ) {
			if ( !g_friendlyFire.integer ) {
				return;
			}
			// Can be used later in case of TK limit reached to protect the last victim
			hsave = targ->health;
		}

		// check for godmode
		if ( targ->flags & FL_GODMODE ) {
			return;
		}
	}

	if ( damage < 1 ) {
		damage = 1;
	}
	take = damage;

	// add to the damage inflicted on a player this frame
	// the total will be turned into screen blends and view angle kicks
	// at the end of the frame
	if ( client && inflictor) {
		if ( attacker ) {
			client->ps.persistant[PERS_ATTACKER] = attacker->s.number;
		} else {
			client->ps.persistant[PERS_ATTACKER] = ENTITYNUM_WORLD;
		}
		client->damage_blood += take;
		client->damage_knockback += knockback;
		if ( dir ) {
			VectorCopy ( dir, client->damage_from );
			client->damage_fromWorld = qfalse;
		} else {
			VectorCopy ( targ->r.currentOrigin, client->damage_from );
			client->damage_fromWorld = qtrue;
		}
	}

	if (targ->client && inflictor->client && attacker->client) {
		// Modify the damage for location damage
		if (point && targ && targ->health > 0 && attacker && take){
			if(targ->client){
				if(targ->client->lasthurt_part == PART_LOWER){
					targ->client->ps.stats[STAT_KNOCKTIME] = -1;
					VectorClear(targ->client->ps.velocity);
				} else {
					targ->client->ps.stats[STAT_KNOCKTIME] = 1;
					VectorScale(targ->client->ps.velocity, 0.6, targ->client->ps.velocity);
				}
			}

			// only modify for bullet weapons
			if(attacker->client->ps.weapon != WP_DYNAMITE &&
				attacker->client->ps.weapon != WP_MOLOTOV &&
				attacker->client->ps.weapon != WP_KNIFE){
				take = G_LocationDamage(point, targ, attacker, take, &mod);
			}
		} else
			targ->client->lasthurt_part = PART_NONE;

		// set the last client who damaged the target
		targ->client->lasthurt_client = attacker->s.number;
		targ->client->lasthurt_mod = mod;

		attacker->client->lasthurt_victim = targ->s.number;
	}

	// figure momentum add, even if the damage won't be taken
	if ( knockback && targ->client ) {
		float knockbackvalue = g_knockback.value;
		vec3_t	kvel;
		float	mass;

		mass = 200;

		switch(mod){
		case MOD_REMINGTON_GAUGE:
		case MOD_SAWEDOFF:
		case MOD_WINCH97:
			if(targ->health-take <= 0)
				knockbackvalue = WINCH97_1_KSCALE;
			else
				knockbackvalue = WINCH97_2_KSCALE;
			break;
		case MOD_DYNAMITE:
			knockbackvalue = DYNA_KSCALE;
			break;
		}

		VectorScale (dir, knockbackvalue * (float)knockback / mass, kvel);
		VectorAdd (targ->client->ps.velocity, kvel, targ->client->ps.velocity);

		// set the timer so that the other client can't cancel
		// out the movement immediately
		if ( !targ->client->ps.pm_time ) {
			int		t;

			t = knockback * 2;
			if ( t < 50 ) {
				t = 50;
			}
			if ( t > 200 ) {
				t = 200;
			}
			targ->client->ps.pm_time = t;
			targ->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
		}
	}

	// do the damage
	if (take) {
		// Tequila comment: only report debug damage when it is taken
		if ( g_debugDamage.integer ) {
			if (client) {
				G_LogPrintf( "%i: client:%i health:%i damage:%.1f where:%s from:%s by:%i\n", level.time,
					targ->s.number,	targ->health, take, client->ps.weapon ?
						hit_info[client->lasthurt_location].forename
						: hit_info[client->lasthurt_location].backname,
					modNames[mod], attacker->s.number );
			} else {
				G_LogPrintf( "%i: world:%i health:%i damage:%.1f from:%s by:%i\n", level.time,
					targ->s.number, targ->health, take, modNames[mod] , attacker->s.number );
			}
		}

		// Tequila fix: take is a float in SG so round it with a cast to got a real damage
		targ->health = targ->health - (int)(take+.5);
		if ( targ->client ) {
			targ->client->ps.stats[STAT_HEALTH] = targ->health;
		}

		if ( targ->health <= 0 ) {
			if ( client )
				targ->flags |= FL_NO_KNOCKBACK;

			if (targ->health < 0)
				targ->health = 0;

			targ->enemy = attacker;

			// Target is not killed if TK limit is hit
			if ( g_friendlyFire.integer && CheckTeamKills(attacker,targ,mod) ) {
				targ->client->ps.stats[STAT_HEALTH] = targ->health = hsave;
				if ( g_debugDamage.integer )
					G_LogPrintf( "%i: client:%i health:%i saved from TK by:%i\n", level.time,
							targ->s.number,	targ->health, attacker->s.number );
				return;
			}

			// do the hit-message before the death-message
			if(attacker->s.angles2[0] != -1 &&
				(attacker->s.eFlags & EF_HIT_MESSAGE)){
				gentity_t *tent;

				// send the hit message
				tent = G_TempEntity( attacker->r.currentOrigin, EV_HIT_MESSAGE );

				if(attacker->client->lasthurt_direction == LOCATION_BACK)
					tent->s.weapon = 0;
				else
					tent->s.weapon = 1;

				tent->s.frame = -1;
				tent->s.otherEntityNum = attacker->client->lasthurt_victim;
				tent->s.otherEntityNum2 = attacker->s.number;
				tent->r.svFlags = SVF_BROADCAST;
				tent->s.angles2[0] = attacker->s.angles2[0];
				tent->s.angles2[1] = attacker->s.angles2[1];
				tent->s.angles2[2] = attacker->s.angles2[2];

				attacker->s.eFlags &= ~EF_HIT_MESSAGE;
				attacker->s.angles2[0] =  attacker->s.angles2[1] = attacker->s.angles2[2] = -1;
			}
			// Tequila comment: ->die can be NULL
			if (targ->die)
			targ->die (targ, inflictor, attacker, take, mod);
			return;
		} else if ( targ->pain ) {
			targ->pain (targ, attacker, take);
		}
	}

}

#define SMALL 2
void G_LookForBreakableType(gentity_t *ent){
	vec3_t	origin, origin2;
	vec3_t	mins, maxs;
	int		j, shaderNum;
	trace_t tr;

	if( ent->flags & FL_BREAKABLE_INIT)
		return;

	VectorAdd (ent->r.absmin, ent->r.absmax, origin);
	VectorScale (origin, 0.5f, origin);
	VectorSubtract(ent->r.absmin, origin, mins);
	VectorSubtract(ent->r.absmax, origin, maxs);

	ent->r.contents |= CONTENTS_JUMPPAD;
	trap_LinkEntity(ent);

	// from the mins
	for(j=0;j<6;j++){
		VectorCopy(origin, origin2);

		if(j < 3){
			origin2[j] += mins[j];
			origin2[j] += -SMALL;
		} else {
			origin2[j-3] += maxs[j-3];
			origin2[j-3] += SMALL;
		}

		shaderNum = trap_Trace_New2 ( &tr, origin2, NULL, NULL, origin, -1, MASK_SHOT|CONTENTS_JUMPPAD);

		if(tr.entityNum == ent->s.number)
			break;

		// test has failed
		shaderNum  = -1;
	}

	ent->r.contents &= ~CONTENTS_JUMPPAD;
	trap_LinkEntity(ent);

	if(shaderNum != -1)
		ent->flags |= FL_BREAKABLE_INIT;

	G_BreakablePrepare(ent, shaderNum);
}


/*
============
CanDamage

Returns qtrue if the inflictor can directly damage the target.  Used for
explosions and melee attacks.
============
*/
qboolean CanDamage (gentity_t *targ, vec3_t origin) {
	vec3_t	dest;
	trace_t	tr;
	vec3_t	midpoint;
	int shaderNum;

	// use the midpoint of the bounds instead of the origin, because
	// bmodels may have their origin is 0,0,0
	VectorAdd (targ->r.absmin, targ->r.absmax, midpoint);
	VectorScale (midpoint, 0.5, midpoint);

	VectorCopy (midpoint, dest);
	//prepare breakable by Spoon
	if(targ->s.eType == ET_BREAKABLE){

		G_LookForBreakableType(targ);

		// try a direct trace
		if(targ->count == -1){
			//ignore other breakables, so temporally change contents-type
			targ->r.contents = CONTENTS_JUMPPAD;
			trap_LinkEntity(targ);
			shaderNum = trap_Trace_New2 ( &tr, origin, NULL, NULL, dest, targ->s.clientNum, CONTENTS_SOLID|CONTENTS_JUMPPAD);
			targ->r.contents = (CONTENTS_MOVER|CONTENTS_BODY);
			trap_LinkEntity(targ);

			G_BreakablePrepare(targ, shaderNum);
		}
	}

	trap_Trace_New ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0 || tr.entityNum == targ->s.number){
		return qtrue;
	}

	// this should probably check in the plane of projection,
	// rather than in world coordinate, and also include Z
	VectorCopy (midpoint, dest);
	dest[0] += 15.0;
	dest[1] += 15.0;
	trap_Trace_New ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0)
		return qtrue;

	VectorCopy (midpoint, dest);
	dest[0] += 15.0;
	dest[1] -= 15.0;
	trap_Trace_New ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0)
		return qtrue;

	VectorCopy (midpoint, dest);
	dest[0] -= 15.0;
	dest[1] += 15.0;
	trap_Trace_New ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0)
		return qtrue;

	VectorCopy (midpoint, dest);
	dest[0] -= 15.0;
	dest[1] -= 15.0;
	trap_Trace_New ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0)
		return qtrue;


	return qfalse;
}


/*
============
G_RadiusDamage
============
*/
qboolean G_RadiusDamage ( vec3_t origin, gentity_t *attacker, float damage, float radius,
					 gentity_t *ignore, int mod) {
	float		points, dist;
	gentity_t	*ent;
	int			entityList[MAX_GENTITIES];
	int			numListedEntities;
	vec3_t		mins, maxs;
	vec3_t		v;
	vec3_t		dir;
	int			i, e;
	qboolean	hitClient = qfalse;

	if ( radius < 1 ) {
		radius = 1;
	}

	for ( i = 0 ; i < 3 ; i++ ) {
		mins[i] = origin[i] - radius;
		maxs[i] = origin[i] + radius;
	}

	numListedEntities = trap_EntitiesInBox( mins, maxs, entityList, MAX_GENTITIES );

	for ( e = 0 ; e < numListedEntities ; e++ ) {
		ent = &g_entities[entityList[ e ]];

		if (ent == ignore)
			continue;
		if (!ent->takedamage)
			continue;

		// find the distance from the edge of the bounding box
		for ( i = 0 ; i < 3 ; i++ ) {
			if ( origin[i] < ent->r.absmin[i] ) {
				v[i] = ent->r.absmin[i] - origin[i];
			} else if ( origin[i] > ent->r.absmax[i] ) {
				v[i] = origin[i] - ent->r.absmax[i];
			} else {
				v[i] = 0;
			}
		}

		dist = VectorLength( v );
		if ( dist >= radius ) {
			continue;
		}

		points = damage * ( 1.0 - dist / radius );

		if( CanDamage (ent, origin) ) {
			if( LogAccuracyHit( ent, attacker ) ) {
				hitClient = qtrue;
			}
			VectorSubtract (ent->r.currentOrigin, origin, dir);
			// push the center of mass higher than the origin so players
			// get knocked into the air more
			dir[2] += 24;
			G_Damage (ent, NULL, attacker, dir, origin, (int)points, DAMAGE_RADIUS, mod);
		}
	}

	return hitClient;
}
