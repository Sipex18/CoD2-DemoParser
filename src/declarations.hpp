#ifndef _DECLARATIONS_HPP_
#define _DECLARATIONS_HPP_

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
typedef struct gclient_s gclient_t;
typedef struct gentity_s gentity_t;
typedef int scr_entref_t;

typedef enum
{
	MOD_UNKNOWN,
	MOD_PISTOL_BULLET,
	MOD_RIFLE_BULLET,
	MOD_GRENADE,
	MOD_GRENADE_SPLASH,
	MOD_PROJECTILE,
	MOD_PROJECTILE_SPLASH,
	MOD_MELEE,
	MOD_HEAD_SHOT,
	MOD_CRUSH,
	MOD_TELEFRAG,
	MOD_FALLING,
	MOD_SUICIDE,
	MOD_TRIGGER_HURT,
	MOD_EXPLOSIVE,
	MOD_BAD
} meansOfDeath_t;

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

char *svc_strings[256] =
{
	"svc_nop",
	"svc_gamestate",
	"svc_configstring",
	"svc_baseline",
	"svc_serverCommand",
	"svc_download",
	"svc_snapshot",
	"svc_EOF"
};

typedef enum
{
	ET_GENERAL = 0,
	ET_PLAYER = 1,
	ET_CORPSE = 2,
	ET_ITEM = 3,
	ET_MISSILE = 4,
	ET_INVISIBLE = 5,
	ET_SCRIPTMOVER = 6
} entityType_t;

typedef enum
{
	TEAM_FREE,
	TEAM_RED,
	TEAM_BLUE,
	TEAM_SPEC,
	TEAM_NUM_TEAMS
} team_t;

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
	STATE_PLAYING,
	STATE_DEAD,
	STATE_SPECTATOR,
	STATE_INTERMISSION
} sessionState_t;

typedef enum
{
	CON_DISCONNECTED,
	CON_CONNECTING,
	CON_CONNECTED
} clientConnected_t;

