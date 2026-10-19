#ifndef _DECLARATIONS_HPP_
#define _DECLARATIONS_HPP_

#include <stdint.h>

#define COD2_1_0 210
#define COD2_1_2 212
#define COD2_1_3 213

// Default value
#ifndef COD_VERSION
#define COD_VERSION COD2_1_3
#endif

#define qboolean int
#define qtrue 1
#define qfalse 0

#define DotProduct(a,b)         ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VectorSubtract(a,b,c)   ((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])
#define VectorAdd(a,b,c)        ((c)[0]=(a)[0]+(b)[0],(c)[1]=(a)[1]+(b)[1],(c)[2]=(a)[2]+(b)[2])
#define VectorCopy(a,b)         ((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])

#define	VectorScale(v, s, o)    ((o)[0]=(v)[0]*(s),(o)[1]=(v)[1]*(s),(o)[2]=(v)[2]*(s))
#define VectorMA(v, s, b, o)    ((o)[0]=(v)[0]+(b)[0]*(s),(o)[1]=(v)[1]+(b)[1]*(s),(o)[2]=(v)[2]+(b)[2]*(s))
#define CrossProduct(a,b,c)     ((c)[0]=(a)[1]*(b)[2]-(a)[2]*(b)[1],(c)[1]=(a)[2]*(b)[0]-(a)[0]*(b)[2],(c)[2]=(a)[0]*(b)[1]-(a)[1]*(b)[0])

#define DotProduct4( x,y )        ( ( x )[0] * ( y )[0] + ( x )[1] * ( y )[1] + ( x )[2] * ( y )[2] + ( x )[3] * ( y )[3] )
#define VectorSubtract4( a,b,c )  ( ( c )[0] = ( a )[0] - ( b )[0],( c )[1] = ( a )[1] - ( b )[1],( c )[2] = ( a )[2] - ( b )[2],( c )[3] = ( a )[3] - ( b )[3] )
#define VectorAdd4( a,b,c )       ( ( c )[0] = ( a )[0] + ( b )[0],( c )[1] = ( a )[1] + ( b )[1],( c )[2] = ( a )[2] + ( b )[2],( c )[3] = ( a )[3] + ( b )[3] )
#define VectorCopy4( a,b )        ( ( b )[0] = ( a )[0],( b )[1] = ( a )[1],( b )[2] = ( a )[2],( b )[3] = ( a )[3] )
#define VectorScale4( v, s, o )   ( ( o )[0] = ( v )[0] * ( s ),( o )[1] = ( v )[1] * ( s ),( o )[2] = ( v )[2] * ( s ),( o )[3] = ( v )[3] * ( s ) )
#define VectorMA4( v, s, b, o )   ( ( o )[0] = ( v )[0] + ( b )[0] * ( s ),( o )[1] = ( v )[1] + ( b )[1] * ( s ),( o )[2] = ( v )[2] + ( b )[2] * ( s ),( o )[3] = ( v )[3] + ( b )[3] * ( s ) )

#define VectorClear(a)		((a)[0]=(a)[1]=(a)[2]=0)
#define VectorNegate( a,b )       ( ( b )[0] = -( a )[0],( b )[1] = -( a )[1],( b )[2] = -( a )[2] )
#define VectorSet(v, x, y, z)	((v)[0]=(x), (v)[1]=(y), (v)[2]=(z))
#define Vector4Copy( a,b )        ( ( b )[0] = ( a )[0],( b )[1] = ( a )[1],( b )[2] = ( a )[2],( b )[3] = ( a )[3] )

#define SnapVector( v ) {v[0] = (int)v[0]; v[1] = (int)v[1]; v[2] = (int)v[2];}

#define	ANGLE2SHORT(x)	((int)((x)*65536.0f/360.0f) & 65535)
#define	SHORT2ANGLE(x)	((x)*(360.0/65536))

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

#define MAX_HUDELEMENTS 31
#define MAX_HUDELEMS_ARCHIVAL MAX_HUDELEMENTS
#define MAX_HUDELEMS_CURRENT MAX_HUDELEMENTS
#define HUDELEM_BITS 5
#define MAX_OBJECTIVES 16

#define MAX_CLIENTS 64
#define PACKET_BACKUP 32
#define MAX_QPATH 64
#define MAX_OSPATH 256
#define FRAMETIME 50
#define MAX_STRING_CHARS 1024
#define MAX_SERVERCMDS		64
#define SERVERCMD_MASK		(MAX_SERVERCMDS-1)

#ifndef MAXPRINTMSG
#define MAXPRINTMSG 4096
#endif

typedef unsigned short u_int16_t;
typedef unsigned char byte;

enum svc_ops_e
{
	svc_nop,
	svc_gamestate,
	svc_configstring,
	svc_baseline,
	svc_serverCommand,
	svc_download,
	svc_snapshot,
	svc_EOF
};

typedef enum
{
	ET_GENERAL = 0x0,
	ET_PLAYER = 0x1,
	ET_CORPSE = 0x2,
	ET_ITEM = 0x3,
	ET_MISSILE = 0x4,
	ET_INVISIBLE = 0x5,
	ET_SCRIPTMOVER = 0x6,
	ET_FX = 0x7,
	ET_LOOP_FX = 0x8,
	ET_TURRET = 0x9,
	ET_EVENTS = 0xA
} entityType_t;


enum clientState_e
{
	CS_FREE,
	CS_ZOMBIE,
	CS_CONNECTED,
	CS_PRIMED,
	CS_ACTIVE
};

typedef enum
{
	NA_BAD = 0,
	NA_LOOPBACK = 2,
	NA_BROADCAST = 3,
	NA_IP = 4,
	NA_IPX = 5,
	NA_BROADCAST_IPX = 6
} netadrtype_t;

typedef struct
{
	netadrtype_t type;
	byte ip[4];
	unsigned short port;
	byte ipx[10];
} netadr_t;

typedef enum
{
	NS_CLIENT,
	NS_SERVER
} netsrc_t;


typedef struct
{
	qboolean overflowed;
	byte *data;
	int maxsize;
	int cursize;
	int readcount;
	int bit;
} msg_t;

typedef struct NetField
{
	const char *name;
	int offset;
	int bits;
} netField_t;

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t vec5_t[5];


#define	CVAR_ARCHIVE		1
#define	CVAR_USERINFO		2
#define	CVAR_SERVERINFO		4
#define	CVAR_SYSTEMINFO		8
#define	CVAR_INIT			16
#define	CVAR_LATCH			32
#define	CVAR_ROM			64
#define CVAR_CHEAT			128
#define	CVAR_TEMP			256
#define CVAR_NORESTART		1024
#define	CVAR_USER_CREATED	16384

#define	CS_SERVERINFO		0
#define	CS_SYSTEMINFO		1

enum cs_index_t
{
	CS_AMBIENT = 3,
	CS_SCORES1 = 5,
	CS_SCORES2 = 6,
	CS_WEAPONS = 7,
	CS_ITEMS = 8,
	CS_FOGVARS = 12,
	CS_VOTE_TIME = 15,
	CS_VOTE_STRING = 16,
	CS_VOTE_YES = 17,
	CS_VOTE_NO = 18,
	CS_VOTE_MAPNAME = 19,
	CS_VOTE_GAMETYPE = 20,
	CS_MULTI_MAPWINNER = 22,
};


typedef int	fileHandle_t;
typedef void (*xfunction_t)();
typedef void (*xcommand_t) (void);


typedef enum
{
	EV_NONE = 0,
	EV_FOOTSTEP_RUN_DEFAULT,
	EV_FOOTSTEP_RUN_BARK,
	EV_FOOTSTEP_RUN_BRICK,
	EV_FOOTSTEP_RUN_CARPET,
	EV_FOOTSTEP_RUN_CLOTH,
	EV_FOOTSTEP_RUN_CONCRETE,
	EV_FOOTSTEP_RUN_DIRT,
	EV_FOOTSTEP_RUN_FLESH,
	EV_FOOTSTEP_RUN_FOLIAGE,
	EV_FOOTSTEP_RUN_GLASS,
	EV_FOOTSTEP_RUN_GRASS,
	EV_FOOTSTEP_RUN_GRAVEL,
	EV_FOOTSTEP_RUN_ICE,
	EV_FOOTSTEP_RUN_METAL,
	EV_FOOTSTEP_RUN_MUD,
	EV_FOOTSTEP_RUN_PAPER,
	EV_FOOTSTEP_RUN_PLASTER,
	EV_FOOTSTEP_RUN_ROCK,
	EV_FOOTSTEP_RUN_SAND,
	EV_FOOTSTEP_RUN_SNOW,
	EV_FOOTSTEP_RUN_WATER,
	EV_FOOTSTEP_RUN_WOOD,
	EV_FOOTSTEP_RUN_ASPHALT,
	EV_FOOTSTEP_WALK_DEFAULT,
	EV_FOOTSTEP_WALK_BARK,
	EV_FOOTSTEP_WALK_BRICK,
	EV_FOOTSTEP_WALK_CARPET,
	EV_FOOTSTEP_WALK_CLOTH,
	EV_FOOTSTEP_WALK_CONCRETE,
	EV_FOOTSTEP_WALK_DIRT,
	EV_FOOTSTEP_WALK_FLESH,
	EV_FOOTSTEP_WALK_FOLIAGE,
	EV_FOOTSTEP_WALK_GLASS,
	EV_FOOTSTEP_WALK_GRASS,
	EV_FOOTSTEP_WALK_GRAVEL,
	EV_FOOTSTEP_WALK_ICE,
	EV_FOOTSTEP_WALK_METAL,
	EV_FOOTSTEP_WALK_MUD,
	EV_FOOTSTEP_WALK_PAPER,
	EV_FOOTSTEP_WALK_PLASTER,
	EV_FOOTSTEP_WALK_ROCK,
	EV_FOOTSTEP_WALK_SAND,
	EV_FOOTSTEP_WALK_SNOW,
	EV_FOOTSTEP_WALK_WATER,
	EV_FOOTSTEP_WALK_WOOD,
	EV_FOOTSTEP_WALK_ASPHALT,
	EV_FOOTSTEP_PRONE_DEFAULT,
	EV_FOOTSTEP_PRONE_BARK,
	EV_FOOTSTEP_PRONE_BRICK,
	EV_FOOTSTEP_PRONE_CARPET,
	EV_FOOTSTEP_PRONE_CLOTH,
	EV_FOOTSTEP_PRONE_CONCRETE,
	EV_FOOTSTEP_PRONE_DIRT,
	EV_FOOTSTEP_PRONE_FLESH,
	EV_FOOTSTEP_PRONE_FOLIAGE,
	EV_FOOTSTEP_PRONE_GLASS,
	EV_FOOTSTEP_PRONE_GRASS,
	EV_FOOTSTEP_PRONE_GRAVEL,
	EV_FOOTSTEP_PRONE_ICE,
	EV_FOOTSTEP_PRONE_METAL,
	EV_FOOTSTEP_PRONE_MUD,
	EV_FOOTSTEP_PRONE_PAPER,
	EV_FOOTSTEP_PRONE_PLASTER,
	EV_FOOTSTEP_PRONE_ROCK,
	EV_FOOTSTEP_PRONE_SAND,
	EV_FOOTSTEP_PRONE_SNOW,
	EV_FOOTSTEP_PRONE_WATER,
	EV_FOOTSTEP_PRONE_WOOD,
	EV_FOOTSTEP_PRONE_ASPHALT,
	EV_JUMP_DEFAULT,
	EV_JUMP_BARK,
	EV_JUMP_BRICK,
	EV_JUMP_CARPET,
	EV_JUMP_CLOTH,
	EV_JUMP_CONCRETE,
	EV_JUMP_DIRT,
	EV_JUMP_FLESH,
	EV_JUMP_FOLIAGE,
	EV_JUMP_GLASS,
	EV_JUMP_GRASS,
	EV_JUMP_GRAVEL,
	EV_JUMP_ICE,
	EV_JUMP_METAL,
	EV_JUMP_MUD,
	EV_JUMP_PAPER,
	EV_JUMP_PLASTER,
	EV_JUMP_ROCK,
	EV_JUMP_SAND,
	EV_JUMP_SNOW,
	EV_JUMP_WATER,
	EV_JUMP_WOOD,
	EV_JUMP_ASPHALT,
	EV_LANDING_DEFAULT,
	EV_LANDING_BARK,
	EV_LANDING_BRICK,
	EV_LANDING_CARPET,
	EV_LANDING_CLOTH,
	EV_LANDING_CONCRETE,
	EV_LANDING_DIRT,
	EV_LANDING_FLESH,
	EV_LANDING_FOLIAGE,
	EV_LANDING_GLASS,
	EV_LANDING_GRASS,
	EV_LANDING_GRAVEL,
	EV_LANDING_ICE,
	EV_LANDING_METAL,
	EV_LANDING_MUD,
	EV_LANDING_PAPER,
	EV_LANDING_PLASTER,
	EV_LANDING_ROCK,
	EV_LANDING_SAND,
	EV_LANDING_SNOW,
	EV_LANDING_WATER,
	EV_LANDING_WOOD,
	EV_LANDING_ASPHALT,
	EV_LANDING_PAIN_DEFAULT,
	EV_LANDING_PAIN_BARK,
	EV_LANDING_PAIN_BRICK,
	EV_LANDING_PAIN_CARPET,
	EV_LANDING_PAIN_CLOTH,
	EV_LANDING_PAIN_CONCRETE,
	EV_LANDING_PAIN_DIRT,
	EV_LANDING_PAIN_FLESH,
	EV_LANDING_PAIN_FOLIAGE,
	EV_LANDING_PAIN_GLASS,
	EV_LANDING_PAIN_GRASS,
	EV_LANDING_PAIN_GRAVEL,
	EV_LANDING_PAIN_ICE,
	EV_LANDING_PAIN_METAL,
	EV_LANDING_PAIN_MUD,
	EV_LANDING_PAIN_PAPER,
	EV_LANDING_PAIN_PLASTER,
	EV_LANDING_PAIN_ROCK,
	EV_LANDING_PAIN_SAND,
	EV_LANDING_PAIN_SNOW,
	EV_LANDING_PAIN_WATER,
	EV_LANDING_PAIN_WOOD,
	EV_LANDING_PAIN_ASPHALT,
	EV_FOLIAGE_SOUND,
	EV_STANCE_FORCE_STAND,
	EV_STANCE_FORCE_CROUCH,
	EV_STANCE_FORCE_PRONE,
	EV_STEP_VIEW,
	EV_ITEM_PICKUP,
	EV_AMMO_PICKUP,
	EV_NOAMMO,
	EV_EMPTYCLIP,
	EV_EMPTY_OFFHAND,
	EV_RESET_ADS,
	EV_RELOAD,
	EV_RELOAD_FROM_EMPTY,
	EV_RELOAD_START,
	EV_RELOAD_END,
	EV_RAISE_WEAPON,
	EV_PUTAWAY_WEAPON,
	EV_WEAPON_ALT,
	EV_PULLBACK_WEAPON,
	EV_FIRE_WEAPON,
	EV_FIRE_WEAPONB,
	EV_FIRE_WEAPON_LASTSHOT,
	EV_RECHAMBER_WEAPON,
	EV_EJECT_BRASS,
	EV_MELEE_SWIPE,
	EV_FIRE_MELEE,
	EV_PREP_OFFHAND,
	EV_USE_OFFHAND,
	EV_SWITCH_OFFHAND,
	EV_BINOCULAR_ENTER,
	EV_BINOCULAR_EXIT,
	EV_BINOCULAR_FIRE,
	EV_BINOCULAR_RELEASE,
	EV_BINOCULAR_DROP,
	EV_MELEE_HIT,
	EV_MELEE_MISS,
	EV_FIRE_WEAPON_MG42,
	EV_FIRE_QUADBARREL_1,
	EV_FIRE_QUADBARREL_2,
	EV_BULLET_TRACER,
	EV_SOUND_ALIAS,
	EV_SOUND_ALIAS_AS_MASTER,
	EV_BULLET_HIT_SMALL,
	EV_BULLET_HIT_LARGE,
	EV_SHOTGUN_HIT,
	EV_BULLET_HIT_AP,
	EV_BULLET_HIT_CLIENT_SMALL,
	EV_BULLET_HIT_CLIENT_LARGE,
	EV_GRENADE_BOUNCE,
	EV_GRENADE_EXPLODE,
	EV_ROCKET_EXPLODE,
	EV_ROCKET_EXPLODE_NOMARKS,
	EV_CUSTOM_EXPLODE,
	EV_CUSTOM_EXPLODE_NOMARKS,
	EV_BULLET,
	EV_PLAY_FX,
	EV_PLAY_FX_ON_TAG,
	EV_EARTHQUAKE,
	EV_GRENADE_SUICIDE,
	EV_OBITUARY
} entity_event_t;