typedef enum
{
	NA_BOT = 0,
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

typedef union
{
	int i;
	byte rgba[4];
} ucolor_t;

typedef struct cvar_s
{
	char *name;
	unsigned short flags;
	byte type;
	byte modified;
	union
	{
		float floatval;
		int integer;
		char* string;
		byte boolean;
		vec2_t vec2;
		vec3_t vec3;
		vec4_t vec4;
		ucolor_t color;
	};
	union
	{
		float latchedFloatval;
		int latchedInteger;
		char* latchedString;
		byte latchedBoolean;
		vec2_t latchedVec2;
		vec3_t latchedVec3;
		vec4_t latchedVec4;
		ucolor_t latchedColor;
	};
	union
	{
		float resetFloatval;
		int resetInteger;
		char* resetString;
		byte resetBoolean;
		vec2_t resetVec2;
		vec3_t resetVec3;
		vec4_t resetVec4;
		ucolor_t resetColor;
	};
	union
	{
		int imin;
		float fmin;
	};
	union
	{
		int imax;
		float fmax;
		const char** enumStr;
	};
	struct cvar_s *next;
	struct cvar_s *hashNext;
} cvar_t;

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

struct VariableStackBuffer
{
	const char *pos;
	u_int16_t size;
	u_int16_t bufLen;
	u_int16_t localId;
	char time;
	char buf[1];
};

union VariableUnion
{
	int intValue;
	float floatValue;
	unsigned int stringValue;
	const float *vectorValue;
	const char *codePosValue;
	unsigned int pointerValue;
	struct VariableStackBuffer *stackValue;
	unsigned int entityOffset;
};

union ObjectInfo_u
{
	u_int16_t size;
	u_int16_t entnum;
	u_int16_t nextEntId;
	u_int16_t self;
};

struct ObjectInfo
{
	u_int16_t refCount;
	union ObjectInfo_u u;
};

union VariableValueInternal_u
{
	u_int16_t next;
	union VariableUnion u;
	struct ObjectInfo o;
};

union VariableValueInternal_w
{
	unsigned int status;
	unsigned int type;
	unsigned int name;
	unsigned int classnum;
	unsigned int notifyName;
	unsigned int waitTime;
	unsigned int parentLocalId;
};

union VariableValueInternal_v
{
	u_int16_t next;
	u_int16_t index;
};

typedef struct
{
	union VariableUnion u;
	int type;
} VariableValue;

union Variable_u
{
	u_int16_t prev;
	u_int16_t prevSibling;
};

struct Variable
{
	u_int16_t id;
	union Variable_u u;
};

typedef struct
{
	struct Variable hash;
	union VariableValueInternal_u u;
	union VariableValueInternal_w w;
	union VariableValueInternal_v v;
	u_int16_t nextSibling;
} VariableValueInternal;

typedef struct
{
	const char *fieldBuffer;
	struct HunkUser *programHunkUser;
	u_int16_t canonicalStrCount;
	byte developer;
	byte developer_script;
	byte evaluate;
	byte pad[3];
	const char *error_message;
	int error_index;
	unsigned int time;
	unsigned int timeArrayId;
	unsigned int pauseArrayId;
	unsigned int levelId;
	unsigned int gameId;
	unsigned int animId;
	unsigned int freeEntList;
	unsigned int tempVariable;
	byte bInited;
	byte pad2;
	u_int16_t savecount;
	unsigned int checksum;
	unsigned int entId;
	unsigned int entFieldName;
	const char *programBuffer;
	const char *endScriptBuffer;
} scrVarPub_t;

struct function_stack_t
{
	const char *pos;
	unsigned int localId;
	unsigned int localVarCount;
	VariableValue *top;
	VariableValue *startTop;
};

struct function_frame_t
{
	struct function_stack_t fs;
	int topType;
};

typedef struct
{
	unsigned int *localVars;
	VariableValue *maxstack;
	int function_count;
	struct function_frame_t *function_frame;
	VariableValue *top;
	byte debugCode;
	byte abort_on_error;
	byte terminal_error;
	byte pad;
	unsigned int inparamcount;
	unsigned int outparamcount;
	struct function_frame_t function_frame_start[32];
	VariableValue stack[2048];
} scrVmPub_t;

typedef int	fileHandle_t;
typedef void (*xfunction_t)();
typedef void (*xmethod_t)(scr_entref_t);
typedef void (*xcommand_t) (void);

typedef struct scr_function_s
{
	const char      *name;
	xfunction_t     call;
	qboolean        developer;
} scr_function_t;

typedef struct scr_method_s
{
	const char     *name;
	xmethod_t      call;
	qboolean       developer;
} scr_method_t;

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

typedef enum
{
	TRACE_HITTYPE_NONE = 0x0,
	TRACE_HITTYPE_ENTITY = 0x1,
	TRACE_HITTYPE_DYNENT_MODEL = 0x2,
	TRACE_HITTYPE_DYNENT_BRUSH = 0x3,
	TRACE_HITTYPE_GLASS = 0x4
} TraceHitType;

typedef struct trace_s
{
	float fraction;
	vec3_t normal;
	int surfaceFlags;
	int contents;
	const char *material;
	int entityNum;
	byte allsolid;
	byte startsolid;
	byte walkable;
	byte padding;
} trace_t;

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

typedef struct
{
	char command[1024];
	int cmdTime;
	int cmdType;
} reliableCommands_t;

typedef struct
{
	netadr_t adr;
	int challenge;
	int time;
	int pingTime;
	int firstTime;
	int firstPing;
	qboolean connected;
	int guid;
#if COD_VERSION == COD2_1_2 || COD_VERSION == COD2_1_3
	char pbguid[64];
	int ipAuthorize;
#endif
} challenge_t; // verified for 1.0, guessed for 1.2 and 1.3

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

typedef struct
{
	byte		linked;
	byte		bmodel;
	byte		svFlags;
	byte		pad1;
	int			clientMask[2];
	byte		inuse;
	byte		pad2[3];
	int			broadcastTime;
	vec3_t		mins, maxs;
	int			contents;
	vec3_t		absmin, absmax;
	vec3_t		currentOrigin;
	vec3_t		currentAngles;
	u_int16_t	ownerNum;
	u_int16_t	pad3;
	int			eventTime;
} entityShared_t; // verified

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

typedef enum
{
	PLAYERVIEWLOCK_NONE = 0x0,
	PLAYERVIEWLOCK_FULL = 0x1,
	PLAYERVIEWLOCK_WEAPONJITTER = 0x2,
	PLAYERVIEWLOCKCOUNT = 0x3,
} ViewLockTypes_t;

typedef enum
{
	OBJST_EMPTY = 0x0,
	OBJST_ACTIVE = 0x1,
	OBJST_INVISIBLE = 0x2,
	OBJST_DONE = 0x3,
	OBJST_CURRENT = 0x4,
	OBJST_FAILED = 0x5,
	OBJST_NUMSTATES = 0x6,
} objectiveState_t;

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

typedef struct
{
	sessionState_t state;
	int forceSpectatorClient;
	int statusIcon;
	int archiveTime;
	int	score;
	int deaths;
	u_int16_t scriptPersId;
	byte pad2;
	byte pad;
	clientConnected_t connected;
	usercmd_t cmd;
	usercmd_t oldcmd;
	qboolean localClient;
	qboolean predictItemPickup;
	char name[32];
	int maxHealth;
	int enterTime;
	int voteCount;
	int teamVoteCount;
	float unknown;
	int viewmodelIndex;
	qboolean noSpectate;
	int teamInfo;
	int clientId;
	sessionTeam_t team;
	int model;
	int attachedModels[6];
	int attachedModelsTags[6];
	char manualModeName[32];
	int psOffsetTime;
} clientSession_t; // verified

struct gclient_s
{
	playerState_t ps;
	clientSession_t sess;
	int spectatorClient;
	qboolean noclip;
	qboolean ufo;
	qboolean bFrozen;
	int lastCmdTime;
	int buttons;
	int oldbuttons;
	int latched_buttons;
	int buttonsSinceLastFrame;
	vec3_t oldOrigin;
	float fGunPitch;
	float fGunYaw;
	int damage_blood;
	vec3_t damage_from;
	qboolean damage_fromWorld;
	int accurateCount; // N/A
	int accuracy_shots; // N/A
	int accuracy_hits; // N/A
	int inactivityTime;
	qboolean inactivityWarning;
	int playerTalkTime;
	int rewardTime; // N/A
	float currentAimSpreadScale; // 10256
	int unknown_space[2];
	int unknownClientEndFrameVar;
	int unknown_space2[3];
	gentity_t *lookAtEntity; // needs a NULL check, otherwise crash.
	int activateEntNumber;
	int activateTime;
	int nonPVSFriendlyEntNum;
	int pingPlayerTime;
	int damageFeedBackTime;
	vec2_t damageFeedBackDir;
	vec3_t swayViewAngles; // 10316
	vec3_t swayOffset;
	vec3_t swayAngles;
	int unknown_space3[7];
	float weaponRecoil; // 10380
	int unknown_space4[3];
	int lastServerTime;
	int lastActivateTime;
}; // verified

struct turretInfo_s
{
	int inuse;
	int flags;
	int fireTime;
	vec2_t arcmin;
	vec2_t arcmax;
	float dropPitch;
	int stance;
	int prevStance;
	int fireSndDelay;
	vec3_t userOrigin;
	float playerSpread;
	int triggerDown;
	char fireSnd;
	char fireSndPlayer;
	char stopSnd;
	char stopSndPlayer;
};

struct gentity_s
{
	entityState_t s;
	entityShared_t r;
	struct gclient_s *client; // 344
	turretInfo_s *pTurretInfo; // 348
	byte physicsObject; // 352
	byte takedamage; // 353
	byte active; // 354
	byte nopickup; // 355 ?
	byte model; // 356
	byte dobjbits; // 357 ?
	byte handler; // 358
	byte team; // 359
	u_int16_t classname; // 360
	u_int16_t target;
	u_int16_t targetname;
	u_int16_t padding;
	int spawnflags;
	int flags;
	int eventTime;
	qboolean freeAfterEvent; // 380
	qboolean unlinkAfterEvent; // 384
	int clipmask; // 388
	int framenum; // 392
	gentity_t *parent; // 396
	int nextthink; // 400
	int healthPoints; // 404
	int reservedHealth; // 408 ?
	int damage; // 412
	int splashDamage; // 416 ?
	int splashRadius; // 420 ?
	float pfDecelTimeMove; // 424
	float pfDecelTimeRotate; // 428
	float pfSpeedMove; // 432
	float pfSpeedRotate; // 436
	float pfMidTimeMove; // 440
	float pfMidTimeRotate; // 444
	vec3_t vPos1Move; // 448 ?
	vec3_t vPos2Move; // 460
	vec3_t vPos3Move; // 472
	vec3_t vPos1Rotate; // 484 ?
	vec3_t vPos2Rotate; // 496
	vec3_t vPos3Rotate; // 508
	int moverState; // 520 ?
	gentity_t** linkedEntities; // 524 ??
	byte attachedModels[6]; // 528
	u_int16_t attachedModelsIndexes; // 536 ?
	u_int16_t numAttachedModels; // 538 ?
	int animTree; // 540 ?
	vec4_t color; // ?
}; // verified

#define MAX_DOWNLOAD_BLKSIZE 1024
#define MAX_DOWNLOAD_WINDOW 8

typedef struct
{
	playerState_t ps;
	int	num_entities;
	int	num_clients;
	int	first_entity;
	int	first_client;
	unsigned int messageSent;
	unsigned int messageAcked;
	int	messageSize;
} clientSnapshot_t;

#pragma pack(push)
#pragma pack(1)
typedef struct
{
	char num;
	char data[256];
	int dataLen;
} voices_t;
#pragma pack(pop)

typedef struct client_s
{
	clientState_e	state;
	int				unksnapshotvar;
	int				unksnapshotvar2;
	char			userinfo[1024];
	reliableCommands_t	reliableCommands[128];
	int				reliableSequence;
	int				reliableAcknowledge;
	int				reliableSent;
	int				messageAcknowledge;
	int				gamestateMessageNum;
	int				challenge;
	usercmd_t  	 	lastUsercmd;
	int				lastClientCommand;
	char			lastClientCommandString[1024];
	gentity_t 		*gentity;
	char 			name[32];
	char			downloadName[MAX_QPATH];
	fileHandle_t	download;
	int				downloadSize;
	int				downloadCount;
	int				downloadClientBlock;
	int				downloadCurrentBlock;
	int				downloadXmitBlock;
	unsigned char	*downloadBlocks[MAX_DOWNLOAD_WINDOW];
	int				downloadBlockSize[MAX_DOWNLOAD_WINDOW];
	qboolean		downloadEOF;
	int				downloadSendTime;
#if COD_VERSION == COD2_1_2 || COD_VERSION == COD2_1_3
	char			wwwDownloadURL[MAX_OSPATH];
	qboolean		wwwDownload;
	qboolean		wwwDownloadStarted;
	qboolean		wwwDlAck;
	qboolean		wwwDl_failed;
#endif
	int				deltaMessage;
	int				floodprotect;
	int				lastPacketTime;
	int				lastConnectTime;
	int				nextSnapshotTime;
	qboolean		rateDelayed;
	int				timeoutCount;
	clientSnapshot_t frames[PACKET_BACKUP];
	int				ping;
	int				rate;
	int				snapshotMsec;
	int				pureAuthentic;
	netchan_t		netchan;
	int 			guid;
	short			clscriptid;
	int				bot;
	int				serverId;
	voices_t		voicedata[40];
	int				unsentVoiceData;
	byte			mutedClients[MAX_CLIENTS];
	byte			hasVoip;
#if COD_VERSION == COD2_1_2 || COD_VERSION == COD2_1_3
	char 			pbguid[64];
#endif
} client_t;

typedef struct archivedSnapshot_s
{
	int start;
	int size;
} archivedSnapshot_t;

typedef struct cachedClient_s
{
	int playerStateExists;
	clientState_e *cs;
	playerState_t *ps;
} cachedClient_t;
/*
typedef struct
{
	qboolean	initialized;
	int			time;
	int			snapFlagServerBit;
	client_t	*clients;
	int			numSnapshotEntities;
	int			numSnapshotClients;
	int			nextSnapshotEntities;
	int			nextSnapshotClients;
	entityState_t *snapshotEntities;
	clientState_t *snapshotClients;
	int 		archivedSnapshotEnabled;
	int 		nextArchivedSnapshotFrames;
	archivedSnapshot_t *archivedSnapshotFrames;
	int 		*archivedSnapshotBuffer;
	int 		nextArchivedSnapshotBuffer;
	int			nextCachedSnapshotEntities;
	int 		nextCachedSnapshotClients;
	int 		nextCachedSnapshotFrames;
	cachedClient_t cachedSnapshotClients;
	int			nextHeartbeatTime;
	int 		nextStatusResponseTime;
	challenge_t	challenges[1024];
	netadr_t	redirectAddress;
	netadr_t	authorizeAddress;
	char 		netProfilingBuf[1504];
} serverStatic_t; // verified
*/
typedef struct
{
	const char *key;
	const char *value;
} keyValueStr_t;

typedef struct
{
	byte spawnVarsValid;
	int numSpawnVars;
	keyValueStr_t spawnVars[64];
	int numSpawnVarChars;
	char spawnVarChars[2048];
} SpawnVar;

typedef struct
{
	u_int16_t entnum;
	u_int16_t otherEntnum;
	int useCount;
	int otherUseCount;
} trigger_info_t;

typedef struct
{
	struct gclient_s *clients;
	struct gentity_s *gentities;
	int gentitySize;
	int num_entities;
	struct gentity_s *firstFreeEnt;
	struct gentity_s *lastFreeEnt;
	fileHandle_t logFile;
	int initializing;
	int clientIsSpawning;
	objective_t objectives[16];
	int maxclients;
	int framenum;
	int time;
	int previousTime;
	int frameTime;
	int startTime;
	int teamScores[TEAM_NUM_TEAMS];
	int lastTeammateHealthTime;
	qboolean bUpdateScoresForIntermission;
	int manualNameChange;
	int numConnectedClients;
	int sortedClients[MAX_CLIENTS];
	char voteString[1024];
	char voteDisplayString[1024];
	int voteTime; // 711
	int voteExecuteTime;
	int voteYes;
	int voteNo;
	int numVotingClients;
	byte gap[2072];
	SpawnVar spawnVars;
	int savePersist;
	struct gentity_s *droppedWeaponCue[32];
	float fFogOpaqueDist;
	float fFogOpaqueDistSqrd;
	int remapCount;
	int currentPlayerClone;
	trigger_info_t pendingTriggerList[256];
	trigger_info_t currentTriggerList[256];
	int pendingTriggerListSize;
	int currentTriggerListSize;
	int finished;
	int bPlayerIgnoreRadiusDamage;
	int bPlayerIgnoreRadiusDamageLatched;
	int registerWeapons;
	int bRegisterItems;
	int currentEntityThink;
	void *openScriptIOFileHandles[1];
	char *openScriptIOFileBuffers[1];
} level_locals_t; // possibly more stuff here

typedef enum
{
	SS_DEAD,
	SS_LOADING,
	SS_GAME
} serverState_t;

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

typedef struct
{
	int svFlags;
	int clientMask[2];
	vec3_t absmin;
	vec3_t absmax;
} archivedEntityShared_t;

typedef struct archivedEntity_s
{
	entityState_t s;
	archivedEntityShared_t r;
} archivedEntity_t;

typedef struct svEntity_s
{
	u_int16_t worldSector;
	u_int16_t nextEntityInWorldSector;
	archivedEntity_t baseline;
	int numClusters;
	int clusternums[MAX_ENT_CLUSTERS];
	int	lastCluster;
	int	linkcontents;
	float linkmin[2];
	float linkmax[2];
} svEntity_t;

typedef struct
{
	serverState_t state;
	qboolean restarting;
	int start_frameTime;
	int	checksumFeed;
	int timeResidual;
	int unk; // ?
	struct cmodel_s *models[MAX_MODELS]; // ?
	char *configstrings[MAX_CONFIGSTRINGS];
	svEntity_t svEntities[MAX_GENTITIES];
	char *entityParsePoint;
	gentity_t *gentities;
	int gentitySize;
	int	num_entities;
	playerState_t *gameClients;
	int gameClientSize;
	int	skelTimeStamp;
	int	skelMemPos;
	int	bpsWindow[MAX_BPS_WINDOW];
	int	bpsWindowSteps;
	int	bpsTotalBytes;
	int	bpsMaxBytes;
	int	ubpsWindow[MAX_BPS_WINDOW];
	int	ubpsTotalBytes;
	int	ubpsMaxBytes;
	float ucompAve;
	int	ucompNum;
	char gametype[MAX_QPATH];
} server_t; // verified

typedef enum weapType_t
{
	WEAPTYPE_BULLET = 0x0,
	WEAPTYPE_GRENADE = 0x1,
	WEAPTYPE_PROJECTILE = 0x2,
	WEAPTYPE_BINOCULARS = 0x3,
	WEAPTYPE_NUM = 0x4
} weapType_t;

typedef enum weapClass_t
{
	WEAPCLASS_RIFLE = 0x0,
	WEAPCLASS_MG = 0x1,
	WEAPCLASS_SMG = 0x2,
	WEAPCLASS_SPREAD = 0x3,
	WEAPCLASS_PISTOL = 0x4,
	WEAPCLASS_GRENADE = 0x5,
	WEAPCLASS_ROCKETLAUNCHER = 0x6,
	WEAPCLASS_TURRET = 0x7,
	WEAPCLASS_NON_PLAYER = 0x8,
	WEAPCLASS_ITEM = 0x9,
	WEAPCLASS_NUM = 0xA
} weapClass_t;

typedef enum ImpactType_t
{
	IMPACT_TYPE_NONE = 0x0,
	IMPACT_TYPE_BULLET_SMALL = 0x1,
	IMPACT_TYPE_BULLET_LARGE = 0x2,
	IMPACT_TYPE_BULLET_AP = 0x3,
	IMPACT_TYPE_SHOTGUN = 0x4,
	IMPACT_TYPE_GRENADE_BOUNCE = 0x5,
	IMPACT_TYPE_GRENADE_EXPLODE = 0x6,
	IMPACT_TYPE_ROCKET_EXPLODE = 0x7,
	IMPACT_TYPE_PROJECTILE_DUD = 0x8,
	IMPACT_TYPE_COUNT = 0x9
} ImpactType_t;

typedef enum weapInventoryType_t
{
	WEAPINVENTORY_PRIMARY = 0x0,
	WEAPINVENTORY_OFFHAND = 0x1,
	WEAPINVENTORY_ITEM = 0x2,
	WEAPINVENTORY_ALTMODE = 0x3,
	WEAPINVENTORYCOUNT = 0x4
} weapInventoryType_t;

typedef enum OffhandClass_t
{
	OFFHAND_CLASS_NONE = 0x0,
	OFFHAND_CLASS_FRAG_GRENADE = 0x1,
	OFFHAND_CLASS_SMOKE_GRENADE = 0x2,
	OFFHAND_CLASS_COUNT = 0x3
} OffhandClass_t;

typedef enum weapStance_t
{
	WEAPSTANCE_STAND = 0x0,
	WEAPSTANCE_DUCK = 0x1,
	WEAPSTANCE_PRONE = 0x2,
	WEAPSTANCE_NUM = 0x3
} weapStance_t;

typedef enum weapOverlayReticle_t
{
	WEAPOVERLAYRETICLE_NONE = 0x0,
	WEAPOVERLAYRETICLE_CROSSHAIR = 0x1,
	WEAPOVERLAYRETICLE_NUM = 0x2
} weapOverlayReticle_t;

typedef enum weaponIconRatioType_t
{
	WEAPON_ICON_RATIO_1TO1 = 0x0,
	WEAPON_ICON_RATIO_2TO1 = 0x1,
	WEAPON_ICON_RATIO_4TO1 = 0x2,
	WEAPON_ICON_RATIO_COUNT = 0x3
} weaponIconRatioType_t;

typedef enum weapProjExposion_t
{
	WEAPPROJEXP_GRENADE = 0x0,
	WEAPPROJEXP_MOLOTOV = 0x1,
	WEAPPROJEXP_ROCKET = 0x2,
	WEAPPROJEXP_NONE = 0x3,
	WEAPPROJEXP_NUM = 0x4
} weapProjExposion_t;

typedef const char FxEffectDef_t;
typedef const char snd_alias_list_t;
typedef const char Material_t;

typedef struct WeaponDef_t
{
	const char *szInternalName;
	const char *szDisplayName;
	const char *szOverlayName;
	const char *szViewModelName;
	const char *szHandModelName;
	int unknown;
	const char *szXAnims[22];
	const char *szModeName;
	int playerAnimType;
	weapType_t weapType;
	weapClass_t weapClass;
	ImpactType_t impactType;
	weapInventoryType_t inventoryType;
	OffhandClass_t offhandClass; // ? not confirmed
	weapStance_t stance; // ? not confirmed
	FxEffectDef_t *viewFlashEffect;
	FxEffectDef_t *worldFlashEffect;
	snd_alias_list_t *pickupSound;
	snd_alias_list_t *ammoPickupSound;
	snd_alias_list_t *projectileSound;
	snd_alias_list_t *pullbackSound;
	snd_alias_list_t *fireSound;
	snd_alias_list_t *fireSoundPlayer;
	snd_alias_list_t *unknown4[4];
	snd_alias_list_t *fireLastSound;
	snd_alias_list_t *fireLastSoundPlayer;
	snd_alias_list_t *meleeSwipeSound;
	snd_alias_list_t *rechamberSound;
	snd_alias_list_t *rechamberSoundPlayer;
	snd_alias_list_t *reloadSound;
	snd_alias_list_t *reloadSoundPlayer;
	snd_alias_list_t *reloadEmptySound;
	snd_alias_list_t *reloadEmptySoundPlayer;
	snd_alias_list_t *reloadStartSound;
	snd_alias_list_t *reloadStartSoundPlayer;
	snd_alias_list_t *reloadEndSound;
	snd_alias_list_t *reloadEndSoundPlayer;
	snd_alias_list_t *altSwitchSound;
	snd_alias_list_t *raiseSound;
	snd_alias_list_t *putawaySound;
	snd_alias_list_t *noteTrackSoundA;
	snd_alias_list_t *noteTrackSoundB;
	snd_alias_list_t *noteTrackSoundC;
	snd_alias_list_t *noteTrackSoundD;
	FxEffectDef_t *shellEjectEffect;
	FxEffectDef_t *lastShotEjectEffect;
	Material_t *reticleCenter;
	Material_t *reticleSide;
	int iReticleCenterSize;
	int iReticleSideSize;
	int iReticleMinOfs;
	float vStandMove[3];
	float vStandRot[3];
	float vDuckedOfs[3];
	float vDuckedMove[3];
	float vDuckedRot[3];
	float vProneOfs[3];
	float vProneMove[3];
	float vProneRot[3];
	float fPosMoveRate;
	float fPosProneMoveRate;
	float fStandMoveMinSpeed;
	float fDuckedMoveMinSpeed;
	float fProneMoveMinSpeed;
	float fPosRotRate;
	float fPosProneRotRate;
	float fStandRotMinSpeed;
	float fDuckedRotMinSpeed;
	float fProneRotMinSpeed;
	const char *worldModel;
	Material_t *hudIcon;
	Material_t *modeIcon;
	int iStartAmmo;
	const char *szAmmoName;
	int iAmmoIndex;
	const char *szClipName;
	int iClipIndex;
	int iMaxAmmo;
	int iClipSize;
	int shotCount;
	const char *szSharedAmmoCapName;
	int iSharedAmmoCapIndex;
	int iSharedAmmoCap;
	int damage;
	int playerDamage;
	int iMeleeDamage;
	int iDamageType;
	int iFireDelay;
	int iMeleeDelay;
	int iFireTime;
	int iRechamberTime;
	int iRechamberBoltTime;
	int iHoldFireTime;
	int iMeleeTime;
	int iReloadTime;
	int iReloadEmptyTime;
	int iReloadAddTime;
	int iReloadStartTime;
	int iReloadStartAddTime;
	int iReloadEndTime;
	int iDropTime;
	int iRaiseTime;
	int iAltDropTime;
	int iAltRaiseTime;
	int quickDropTime;
	int quickRaiseTime;
	int fuseTime;
	float autoAimRange;
	float aimAssistRange;
	float aimAssistRangeAds;
	float aimPadding;
	float enemyCrosshairRange;
	int crosshairColorChange;
	float moveSpeedScale;
	float fAdsZoomFov;
	float fAdsZoomInFrac;
	float fAdsZoomOutFrac;
	Material_t *overlayMaterial;
	weapOverlayReticle_t overlayReticle;
	float overlayWidth;
	float overlayHeight;
	float fAdsBobFactor;
	float fAdsViewBobMult;
	float fHipSpreadStandMin;
	float fHipSpreadDuckedMin;
	float fHipSpreadProneMin;
	float hipSpreadStandMax;
	float hipSpreadDuckedMax;
	float hipSpreadProneMax;
	float fHipSpreadDecayRate;
	float fHipSpreadFireAdd;
	float fHipSpreadTurnAdd;
	float fHipSpreadMoveAdd;
	float fHipSpreadDuckedDecay;
	float fHipSpreadProneDecay;
	float fHipReticleSidePos;
	int iAdsTransInTime;
	int iAdsTransOutTime;
	float fAdsIdleAmount;
	float fHipIdleAmount;
	float adsIdleSpeed;
	float hipIdleSpeed;
	float fIdleCrouchFactor;
	float fIdleProneFactor;
	float fGunMaxPitch;
	float fGunMaxYaw;
	float swayMaxAngle;
	float swayLerpSpeed;
	float swayPitchScale;
	float swayYawScale;
	float swayHorizScale;
	float swayVertScale;
	float swayShellShockScale;
	float adsSwayMaxAngle;
	float adsSwayLerpSpeed;
	float adsSwayPitchScale;
	float adsSwayYawScale;
	float adsSwayHorizScale;
	float adsSwayVertScale;
	int bRifleBullet;
	int armorPiercing;
	int semiAuto;
	int bBoltAction;
	int aimDownSight;
	int bRechamberWhileAds;
	float adsViewErrorMin;
	float adsViewErrorMax;
	int bCookOffHold;
	int bClipOnly;
	int cancelAutoHolsterWhenEmpty; // ?
	int suppressAmmoReserveDisplay; // ?
	Material_t *killIcon;
	weaponIconRatioType_t killIconRatio; // ?
	int flipKillIcon;
	int bNoPartialReload;
	int bSegmentedReload;
	int iReloadAmmoAdd;
	int iReloadStartAdd;
	const char *szAltWeaponName;
	unsigned int altWeaponIndex;
	int iDropAmmoMin;
	int iDropAmmoMax;
	int iExplosionRadius;
	int iExplosionInnerDamage;
	int iExplosionOuterDamage;
	int iProjectileSpeed;
	int iProjectileSpeedUp;
	const char *projectileModel;
	weapProjExposion_t projExplosion;
	FxEffectDef_t *projExplosionEffect;
	snd_alias_list_t *projExplosionSound;
	int bProjImpactExplode;
	float parallelBounce[23];
	float perpendicularBounce[23];
	FxEffectDef_t *projTrailEffect;
	int unknown2[4];
	float fAdsAimPitch;
	float fAdsCrosshairInFrac;
	float fAdsCrosshairOutFrac;
	int adsGunKickReducedKickBullets;
	float adsGunKickReducedKickPercent;
	float fAdsGunKickPitchMin;
	float fAdsGunKickPitchMax;
	float fAdsGunKickYawMin;
	float fAdsGunKickYawMax;
	float fAdsGunKickAccel;
	float fAdsGunKickSpeedMax;
	float fAdsGunKickSpeedDecay;
	float fAdsGunKickStaticDecay;
	float fAdsViewKickPitchMin;
	float fAdsViewKickPitchMax;
	float fAdsViewKickYawMin;
	float fAdsViewKickYawMax;
	float fAdsViewKickCenterSpeed;
	float fAdsViewScatterMin;
	float fAdsViewScatterMax;
	float fAdsSpread;
	int hipGunKickReducedKickBullets;
	float hipGunKickReducedKickPercent;
	float fHipGunKickPitchMin;
	float fHipGunKickPitchMax;
	float fHipGunKickYawMin;
	float fHipGunKickYawMax;
	float fHipGunKickAccel;
	float fHipGunKickSpeedMax;
	float fHipGunKickSpeedDecay;
	float fHipGunKickStaticDecay;
	float fHipViewKickPitchMin;
	float fHipViewKickPitchMax;
	float fHipViewKickYawMin;
	float fHipViewKickYawMax;
	float fHipViewKickCenterSpeed;
	float fHipViewScatterMin;
	float fHipViewScatterMax;
	float fightDist;
	float maxDist;
	const char *aiVsAiAccuracyGraph;
	const char *aiVsPlayerAccuracyGraph;
	int accuracyGraphKnotCount[2];
	int originalAccuracyGraphKnotCount[2];
	int iPositionReloadTransTime;
	float leftArc;
	float rightArc;
	float topArc;
	float bottomArc;
	float accuracy;
	float aiSpread;
	float playerSpread;
	int minVertTurnSpeed;
	int minHorTurnSpeed;
	int maxVertTurnSpeed;
	int maxHorTurnSpeed;
	float pitchConvergenceTime;
	float yawConvergenceTime;
	float suppressTime;
	float maxRange;
	float fAnimHorRotateInc;
	float fPlayerPositionDist;
	const char *szUseHintString;
	const char *dropHintString;
	int iUseHintStringIndex;
	int dropHintStringIndex;
	float horizViewJitter;
	float vertViewJitter;
	const char *szScript;
	float fOOPosAnimLength[2];
	int minDamage;
	int minPlayerDamage;
	float fMaxDamageRange;
	float fMinDamageRange;
	int unknown5[4];
	float locationDamageMultipliers[19];
	const char *fireRumble;
	const char *meleeImpactRumble;
} WeaponDef_t;

typedef enum
{
	ANIM_BP_UNUSED,
	ANIM_BP_LEGS,
	ANIM_BP_TORSO,
	ANIM_BP_BOTH,
	NUM_ANIM_BODYPARTS
} animBodyPart_t;

typedef enum
{
	IT_BAD,
	IT_WEAPON,
	IT_AMMO,
	IT_HEALTH,
	IT_HOLDABLE,
} itemType_t;

typedef struct gitem_s
{
	char *classname; // ??? They don't set classname or what? Other fields work fine.
	char *pickup_sound;
	char *world_model;
	int giTag; // ?
	char *icon;
	char *display_name; // Weapon string e.g WEAPON_STEN
	int quantity; // ammo for weapons
	itemType_t giType;
	int giAmmoIndex;
	int giClipIndex;
	int giSharedAmmoCapIndex; // guessed
} gitem_t;

typedef struct XBoneInfo_s
{
	float bounds[2][3];
	float offset[3];
	float radiusSquared;
} XBoneInfo_t;

typedef struct XModelCollSurf_s
{
	float mins[3];
	float maxs[3];
	int boneIdx;
	int contents;
	int surfFlags;
} XModelCollSurf_t;

typedef struct XModelHighMipBounds_s
{
	float mins[3];
	float maxs[3];
} XModelHighMipBounds_t;

typedef struct XModelStreamInfo_s
{
	XModelHighMipBounds_t *highMipBounds;
} XModelStreamInfo_t;

typedef struct XModel_s
{
	char numBones;
	char numRootBones;
	u_int16_t *boneNames;
	char *parentList;
	byte unk[72];
	XModelCollSurf_t *collSurfs; // 84
	int numCollSurfs; // 88
	int contents; // 92
	XBoneInfo_t *boneInfo; // 96
	vec3_t mins; // 100
	vec3_t maxs;
	short numLods; // 124
	short collLod;
	XModelStreamInfo_t streamInfo; // 128
	int memUsage; // 132
	const char *name; // 136
	char flags; // 140
	char bad; // 141
} XModel_t;

typedef struct DObjSkeletonPartMatrix_s
{
	float p1[4];
	float p2[4];
} DObjSkeletonPartMatrix_t;

typedef struct DSkelPartBits_s
{
	int anim[4];
	int control[4];
	int skel[4];
} DSkelPartBits_t;

typedef struct DSkel_s
{
	DSkelPartBits_t *partBits;
	int timeStamp;
	DObjSkeletonPartMatrix_t *mat;
} DSkel_t;

typedef struct DObj_s
{
	int *tree;
	DSkel_t skel;
	unsigned short duplicateParts; // 16
	int unk2;
	byte numModels; // 24
	byte numBones; // 25
	byte duplicatePartsSize; // 26
	byte pad;
	XModel_t *models; // 28
} DObj_t;

struct pmove_t
{
	struct playerState_s *ps;
	usercmd_t cmd;
	usercmd_t oldcmd;
	int tracemask;
	int numtouch;
	int touchents[32];
	vec3_t mins;
	vec3_t maxs;
	float xyspeed;
	int proneChange;
	byte mantleStarted; // 229
	vec3_t mantleEndPos;
	int mantleDuration;
};

struct pml_t
{
	vec3_t forward;
	vec3_t right;
	vec3_t up;
	float frametime;
	int msec;
	int walking;
	int groundPlane;
	int almostGroundPlane;
	trace_t groundTrace;
	float impactSpeed;
	vec3_t previous_origin;
	vec3_t previous_velocity;
};

typedef struct
{
	short emptystring;
	short allies;
	short axis;
	short current;
	short damage;
	short death;
	short dlight;
	short done;
	short empty;
	short entity;
	short failed;
	short fraction;
	short goal;
	short grenade;
	short info_notnull;
	short invisible;
	short key1;
	short key2;
	short killanimscript;
	short left;
	short movedone;
	short noclass;
	short normal;
	short pistol;
	short plane_waypoint;
	short player;
	short position;
	short primary;
	short primaryb;
	short prone;
	short right;
	short rocket;
	short rotatedone;
	short script_brushmodel;
	short script_model;
	short script_origin;
	short spectator;
	short stand;
	short surfacetype;
	short target_script_trigger;
	short tempEntity;
	short touch;
	short trigger;
	short trigger_use;
	short trigger_use_touch;
	short trigger_damage;
	short trigger_lookat;
	short truck_cam;
	short worldspawn;
	short binocular_enter;
	short binocular_exit;
	short binocular_fire;
	short binocular_release;
	short binocular_drop;
	short begin;
	short intermission;
	short menuresponse;
	short playing;
	short none;
	short dead;
	short auto_change;
	short manual_change;
	short freelook;
	short call_vote;
	short vote;
	short snd_enveffectsprio_level;
	short snd_enveffectsprio_shellshock;
	short snd_channelvolprio_holdbreath;
	short snd_channelvolprio_pain;
	short snd_channelvolprio_shellshock;
	short tag_flash;
	short tag_flash_11;
	short tag_flash_2;
	short tag_flash_22;
	short tag_brass;
	short j_head;
	short tag_weapon;
	short tag_player;
	short tag_camera;
	short tag_aim;
	short tag_aim_animated;
	short tag_origin;
	short tag_butt;
	short tag_weapon_right;
	short back_low;
	short back_mid;
	short back_up;
	short neck;
	short head;
	short pelvis;
} stringIndex_t;


typedef struct clientInfo_s
{
	int infoValid;
	int nextValid;
	int clientNum;
	char name[32];
	int team;
	int oldteam;
	int score;
	int location;
	int health;
	char model[64];
	char attachModelNames[6][64];
	char attachTagNames[6][64];
	//lerpFrame_t legs;
	//lerpFrame_t torso;
	float lerpMoveDir;
	float lerpLean;
	vec3_t playerAngles;
	int leftHandGun;
	int dobjDirty;
	//clientControllers_t control;
	//int clientConditions[NUM_ANIM_CONDITIONS][2];
	XAnimTree_s *pXAnimTree;
	int iDObjWeapon;
	int stanceTransitionTime;
	int turnAnimEndTime;
	char turnAnimType;
} clientInfo_t;

struct bgs_s
{
	byte animScriptData[0xB3BC8u];
	int multiplayer;
	int root;
	int torso;
	int legs;
	int turning;
	int turnAnimEndTime;
	int frametime;
	float angle;
	struct XModel *(*GetXModel)(const char *);
	void (*CreateDObj)(struct DObjModel_s *, u_int16_t, struct XAnimTree_s *, int, int, clientInfo_t *);
	u_int16_t (*AttachWeapon)(struct DObjModel_s *, u_int16_t, clientInfo_t *);
	struct DObj_s *(*GetDObj)(int, int);
	void *(*AllocXAnim)(int);
};

typedef struct
{
	char material[64];
	int surfaceFlags;
	int contentFlags;
} dmaterial_t;

typedef struct cStaticModel_s
{
	u_int16_t writable;
	XModel_t *xmodel;
	vec3_t origin;
	vec3_t invScaledAxis[3];
	vec3_t absmin;
	vec3_t absmax;
} cStaticModel_t;

typedef struct cplane_s
{
	vec3_t normal;
	float dist;
	byte type;
	byte signbits;
	byte pad[2];
} cplane_t;

typedef struct cbrushside_s
{
	cplane_t *plane;
	unsigned int materialNum;
} cbrushside_t;

typedef struct
{
	cplane_t *plane;
	int16_t children[2];
} cNode_t;

typedef struct cLeaf_s
{
	u_int16_t firstCollAabbIndex;
	u_int16_t collAabbCount;
	int brushContents;
	int terrainContents;
	vec3_t mins;
	vec3_t maxs;
	int leafBrushNode;
	int16_t cluster;
	int16_t pad;
} cLeaf_t;

typedef struct
{
	u_int16_t *brushes;
} cLeafBrushNodeLeaf_t;

typedef struct
{
	float dist;
	float range;
	u_int16_t childOffset[2];
} cLeafBrushNodeChildren_t;

typedef union
{
	cLeafBrushNodeLeaf_t leaf;
	cLeafBrushNodeChildren_t children;
} cLeafBrushNodeData_t;

#pragma pack(push, 2)
typedef struct cLeafBrushNode_s
{
	byte axis;
	u_int16_t leafBrushCount;
	int contents;
	cLeafBrushNodeData_t data;
} cLeafBrushNode_t;
#pragma pack(pop)

typedef struct CollisionBorder
{
	float distEq[3];
	float zBase;
	float zSlope;
	float start;
	float length;
} CollisionBorder_t;

typedef struct CollisionPartition
{
	char triCount;
	char borderCount;
	int firstTri;
	CollisionBorder_t *borders;
} CollisionPartition_t;

typedef union
{
	int firstChildIndex;
	int partitionIndex;
} CollisionAabbTreeIndex_t;

typedef struct CollisionAabbTree_s
{
	float origin[3];
	float halfSize[3];
	u_int16_t materialIndex;
	u_int16_t childCount;
	CollisionAabbTreeIndex_t u;
} CollisionAabbTree_t;

typedef struct cmodel_s
{
	vec3_t mins;
	vec3_t maxs;
	float radius;
	cLeaf_t leaf;
} cmodel_t;

typedef struct __attribute__((aligned(16))) cbrush_t
{
	float mins[3];
	int contents;
	float maxs[3];
	unsigned int numsides;
	cbrushside_t *sides;
	int16_t axialMaterialNum[2][3];
} cbrush_t;

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

typedef struct clipMap_s
{
	const char *name;
	unsigned int numStaticModels;
	cStaticModel_t *staticModelList;
	unsigned int numMaterials;
	dmaterial_t *materials;
	unsigned int numBrushSides;
	cbrushside_t *brushsides;
	unsigned int numNodes;
	cNode_t *nodes;
	unsigned int numLeafs;
	cLeaf_t *leafs;
	unsigned int leafbrushNodesCount;
	cLeafBrushNode_t *leafbrushNodes;
	unsigned int numLeafBrushes;
	u_int16_t *leafbrushes;
	unsigned int numLeafSurfaces;
	unsigned int *leafsurfaces;
	unsigned int vertCount;
	float (*verts)[3];
	unsigned int edgeCount;
	CollisionEdge_t *edges;
	int triCount;
	CollisionTriangle_t *triIndices;
	int borderCount;
	CollisionBorder_t *borders;
	int partitionCount;
	CollisionPartition_t *partitions;
	int aabbTreeCount;
	CollisionAabbTree_t *aabbTrees;
	unsigned int numSubModels;
	cmodel_t *cmodels;
	u_int16_t numBrushes;
	cbrush_t *brushes;
	int numClusters;
	int clusterBytes;
	byte *visibility;
	int vised;
	int numEntityChars;
	char *entityString;
	cbrush_t *box_brush;
	cmodel_t box_model;
	u_int16_t dynEntCount[2];
	DynEntityDef *dynEntDefList[2];
	DynEntityPose *dynEntPoseList[2];
	DynEntityClient *dynEntClientList[2];
	DynEntityColl *dynEntCollList[2];
	unsigned int checksum;
} clipMap_t; // verified

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

struct BspChunk
{
	enum LumpType type;
	unsigned int length;
};

typedef struct BspHeader
{
	unsigned int ident;
	unsigned int version;
	unsigned int chunkCount;
	struct BspChunk chunks[100];
} BspHeader;

typedef struct comBspGlob_t
{
	char name[64];
	BspHeader *header;
	unsigned int fileSize;
	unsigned int checksum;
	enum LumpType loadedLumpType;
	const void *loadedLumpData;
} comBspGlob_t;

#define MAX_VASTRINGS 2

struct va_info_t
{
	char va_string[MAX_VASTRINGS][1024];
	int index;
};

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
#define MAX_PARSE_CLIENTS	(MAX_GCLIENTS*2)
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

demoFileState_t	demo;


#define MAX_RELIABLE_COMMANDS 128



#define NETF( x ) # x,(int)&( (entityState_t*)0 )->x
#define FLOAT_INT_BITS  13
#define FLOAT_INT_BIAS  ( 1 << ( FLOAT_INT_BITS - 1 ) )
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

static clientConnection_t clc;



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

clientActive_t cl;




#define MAX_PREDICTED_EVENTS	16
 
typedef struct {
	int			clientFrame;		// incremented each frame

	int			clientNum;
	
	qboolean	demoPlayback;
	qboolean	levelShot;			// taking a level menu screenshot
	int			deferredPlayerLoading;
	qboolean	loading;			// don't defer players at initial startup
	qboolean	intermissionStarted;	// don't play voice rewards, because game will end shortly

	// there are only one or two snapshot_t that are relevent at a time
	int			latestSnapshotNum;	// the number of snapshots the client system has received
	int			latestSnapshotTime;	// the time from latestSnapshotNum, so we don't need to read the snapshot yet

	snapshot_t	*snap;				// cg.snap->serverTime <= cg.time
	snapshot_t	*nextSnap;			// cg.nextSnap->serverTime > cg.time, or NULL
	snapshot_t	activeSnapshots[2];

	float		frameInterpolation;	// (float)( cg.time - cg.frame->serverTime ) / (cg.nextFrame->serverTime - cg.frame->serverTime)

	qboolean	thisFrameTeleport;
	qboolean	nextFrameTeleport;

	int			frametime;		// cg.time - cg.oldTime

	int			time;			// this is the time value that the client
								// is rendering at.
	int			oldTime;		// time at last frame, used for missile trails and prediction checking

	int			physicsTime;	// either cg.snap->time or cg.nextSnap->time

	int			timelimitWarnings;	// 5 min, 1 min, overtime
	int			fraglimitWarnings;

	qboolean	mapRestart;			// set on a map restart to set back the weapon

	qboolean	renderingThirdPerson;		// during deaths, chasecams, etc

	// prediction state
	qboolean	hyperspace;				// true if prediction has hit a trigger_teleport
	playerState_t	predictedPlayerState;
	//centity_t		predictedPlayerEntity;
	qboolean	validPPS;				// clear until the first call to CG_PredictPlayerState
	int			predictedErrorTime;
	vec3_t		predictedError;

	int			eventSequence;
	int			predictableEvents[MAX_PREDICTED_EVENTS];

	float		stepChange;				// for stair up smoothing
	int			stepTime;

	float		duckChange;				// for duck viewheight smoothing
	int			duckTime;

	float		landChange;				// for landing hard
	int			landTime;

	// input state sent to server
	int			weaponSelect;

	// auto rotating items
	vec3_t		autoAngles;
	vec3_t		autoAxis[3];
	vec3_t		autoAnglesFast;
	vec3_t		autoAxisFast[3];

	// view rendering
	//refdef_t	refdef;
	vec3_t		refdefViewAngles;		// will be converted to refdef.viewaxis

	// zoom key
	qboolean	zoomed;
	int			zoomTime;
	float		zoomSensitivity;

	// information screen text during loading
	char		infoScreenText[MAX_STRING_CHARS];

	// scoreboard
	int			scoresRequestTime;
	int			numScores;
	int			selectedScore;
	int			teamScores[2];
	//score_t		scores[MAX_CLIENTS];
	qboolean	showScores;
	qboolean	scoreBoardShowing;
	int			scoreFadeTime;
	//char		killerName[MAX_NAME_LENGTH];
	char			spectatorList[MAX_STRING_CHARS];		// list of names
	int				spectatorLen;												// length of list
	float			spectatorWidth;											// width in device units
	int				spectatorTime;											// next time to offset
	int				spectatorPaintX;										// current paint x
	int				spectatorPaintX2;										// current paint x
	int				spectatorOffset;										// current offset from start
	int				spectatorPaintLen; 									// current offset from start

	// skull trails
	//skulltrail_t	skulltrails[MAX_CLIENTS];

	// centerprinting
	int			centerPrintTime;
	int			centerPrintCharWidth;
	int			centerPrintY;
	char		centerPrint[1024];
	int			centerPrintLines;

	// low ammo warning state
	int			lowAmmoWarning;		// 1 = low, 2 = empty

	// kill timers for carnage reward
	int			lastKillTime;

	// crosshair client ID
	int			crosshairClientNum;
	int			crosshairClientTime;

	// powerup active flashing
	int			powerupActive;
	int			powerupTime;

	// attacking player
	int			attackerTime;
	int			voiceTime;

	// reward medals
	int			rewardStack;
	int			rewardTime;
	//int			rewardCount[MAX_REWARDSTACK];
	//qhandle_t	rewardShader[MAX_REWARDSTACK];
	//qhandle_t	rewardSound[MAX_REWARDSTACK];

	// sound buffer mainly for announcer sounds
	int			soundBufferIn;
	int			soundBufferOut;
	int			soundTime;
	//qhandle_t	soundBuffer[MAX_SOUNDBUFFER];

	// for voice chat buffer
	int			voiceChatTime;
	int			voiceChatBufferIn;
	int			voiceChatBufferOut;

	// warmup countdown
	int			warmup;
	int			warmupCount;

	//==========================

	int			itemPickup;
	int			itemPickupTime;
	int			itemPickupBlendTime;	// the pulse around the crosshair is timed seperately

	int			weaponSelectTime;
	int			weaponAnimation;
	int			weaponAnimationTime;

	// blend blobs
	float		damageTime;
	float		damageX, damageY, damageValue;

	// status bar head
	float		headYaw;
	float		headEndPitch;
	float		headEndYaw;
	int			headEndTime;
	float		headStartPitch;
	float		headStartYaw;
	int			headStartTime;

	// view movement
	float		v_dmg_time;
	float		v_dmg_pitch;
	float		v_dmg_roll;

	vec3_t		kick_angles;	// weapon kicks
	vec3_t		kick_origin;

	// temp working variables for player view
	float		bobfracsin;
	int			bobcycle;
	float		xyspeed;
	int     nextOrbitTime;

	//qboolean cameraMode;		// if rendering from a loaded camera


	// development tool
	//refEntity_t		testModelEntity;
	char			testModelName[MAX_QPATH];
	qboolean		testGun;

} cg_t;

cg_t				cg;



// The client game static (cgs) structure hold everything
// loaded or calculated from the gamestate.  It will NOT
// be cleared when a tournement restart is done, allowing
// all clients to begin playing instantly
typedef struct {
	gameState_t		gameState;			// gamestate from server
	//glconfig_t		glconfig;			// rendering configuration
	float			screenXScale;		// derived from glconfig
	float			screenYScale;
	float			screenXBias;

	int				serverCommandSequence;	// reliable command stream counter
	int				processedSnapshotNum;// the number of snapshots cgame has requested

	qboolean		localServer;		// detected on startup by checking sv_running

	// parsed from serverinfo
	//gametype_t		gametype;
	int				dmflags;
	int				teamflags;
	int				fraglimit;
	int				capturelimit;
	int				timelimit;
	int				maxclients;
	char			mapname[MAX_QPATH];
	char			redTeam[MAX_QPATH];
	char			blueTeam[MAX_QPATH];

	int				voteTime;
	int				voteYes;
	int				voteNo;
	qboolean		voteModified;			// beep whenever changed
	char			voteString[MAX_STRING_TOKENS];

	int				teamVoteTime[2];
	int				teamVoteYes[2];
	int				teamVoteNo[2];
	qboolean		teamVoteModified[2];	// beep whenever changed
	char			teamVoteString[2][MAX_STRING_TOKENS];

	int				levelStartTime;

	int				scores1, scores2;		// from configstrings
	int				redflag, blueflag;		// flag status from configstrings
	int				flagStatus;

	qboolean  newHud;

	//
	// locally derived information from gamestate
	//
	//qhandle_t		gameModels[MAX_MODELS];
	//sfxHandle_t		gameSounds[MAX_SOUNDS];

	int				numInlineModels;
	//qhandle_t		inlineDrawModel[MAX_MODELS];
	vec3_t			inlineModelMidpoints[MAX_MODELS];

	clientInfo_t	clientinfo[MAX_CLIENTS];

	// teamchat width is *3 because of embedded color codes
//	char			teamChatMsgs[TEAMCHAT_HEIGHT][TEAMCHAT_WIDTH*3+1];
//	int				teamChatMsgTimes[TEAMCHAT_HEIGHT];
	int				teamChatPos;
	int				teamLastChatPos;

	int cursorX;
	int cursorY;
	qboolean eventHandling;
	qboolean mouseCaptured;
	qboolean sizingHud;
	void *capturedItem;
//	qhandle_t activeCursor;

	// orders
	int currentOrder;
	qboolean orderPending;
	int orderTime;
	int currentVoiceClient;
	int acceptOrderTime;
	int acceptTask;
	int acceptLeader;
//	char acceptVoice[MAX_NAME_LENGTH];

	// media
//	cgMedia_t		media;

} cgs_t;

cgs_t				cgs;




#if COD_VERSION == COD2_1_0
static const int gentities_offset = 0x08665480;
#elif COD_VERSION == COD2_1_2
static const int gentities_offset = 0x08679380;
#elif COD_VERSION == COD2_1_3
static const int gentities_offset = 0x08716400;
#endif

#if COD_VERSION == COD2_1_0
static const int gclients_offset = 0x086F1480;
#elif COD_VERSION == COD2_1_2
static const int gclients_offset = 0x08705480;
#elif COD_VERSION == COD2_1_3
static const int gclients_offset = 0x087A2500;
#endif

#define g_entities ((gentity_t*)(gentities_offset))
#define g_clients ((gclient_t*)(gclients_offset))

#if COD_VERSION == COD2_1_0
static const int varpub_offset = 0x08394000;
#elif COD_VERSION == COD2_1_2
static const int varpub_offset = 0x08396480;
#elif COD_VERSION == COD2_1_3
static const int varpub_offset = 0x08397500;
#endif

#if COD_VERSION == COD2_1_0
static const int varglob_offset = 0x08294000;
#elif COD_VERSION == COD2_1_2
static const int varglob_offset = 0x08296480;
#elif COD_VERSION == COD2_1_3
static const int varglob_offset = 0x08297500;
#endif

#if COD_VERSION == COD2_1_0
static const int vmpub_offset = 0x083D7600;
#elif COD_VERSION == COD2_1_2
static const int vmpub_offset = 0x083D7A00;
#elif COD_VERSION == COD2_1_3
static const int vmpub_offset = 0x083D8A80;
#endif

#if COD_VERSION == COD2_1_0
static const int sv_offset = 0x0842BC80;
#elif COD_VERSION == COD2_1_2
static const int sv_offset = 0x0843F180;
#elif COD_VERSION == COD2_1_3
static const int sv_offset = 0x08440200;
#endif

#if COD_VERSION == COD2_1_0
static const int svs_offset = 0x0841FB00;
#elif COD_VERSION == COD2_1_2
static const int svs_offset = 0x08422000;
#elif COD_VERSION == COD2_1_3
static const int svs_offset = 0x08423080;
#endif

#if COD_VERSION == COD2_1_0
static const int level_offset = 0x0859B400;
#elif COD_VERSION == COD2_1_2
static const int level_offset = 0x085AF300;
#elif COD_VERSION == COD2_1_3
static const int level_offset = 0x0864C380;
#endif

#if COD_VERSION == COD2_1_0
static const int const_offset = 0x087A22A0;
#elif COD_VERSION == COD2_1_2
static const int const_offset = 0x087B61A0;
#elif COD_VERSION == COD2_1_3
static const int const_offset = 0x08853220;
#endif

#if COD_VERSION == COD2_1_0
static const int bgs_offset = 0x0855A4E0;
#elif COD_VERSION == COD2_1_2
static const int bgs_offset = 0x0856E3A0;
#elif COD_VERSION == COD2_1_3
static const int bgs_offset = 0x0860B420;
#endif

#if COD_VERSION == COD2_1_0
static const int cm_offset = 0x08185BE0;
#elif COD_VERSION == COD2_1_2
static const int cm_offset = 0x08187D40;
#elif COD_VERSION == COD2_1_3
static const int cm_offset = 0x08188DC0;
#endif

#if COD_VERSION == COD2_1_0
static const int bspglob_offset = 0x08185BC8;
#elif COD_VERSION == COD2_1_2
static const int bspglob_offset = 0x08187D28;
#elif COD_VERSION == COD2_1_3
static const int bspglob_offset = 0x08188DA8;
#endif

#define scrVarPub (*((scrVarPub_t*)( varpub_offset )))
#define scrVmPub (*((scrVmPub_t*)( vmpub_offset )))
#define scrVarGlob (((VariableValueInternal*)( varglob_offset )))
#define scrVarGlob_high (((VariableValueInternal*)( varglob_offset + 16 * 32770 )))
#define sv (*((server_t*)( sv_offset )))
#define svs (*((serverStatic_t*)( svs_offset )))
#define level (*((level_locals_t*)( level_offset )))
#define scr_const (*((stringIndex_t*)( const_offset )))
#define level_bgs (*((bgs_s*)( bgs_offset )))
#define cm (*((clipMap_t*)( cm_offset )))
#define comBspGlob (*((comBspGlob_t*)( bspglob_offset ))) // freed by Com_UnloadBsp() after initialization

// Check for critical structure sizes and fail if not match
#if __GNUC__ >= 6
#if COD_VERSION == COD2_1_0
static_assert((sizeof(client_t) == 0x78F14), "ERROR: client_t size is invalid!");
#elif COD_VERSION == COD2_1_2
static_assert((sizeof(client_t) == 0x79064), "ERROR: client_t size is invalid!");
#elif COD_VERSION == COD2_1_3
static_assert((sizeof(client_t) == 0xB1064), "ERROR: client_t size is invalid!");
#endif

static_assert((sizeof(gentity_t) == 560), "ERROR: gentity_t size is invalid!");
static_assert((sizeof(gclient_t) == 0x28A4), "ERROR: gclient_t size is invalid!");
static_assert((sizeof(gitem_t) == 44), "ERROR: gitem_t size is invalid!");
static_assert((sizeof(XModel_t) == 144), "ERROR: XModel_t size is invalid!");
#endif

#endif