enum scriptAnimEventTypes_t
{
	ANIM_ET_PAIN = 0x0,
	ANIM_ET_DEATH = 0x1,
	ANIM_ET_FIREWEAPON = 0x2,
	ANIM_ET_JUMP = 0x3,
	ANIM_ET_JUMPBK = 0x4,
	ANIM_ET_LAND = 0x5,
	ANIM_ET_DROPWEAPON = 0x6,
	ANIM_ET_RAISEWEAPON = 0x7,
	ANIM_ET_CLIMB_MOUNT = 0x8,
	ANIM_ET_CLIMB_DISMOUNT = 0x9,
	ANIM_ET_RELOAD = 0xA,
	ANIM_ET_CROUCH_TO_PRONE = 0xB,
	ANIM_ET_PRONE_TO_CROUCH = 0xC,
	ANIM_ET_STAND_TO_CROUCH = 0xD,
	ANIM_ET_CROUCH_TO_STAND = 0xE,
	ANIM_ET_STAND_TO_PRONE = 0xF,
	ANIM_ET_PRONE_TO_STAND = 0x10,
	ANIM_ET_MELEEATTACK = 0x11,
	ANIM_ET_SHELLSHOCK = 0x12,
	NUM_ANIM_EVENTTYPES = 0x13,
};

enum StanceState
{
	CL_STANCE_STAND = 0x0,
	CL_STANCE_CROUCH = 0x1,
	CL_STANCE_PRONE = 0x2,
	CL_STANCE_DIVE_TO_PRONE = 0x3,
};


typedef struct leakyBucket_s leakyBucket_t;
struct leakyBucket_s
{
	netadrtype_t type;
	unsigned char adr[4];
	int	lastTime;
	signed char	burst;
	long hash;
	leakyBucket_t *prev, *next;
};

typedef struct usercmd_s
{
	int serverTime;
	int buttons;
	byte weapon;
	byte offHandIndex;
	int angles[3];
	char forwardmove;
	char rightmove;
} usercmd_t;

#if COD_VERSION == COD2_1_0 || COD_VERSION == COD2_1_2
#define MAX_MSGLEN 0x4000
#elif COD_VERSION == COD2_1_3
#define MAX_MSGLEN 0x20000
#endif

typedef void netProfileInfo_t;

typedef struct
{
	int			outgoingSequence;
	netsrc_t	sock;
	int			dropped;
	int			incomingSequence;
	netadr_t	remoteAddress;
	int 		qport;
	int			fragmentSequence;
	int			fragmentLength;
	byte		fragmentBuffer[MAX_MSGLEN];
	qboolean	unsentFragments;
	int			unsentFragmentStart;
	int			unsentLength;
	byte		unsentBuffer[MAX_MSGLEN];
	netProfileInfo_t *netProfile;
} netchan_t;


typedef enum
{
	TR_STATIONARY = 0,
	TR_INTERPOLATE = 1,
	TR_LINEAR = 2,
	TR_LINEAR_STOP = 3,
	TR_SINE = 4,
	TR_GRAVITY = 5,
	TR_GRAVITY_PAUSED = 6,
	TR_ACCELERATE = 7,
	TR_DECCELERATE = 8
} trType_t;

typedef struct
{
	trType_t	trType;
	int			trTime;
	int			trDuration;
	vec3_t		trBase;
	vec3_t		trDelta;
} trajectory_t;

typedef struct entityState_s
{
	int number;
	int eType;
	int eFlags;
	trajectory_t pos;
	trajectory_t apos;
	int time;
	int time2;
	vec3_t origin2;
	vec3_t angles2;
	int otherEntityNum;
	int attackerEntityNum;
	int groundEntityNum;
	int constantLight;
	int loopSound;
	int surfType;
	int index;
	int clientNum;
	int iHeadIcon;
	int iHeadIconTeam;
	int solid;
	int eventParm;
	int eventSequence;
	int events[4];
	int eventParms[4];
	int weapon;
	int legsAnim;
	int torsoAnim;
	float leanf;
	int scale;
	int dmgFlags;
	int animMovetype;
	float fTorsoHeight;
	float fTorsoPitch;
	float fWaistPitch;
} entityState_t;


enum statIndex_t
{
	STAT_HEALTH = 0x0,
	STAT_DEAD_YAW = 0x1,
	STAT_MAX_HEALTH = 0x2,
	STAT_IDENT_CLIENT_NUM = 0x3,
	STAT_IDENT_CLIENT_HEALTH = 0x4,
	STAT_SPAWN_COUNT = 0x5,
	MAX_STATS = 0x6,
};


typedef struct objective_s
{
	int state;
	vec3_t origin;
	int entNum;
	int teamNum;
	int icon;
} objective_t;

#define OBJF( x ) # x,(intptr_t)&( (objective_t*)0 )->x
netField_t objectiveFields[] =
{
	{ OBJF( origin[0] ), 0},
	{ OBJF( origin[1] ), 0},
	{ OBJF( origin[2] ), 0},
	{ OBJF( icon ), 12},
	{ OBJF( entNum ), 10},
	{ OBJF( teamNum ), 4},
};

typedef enum
{
	HE_TYPE_FREE = 0x0,
	HE_TYPE_TEXT = 0x1,
	HE_TYPE_VALUE = 0x2,
	HE_TYPE_PLAYERNAME = 0x3,
	HE_TYPE_MAPNAME = 0x4,
	HE_TYPE_GAMETYPE = 0x5,
	HE_TYPE_MATERIAL = 0x6,
	HE_TYPE_TIMER_DOWN = 0x7,
	HE_TYPE_TIMER_UP = 0x8,
	HE_TYPE_TENTHS_TIMER_DOWN = 0x9,
	HE_TYPE_TENTHS_TIMER_UP = 0xA,
	HE_TYPE_CLOCK_DOWN = 0xB,
	HE_TYPE_CLOCK_UP = 0xC,
	HE_TYPE_WAYPOINT = 0xD,
	HE_TYPE_COUNT = 0xE,
} he_type_t;

typedef struct
{
	char r;
	char g;
	char b;
	char a;
} hudelem_colorsplit_t;

typedef union
{
	hudelem_colorsplit_t split;
	int rgba;
} hudelem_color_t;

typedef struct hudelem_s
{
	int type;
	float x;
	float y;
	float z;
	float fontScale;
	int font;
	int alignOrg;
	int alignScreen;
	hudelem_color_t color;
	hudelem_color_t fromColor;
	int fadeStartTime;
	int fadeTime;
	int label;
	int width;
	int height;
	int materialIndex;
	int fromWidth;
	int fromHeight;
	int scaleStartTime;
	int scaleTime;
	float fromX;
	float fromY;
	int fromAlignOrg;
	int fromAlignScreen;
	int moveStartTime;
	int moveTime;
	int time;
	int duration;
	float value;
	int text;
	float sort;
	hudelem_color_t foreground;
} hudelem_t;


#define HEF( x ) # x,(intptr_t)&( (hudelem_t*)0 )->x
netField_t hudElemFields[] =
{
	{ HEF( y ), -99},
	{ HEF( type ), 4},
	{ HEF( color.rgba ), 32},
	{ HEF( x ), -99},
	{ HEF( alignScreen ), 6},
	{ HEF( fontScale ), 0},
	{ HEF( materialIndex ), 8},
	{ HEF( width ), 10},
	{ HEF( height ), 10},
	{ HEF( fadeStartTime ), 32},
	{ HEF( fromColor.rgba ), 32},
	{ HEF( fadeTime ), 16},
	{ HEF( value ), 0},
	{ HEF( time ), 32},
	{ HEF( z ), -99},
	{ HEF( alignOrg ), 4},
	{ HEF( sort ), 0},
	{ HEF( text ), 8},
	{ HEF( font ), 4},
	{ HEF( scaleStartTime ), 32},
	{ HEF( scaleTime ), 16},
	{ HEF( fromHeight ), 10},
	{ HEF( label ), 8},
	{ HEF( fromWidth ), 10},
	{ HEF( moveStartTime ), 32},
	{ HEF( moveTime ), 16},
	{ HEF( fromX ), -99},
	{ HEF( fromY ), -99},
	{ HEF( fromAlignScreen ), 6},
	{ HEF( fromAlignOrg ), 4},
	{ HEF( duration ), 32},
	{ HEF( foreground ), 1},
};

typedef struct hudElemState_s
{
	hudelem_t current[31];
	hudelem_t archival[31];
} hudElemState_t;

typedef struct
{
	float yaw;
	int timer;
	int transIndex;
	int flags;
} MantleState;

typedef struct playerState_s
{
	int commandTime;
	int pm_type;
	int bobCycle;
	int pm_flags;
	int pm_time;
	vec3_t origin;
	vec3_t velocity;
	vec2_t oldVelocity;
	int weaponTime;
	int weaponDelay;
	int grenadeTimeLeft;
	int weaponRestrictKickTime;
	int foliageSoundTime;
	int gravity;
	float leanf;
	int speed;
	int delta_angles[3];
	int groundEntityNum;
	vec3_t vLadderVec;
	int jumpTime;
	float jumpOriginZ;
	int legsTimer;
	int legsAnim;
	int torsoTimer;
	int torsoAnim;
	int legsAnimDuration;
	int torsoAnimDuration;
	int damageTimer;
	int damageDuration;
	int flinchYaw;
	int movementDir;
	int eFlags;
	int eventSequence;
	int events[4];
	int eventParms[4];
	int oldEventSequence;
	int clientNum;
	int offHandIndex;
	unsigned int weapon;
	int weaponstate;
	float fWeaponPosFrac;
	int adsDelayTime;
	int viewmodelIndex;
	vec3_t viewangles;
	int viewHeightTarget;
	float viewHeightCurrent;
	int viewHeightLerpTime;
	int viewHeightLerpTarget;
	int viewHeightLerpDown;
	float viewHeightLerpPosAdj;
	vec2_t viewAngleClampBase;
	vec2_t viewAngleClampRange;
	int damageEvent;
	int damageYaw;
	int damagePitch;
	int damageCount;
	int stats[MAX_STATS];
	int ammo[128];
	int ammoclip[128];
	int weapons[2];
	int oldweapons[2];
	char weaponslots[8];
	int weaponrechamber[2];
	int oldweaponrechamber[2];
	vec3_t mins;
	vec3_t maxs;
	float proneDirection;
	float proneDirectionPitch;
	float proneTorsoPitch;
	int viewlocked;
	int viewlocked_entNum;
	int cursorHint;
	int cursorHintString;
	int cursorHintEntIndex;
	int iCompassFriendInfo;
	float fTorsoHeight;
	float fTorsoPitch;
	float fWaistPitch;
	float holdBreathScale;
	int holdBreathTimer;
	MantleState mantleState;
	int entityEventSequence;
	int weapAnim;
	float aimSpreadScale;
	int shellshockIndex;
	int shellshockTime;
	int shellshockDuration;
	objective_t objective[MAX_OBJECTIVES];
	int deltaTime;
	hudElemState_t hud;
} playerState_t;

#define PSF( x ) # x,(intptr_t)&( (playerState_t*)0 )->x
netField_t playerStateFields[] =
{
	{ PSF( commandTime ), 32},
	{ PSF( origin[1] ), 0},
	{ PSF( origin[0] ), 0},
	{ PSF( bobCycle ), 8},
	{ PSF( viewangles[1] ), -100},
	{ PSF( origin[2] ), 0},
	{ PSF( velocity[1] ), 0},
	{ PSF( velocity[0] ), 0},
	{ PSF( viewangles[0] ), -100},
	{ PSF( movementDir ), -8},
	{ PSF( velocity[2] ), 0},
	{ PSF( eventSequence ), 8},
	{ PSF( legsAnim ), 10},
	{ PSF( aimSpreadScale ), 0},
	{ PSF( weaponTime ), -16},
	{ PSF( pm_flags ), 27},
	{ PSF( events[0] ), 8},
	{ PSF( events[1] ), 8},
	{ PSF( events[2] ), 8},
	{ PSF( events[3] ), 8},
	{ PSF( weapAnim ), 10},
	{ PSF( viewHeightCurrent ), 0},
	{ PSF( torsoTimer ), 16},
	{ PSF( torsoAnim ), 10},
	{ PSF( eFlags ), 24},
	{ PSF( fWeaponPosFrac ), 0},
	{ PSF( holdBreathScale ), 0},
	{ PSF( weaponstate ), 5},
	{ PSF( viewHeightTarget ), -8},
	{ PSF( weaponDelay ), -16},
	{ PSF( legsTimer ), 16},
	{ PSF( viewHeightLerpTarget ), -8},
	{ PSF( groundEntityNum ), 10},
	{ PSF( pm_time ), -16},
	{ PSF( eventParms[3] ), 8},
	{ PSF( eventParms[1] ), 8},
	{ PSF( eventParms[0] ), 8},
	{ PSF( eventParms[2] ), 8},
	{ PSF( weapon ), 7},
	{ PSF( weapons[0] ), 32},
	{ PSF( viewHeightLerpDown ), 1},
	{ PSF( weaponslots[0] ), 32},
	{ PSF( delta_angles[0] ), 16},
	{ PSF( delta_angles[1] ), 16},
	{ PSF( cursorHintString ), -8},
	{ PSF( offHandIndex ), 7},
	{ PSF( clientNum ), 8},
	{ PSF( viewlocked_entNum ), 16},
	{ PSF( viewmodelIndex ), 8},
	{ PSF( viewHeightLerpTime ), 32},
	{ PSF( speed ), 16},
	{ PSF( mins[1] ), 0},
	{ PSF( mins[0] ), 0},
	{ PSF( maxs[2] ), 0},
	{ PSF( maxs[1] ), 0},
	{ PSF( maxs[0] ), 0},
	{ PSF( gravity ), 16},
	{ PSF( damageTimer ), 16},
	{ PSF( cursorHint ), 8},
	{ PSF( mantleState.flags ), 4},
	{ PSF( flinchYaw ), 16},
	{ PSF( fWaistPitch ), 0},
	{ PSF( mantleState.timer ), 32},
	{ PSF( fTorsoPitch ), 0},
	{ PSF( proneTorsoPitch ), 0},
	{ PSF( holdBreathTimer ), 16},
	{ PSF( jumpTime ), 32},
	{ PSF( viewangles[2] ), -100},
	{ PSF( foliageSoundTime ), 32},
	{ PSF( weapons[1] ), 32},
	{ PSF( damageEvent ), 8},
	{ PSF( damageDuration ), 16},
	{ PSF( damageYaw ), 8},
	{ PSF( proneDirection ), 0},
	{ PSF( proneDirectionPitch ), 0},
	{ PSF( mantleState.yaw ), 0},
	{ PSF( mantleState.transIndex ), 4},
	{ PSF( fTorsoHeight ), 0},
	{ PSF( damagePitch ), 8},
	{ PSF( jumpOriginZ ), 0},
	{ PSF( pm_type ), 8},
	{ PSF( viewlocked ), 8},
	{ PSF( weaponrechamber[0] ), 32},
	{ PSF( vLadderVec[0] ), 0},
	{ PSF( weaponslots[4] ), 32},
	{ PSF( weaponRestrictKickTime ), -16},
	{ PSF( vLadderVec[1] ), 0},
	{ PSF( viewAngleClampRange[1] ), 0},
	{ PSF( viewAngleClampRange[0] ), 0},
	{ PSF( viewAngleClampBase[1] ), 0},
	{ PSF( weaponrechamber[1] ), 32},
	{ PSF( leanf ), 0},
	{ PSF( damageCount ), 7},
	{ PSF( grenadeTimeLeft ), -16},
	{ PSF( deltaTime ), 32},
	{ PSF( shellshockTime ), 32},
	{ PSF( shellshockIndex ), 4},
	{ PSF( shellshockDuration ), 16},
	{ PSF( vLadderVec[2] ), 0},
	{ PSF( delta_angles[2] ), 16},
	{ PSF( viewHeightLerpPosAdj ), 0},
	{ PSF( mins[2] ), 0},
	{ PSF( viewAngleClampBase[0] ), 0},
	{ PSF( adsDelayTime ), 32},
	{ PSF( iCompassFriendInfo ), 32},
};


typedef enum
{
	TEAM_NONE,
	TEAM_AXIS,
	TEAM_ALLIES,
	TEAM_SPECTATOR
} sessionTeam_t;


#define MAX_DOWNLOAD_BLKSIZE 1024
#define MAX_DOWNLOAD_WINDOW 8


#pragma pack(push)
#pragma pack(1)
#pragma pack(pop)


/*
*/


#define MAX_CONFIGSTRINGS   2048
#define	RESERVED_CONFIGSTRINGS	2
#define MAX_MODELS          256
#define GENTITYNUM_BITS     10
#define MAX_GENTITIES       ( 1 << GENTITYNUM_BITS )
#define MAX_ENT_CLUSTERS    16
#define MAX_BPS_WINDOW 		20

// eyza
#define GCLIENTNUM_BITS     6
#define MAX_GCLIENTS       ( 1 << GCLIENTNUM_BITS )


typedef const char FxEffectDef_t;
typedef const char snd_alias_list_t;
typedef const char Material_t;


#pragma pack(push, 2)
#pragma pack(pop)


typedef struct
{
	float position[3];
	float normal[3][3];
}
CollisionEdge_t;

typedef struct
{
	float normal[3];
	float distance;
	float unknown[8];
	unsigned int vertex_id[3];
	int edge_id[3];
}
CollisionTriangle_t;

typedef void DynEntityDef;
typedef void DynEntityPose;
typedef void DynEntityClient;
typedef void DynEntityColl;


enum LumpType
{
	LUMP_MATERIALS = 0,
	LUMP_LIGHTBYTES = 1,
	LUMP_LIGHTGRIDENTRIES = 2,
	LUMP_LIGHTGRIDCOLORS = 3,
	LUMP_PLANES = 4,
	LUMP_BRUSHSIDES = 5,
	LUMP_BRUSHES = 6,
	LUMP_TRIANGLES = 7,
	LUMP_DRAWVERTS = 8,
	LUMP_DRAWINDICES = 9,
	LUMP_CULLGROUPS = 10,
	LUMP_CULLGROUPINDICES = 11,
	LUMP_OBSOLETE_1 = 12,
	LUMP_OBSOLETE_2 = 13,
	LUMP_OBSOLETE_3 = 14,
	LUMP_OBSOLETE_4 = 15,
	LUMP_OBSOLETE_5 = 16,
	LUMP_PORTALVERTS = 17,
	LUMP_OCCLUDER = 18,
	LUMP_OCCLUDERPLANES = 19,
	LUMP_OCCLUDEREDGES = 20,
	LUMP_OCCLUDERINDICES = 21,
	LUMP_AABBTREES = 22,
	LUMP_CELLS = 23,
	LUMP_PORTALS = 24,
	LUMP_NODES = 25,
	LUMP_LEAFS = 26,
	LUMP_LEAFBRUSHES = 27,
	LUMP_LEAFSURFACES = 28,
	LUMP_COLLISIONVERTS = 29,
	LUMP_COLLISIONEDGES = 30,
	LUMP_COLLISIONTRIS = 31,
	LUMP_COLLISIONBORDERS = 32,
	LUMP_COLLISIONPARTITIONS = 33,
	LUMP_COLLISIONAABBS = 34,
	LUMP_MODELS = 35,
	LUMP_VISIBILITY = 36,
	LUMP_ENTITIES = 37,
	LUMP_PATHCONNECTIONS = 38,
	LUMP_PRIMARY_LIGHTS = 39,
};


#define MAX_VASTRINGS 2


#define	SVF_NOCLIENT  0x00000001
#define	SVF_BROADCAST 0x00000008

#define KEY_MASK_NONE        	0

#define KEY_MASK_FORWARD        127
#define KEY_MASK_BACK           -127
#define KEY_MASK_MOVERIGHT      127
#define KEY_MASK_MOVELEFT       -127

#define KEY_MASK_FIRE           1
#define KEY_MASK_MELEE          4
#define KEY_MASK_USE            8
#define KEY_MASK_RELOAD         16
#define KEY_MASK_LEANLEFT       64
#define KEY_MASK_LEANRIGHT      128
#define KEY_MASK_PRONE          256
#define KEY_MASK_CROUCH         512
#define KEY_MASK_JUMP           1024
#define KEY_MASK_ADS_MODE       4096
#define KEY_MASK_MELEE_BREATH   32772
#define KEY_MASK_HOLDBREATH     32768
#define KEY_MASK_FRAG           65536
#define KEY_MASK_SMOKE          131072

#define EF_VOTED 0x00100000
#define EF_TALK 0x00200000
#define EF_TAUNT 0x00400000
#define EF_FIRING 0x00000020
#define EF_MANTLE 0x00004000
#define EF_CROUCHING 0x00000004
#define EF_PRONE 0x00000008
#define EF_DEAD 0x00020000
#define EF_USETURRET 0x00000300
#define EF_AIMDOWNSIGHT 0x00040000

#define PMF_PRONE 				1
#define PMF_CROUCH 				2
#define PMF_MANTLE 				4
#define PMF_FRAG				16
#define PMF_LADDER 				32
#define PMF_BACKWARDS_RUN 		128
#define PMF_SLIDING 			512
#define PMF_MELEE 				8192
#define PMF_JUMPING 			524288
#define PMF_SPECTATING 			16777216
#define PMF_DISABLEWEAPON 		67108864

#define CONTENTS_SOLID          1
#define CONTENTS_NONCOLLIDING   4
#define CONTENTS_LAVA           8
#define CONTENTS_WATER          0x20
#define CONTENTS_CANSHOTCLIP    0x40
#define CONTENTS_MISSILECLIP    0x80
#define CONTENTS_VEHICLECLIP	0x200
#define CONTENTS_ITEMCLIP       0x400
#define CONTENTS_SKY            0x800
#define CONTENTS_AI_NOSIGHT     0x1000
#define CONTENTS_CLIPSHOT       0x2000
#define CONTENTS_MOVER          0x4000
#define CONTENTS_PLAYERCLIP     0x10000
#define CONTENTS_MONSTERCLIP    0x20000
#define CONTENTS_TELEPORTER     0x40000
#define CONTENTS_JUMPPAD        0x80000
#define CONTENTS_CLUSTERPORTAL  0x100000
#define CONTENTS_DONOTENTER     0x200000
#define CONTENTS_DONOTENTER_LARGE 0x400000

#define CONTENTS_MANTLE         0x1000000
#define CONTENTS_DETAIL         0x8000000
#define CONTENTS_STRUCTURAL     0x10000000
#define CONTENTS_TRANSPARENT    0x20000000
#define CONTENTS_NODROP         0x80000000

#define CONTENTS_BODY           0x2000000
#define CONTENTS_TRIGGER        0x40000000

#define SURF_NOLIGHTMAP         0x0
#define SURF_NODAMAGE           0x1
#define SURF_SLICK              0x2
#define SURF_SKY                0x4
#define SURF_LADDER             0x8
#define SURF_NOIMPACT           0x10
#define SURF_NOMARKS            0x20
#define SURF_NODRAW             0x80
#define SURF_NOSTEPS            0x2000
#define SURF_NONSOLID           0x4000
#define SURF_NODLIGHT           0x20000
#define SURF_NOCASTSHADOW       0x40000
#define SURF_MANTLEON           0x2000000
#define SURF_MANTLEOVER         0x4000000
#define SURF_PORTAL             0x80000000

#define SURF_BARK               0x100000
#define SURF_BRICK              0x200000
#define SURF_CARPET             0x300000
#define SURF_CLOTH              0x400000
#define SURF_CONCRETE           0x500000
#define SURF_DIRT               0x600000
#define SURF_FLESH              0x700000
#define SURF_FOLIAGE            0x800000
#define SURF_GLASS              0x900000
#define SURF_GRASS              0xa00000
#define SURF_GRAVEL             0xb00000
#define SURF_ICE                0xc00000
#define SURF_METAL              0xd00000
#define SURF_MUD                0xe00000
#define SURF_PAPER              0xf00000
#define SURF_PLASTER            0x1000000
#define SURF_ROCK               0x1100000
#define SURF_SAND               0x1200000
#define SURF_SNOW               0x1300000
#define SURF_WATER              0x1400000
#define SURF_WOOD               0x1500000
#define SURF_ASPHALT            0x1600000

#define TOOL_OCCLUDER           0x1
#define TOOL_DRAWTOGGLE         0x2
#define TOOL_ORIGIN             0x4
#define TOOL_RADIALNORMALS      0x8

#define MASK_ALL                ( -1 )
#define MASK_SOLID              ( CONTENTS_SOLID )
#define MASK_PLAYERSOLID        ( CONTENTS_SOLID | CONTENTS_PLAYERCLIP | CONTENTS_BODY )
#define MASK_DEADSOLID          ( CONTENTS_SOLID | CONTENTS_PLAYERCLIP )
#define MASK_WATER              ( CONTENTS_WATER | CONTENTS_LAVA | CONTENTS_SLIME )
#define MASK_OPAQUE             ( CONTENTS_SOLID | CONTENTS_LAVA )
#define MASK_SHOT               ( CONTENTS_SOLID | CONTENTS_BODY | CONTENTS_CORPSE )
#define MASK_MISSILESHOT        ( MASK_SHOT | CONTENTS_MISSILECLIP )

#define NYT HMAX
#define INTERNAL_NODE (HMAX+1)

typedef struct nodetype
{
	struct	nodetype *left, *right, *parent;
	struct	nodetype *next, *prev;
	struct	nodetype **head;
	int		weight;
	int		symbol;
} node_t;

#define HMAX 256

typedef struct
{
	int			blocNode;
	int			blocPtrs;
	node_t*		tree;
	node_t*		lhead;
	node_t*		ltail;
	node_t*		loc[HMAX+1];
	node_t**	freelist;
	node_t		nodeList[768];
	node_t*		nodePtrs[768];
} huff_t;

typedef struct
{
	huff_t		compressor;
	huff_t		decompressor;
} huffman_t;

int msg_hData[256] =
{
	250315,			// 0
	41193,			// 1
	6292,			// 2
	7106,			// 3
	3730,			// 4
	3750,			// 5
	6110,			// 6
	23283,			// 7
	33317,			// 8
	6950,			// 9
	7838,			// 10
	9714,			// 11
	9257,			// 12
	17259,			// 13
	3949,			// 14
	1778,			// 15
	8288,			// 16
	1604,			// 17
	1590,			// 18
	1663,			// 19
	1100,			// 20
	1213,			// 21
	1238,			// 22
	1134,			// 23
	1749,			// 24
	1059,			// 25
	1246,			// 26
	1149,			// 27
	1273,			// 28
	4486,			// 29
	2805,			// 30
	3472,			// 31
	21819,			// 32
	1159,			// 33
	1670,			// 34
	1066,			// 35
	1043,			// 36
	1012,			// 37
	1053,			// 38
	1070,			// 39
	1726,			// 40
	888,			// 41
	1180,			// 42
	850,			// 43
	960,			// 44
	780,			// 45
	1752,			// 46
	3296,			// 47
	10630,			// 48
	4514,			// 49
	5881,			// 50
	2685,			// 51
	4650,			// 52
	3837,			// 53
	2093,			// 54
	1867,			// 55
	2584,			// 56
	1949,			// 57
	1972,			// 58
	940,			// 59
	1134,			// 60
	1788,			// 61
	1670,			// 62
	1206,			// 63
	5719,			// 64
	6128,			// 65
	7222,			// 66
	6654,			// 67
	3710,			// 68
	3795,			// 69
	1492,			// 70
	1524,			// 71
	2215,			// 72
	1140,			// 73
	1355,			// 74
	971,			// 75
	2180,			// 76
	1248,			// 77
	1328,			// 78
	1195,			// 79
	1770,			// 80
	1078,			// 81
	1264,			// 82
	1266,			// 83
	1168,			// 84
	965,			// 85
	1155,			// 86
	1186,			// 87
	1347,			// 88
	1228,			// 89
	1529,			// 90
	1600,			// 91
	2617,			// 92
	2048,			// 93
	2546,			// 94
	3275,			// 95
	2410,			// 96
	3585,			// 97
	2504,			// 98
	2800,			// 99
	2675,			// 100
	6146,			// 101
	3663,			// 102
	2840,			// 103
	14253,			// 104
	3164,			// 105
	2221,			// 106
	1687,			// 107
	3208,			// 108
	2739,			// 109
	3512,			// 110
	4796,			// 111
	4091,			// 112
	3515,			// 113
	5288,			// 114
	4016,			// 115
	7937,			// 116
	6031,			// 117
	5360,			// 118
	3924,			// 119
	4892,			// 120
	3743,			// 121
	4566,			// 122
	4807,			// 123
	5852,			// 124
	6400,			// 125
	6225,			// 126
	8291,			// 127
	23243,			// 128
	7838,			// 129
	7073,			// 130
	8935,			// 131
	5437,			// 132
	4483,			// 133
	3641,			// 134
	5256,			// 135
	5312,			// 136
	5328,			// 137
	5370,			// 138
	3492,			// 139
	2458,			// 140
	1694,			// 141
	1821,			// 142
	2121,			// 143
	1916,			// 144
	1149,			// 145
	1516,			// 146
	1367,			// 147
	1236,			// 148
	1029,			// 149
	1258,			// 150
	1104,			// 151
	1245,			// 152
	1006,			// 153
	1149,			// 154
	1025,			// 155
	1241,			// 156
	952,			// 157
	1287,			// 158
	997,			// 159
	1713,			// 160
	1009,			// 161
	1187,			// 162
	879,			// 163
	1099,			// 164
	929,			// 165
	1078,			// 166
	951,			// 167
	1656,			// 168
	930,			// 169
	1153,			// 170
	1030,			// 171
	1262,			// 172
	1062,			// 173
	1214,			// 174
	1060,			// 175
	1621,			// 176
	930,			// 177
	1106,			// 178
	912,			// 179
	1034,			// 180
	892,			// 181
	1158,			// 182
	990,			// 183
	1175,			// 184
	850,			// 185
	1121,			// 186
	903,			// 187
	1087,			// 188
	920,			// 189
	1144,			// 190
	1056,			// 191
	3462,			// 192
	2240,			// 193
	4397,			// 194
	12136,			// 195
	7758,			// 196
	1345,			// 197
	1307,			// 198
	3278,			// 199
	1950,			// 200
	886,			// 201
	1023,			// 202
	1112,			// 203
	1077,			// 204
	1042,			// 205
	1061,			// 206
	1071,			// 207
	1484,			// 208
	1001,			// 209
	1096,			// 210
	915,			// 211
	1052,			// 212
	995,			// 213
	1070,			// 214
	876,			// 215
	1111,			// 216
	851,			// 217
	1059,			// 218
	805,			// 219
	1112,			// 220
	923,			// 221
	1103,			// 222
	817,			// 223
	1899,			// 224
	1872,			// 225
	976,			// 226
	841,			// 227
	1127,			// 228
	956,			// 229
	1159,			// 230
	950,			// 231
	7791,			// 232
	954,			// 233
	1289,			// 234
	933,			// 235
	1127,			// 236
	3207,			// 237
	1020,			// 238
	927,			// 239
	1355,			// 240
	768,			// 241
	1040,			// 242
	745,			// 243
	952,			// 244
	805,			// 245
	1073,			// 246
	740,			// 247
	1013,			// 248
	805,			// 249
	1008,			// 250
	796,			// 251
	996,			// 252
	1057,			// 253
	11457,			// 254
	13504,			// 255
};

#define MAX_PARSE_ENTITIES	(MAX_GENTITIES*2)
#define MAX_PARSE_CLIENTS	(MAX_GENTITIES*2)
#define PARSE_ENTITIES_MASK	(MAX_PARSE_ENTITIES-1)
#define PARSE_CLIENTS_MASK	(MAX_PARSE_CLIENTS-1)

#define MAX_GAMESTATE_CHARS MAX_MSGLEN
typedef struct
{
	int			stringOffsets[MAX_CONFIGSTRINGS];
	char		stringData[MAX_GAMESTATE_CHARS];
	int			dataCount;
} gameState_t;


typedef struct
{
	FILE		*demofile;

	int demoMessageSequence;
	int gameStatesParsed;
} demoFileState_t;


#define MAX_RELIABLE_COMMANDS 128


#define NETF( x ) # x,(intptr_t)&( (entityState_t*)0 )->x
#define FLOAT_INT_BITS  13
#define FLOAT_INT_BIAS  ( 1 << ( FLOAT_INT_BITS - 1 ) )
#define HUDELEM_COORD_BITS  10
#define HUDELEM_COORD_BIAS  ( 1 << ( HUDELEM_COORD_BITS - 1 ) )
#define ANIM_BITS       10
netField_t entityStateFields[] =
{
	{ NETF(pos.trTime), 32},
	{ NETF(pos.trBase[1]), 0},
	{ NETF(pos.trBase[0]), 0},
	{ NETF(pos.trDelta[0]), 0},
	{ NETF(pos.trDelta[1]), 0},
	{ NETF(angles2[1]), 0},
	{ NETF(apos.trBase[1]), -100},
	{ NETF(pos.trDelta[2]), 0},
	{ NETF(pos.trBase[2]), 0},
	{ NETF(apos.trBase[0]), -100},
	{ NETF(eventSequence), 8},
	{ NETF(legsAnim), 10},
	{ NETF(eType), 8},
	{ NETF(eFlags), 24},
	{ NETF(otherEntityNum), 10},
	{ NETF(surfType), 8},
	{ NETF(eventParm), 8},
	{ NETF(scale), 8},
	{ NETF(clientNum), 8},
	{ NETF(torsoAnim), 10},
	{ NETF(groundEntityNum), 10},
	{ NETF(events[0]), 8},
	{ NETF(events[1]), 8},
	{ NETF(events[2]), 8},
	{ NETF(angles2[0]), 0},
	{ NETF(events[3]), 8},
	{ NETF(apos.trBase[2]), -100},
	{ NETF(pos.trType), 8},
	{ NETF(fWaistPitch), 0},
	{ NETF(fTorsoPitch), 0},
	{ NETF(apos.trTime), 32},
	{ NETF(solid), 24},
	{ NETF(apos.trDelta[0]), 0},
	{ NETF(apos.trType), 8},
	{ NETF(animMovetype), 4},
	{ NETF(fTorsoHeight), 0},
	{ NETF(apos.trDelta[2]), 0},
	{ NETF(weapon), 7},
	{ NETF(index), 10},
	{ NETF(apos.trDelta[1]), 0},
	{ NETF(eventParms[0]), 8},
	{ NETF(eventParms[1]), 8},
	{ NETF(eventParms[2]), 8},
	{ NETF(eventParms[3]), 8},
	{ NETF(iHeadIcon), 4},
	{ NETF(pos.trDuration), 32},
	{ NETF(iHeadIconTeam), 2},
	{ NETF(time), 32},
	{ NETF(leanf), 0},
	{ NETF(attackerEntityNum), 10},
	{ NETF(time2), 32},
	{ NETF(loopSound), 8},
	{ NETF(origin2[2]), 0},
	{ NETF(origin2[0]), 0},
	{ NETF(origin2[1]), 0},
	{ NETF(angles2[2]), 0},
	{ NETF(constantLight), 32},
	{ NETF(apos.trDuration), 32},
	{ NETF(dmgFlags), 32},
};

typedef struct
{
	int number;
	int team;
	int modelindex;
	int attachModelIndex[6];
	int attachTagIndex[6];
	char name[32];
} clientState_t;

#define CSF( x ) # x,(intptr_t)&( (clientState_t*)0 )->x
netField_t clientStateFields[] =
{
	{ CSF( team ), 2},
	{ CSF( name[0] ), 32},
	{ CSF( name[4] ), 32},
	{ CSF( modelindex ), 8},
	{ CSF( attachModelIndex[1] ), 8},
	{ CSF( attachModelIndex[0] ), 8},
	{ CSF( name[8] ), 32},
	{ CSF( name[12] ), 32},
	{ CSF( name[16] ), 32},
	{ CSF( name[20] ), 32},
	{ CSF( name[24] ), 32},
	{ CSF( name[28] ), 32},
	{ CSF( attachTagIndex[5] ), 5},
	{ CSF( attachTagIndex[0] ), 5},
	{ CSF( attachTagIndex[1] ), 5},
	{ CSF( attachTagIndex[2] ), 5},
	{ CSF( attachTagIndex[3] ), 5},
	{ CSF( attachTagIndex[4] ), 5},
	{ CSF( attachModelIndex[2] ), 8},
	{ CSF( attachModelIndex[3] ), 8},
	{ CSF( attachModelIndex[4] ), 8},
	{ CSF( attachModelIndex[5] ), 8},
};


#define	MAX_ENTITIES_IN_SNAPSHOT	256
#define	MAX_CLIENTS_IN_SNAPSHOT	64
#define	MAX_MAP_AREA_BYTES		32
#define MAX_SNAPSHOTS             32
typedef struct
{
	int				snapFlags;
	int				ping;

	int				serverTime;

	byte			areamask[MAX_MAP_AREA_BYTES];

	playerState_t	ps;

	int				numEntities;
	entityState_t	entities[MAX_ENTITIES_IN_SNAPSHOT];

	int				numClients;
	clientState_t	clients[MAX_CLIENTS_IN_SNAPSHOT];

	int				numServerCommands;
	int				serverCommandSequence;
} snapshot_t;


typedef struct
{
	qboolean valid;
	int snapFlags;

	int serverTime;

	int messageNum;
	int deltaNum;
	int ping;
	byte areamask[MAX_MAP_AREA_BYTES];

	int cmdNum;
	playerState_t ps;

	int numEntities;
	int parseEntitiesNum;

	int numClients;
	int parseClientsNum;

	int serverCommandNum;
} clSnapshot_t;


#define	MAX_STRING_CHARS	1024	// max length of a string passed to Cmd_TokenizeString
#define	MAX_STRING_TOKENS	512		// max tokens resulting from Cmd_TokenizeString
#define	MAX_TOKEN_CHARS		1024	// max length of an individual token
#define	MAX_INFO_STRING		1024
#define MAX_BINARY_MESSAGE  32768   // max length of binary message


#define PACKET_BACKUP   32  // number of old messages that must be kept on client and
							// server for delta comrpession and ping estimation
#define PACKET_MASK     ( PACKET_BACKUP - 1 )


typedef struct {

	int clientNum;
	int lastPacketSentTime;                 // for retransmits during connection
	int lastPacketTime;                     // for timeouts

	netadr_t serverAddress;
	int connectTime;                        // for connection retransmits
	int connectPacketCount;                 // for display on connection dialog
	char serverMessage[MAX_STRING_TOKENS];          // for display on connection dialog

	int challenge;                          // from the server to use for connecting
	int checksumFeed;                       // from the server for checksum calculations

	int onlyVisibleClients;                 // DHM - Nerve

	// these are our reliable messages that go to the server
	int reliableSequence;
	int reliableAcknowledge;                // the last one the server has executed
	// TTimo - NOTE: incidentally, reliableCommands[0] is never used (always start at reliableAcknowledge+1)
	char reliableCommands[MAX_RELIABLE_COMMANDS][MAX_TOKEN_CHARS];

	// unreliable binary data to send to server
	int binaryMessageLength;
	char binaryMessage[MAX_BINARY_MESSAGE];
	qboolean binaryMessageOverflowed;

	// server message (unreliable) and command (reliable) sequence
	// numbers are NOT cleared at level changes, but continue to
	// increase as long as the connection is valid

	// message sequence is used by both the network layer and the
	// delta compression layer
	int serverMessageSequence;

	// reliable messages received from server
	int serverCommandSequence;
	int lastExecutedServerCommand;              // last server command grabbed or executed with CL_GetServerCommand
	char serverCommands[MAX_RELIABLE_COMMANDS][MAX_TOKEN_CHARS];

	// file transfer from server
	fileHandle_t download;
	int downloadNumber;
	int downloadBlock;          // block we are waiting for
	int downloadCount;          // how many bytes we got
	int downloadSize;           // how many bytes we got
	int downloadFlags;         // misc download behaviour flags sent by the server
	char downloadList[MAX_INFO_STRING];        // list of paks we need to download

	// www downloading
	qboolean bWWWDl;    // we have a www download going
	qboolean bWWWDlAborting;    // disable the CL_WWWDownload until server gets us a gamestate (used for aborts)
	char redirectedList[MAX_INFO_STRING];        // list of files that we downloaded through a redirect since last FS_ComparePaks
	char badChecksumList[MAX_INFO_STRING];        // list of files for which wwwdl redirect is broken (wrong checksum)

	// demo information
	char demoName[MAX_QPATH];
	qboolean demorecording;
	qboolean demoplaying;
	qboolean demowaiting;       // don't record until a non-delta message is received
	qboolean firstDemoFrameSkipped;
	fileHandle_t demofile;

	qboolean waverecording;
	fileHandle_t wavefile;
	int wavetime;

	int timeDemoFrames;             // counter of rendered frames
	int timeDemoStart;              // cls.realtime before first frame
	int timeDemoBaseTime;           // each frame will be at this time + frameNum * 50

	// big stuff at end of structure so most offsets are 15 bits or less
	netchan_t netchan;
} clientConnection_t;


#define CMD_BACKUP          64
#define CMD_MASK            ( CMD_BACKUP - 1 )

typedef struct {
	int timeoutcount;               // it requres several frames in a timeout condition
									// to disconnect, preventing debugging breaks from
									// causing immediate disconnects on continue
	clSnapshot_t snap;              // latest received from server

	int serverTime;                 // may be paused during play
	int oldServerTime;              // to prevent time from flowing bakcwards
	int oldFrameServerTime;         // to check tournament restarts
	int serverTimeDelta;            // cl.serverTime = cls.realtime + cl.serverTimeDelta
									// this value changes as net lag varies
	qboolean extrapolatedSnapshot;      // set if any cgame frame has been forced to extrapolate
	// cleared when CL_AdjustTimeDelta looks at it
	qboolean newSnapshots;          // set on parse of any valid packet

	gameState_t gameState;          // configstrings
	char mapname[MAX_QPATH];        // extracted from CS_SERVERINFO

	int parseEntitiesNum;           // index (not anded off) into cl_parse_entities[]
	
	// eyza
	int parseClientsNum;           // index (not anded off) into cl_parse_entities[]

	int mouseDx[2], mouseDy[2];         // added to by mouse events
	int mouseIndex;
	//P int joystickAxis[MAX_JOYSTICK_AXIS];            // set by joystick events

	// cgame communicates a few values to the client system
	int cgameUserCmdValue;              // current weapon to add to usercmd_t
	int cgameFlags;                     // flags that can be set by the gamecode
	float cgameSensitivity;
	int cgameMpIdentClient;             // NERVE - SMF
	vec3_t cgameClientLerpOrigin;       // DHM - Nerve

	// cmds[cmdNumber] is the predicted command, [cmdNumber-1] is the last
	// properly generated command
	usercmd_t cmds[CMD_BACKUP];     // each mesage will send several old cmds
	int cmdNumber;                  // incremented each frame, because multiple
									// frames may need to be packed into a single packet


	int serverId;                   // included in each client message so the server
									// can tell if it is for a prior map_restart
	// big stuff at end of structure so most offsets are 15 bits or less
	clSnapshot_t snapshots[PACKET_BACKUP];

	entityState_t entityBaselines[MAX_GENTITIES];   // for delta compression when not in previous frame

	entityState_t parseEntities[MAX_PARSE_ENTITIES];


	clientState_t clientBaselines[MAX_GCLIENTS];   // for delta compression when not in previous frame

	clientState_t parseClients[MAX_PARSE_CLIENTS];

} clientActive_t;


#endif
