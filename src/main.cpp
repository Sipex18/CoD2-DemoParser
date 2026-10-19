#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <direct.h>

#include "declarations.hpp"

#define Com_DPrintf Com_Printf
#define Com_Memset memset
#define Q_vsnprintf vsnprintf
#define Q_strncpyz I_strncpyz
#define Com_Memcpy memcpy

static void Q_strncat( char *dst, const char *src, int dstsize )
{
	int l = (int)strlen( dst );
	if ( l < dstsize - 1 )
	{
		strncat( dst, src, dstsize - 1 - l );
	}
}

#define SNAPFLAG_NOT_ACTIVE 2
#define CA_ACTIVE 5

static char *CL_GetConfigstring( int index );
static void CL_ReplaceConfigstring( int index, const char *val );
static void CL_ParseWeaponList( void );
static const char *CL_WeaponName( int weaponIndex );
static void CL_ProcessServerCommand( const char *s );

static void Export_Reset( void );
static void Export_MatchInfoChanged( int t );
static void Export_Announcement( const char *text );
static void Export_Chat( char type, const char *cmd );
static void Export_ProcessSnapshot( const snapshot_t *snap );
static void Export_WriteJsonFiles( const char *demoName );

static int exportLastSnapshotTime = 0;

#define BIG_INFO_STRING 8192

static demoFileState_t	demo;
static clientConnection_t clc;
static clientActive_t cl;

static char *clCs[MAX_CONFIGSTRINGS];
static char bigConfigString[BIG_INFO_STRING];
static char *weaponNames[256];
static int weaponNamesCount = 0;
static int clScores[MAX_CLIENTS];
static int clDeaths[MAX_CLIENTS];
static int clPings[MAX_CLIENTS];
static int clScoreAxis;
static int clScoreAllies;

int CL_ReadDemoMessage(void);

static char logFileName[2048];
static FILE *logFile = NULL;


#define ERR_FATAL 0
#define ERR_DROP 1
void Com_Error(int err, char* fmt,...)
{
	char buf[MAX_STRING_CHARS];

	va_list		argptr;

	va_start (argptr,fmt);
	Q_vsnprintf(buf, sizeof(buf), fmt, argptr);
	va_end (argptr);

	fputs(buf, stderr);
	exit(1);
}

void Com_Printf( const char *fmt, ...)
{
	va_list		argptr;
	char		msg[MAXPRINTMSG];

	va_start (argptr,fmt);
	Q_vsnprintf (msg, sizeof(msg), fmt, argptr);
	va_end (argptr);

	msg[MAXPRINTMSG - 1] = 0;

	fputs( msg, stdout );

	if ( logFile )
	{
		fprintf( logFile, "%s", msg );
	}
}

void I_strncpyz( char *dest, const char *src, int destsize )
{
	strncpy( dest, src, destsize-1 );
	dest[destsize-1] = 0;
}


static huffman_t msgHuff;
static qboolean	msgInit = qfalse;

int LittleLong( int l )
{
	return l;
}

// huffman.c from enemy territory
static int bloc = 0;

//bani - optimized version
//clears data along the way so we dont have to memset() it ahead of time
int get_bit( byte *fin )
{
	int t;
	t = fin[ bloc >> 3 ] >> ( bloc & 7 ) & 0x1;
	bloc++;
	return t;
}

static node_t **get_ppnode( huff_t* huff )
{
	node_t **tppnode;
	if ( !huff->freelist )
	{
		return &( huff->nodePtrs[huff->blocPtrs++] );
	}
	else
	{
		tppnode = huff->freelist;
		huff->freelist = (node_t **)*tppnode;
		return tppnode;
	}
}

static void free_ppnode( huff_t* huff, node_t **ppnode )
{
	*ppnode = (node_t *)huff->freelist;
	huff->freelist = ppnode;
}

/* Swap the location of these two nodes in the tree */
static void swap( huff_t* huff, node_t *node1, node_t *node2 )
{
	node_t *par1, *par2;

	par1 = node1->parent;
	par2 = node2->parent;

	if ( par1 )
	{
		if ( par1->left == node1 )
		{
			par1->left = node2;
		}
		else
		{
			par1->right = node2;
		}
	}
	else
	{
		huff->tree = node2;
	}

	if ( par2 )
	{
		if ( par2->left == node2 )
		{
			par2->left = node1;
		}
		else
		{
			par2->right = node1;
		}
	}
	else
	{
		huff->tree = node1;
	}

	node1->parent = par2;
	node2->parent = par1;
}

/* Swap these two nodes in the linked list (update ranks) */
static void swaplist( node_t *node1, node_t *node2 )
{
	node_t *par1;

	par1 = node1->next;
	node1->next = node2->next;
	node2->next = par1;

	par1 = node1->prev;
	node1->prev = node2->prev;
	node2->prev = par1;

	if ( node1->next == node1 )
	{
		node1->next = node2;
	}
	if ( node2->next == node2 )
	{
		node2->next = node1;
	}
	if ( node1->next )
	{
		node1->next->prev = node1;
	}
	if ( node2->next )
	{
		node2->next->prev = node2;
	}
	if ( node1->prev )
	{
		node1->prev->next = node1;
	}
	if ( node2->prev )
	{
		node2->prev->next = node2;
	}
}

/* Do the increments */
static void increment( huff_t* huff, node_t *node )
{
	node_t *lnode;

	if ( !node )
	{
		return;
	}

	if ( node->next != NULL && node->next->weight == node->weight )
	{
		lnode = *node->head;
		if ( lnode != node->parent )
		{
			swap( huff, lnode, node );
		}
		swaplist( lnode, node );
	}
	if ( node->prev && node->prev->weight == node->weight )
	{
		*node->head = node->prev;
	}
	else
	{
		*node->head = NULL;
		free_ppnode( huff, node->head );
	}
	node->weight++;
	if ( node->next && node->next->weight == node->weight )
	{
		node->head = node->next->head;
	}
	else
	{
		node->head = get_ppnode( huff );
		*node->head = node;
	}
	if ( node->parent )
	{
		increment( huff, node->parent );
		if ( node->prev == node->parent )
		{
			swaplist( node, node->parent );
			if ( *node->head == node )
			{
				*node->head = node->parent;
			}
		}
	}
}

void Huff_addRef( huff_t* huff, byte ch )
{
	node_t *tnode, *tnode2;
	if ( huff->loc[ch] == NULL )   /* if this is the first transmission of this node */
	{
		tnode = &( huff->nodeList[huff->blocNode++] );
		tnode2 = &( huff->nodeList[huff->blocNode++] );

		tnode2->symbol = INTERNAL_NODE;
		tnode2->weight = 1;
		tnode2->next = huff->lhead->next;
		if ( huff->lhead->next )
		{
			huff->lhead->next->prev = tnode2;
			if ( huff->lhead->next->weight == 1 )
			{
				tnode2->head = huff->lhead->next->head;
			}
			else
			{
				tnode2->head = get_ppnode( huff );
				*tnode2->head = tnode2;
			}
		}
		else
		{
			tnode2->head = get_ppnode( huff );
			*tnode2->head = tnode2;
		}
		huff->lhead->next = tnode2;
		tnode2->prev = huff->lhead;

		tnode->symbol = ch;
		tnode->weight = 1;
		tnode->next = huff->lhead->next;
		if ( huff->lhead->next )
		{
			huff->lhead->next->prev = tnode;
			if ( huff->lhead->next->weight == 1 )
			{
				tnode->head = huff->lhead->next->head;
			}
			else
			{
				/* this should never happen */
				tnode->head = get_ppnode( huff );
				*tnode->head = tnode2;
			}
		}
		else
		{
			/* this should never happen */
			tnode->head = get_ppnode( huff );
			*tnode->head = tnode;
		}
		huff->lhead->next = tnode;
		tnode->prev = huff->lhead;
		tnode->left = tnode->right = NULL;

		if ( huff->lhead->parent )
		{
			if ( huff->lhead->parent->left == huff->lhead )   /* lhead is guaranteed to by the NYT */
			{
				huff->lhead->parent->left = tnode2;
			}
			else
			{
				huff->lhead->parent->right = tnode2;
			}
		}
		else
		{
			huff->tree = tnode2;
		}

		tnode2->right = tnode;
		tnode2->left = huff->lhead;

		tnode2->parent = huff->lhead->parent;
		huff->lhead->parent = tnode->parent = tnode2;

		huff->loc[ch] = tnode;

		increment( huff, tnode2->parent );
	}
	else
	{
		increment( huff, huff->loc[ch] );
	}
}

/* Get a symbol */
/* get a symbol at a bit offset */
void Huff_offsetReceive( node_t *node, int *ch, byte *fin, int *offset )
{
	bloc = *offset;
	while ( node && node->symbol == INTERNAL_NODE )
	{
		if ( get_bit( fin ) )
		{
			node = node->right;
		}
		else
		{
			node = node->left;
		}
	}
	if ( !node )
	{
		*ch = 0;
		return;
	}
	*ch = node->symbol;
	*offset = bloc;
}

void Huff_Init( huffman_t *huff )
{

	Com_Memset( &huff->compressor, 0, sizeof( huff_t ) );
	Com_Memset( &huff->decompressor, 0, sizeof( huff_t ) );

	// Initialize the tree & list with the NYT node
	huff->decompressor.tree = huff->decompressor.lhead = huff->decompressor.ltail = huff->decompressor.loc[NYT] = &( huff->decompressor.nodeList[huff->decompressor.blocNode++] );
	huff->decompressor.tree->symbol = NYT;
	huff->decompressor.tree->weight = 0;
	huff->decompressor.lhead->next = huff->decompressor.lhead->prev = NULL;
	huff->decompressor.tree->parent = huff->decompressor.tree->left = huff->decompressor.tree->right = NULL;

	// Add the NYT (not yet transmitted) node into the tree/list */
	huff->compressor.tree = huff->compressor.lhead = huff->compressor.loc[NYT] =  &( huff->compressor.nodeList[huff->compressor.blocNode++] );
	huff->compressor.tree->symbol = NYT;
	huff->compressor.tree->weight = 0;
	huff->compressor.lhead->next = huff->compressor.lhead->prev = NULL;
	huff->compressor.tree->parent = huff->compressor.tree->left = huff->compressor.tree->right = NULL;
	huff->compressor.loc[NYT] = huff->compressor.tree;
}
// end huffman.c

void MSG_initHuffman()
{
	int i,j;

	msgInit = qtrue;
	Huff_Init(&msgHuff);
	for(i=0; i<256; i++)
	{
		for (j=0; j<msg_hData[i]; j++)
		{
			Huff_addRef(&msgHuff.compressor,	(byte)i);			// Do update
			Huff_addRef(&msgHuff.decompressor,	(byte)i);			// Do update
		}
	}
}

void MSG_Init( msg_t *buf, byte *data, int length )
{
	if (!msgInit)
	{
		MSG_initHuffman();
	}
	buf->data = data;
	buf->maxsize = length;
	buf->overflowed = qfalse;
	buf->cursize = 0;
	buf->readcount = 0;
	buf->bit = 0;
}

int MSG_ReadBitsCompress(const byte* input, int readsize, byte* outputBuf, int outputBufSize)
{
	readsize = readsize * 8;
	byte *outptr = outputBuf;

	int get;
	int offset;
	int i;

	if(readsize <= 0)
	{
		return 0;
	}

	for(offset = 0, i = 0; offset < readsize && i < outputBufSize; i++)
	{
		Huff_offsetReceive( msgHuff.decompressor.tree, &get, (byte*)input, &offset);
		*outptr = (byte)get;
		outptr++;
	}
	return i;
}

int MSG_GetByte(msg_t *msg, int where)
{
	return msg->data[where];
}

int MSG_GetShort(msg_t *msg, int where)
{
	return *(int16_t*)&msg->data[where];
}

int MSG_GetLong(msg_t *msg, int where)
{
	return *(int32_t*)&msg->data[where];
}

int MSG_ReadByte( msg_t *msg )
{
	byte	c;

	if ( msg->readcount + 1 > msg->cursize )
	{
		msg->overflowed = 1;
		return -1;
	}
	c = MSG_GetByte(msg, msg->readcount);
	msg->readcount += sizeof(byte);
	return c;
}

int MSG_ReadShort( msg_t *msg )
{
	int16_t	c;

	if ( msg->readcount + 2 > msg->cursize )
	{
		msg->overflowed = 1;
		return -1;
	}
	c = MSG_GetShort(msg, msg->readcount);
	msg->readcount += sizeof(short);
	return c;
}

int32_t MSG_ReadLong( msg_t *msg )
{
	int32_t	c;

	if ( msg->readcount + 4 > msg->cursize )
	{
		msg->overflowed = 1;
		return -1;
	}
	c = MSG_GetLong(msg, msg->readcount);
	msg->readcount += sizeof(int32_t);
	return c;
}

#define BIG_INFO_STRING 8192
char *MSG_ReadBigString( msg_t *msg )
{
	static char string[BIG_INFO_STRING];
	int l,c;

	l = 0;
	do
	{
		c = MSG_ReadByte( msg );      // use ReadByte so -1 is out of bounds
		if ( c == -1 || c == 0 )
		{
			break;
		}
		// translate all fmt spec to avoid crash bugs
		if ( c == '%' )
		{
			c = '.';
		}

		string[l] = c;
		l++;
	}
	while ( l < (int)sizeof( string ) - 1 );

	string[l] = 0;

	return string;
}

int MSG_ReadBits(msg_t* msg, int bits)
{
	int i;
	int bit;
	int value;

	value = 0;

	for (i = 0; i < bits; ++i)
	{
		bit = msg->bit & 7;

		if (!bit)
		{
			if (msg->readcount >= msg->cursize)
			{
				msg->overflowed = qtrue;
				return -1;
			}

			msg->bit = 8 * msg->readcount++;
		}

		value |= ((msg->data[msg->bit++ >> 3] >> bit) & 1) << i;
	}

	return value;
}

int MSG_ReadBit(msg_t* msg)
{
	int bit;

	bit = msg->bit & 7;

	if (!bit)
	{
		if (msg->readcount >= msg->cursize)
		{
			msg->overflowed = qtrue;
			return -1;
		}

		msg->bit = 8 * msg->readcount++;
	}

	return (msg->data[msg->bit++ >> 3] >> bit) & 1;
}

void MSG_ReadData(msg_t *msg, void *data, int len)
{
	int size = msg->readcount + len;

	if ( size > msg->cursize )
	{
		msg->overflowed = qtrue;
		Com_Memset(data, -1, len);
	}
	else
	{
		Com_Memcpy(data, &msg->data[msg->readcount], len);
		msg->readcount = size;
	}
}


/* reconstruct a stored snapshot from the parse rings */
qboolean CL_GetSnapshot( int snapshotNumber, snapshot_t *snapshot ) {
	clSnapshot_t    *clSnap;
	int i, count;

	if ( snapshotNumber > cl.snap.messageNum ) {
		Com_Error( ERR_DROP, "CL_GetSnapshot: snapshotNumber > cl.snapshot.messageNum" );
	}

	// frame fell out of the circular buffer
	if ( cl.snap.messageNum - snapshotNumber >= PACKET_BACKUP ) {
		return qfalse;
	}

	clSnap = &cl.snapshots[snapshotNumber & PACKET_MASK];
	if ( !clSnap->valid ) {
		return qfalse;
	}

	// entities fell out of their circular buffer
	if ( cl.parseEntitiesNum - clSnap->parseEntitiesNum >= MAX_PARSE_ENTITIES ) {
		return qfalse;
	}

	snapshot->snapFlags = clSnap->snapFlags;
	snapshot->serverCommandSequence = clSnap->serverCommandNum;
	snapshot->ping = clSnap->ping;
	snapshot->serverTime = clSnap->serverTime;
	memcpy( snapshot->areamask, clSnap->areamask, sizeof( snapshot->areamask ) );
	snapshot->ps = clSnap->ps;

	count = clSnap->numEntities;
	if ( count > MAX_ENTITIES_IN_SNAPSHOT ) {
		count = MAX_ENTITIES_IN_SNAPSHOT;
	}

	snapshot->numEntities = count;
	for ( i = 0 ; i < count ; i++ ) {
		snapshot->entities[i] =
			cl.parseEntities[ ( clSnap->parseEntitiesNum + i ) & ( MAX_PARSE_ENTITIES - 1 ) ];
	}

	count = clSnap->numClients;
	snapshot->numClients = count;
	for ( i = 0 ; i < count ; i++ ) {
		snapshot->clients[i] =
			cl.parseClients[ ( clSnap->parseClientsNum + i ) & ( MAX_PARSE_CLIENTS - 1 ) ];
	}

	return qtrue;
}

float MSG_ReadAngle16(msg_t* msg)
{
	return SHORT2ANGLE((float)MSG_ReadShort(msg));
}

void MSG_ReadDeltaField(msg_t* msg, const void* from, const void* to, netField_t* field, qboolean print)
{
	int* fromF, * toF;
	int32_t readbits;
	int32_t readbyte;
	uint32_t unsignedbits;
	int i;

	fromF = (int32_t*)((byte*)from + field->offset);
	toF = (int32_t*)((byte*)to + field->offset);

	if (!MSG_ReadBit(msg))
	{
		*toF = *fromF;
		return;
	}

	switch (field->bits)
	{
	case 0:
		if (!MSG_ReadBit(msg))
		{
			*toF = 0;
			return;
		}
		if (MSG_ReadBit(msg) == 0)
		{
			readbits = MSG_ReadBits(msg, 5);
			readbyte = 32 * MSG_ReadByte(msg) + readbits - FLOAT_INT_BIAS;
			*(float*)toF = (float)readbyte;
			if (print)
				Com_Printf("%s:%i ", field->name, readbyte);
		}
		else
		{
			*toF = MSG_ReadLong(msg);
			if (print)
				Com_Printf("%s:%f ", field->name, *(float*)toF);
		}
		return;

	case -99:
		if (!MSG_ReadBit(msg))
		{
			*toF = 0;
			return;
		}
		if (MSG_ReadBit(msg) == 0)
		{
			readbits = MSG_ReadBits(msg, 2);
			readbyte = 4 * MSG_ReadByte(msg) + readbits - HUDELEM_COORD_BIAS;
			*(float*)toF = (float)readbyte;
			if (print)
				Com_Printf("%s:%i ", field->name, readbyte);
		}
		else
		{
			*toF = MSG_ReadLong(msg);
			if (print)
				Com_Printf("%s:%f ", field->name, *(float*)toF);
		}
		return;

	case -100:
		if (!MSG_ReadBit(msg))
		{
			*toF = 0;
			return;
		}
		*(float*)toF = MSG_ReadAngle16(msg);
		return;

	default:
		if (!MSG_ReadBit(msg))
		{
			*toF = 0;
			return;
		}
		unsignedbits = (unsigned int)field->bits >> 31;
		readbits = abs(field->bits);
		if ((readbits & 7) != 0)
			readbyte = MSG_ReadBits(msg, readbits & 7);
		else
			readbyte = 0;
		for (i = readbits & 7; i < readbits; i += 8)
			readbyte |= MSG_ReadByte(msg) << i;
		if (unsignedbits && ((readbyte >> (readbits - 1)) & 1) != 0)
			readbyte |= ~((1 << readbits) - 1);
		*toF = readbyte;
		if (print)
			Com_Printf("%s:%i ", field->name, *toF);
		return;
	}
}

int MSG_ReadDeltaStruct(msg_t* msg, const void* from, void* to, unsigned int number, int numFields, int indexBits, netField_t* stateFields)
{
	netField_t* field;
	int lc;
	int i;

	// check for a remove
	if (MSG_ReadBit(msg) == 1) {
		return 1;
	}

	// check for no delta
	if (MSG_ReadBit(msg) == 0)
	{
		Com_Memcpy(to, from, 4 * numFields + 4);
		return 0;
	}

	lc = MSG_ReadByte(msg);

	if (lc > numFields)
	{
		msg->overflowed = qtrue;
		return 0;
	}

	*(uint32_t*)to = number;

	for (i = 0, field = stateFields; i < lc; i++, field++)
	{
		MSG_ReadDeltaField(msg, from, to, field, qfalse);
	}

	for (i = lc, field = &stateFields[lc]; i < numFields; i++, field++)
	{
		*(uint32_t*)((byte*)to + field->offset) = *(uint32_t*)((byte*)from + field->offset);
	}

	return 0;
}

int MSG_ReadDeltaEntity(msg_t* msg, entityState_t* from, entityState_t* to, int number)
{
	return MSG_ReadDeltaStruct(msg, from, to, number, COUNT_OF(entityStateFields), 10, entityStateFields);
}

int MSG_ReadDeltaClient(msg_t *msg, clientState_t *from, clientState_t *to, int number)
{
	clientState_t nullstate;

	if ( !from )
	{
		from = &nullstate;
		Com_Memset(&nullstate, 0, sizeof(nullstate));
	}

	return MSG_ReadDeltaStruct(msg, from, to, number, COUNT_OF(clientStateFields), 6, clientStateFields);
}


void MSG_ReadDeltaHudElems(msg_t *msg, hudelem_t *from, hudelem_t *to, int count)
{
	int inuse, lc, i, j;

	inuse = MSG_ReadBits(msg, HUDELEM_BITS);

	for ( i = 0; i < inuse; ++i )
	{
		lc = MSG_ReadBits(msg, HUDELEM_BITS);

		for ( j = 0; j <= lc; ++j )
		{
			MSG_ReadDeltaField(msg, &from[i], &to[i], &hudElemFields[j], qfalse);
		}

		for ( ; j < (int)COUNT_OF(hudElemFields); ++j )
		{
			*(int32_t *)((byte *)(to + i) + hudElemFields[j].offset) = *(int32_t *)((byte *)(from + i) + hudElemFields[j].offset);
		}
	}

	while ( inuse < count && to[inuse].type )
	{
		memset(&to[inuse], 0, sizeof(hudelem_t));
		//assert(to[inuse].type == HE_TYPE_FREE);
		++inuse;
	}
}

void MSG_ReadDeltaObjective(msg_t *msg, objective_t *from, objective_t *to, int numFields, netField_t *objFields)
{
	int i = 0;

	if ( MSG_ReadBit(msg) )
	{
		for ( i = 0; i < numFields; i++ )
			MSG_ReadDeltaField(msg, from, to, &objFields[i], qfalse);
	}
	else
	{
		for ( i = 0; i < numFields; i++ )
		{
			*(int32_t *)((byte *)to + objFields[i].offset) = *(int32_t *)((byte *)from + objFields[i].offset);
		}
	}
}

void MSG_ReadDeltaPlayerstate(msg_t *msg, playerState_t *from, playerState_t *to)
{
	int *fromF, *toF;
	netField_t *field;
	playerState_t nullstate;
	int readbits;
	int readbyte;
	uint32_t unsignedbits;
	int i, j, k, lc;

	if ( !from )
	{
		from = &nullstate;
		Com_Memset(&nullstate, 0, sizeof(nullstate));
	}

	Com_Memcpy(to, from, sizeof(playerState_t));

	lc = MSG_ReadByte( msg );

	for ( i = 0, field = playerStateFields ; i < lc ; i++, field++ )
	{
		fromF = ( int32_t * )( (byte *)from + field->offset );
		toF = ( int32_t * )( (byte *)to + field->offset );

		if ( !MSG_ReadBit(msg) )
		{
			*toF = *fromF;
			continue;
		}

		switch (field->bits)
		{
		case 0:
			if ( MSG_ReadBit(msg) )
			{
				*toF = MSG_ReadLong(msg);
			}
			else
			{
				readbits = MSG_ReadBits(msg, 5);
				readbyte = (MSG_ReadByte(msg) << 5) + readbits - FLOAT_INT_BIAS;
				*(float *)toF = (float)readbyte;
			}
			break;

		case -100:
			if ( MSG_ReadBit(msg) )
				*(float *)toF = MSG_ReadAngle16(msg);
			else
				*toF = 0;
			break;

		default:
			unsignedbits = (unsigned int)field->bits >> 31;
			readbits = abs(field->bits);
			if ( (readbits & 7) != 0 )
				readbyte = MSG_ReadBits(msg, readbits & 7);
			else
				readbyte = 0;
			for ( k = readbits & 7; k < readbits; k += 8 )
				readbyte |= MSG_ReadByte(msg) << k;
			if ( unsignedbits && ((readbyte >> (readbits - 1)) & 1) != 0 )
				readbyte |= ~((1 << readbits) - 1);
			*toF = readbyte;
			break;
		}
	}

	for ( i = lc, field = &playerStateFields[lc]; i < (int)COUNT_OF(playerStateFields); i++, field++ )
	{
		fromF = ( int32_t * )( (byte *)from + field->offset );
		toF = ( int32_t * )( (byte *)to + field->offset );

		*toF = *fromF;
	}

	// stats
	int statsbits = 0;


	if ( MSG_ReadBit( msg ) ) {  // one general bit tells if any of this infrequently changing stuff has changed
		statsbits = MSG_ReadBits(msg, MAX_STATS);

		if ( (statsbits & 1) != 0 )
			to->stats[STAT_HEALTH] = MSG_ReadShort(msg);

		if ( (statsbits & 2) != 0 )
			to->stats[STAT_DEAD_YAW] = MSG_ReadShort(msg);

		if ( (statsbits & 4) != 0 )
			to->stats[STAT_MAX_HEALTH] = MSG_ReadShort(msg);

		if ( (statsbits & 8) != 0 )
			to->stats[STAT_IDENT_CLIENT_NUM] = MSG_ReadBits(msg, GCLIENTNUM_BITS);

		if ( (statsbits & 0x10) != 0 )
			to->stats[STAT_IDENT_CLIENT_HEALTH] = MSG_ReadShort(msg);

		if ( (statsbits & 0x20) != 0 )
			to->stats[STAT_SPAWN_COUNT] = MSG_ReadByte(msg);
	}


	// ammo stored
	if ( MSG_ReadBit(msg) )
	{
		for ( j = 0; j < 4; j++ )
		{
			if ( MSG_ReadBit(msg) )
			{
				int ammobits = MSG_ReadShort(msg);

				for ( i = 0; i < 16; i++ )
				{
					if ( ammobits & ( 1 << i ) )
					{
						to->ammo[i + ( j * 16 )] = MSG_ReadShort(msg);
					}
				}
			}
		}
	}

	// ammo in clip
	for ( j = 0; j < 4; j++ )
	{
		if ( MSG_ReadBit(msg) )
		{
			int clipbits = MSG_ReadShort(msg);

			for ( i = 0; i < 16; i++ )
			{
				if ( clipbits & ( 1 << i ) )
				{
					to->ammoclip[i + ( j * 16 )] = MSG_ReadShort(msg);
				}
			}
		}
	}

	// Objectives
	if ( MSG_ReadBit(msg) )
	{
		for ( i = 0; i < MAX_OBJECTIVES; ++i )
		{
			to->objective[i].state = MSG_ReadBits(msg, 3);
			MSG_ReadDeltaObjective(msg, &from->objective[i], &to->objective[i], COUNT_OF(objectiveFields), objectiveFields);
		}
	}

	// Hud-elements
	if ( MSG_ReadBit(msg) )
	{
		MSG_ReadDeltaHudElems(msg, from->hud.archival, to->hud.archival, MAX_HUDELEMS_ARCHIVAL);
		MSG_ReadDeltaHudElems(msg, from->hud.current, to->hud.current, MAX_HUDELEMS_CURRENT);
	}
}


void CL_DeltaEntity( msg_t *msg, clSnapshot_t *frame, int newnum, entityState_t *old,
					 qboolean unchanged ) {
	
	entityState_t   *state = NULL;
	int removed;

	// save the parsed entity state into the big circular buffer so
	// it can be used as the source for a later delta
	state = &cl.parseEntities[cl.parseEntitiesNum & ( MAX_PARSE_ENTITIES - 1 )];


	if ( unchanged ) {
		*state = *old;
		removed = 0;
	} else {
		removed = MSG_ReadDeltaEntity( msg, old, state, newnum );
	}

	if ( removed ) {
		return;     // entity was delta removed
	}

	cl.parseEntitiesNum++;
	frame->numEntities++;
}


void CL_DeltaClient( msg_t *msg, clSnapshot_t *frame, int newnum, clientState_t *old,
					 qboolean unchanged ) {
	
	clientState_t   *state = NULL;
	int removed;

	// save the parsed entity state into the big circular buffer so
	// it can be used as the source for a later delta
	state = &cl.parseClients[cl.parseClientsNum & ( MAX_PARSE_CLIENTS - 1 )];


	if ( unchanged ) {
		*state = *old;
		removed = 0;
	} else {
		removed = MSG_ReadDeltaClient( msg, old, state, newnum );
	}

	if ( removed ) {
		return;     // client was delta removed
	}

	cl.parseClientsNum++;
	frame->numClients++;
}


void CL_ParsePacketEntities( msg_t *msg, clSnapshot_t *oldframe, clSnapshot_t *newframe ) {
	int newnum;
	entityState_t   *oldstate;
	int oldindex, oldnum;

	newframe->parseEntitiesNum = cl.parseEntitiesNum;
	newframe->numEntities = 0;

	// delta from the entities present in oldframe
	oldindex = 0;
	oldstate = NULL;
	if ( !oldframe ) {
		oldnum = 99999;
	} else {
		if ( oldindex >= oldframe->numEntities ) {
			oldnum = 99999;
		} else {
			oldstate = &cl.parseEntities[
				( oldframe->parseEntitiesNum + oldindex ) & ( MAX_PARSE_ENTITIES - 1 )];
			oldnum = oldstate->number;
		}
	}

	while ( 1 ) {
		// read the entity index number
		newnum = MSG_ReadBits( msg, GENTITYNUM_BITS );

		if ( newnum == ( MAX_GENTITIES - 1 ) ) {
			break;
		}

		if ( msg->readcount > msg->cursize ) {
			Com_Error( ERR_DROP,"CL_ParsePacketEntities: end of message" );
		}

		while ( oldnum < newnum ) {
			// unchanged entities copied from old packet
			CL_DeltaEntity( msg, newframe, oldnum, oldstate, qtrue );

			oldindex++;

			if ( oldindex >= oldframe->numEntities ) {
				oldnum = 99999;
			} else {
				oldstate = &cl.parseEntities[
					( oldframe->parseEntitiesNum + oldindex ) & ( MAX_PARSE_ENTITIES - 1 )];
				oldnum = oldstate->number;
			}
		}
		if ( oldnum == newnum ) {
			// delta from previous state
			CL_DeltaEntity( msg, newframe, newnum, oldstate, qfalse );

			oldindex++;

			if ( oldindex >= oldframe->numEntities ) {
				oldnum = 99999;
			} else {
				oldstate = &cl.parseEntities[
					( oldframe->parseEntitiesNum + oldindex ) & ( MAX_PARSE_ENTITIES - 1 )];
				oldnum = oldstate->number;
			}
			continue;
		}

		if ( oldnum > newnum ) {
			// delta from baseline
			CL_DeltaEntity( msg, newframe, newnum, &cl.entityBaselines[newnum], qfalse );
			continue;
		}

	}

	// any remaining entities in the old frame are copied over
	while ( oldnum != 99999 ) {
		CL_DeltaEntity( msg, newframe, oldnum, oldstate, qtrue );

		oldindex++;

		if ( oldindex >= oldframe->numEntities ) {
			oldnum = 99999;
		} else {
			oldstate = &cl.parseEntities[
				( oldframe->parseEntitiesNum + oldindex ) & ( MAX_PARSE_ENTITIES - 1 )];
			oldnum = oldstate->number;
		}
	}

}

void CL_ParsePacketClients( msg_t *msg, clSnapshot_t *oldframe, clSnapshot_t *newframe )
{
	int newnum, clientAvaible;
	clientState_t   *oldstate;
	int oldindex, oldnum;

	newframe->parseClientsNum = cl.parseClientsNum;
	newframe->numClients = 0;

	// delta from the Clients present in oldframe
	oldindex = 0;
	oldstate = NULL;
	if ( !oldframe ) {
		oldnum = 99999;
	} else {
		if ( oldindex >= oldframe->numClients ) {
			oldnum = 99999;
		} else {
			oldstate = &cl.parseClients[
				( oldframe->parseClientsNum + oldindex ) & ( PARSE_CLIENTS_MASK )];
			oldnum = oldstate->number;
		}
	}

	while ( 1 ) {

		// next client present?
		clientAvaible = MSG_ReadBit( msg );
      	if (clientAvaible == 0) {
			break;
		}

		// client slot number; clients run until the 0 bit terminator
		newnum = MSG_ReadBits( msg, GCLIENTNUM_BITS );

		if ( msg->readcount > msg->cursize ) {
			Com_Error( ERR_DROP,"CL_ParsePacketClients: end of message" );
		}

		while ( oldnum < newnum ) {
			// unchanged entities copied from old packet
			CL_DeltaClient( msg, newframe, oldnum, oldstate, qtrue );

			oldindex++;

			if ( oldindex >= oldframe->numClients ) {
				oldnum = 99999;
			} else {
				oldstate = &cl.parseClients[
					( oldframe->parseClientsNum + oldindex ) & ( PARSE_CLIENTS_MASK )];
				oldnum = oldstate->number;
			}
		}
		if ( oldnum == newnum ) {
			// delta from previous state
			CL_DeltaClient( msg, newframe, newnum, oldstate, qfalse );

			oldindex++;

			if ( oldindex >= oldframe->numClients ) {
				oldnum = 99999;
			} else {
				oldstate = &cl.parseClients[
					( oldframe->parseClientsNum + oldindex ) & ( PARSE_CLIENTS_MASK )];
				oldnum = oldstate->number;
			}
			continue;
		}

		if ( oldnum > newnum ) {
			// delta from baseline
			CL_DeltaClient( msg, newframe, newnum, &cl.clientBaselines[newnum], qfalse );
			continue;
		}

	}

	// any remaining entities in the old frame are copied over
	while ( oldnum != 99999 ) {
		CL_DeltaClient( msg, newframe, oldnum, oldstate, qtrue );

		oldindex++;

		if ( oldindex >= oldframe->numClients ) {
			oldnum = 99999;
		} else {
			oldstate = &cl.parseClients[
				( oldframe->parseClientsNum + oldindex ) & ( PARSE_CLIENTS_MASK )];
			oldnum = oldstate->number;
		}
	}

}


void CL_ParseSnapshot( msg_t *msg ) {
	clSnapshot_t    *old;
	clSnapshot_t newSnap;
	int deltaNum;
	int oldMessageNum;


	// get the reliable sequence acknowledge number
	// NOTE: now sent with all server to client messages
	//clc.reliableAcknowledge = MSG_ReadLong( msg );

	// read in the new snapshot to a temporary buffer
	// we will only copy to cl.snap if it is valid
	memset( &newSnap, 0, sizeof( newSnap ) );

	// we will have read any new server commands in this
	// message before we got to svc_snapshot
	newSnap.serverCommandNum = clc.serverCommandSequence;

	newSnap.serverTime = MSG_ReadLong( msg );

	newSnap.messageNum = clc.serverMessageSequence;

	deltaNum = MSG_ReadByte( msg );
	if ( !deltaNum ) {
		newSnap.deltaNum = -1;
	} else {
		newSnap.deltaNum = newSnap.messageNum - deltaNum;
	}
	newSnap.snapFlags = MSG_ReadByte( msg );

	// If the frame is delta compressed from data that we
	// no longer have available, we must suck up the rest of
	// the frame, but not use it, then ask for a non-compressed
	// message
	if ( newSnap.deltaNum <= 0 ) {
		newSnap.valid = qtrue;      // uncompressed frame
		old = NULL;
	} else {
		
		old = &cl.snapshots[newSnap.deltaNum & PACKET_MASK];
		if ( !old->valid ) {
			// should never happen
			Com_Printf( "Delta from invalid frame (not supposed to happen!).\n" );
		} else if ( old->messageNum != newSnap.deltaNum ) {
			// The frame that the server did the delta from
			// is too old, so we can't reconstruct it properly.
			Com_DPrintf( "Delta frame too old.\n" );
		} else if ( cl.parseEntitiesNum - old->parseEntitiesNum > MAX_PARSE_ENTITIES - 128 ) {
			Com_DPrintf( "Delta parseEntitiesNum too old.\n" );
		} else if ( cl.parseClientsNum - old->parseClientsNum > MAX_PARSE_CLIENTS - 128 ) {
			Com_DPrintf( "Delta parseClientsNum too old.\n" );
		} else {
			newSnap.valid = qtrue;  // valid delta parse
		}
	}

	// cod2 snapshots carry no areamask

	// read playerinfo
	if ( old ) {
		MSG_ReadDeltaPlayerstate( msg, &old->ps, &newSnap.ps );
	} else {
		MSG_ReadDeltaPlayerstate( msg, NULL, &newSnap.ps );
	}

	// read packet entities
	CL_ParsePacketEntities( msg, old, &newSnap );

	// read packet clients
	CL_ParsePacketClients(msg, old, &newSnap);


	// if not valid, dump the entire thing now that it has
	// been properly read
	if ( !newSnap.valid ) {
		return;
	}


	// clear the valid flags of any snapshots between the last
	// received and this one, so if there was a dropped packet
	// it won't look like something valid to delta from next
	// time we wrap around in the buffer
	oldMessageNum = cl.snap.messageNum + 1;

	if ( newSnap.messageNum - oldMessageNum >= PACKET_BACKUP ) {
		oldMessageNum = newSnap.messageNum - ( PACKET_BACKUP - 1 );
	}
	for ( ; oldMessageNum < newSnap.messageNum ; oldMessageNum++ ) {
		cl.snapshots[oldMessageNum & PACKET_MASK].valid = qfalse;
	}

	// copy to the current good spot
	cl.snap = newSnap;
	cl.snap.ping = 999;

	// save the frame off in the backup array for later delta comparisons
	cl.snapshots[cl.snap.messageNum & PACKET_MASK] = cl.snap;

	cl.newSnapshots = qtrue;
}


void CL_ParseGamestate( msg_t *msg )
{
	int i;
	entityState_t   *es;
	int newnum;
	entityState_t nullstate;
	int cmd;
	char            *s;

	// a gamestate always marks a server command sequence
	clc.serverCommandSequence = MSG_ReadLong( msg );

	// parse all the configstrings and baselines
	cl.gameState.dataCount = 1; // leave a 0 at the beginning for uninitialized configstrings
	while ( 1 )
	{
		cmd = MSG_ReadByte( msg );

		if ( cmd == svc_EOF )
		{
			break;
		}

		if ( cmd == svc_configstring )
		{
			int len;

			i = MSG_ReadShort( msg );
			if ( i < 0 || i >= MAX_CONFIGSTRINGS )
			{
				Com_Error( ERR_DROP, "configstring > MAX_CONFIGSTRINGS" );
			}
			s = MSG_ReadBigString( msg );

			len = strlen( s );

			if ( logFile )
			{
				fprintf( logFile, "    configstring[%i]: %s\n", i, s );
			}

			if ( len + 1 + cl.gameState.dataCount > MAX_GAMESTATE_CHARS )
			{
				Com_Error( ERR_DROP, "MAX_GAMESTATE_CHARS exceeded" );
			}

			// append it to the gameState string buffer
			cl.gameState.stringOffsets[ i ] = cl.gameState.dataCount;
			memcpy( cl.gameState.stringData + cl.gameState.dataCount, s, len + 1 );
			cl.gameState.dataCount += len + 1;

			CL_ReplaceConfigstring( i, s );

		}
		else if ( cmd == svc_baseline )
		{
			newnum = MSG_ReadBits( msg, GENTITYNUM_BITS );
			if ( newnum < 0 || newnum >= MAX_GENTITIES )
			{
				Com_Error( ERR_DROP, "Baseline number out of range: %i", newnum );
			}
			memset( &nullstate, 0, sizeof( nullstate ) );
			es = &cl.entityBaselines[ newnum ];
			MSG_ReadDeltaEntity( msg, &nullstate, es, newnum );
		}
		else
		{
			Com_Error( ERR_DROP, "CL_ParseGamestate: bad command byte" );
		}
	}

	clc.clientNum = MSG_ReadLong( msg );
	// read the checksum feed
	clc.checksumFeed = MSG_ReadLong( msg );

	demo.gameStatesParsed++;

	Export_Reset();

	memset( clScores, 0, sizeof( clScores ) );
	memset( clDeaths, 0, sizeof( clDeaths ) );
	memset( clPings, 0, sizeof( clPings ) );
	clScoreAxis = 0;
	clScoreAllies = 0;

	CL_ParseWeaponList();
}


/*
=====================
CL_ParseCommandString

Command strings are just saved off until cgame asks for them
when it transitions a snapshot
=====================
*/
void CL_ParseCommandString( msg_t *msg ) {
	char	*s;
	int		seq;
	int		index;

	seq = MSG_ReadLong( msg );
	s = MSG_ReadBigString( msg );

	// see if we have already executed stored it off
	if ( clc.serverCommandSequence >= seq ) {
		return;
	}
	clc.serverCommandSequence = seq;

	index = seq & (MAX_RELIABLE_COMMANDS-1);
	Q_strncpyz( clc.serverCommands[ index ], s, sizeof( clc.serverCommands[ index ] ) );

	CL_ProcessServerCommand( s );
}


int CL_ParseServerMessage( msg_t *msg )
{
	int cmd;
	static byte buffer[MAX_MSGLEN];
	msg_t decompressMsg;

	// get the reliable sequence acknowledge number
	clc.reliableAcknowledge = MSG_ReadLong( msg );

	// if the server has not acknowledged most of our reliable commands, skip the message
	if ( clc.reliableAcknowledge < clc.reliableSequence - MAX_RELIABLE_COMMANDS )
	{
		clc.reliableAcknowledge = clc.reliableSequence;
		return 1;
	}

	MSG_Init(&decompressMsg, buffer, sizeof(buffer));
	decompressMsg.cursize = MSG_ReadBitsCompress(msg->data + msg->readcount, msg->cursize - msg->readcount, decompressMsg.data, decompressMsg.maxsize);

	//
	// parse the message
	//
	while ( 1 )
	{
		if ( decompressMsg.readcount > decompressMsg.cursize )
		{
			Com_Error( ERR_DROP,"CL_ParseServerMessage: read past end of server message" );
			return 0;
		}

		cmd = MSG_ReadByte( &decompressMsg );

		if ( cmd == svc_EOF )
		{
			return 1;
		}

		// other commands
		switch ( cmd )
		{
		default:
			Com_Error( ERR_DROP,"CL_ParseServerMessage: Illegible server message %d\n", cmd );
			return 0;

		case svc_nop:
			break;
		case svc_serverCommand:
			CL_ParseCommandString( &decompressMsg );
			break;
		case svc_gamestate:
			CL_ParseGamestate( &decompressMsg );
			break;
		case svc_snapshot:
			CL_ParseSnapshot( &decompressMsg );
			break;
		case svc_download:
		{
			static byte dlbuf[MAX_MSGLEN];
			int blk = MSG_ReadShort( &decompressMsg );

			if ( blk == -1 )
			{
				MSG_ReadBigString( &decompressMsg );
				MSG_ReadLong( &decompressMsg );
				MSG_ReadLong( &decompressMsg );
				break;
			}

			if ( blk == 0 )
			{
				int dlblock = MSG_ReadLong( &decompressMsg );

				if ( dlblock < 0 )
				{
					MSG_ReadBigString( &decompressMsg );
					break;
				}
			}

			blk = MSG_ReadShort( &decompressMsg );
			if ( blk > 0 )
			{
				MSG_ReadData( &decompressMsg, dlbuf, blk );
			}
			break;
		}
		}
	}

	return 0;
}

int FS_Read( void *buffer, int len, FILE *f )
{
	return fread( buffer, 1, len, f );
}

int CL_ReadDemoMessage( void )
{
	int r;
	msg_t buf;
	static byte bufData[ MAX_MSGLEN ];
	int s;

	if ( !demo.demofile )
	{
		return 0;
	}

	// get the sequence number
	r = FS_Read( &s, 4, demo.demofile );
	if ( r != 4 )
	{
		return 0;
	}

	clc.serverMessageSequence = LittleLong( s );

	// init the message
	MSG_Init( &buf, bufData, sizeof( bufData ) );

	// get the length
	r = FS_Read( &buf.cursize, 4, demo.demofile );

	if ( r != 4 )
	{
		return 0;
	}
	buf.cursize = LittleLong( buf.cursize );
	if ( buf.cursize == -1 )
	{
		return 0;
	}
	if ( buf.cursize > buf.maxsize )
	{
		Com_Error( ERR_DROP, "CL_ReadDemoMessage: demoMsglen > MAX_MSGLEN" );
	}
	r = FS_Read( buf.data, buf.cursize, demo.demofile );
	if ( r != buf.cursize )
	{
		Com_Printf( "Demo file was truncated.\n" );
		return 0;
	}

	return CL_ParseServerMessage( &buf );
}

int FS_FOpenFileRead( const char *filename, FILE **file, qboolean uniqueFILE )
{
	*file = fopen( filename, "rb" );
	return ( *file != NULL );
}

static char *CL_GetConfigstring( int index )
{
	if ( index < 0 || index >= MAX_CONFIGSTRINGS || !clCs[index] )
	{
		return "";
	}
	return clCs[index];
}

static void CL_ReplaceConfigstring( int index, const char *val )
{
	char *copy;

	if ( index < 0 || index >= MAX_CONFIGSTRINGS )
	{
		return;
	}

	copy = (char *)malloc( strlen( val ) + 1 );
	if ( !copy )
	{
		Com_Error( ERR_DROP, "CL_ReplaceConfigstring: out of memory" );
	}
	strcpy( copy, val );

	if ( clCs[index] )
	{
		free( clCs[index] );
	}
	clCs[index] = copy;
}

static void Info_ValueForKey( const char *s, const char *key, char *out, int outLen )
{
	char keybuf[64];
	const char *p;
	int kl;

	out[0] = 0;
	if ( !s || !key )
	{
		return;
	}

	kl = strlen( key );
	if ( kl > (int)sizeof( keybuf ) - 1 )
	{
		kl = sizeof( keybuf ) - 1;
	}
	strncpy( keybuf, key, kl );
	keybuf[kl] = 0;

	p = s;
	while ( *p && *p != '\\' )
		p++;

	while ( *p == '\\' )
	{
		const char *k, *v;
		int klen, vlen;

		p++;
		k = p;
		while ( *p && *p != '\\' )
			p++;
		klen = p - k;

		v = NULL;
		vlen = 0;
		if ( *p == '\\' )
		{
			p++;
			v = p;
			while ( *p && *p != '\\' )
				p++;
			vlen = p - v;
		}

		if ( klen == kl && strncasecmp( k, keybuf, kl ) == 0 && v )
		{
			if ( vlen >= outLen )
			{
				vlen = outLen - 1;
			}
			memcpy( out, v, vlen );
			out[vlen] = 0;
			return;
		}
	}
}

static void CL_ParseWeaponList( void )
{
	const char *s;
	int i;

	for ( i = 0; i < weaponNamesCount; i++ )
	{
		free( weaponNames[i] );
		weaponNames[i] = NULL;
	}
	weaponNamesCount = 0;

	s = CL_GetConfigstring( CS_WEAPONS );
	while ( *s && weaponNamesCount < (int)COUNT_OF( weaponNames ) )
	{
		const char *start = s;
		int len;
		char *name;

		while ( *s && *s != ' ' )
			s++;
		len = s - start;

		name = (char *)malloc( len + 1 );
		if ( !name )
		{
			Com_Error( ERR_DROP, "CL_ParseWeaponList: out of memory" );
		}
		memcpy( name, start, len );
		name[len] = 0;
		weaponNames[weaponNamesCount++] = name;

		while ( *s == ' ' )
			s++;
	}
}

static const char *CL_WeaponName( int weaponIndex )
{
	if ( weaponIndex <= 0 || weaponIndex > weaponNamesCount )
	{
		return "?";
	}
	return weaponNames[weaponIndex - 1];
}

// grenades/smoke/binocs etc. - throws do not count as shots
static qboolean Export_IsNonBallisticWeapon( int weaponIndex )
{
	const char *n = CL_WeaponName( weaponIndex );

	if ( strstr( n, "grenade" ) || strstr( n, "smoke" ) || strstr( n, "binocular" ) ||
		strstr( n, "satchel" ) || strstr( n, "tnt" ) || strstr( n, "tripwire" ) || strstr( n, "c4" ) )
	{
		return qtrue;
	}
	return qfalse;
}

static void CL_ConfigstringModified( const char *cmd )
{
	const char *p;
	int index;

	p = cmd + 1;
	while ( *p == ' ' )
		p++;

	index = atoi( p );
	if ( index < 0 || index >= MAX_CONFIGSTRINGS )
	{
		Com_Error( ERR_DROP, "configstring > MAX_CONFIGSTRINGS" );
	}

	while ( *p && *p != ' ' )
		p++;
	while ( *p == ' ' )
		p++;

	CL_ReplaceConfigstring( index, p );

	if ( index == CS_WEAPONS )
	{
		CL_ParseWeaponList();
	}
}

static int Cmd_ParseInt( const char **p )
{
	const char *s = *p;
	int neg = 0;
	int v = 0;

	while ( *s == ' ' )
		s++;
	if ( *s == '-' )
	{
		neg = 1;
		s++;
	}
	while ( *s >= '0' && *s <= '9' )
	{
		v = v * 10 + ( *s - '0' );
		s++;
	}
	*p = s;
	return neg ? -v : v;
}

static void CL_ParseScores( const char *s )
{
	const char *p = s + 2;
	int numSorted, k;

	numSorted = Cmd_ParseInt( &p );
	clScoreAxis = Cmd_ParseInt( &p );
	clScoreAllies = Cmd_ParseInt( &p );

	memset( clScores, 0, sizeof( clScores ) );
	memset( clDeaths, 0, sizeof( clDeaths ) );
	memset( clPings, 0, sizeof( clPings ) );

	for ( k = 0; k < numSorted && k < MAX_CLIENTS; k++ )
	{
		int cnum = Cmd_ParseInt( &p );
		int score = Cmd_ParseInt( &p );
		int ping;
		int deaths;

		ping = Cmd_ParseInt( &p );
		deaths = Cmd_ParseInt( &p );
		Cmd_ParseInt( &p ); // statusIcon

		if ( cnum >= 0 && cnum < MAX_CLIENTS )
		{
			clScores[cnum] = score;
			clDeaths[cnum] = deaths;
			clPings[cnum] = ping;
		}
	}
}

static void CL_ProcessServerCommand( const char *s )
{
	char c;

	if ( !s || !s[0] )
	{
		return;
	}

	c = s[0];
	if ( s[1] != ' ' )
	{
		return;
	}

	if ( c == 'd' )
	{
		int csIndex = atoi( s + 2 );

		CL_ConfigstringModified( s );

		if ( csIndex == CS_SERVERINFO )
		{
			Export_MatchInfoChanged( exportLastSnapshotTime );
		}
	}
	else if ( c == 'b' )
	{
		CL_ParseScores( s );
	}
	else if ( c == 'h' || c == 'i' )
	{
		Export_Chat( c, s );
	}
	else if ( c == 'f' )
	{
		const char *p = s + 1;
		char text[192];
		int len = 0;

		while ( *p == ' ' )
			p++;
		if ( *p == '"' )
		{
			p++;
		}
		while ( *p && *p != '"' && len < (int)sizeof( text ) - 1 )
		{
			text[len++] = *p++;
		}
		text[len] = 0;
		Export_Announcement( text );
	}
	else if ( c == 'x' || c == 'y' || c == 'z' )
	{
		const char *p = s + 2;
		char tmp[64];
		int i = 0;

		while ( *p && *p != ' ' && i < (int)sizeof( tmp ) - 1 )
		{
			tmp[i++] = *p++;
		}
		tmp[i] = 0;
		while ( *p == ' ' )
			p++;

		if ( c == 'x' )
		{
			snprintf( bigConfigString, sizeof( bigConfigString ), "d %s %s", tmp, p );
		}
		else
		{
			if ( (int)strlen( bigConfigString ) + (int)strlen( p ) + 1 >= (int)sizeof( bigConfigString ) )
			{
				Com_Error( ERR_DROP, "bcs exceeded BIG_INFO_STRING" );
			}
			strcat( bigConfigString, p );
			if ( c == 'z' )
			{
				CL_ConfigstringModified( bigConfigString );
				if ( atoi( bigConfigString + 2 ) == CS_SERVERINFO )
				{
					Export_MatchInfoChanged( exportLastSnapshotTime );
				}
				bigConfigString[0] = 0;
			}
		}
	}
}

// ============================ json export ============================

#define MAX_EXPORT_KILLS      65536
#define MAX_EXPORT_CHAT       65536
#define MAX_EXPORT_ROUNDS     256
#define MAX_ROUND_EVENTS      512
#define MAX_EXPORT_POVSAMPLES 400000
#define MAX_EXPORT_ENTSAMPLES 800000
#define MAX_EXPORT_GRENADES   8192

typedef struct
{
	int t;
	int round;
	int attacker;
	int victim;
	int weapon;
	int mod;
	int attackerTeam;
	int victimTeam;
	char attackerName[32];
	char victimName[32];
	char icon[32];
	float apos[3];
	float vpos[3];
} killRec_t;

typedef struct
{
	int t;
	int round;
	char type;
	char name[64];
	char color;
	char text[192];
	char team[16];
	int dead;
} chatRec_t;

typedef struct
{
	int t;
	char kind; // 's' score change, 'm' message
	char text[192];
	int score1;
	int score2;
} roundEventRec_t;

typedef struct
{
	int round;
	int ready;
	int startT;
	int endT;
	int score1;
	int score2;
	int hasScore;
	char team1[64];
	char team2[64];
	char endReason[64];
	roundEventRec_t events[MAX_ROUND_EVENTS];
	int eventCount;
} roundRec_t;

typedef struct
{
	int t;
	int round;
	float x, y, z;
	float vx, vy, vz;
	float pitch, yaw, roll;
	int weapon;
	int health;
	int eFlags;
	int pmFlags;
	int legsAnim;
	int torsoAnim;
	int weapAnim;
} povSample_t;

typedef struct
{
	int t;
	int round;
	int num;
	float x, y, z;
	float yaw, pitch;
	int weapon;
	int eFlags;
	int legsAnim;
	int torsoAnim;
} entSample_t;

typedef struct
{
	char name[32];
	int team;
	int model;
	int valid;
} rosterEntry_t;

typedef struct
{
	int entNum;
	int round;
	int spawnT;
	int lastT;
	int explodeT;
	float spawn[3];
	float vel[3];
	float last[3];
	float explodePos[3];
	int weapon;
	int bounces;
	int exploded;
	int thrower;
} grenadeRec_t;

static killRec_t exportKills[MAX_EXPORT_KILLS];
static int exportKillCount = 0;
static chatRec_t exportChat[MAX_EXPORT_CHAT];
static int exportChatCount = 0;
static roundRec_t exportRounds[MAX_EXPORT_ROUNDS];
static int exportRoundCount = 0;
static povSample_t exportPov[MAX_EXPORT_POVSAMPLES];
static int exportPovCount = 0;
static entSample_t exportEnts[MAX_EXPORT_ENTSAMPLES];
static int exportEntCount = 0;
static grenadeRec_t exportGrenades[MAX_EXPORT_GRENADES];
static int exportGrenadeCount = 0;
static int exportGrenadeOpen[MAX_GENTITIES];
static int exportShots[MAX_CLIENTS];
static int exportPrevClip[128];
static qboolean exportPrevClipValid = qfalse;
static int exportPrevPsWeapon = 0;
static int exportClipBind[128];	// clip slot -> weapon index, learned at runtime
static rosterEntry_t exportRoster[MAX_CLIENTS];
static float exportLastKnownPos[MAX_CLIENTS][3];
static int exportPrevPsEventSeq = 0;
static int exportEntPrevEventSeq[MAX_GENTITIES];
static byte exportEntPresentInPrev[MAX_GENTITIES];
static byte exportEntPrevEFlags2[MAX_GENTITIES];
static int exportFirstSnapshotTime = 0;
static int exportSnapshotCount = 0;
static char exportOutDir[1032];

static void Export_Path( char *dst, int size, const char *file )
{
	Q_strncpyz( dst, exportOutDir, size - 16 );
	Q_strncat( dst, file, size );
}

static const char *exportModNames[16] =
{
	"unknown",
	"pistol_bullet",
	"rifle_bullet",
	"grenade",
	"grenade_splash",
	"projectile",
	"projectile_splash",
	"melee",
	"headshot",
	"crush",
	"telefrag",
	"falling",
	"suicide",
	"trigger_hurt",
	"explosive",
	"bad"
};

static const char *exportTeamName( int team )
{
	switch ( team )
	{
		case TEAM_AXIS: return "axis";
		case TEAM_ALLIES: return "allies";
		case TEAM_SPECTATOR: return "spectator";
		default: return "none";
	}
}

static const unsigned short cp1250ToUnicode[128] =
{
	0x20AC, 0xFFFD, 0x201A, 0xFFFD, 0x201E, 0x2026, 0x2020, 0x2021,
	0xFFFD, 0x2030, 0x0160, 0x2039, 0x015A, 0x0164, 0x017D, 0x0179,
	0xFFFD, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,
	0xFFFD, 0x2122, 0x0161, 0x203A, 0x015B, 0x0165, 0x017E, 0x017A,
	0x00A0, 0x02C7, 0x02D8, 0x0141, 0x00A4, 0x0104, 0x00A6, 0x00A7,
	0x00A8, 0x00A9, 0x015E, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x017B,
	0x00B0, 0x00B1, 0x02DB, 0x0142, 0x00B4, 0x00B5, 0x00B6, 0x00B7,
	0x00B8, 0x0105, 0x015F, 0x00BB, 0x013D, 0x02DD, 0x013E, 0x017C,
	0x0154, 0x00C1, 0x00C2, 0x0102, 0x00C4, 0x0139, 0x0106, 0x00C7,
	0x010C, 0x00C9, 0x0118, 0x00CB, 0x011A, 0x00CD, 0x00CE, 0x010E,
	0x0110, 0x0143, 0x0147, 0x00D3, 0x00D4, 0x0150, 0x00D6, 0x00D7,
	0x0158, 0x016E, 0x00DA, 0x0170, 0x00DC, 0x00DD, 0x0162, 0x00DF,
	0x0155, 0x00E1, 0x00E2, 0x0103, 0x00E4, 0x013A, 0x0107, 0x00E7,
	0x010D, 0x00E9, 0x0119, 0x00EB, 0x011B, 0x00ED, 0x00EE, 0x010F,
	0x0111, 0x0144, 0x0148, 0x00F3, 0x00F4, 0x0151, 0x00F6, 0x00F7,
	0x0159, 0x016F, 0x00FA, 0x0171, 0x00FC, 0x00FD, 0x0163, 0x02D9
};

static void jsonWriteUtf8( FILE *f, unsigned int cp )
{
	if ( cp < 0x80 )
	{
		fputc( cp, f );
	}
	else if ( cp < 0x800 )
	{
		fputc( 0xC0 | ( cp >> 6 ), f );
		fputc( 0x80 | ( cp & 0x3F ), f );
	}
	else
	{
		fputc( 0xE0 | ( cp >> 12 ), f );
		fputc( 0x80 | ( ( cp >> 6 ) & 0x3F ), f );
		fputc( 0x80 | ( cp & 0x3F ), f );
	}
}

static void jsonWriteEscaped( FILE *f, const char *s )
{
	unsigned char c;

	fputc( '"', f );
	for ( ; *s; s++ )
	{
		c = (unsigned char)*s;
		switch ( c )
		{
			case '"': fputs( "\\\"", f ); break;
			case '\\': fputs( "\\\\", f ); break;
			case '\n': fputs( "\\n", f ); break;
			case '\r': fputs( "\\r", f ); break;
			case '\t': fputs( "\\t", f ); break;
			default:
				if ( c < 0x20 )
				{
					fprintf( f, "\\u%04x", c );
				}
				else if ( c < 0x80 )
				{
					fputc( c, f );
				}
				else
				{
					jsonWriteUtf8( f, cp1250ToUnicode[c - 0x80] );
				}
		}
	}
	fputc( '"', f );
}

static roundRec_t *Export_CurrentRound( void )
{
	if ( exportRoundCount == 0 )
	{
		return NULL;
	}
	return &exportRounds[exportRoundCount - 1];
}

static roundRec_t *Export_OpenRound( int round, int ready, int t )
{
	roundRec_t *r;

	if ( exportRoundCount >= MAX_EXPORT_ROUNDS )
	{
		return Export_CurrentRound();
	}

	r = &exportRounds[exportRoundCount++];
	memset( r, 0, sizeof( *r ) );
	r->round = round;
	r->ready = ready;
	r->startT = t;
	r->endT = t;
	return r;
}

static void Export_CloseRounds( int t )
{
	roundRec_t *r = Export_CurrentRound();

	if ( r )
	{
		r->endT = t;
	}
}

static void Export_RoundEvent( char kind, const char *text, int score1, int score2 )
{
	roundRec_t *r = Export_CurrentRound();
	roundEventRec_t *ev;

	if ( !r )
	{
		return;
	}

	if ( r->eventCount >= MAX_ROUND_EVENTS )
	{
		return;
	}

	ev = &r->events[r->eventCount++];
	memset( ev, 0, sizeof( *ev ) );
	ev->t = exportLastSnapshotTime;
	ev->kind = kind;
	ev->score1 = score1;
	ev->score2 = score2;
	if ( text )
	{
		Q_strncpyz( ev->text, text, sizeof( ev->text ) );
	}
}

static void Export_MatchInfoChanged( int t )
{
	char info[192];
	char roundStr[192];
	char scoreStr[64];
	char team1[64];
	char team2[64];
	const char *cs;
	int round = -1;
	int ready = 0;
	int score1 = 0;
	int score2 = 0;
	int hasScore = 0;
	roundRec_t *r;

	cs = CL_GetConfigstring( CS_SERVERINFO );
	Info_ValueForKey( cs, "_match_info", info, sizeof( info ) );
	Info_ValueForKey( cs, "_match_round", roundStr, sizeof( roundStr ) );
	Info_ValueForKey( cs, "_match_score", scoreStr, sizeof( scoreStr ) );
	Info_ValueForKey( cs, "_match_team1", team1, sizeof( team1 ) );
	Info_ValueForKey( cs, "_match_team2", team2, sizeof( team2 ) );

	{
		const char *p = info;
		if ( !*p )
		{
			p = roundStr;
		}

		if ( *p )
		{
			if ( strstr( p, "Ready-up" ) )
			{
				ready = 1;
			}

			if ( strncmp( p, "Round ", 6 ) == 0 )
			{
				round = atoi( p + 6 );
			}
		}
	}

	if ( scoreStr[0] && strcmp( scoreStr, "-" ) != 0 )
	{
		if ( sscanf( scoreStr, "%d:%d", &score1, &score2 ) == 2 )
		{
			hasScore = 1;
		}
	}

	if ( round < 0 && !ready )
	{
		return;
	}

	r = Export_CurrentRound();

	if ( !r || r->round != round || r->ready != ready )
	{
		Export_CloseRounds( t );
		r = Export_OpenRound( round < 0 ? 0 : round, ready, t );
	}

	if ( hasScore )
	{
		if ( !r->hasScore || r->score1 != score1 || r->score2 != score2 )
		{
			r->hasScore = 1;
			r->score1 = score1;
			r->score2 = score2;
			Export_RoundEvent( 's', NULL, score1, score2 );
		}
	}

	if ( team1[0] )
	{
		Q_strncpyz( r->team1, team1, sizeof( r->team1 ) );
	}
	if ( team2[0] )
	{
		Q_strncpyz( r->team2, team2, sizeof( r->team2 ) );
	}
}

static void Export_Announcement( const char *text )
{
	roundRec_t *r;
	const char *reason = NULL;

	if ( !text || !text[0] )
	{
		return;
	}

	if ( strcmp( text, "MP_ALLIESHAVEBEENELIMINATED" ) == 0 )
	{
		reason = "allies_eliminated";
	}
	else if ( strcmp( text, "MP_AXISHAVEBEENELIMINATED" ) == 0 )
	{
		reason = "axis_eliminated";
	}
	else if ( strcmp( text, "MP_TIMEHASEXPIRED" ) == 0 )
	{
		reason = "time_expired";
	}
	else if ( strcmp( text, "MP_EXPLOSIVESDEFUSED" ) == 0 || strcmp( text, "MP_BOMBDEFUSED" ) == 0 )
	{
		reason = "bomb_defused";
	}
	else if ( strcmp( text, "MP_ALLIEDMISSIONACCOMPLISHED" ) == 0 )
	{
		reason = "allies_mission";
	}
	else if ( strcmp( text, "MP_AXISMISSIONACCOMPLISHED" ) == 0 )
	{
		reason = "axis_mission";
	}
	else if ( strcmp( text, "MP_TARGETDESTROYED" ) == 0 )
	{
		reason = "target_destroyed";
	}
	else if ( strcmp( text, "MP_ROUNDDRAW" ) == 0 )
	{
		reason = "draw";
	}
	else if ( strcmp( text, "MP_EXPLOSIVESPLANTED" ) == 0 )
	{
		Export_RoundEvent( 'm', "bomb_planted", 0, 0 );
		return;
	}
	else
	{
		char clean[192];
		char *w = clean;
		const char *r = text;

		while ( *r && w < clean + sizeof( clean ) - 1 )
		{
			if ( *r != 0x15 && *r != 0x14 )
			{
				*w++ = *r;
			}
			r++;
		}
		*w = 0;

		Export_RoundEvent( 'm', clean, 0, 0 );
		return;
	}

	r = Export_CurrentRound();
	if ( r )
	{
		Q_strncpyz( r->endReason, reason, sizeof( r->endReason ) );
	}
	Export_RoundEvent( 'm', reason, 0, 0 );
}

static void Export_Chat( char type, const char *cmd )
{
	chatRec_t *c;
	const char *p;
	char buf[256];
	char *name;
	char *sep;
	char *msg;
	int len;

	if ( exportChatCount >= MAX_EXPORT_CHAT )
	{
		return;
	}

	p = cmd + 1;
	while ( *p == ' ' )
		p++;
	if ( *p != '"' )
	{
		return;
	}
	p++;
	len = 0;
	while ( *p && *p != '"' && len < (int)sizeof( buf ) - 1 )
	{
		buf[len++] = *p++;
	}
	buf[len] = 0;

	c = &exportChat[exportChatCount];
	memset( c, 0, sizeof( *c ) );
	c->t = exportLastSnapshotTime;
	c->round = Export_CurrentRound() ? Export_CurrentRound()->round : -1;
	c->type = type;
	c->color = 0;

	name = buf;
	{
		char *w = buf;
		char *r = buf;

		while ( *r )
		{
			if ( *r != 0x15 && *r != 0x14 )
			{
				*w++ = *r;
			}
			r++;
		}
		*w = 0;
	}

	if ( name[0] == '^' && name[1] >= '0' && name[1] <= '9' )
	{
		if ( name[1] == '9' )
		{
			Q_strncpyz( c->team, "axis", sizeof( c->team ) );
		}
		else if ( name[1] == '8' )
		{
			Q_strncpyz( c->team, "allies", sizeof( c->team ) );
		}
		name += 2;
	}

	if ( strncmp( name, "(GAME_DEAD)", 11 ) == 0 )
	{
		c->dead = 1;
		name += 11;
	}
	else if ( strncmp( name, "(GAME_SPECTATOR)", 16 ) == 0 )
	{
		Q_strncpyz( c->team, "spectator", sizeof( c->team ) );
		name += 16;
	}

	if ( strncmp( name, "(GAME_AXIS)", 11 ) == 0 )
	{
		Q_strncpyz( c->team, "axis", sizeof( c->team ) );
		name += 11;
	}
	else if ( strncmp( name, "(GAME_ALLIES)", 13 ) == 0 )
	{
		Q_strncpyz( c->team, "allies", sizeof( c->team ) );
		name += 13;
	}

	while ( *name == '^' && name[1] >= '0' && name[1] <= '9' )
	{
		name += 2;
	}

	sep = strstr( name, "^7: " );
	if ( sep )
	{
		*sep = 0;
		msg = sep + 4;
	}
	else
	{
		msg = name + strlen( name );
	}

	Q_strncpyz( c->name, name, sizeof( c->name ) );

	if ( *msg == '^' && msg[1] )
	{
		c->color = msg[1];
		msg += 2;
	}

	Q_strncpyz( c->text, msg, sizeof( c->text ) );

	exportChatCount++;
}

static void Export_FireEntityEvent( int entityNum, int event, int eventParm, const snapshot_t *snap, qboolean fromPs )
{
	killRec_t *k;
	const entityState_t *victimEnt;
	const entityState_t *attackerEnt;
	int i;
	int victimClient;
	int attackerClient;
	int weapon;
	int mod;

	if ( !fromPs && entityNum >= 0 && entityNum < MAX_CLIENTS &&
		( event == EV_FIRE_WEAPON || event == EV_FIRE_WEAPONB || event == EV_FIRE_WEAPON_LASTSHOT || event == EV_FIRE_WEAPON_MG42 ) )
	{
		exportShots[entityNum]++;
	}
	else if ( ( event == EV_GRENADE_BOUNCE || event == EV_GRENADE_EXPLODE || event == EV_CUSTOM_EXPLODE ) &&
		entityNum >= 0 && entityNum < MAX_GENTITIES )
	{
		int gi = exportGrenadeOpen[entityNum] - 1;

		if ( gi >= 0 )
		{
			grenadeRec_t *g = &exportGrenades[gi];

			if ( event == EV_GRENADE_BOUNCE )
			{
				g->bounces++;
			}
			else if ( !g->exploded )
			{
				int j;

				g->exploded = 1;
				g->explodeT = snap->serverTime;
				for ( j = 0; j < snap->numEntities; j++ )
				{
					if ( snap->entities[j].number == entityNum )
					{
						g->explodePos[0] = snap->entities[j].pos.trBase[0];
						g->explodePos[1] = snap->entities[j].pos.trBase[1];
						g->explodePos[2] = snap->entities[j].pos.trBase[2];
						break;
					}
				}
			}
		}
	}

	if ( event != EV_OBITUARY )
	{
		return;
	}

	if ( exportKillCount >= MAX_EXPORT_KILLS )
	{
		return;
	}

	weapon = -1;
	mod = -1;
	victimClient = -1;
	attackerClient = -1;
	victimEnt = NULL;
	attackerEnt = NULL;

	if ( fromPs )
	{
		victimClient = snap->ps.clientNum;
	}
	else
	{
		const entityState_t *carrier = NULL;

		for ( i = 0; i < snap->numEntities; i++ )
		{
			if ( snap->entities[i].number == entityNum )
			{
				carrier = &snap->entities[i];
				break;
			}
		}

		if ( !carrier )
		{
			return;
		}

		victimClient = carrier->otherEntityNum;
		attackerClient = carrier->attackerEntityNum;
		if ( attackerClient >= MAX_CLIENTS )
		{
			attackerClient = -1;
		}
	}

	if ( eventParm & 0x80 )
	{
		mod = eventParm & 0x7f;
		if ( mod < 0 || mod > 15 )
		{
			mod = 0;
		}
	}
	else
	{
		weapon = eventParm;
	}

	for ( i = 0; i < snap->numEntities; i++ )
	{
		const entityState_t *es = &snap->entities[i];

		if ( es->eType != ET_PLAYER && es->eType != ET_CORPSE )
		{
			continue;
		}

		if ( victimEnt == NULL && es->clientNum == victimClient )
		{
			victimEnt = es;
		}

		if ( attackerEnt == NULL && attackerClient >= 0 && es->clientNum == attackerClient && es->eType == ET_PLAYER )
		{
			attackerEnt = es;
		}
	}

	k = &exportKills[exportKillCount++];
	memset( k, 0, sizeof( *k ) );
	k->t = snap->serverTime;
	k->round = Export_CurrentRound() ? Export_CurrentRound()->round : -1;
	k->weapon = weapon;
	k->mod = mod;
	k->victim = victimClient;
	k->attacker = attackerClient;

	if ( victimClient >= 0 && victimClient < MAX_CLIENTS && exportRoster[victimClient].valid )
	{
		Q_strncpyz( k->victimName, exportRoster[victimClient].name, sizeof( k->victimName ) );
		k->victimTeam = exportRoster[victimClient].team;
	}
	else
	{
		k->victimTeam = -1;
	}

	if ( attackerClient >= 0 && attackerClient < MAX_CLIENTS && exportRoster[attackerClient].valid )
	{
		Q_strncpyz( k->attackerName, exportRoster[attackerClient].name, sizeof( k->attackerName ) );
		k->attackerTeam = exportRoster[attackerClient].team;
	}
	else
	{
		k->attackerTeam = -1;
	}

	if ( mod >= 0 )
	{
		switch ( mod )
		{
			case 7: Q_strncpyz( k->icon, "killiconmelee", sizeof( k->icon ) ); break;
			case 8: Q_strncpyz( k->icon, "killiconheadshot", sizeof( k->icon ) ); break;
			case 9: Q_strncpyz( k->icon, "killiconcrush", sizeof( k->icon ) ); break;
			case 11: Q_strncpyz( k->icon, "killiconfalling", sizeof( k->icon ) ); break;
			case 12: Q_strncpyz( k->icon, "killiconsuicide", sizeof( k->icon ) ); break;
			default: Q_strncpyz( k->icon, "killicondied", sizeof( k->icon ) ); break;
		}
	}

	if ( victimEnt )
	{
		k->vpos[0] = victimEnt->pos.trBase[0];
		k->vpos[1] = victimEnt->pos.trBase[1];
		k->vpos[2] = victimEnt->pos.trBase[2];
	}
	else if ( victimClient >= 0 && victimClient < MAX_CLIENTS )
	{
		k->vpos[0] = exportLastKnownPos[victimClient][0];
		k->vpos[1] = exportLastKnownPos[victimClient][1];
		k->vpos[2] = exportLastKnownPos[victimClient][2];
	}

	if ( attackerEnt )
	{
		k->apos[0] = attackerEnt->pos.trBase[0];
		k->apos[1] = attackerEnt->pos.trBase[1];
		k->apos[2] = attackerEnt->pos.trBase[2];
		if ( k->weapon < 0 )
		{
			k->weapon = attackerEnt->weapon;
		}
	}
	else if ( attackerClient >= 0 && attackerClient < MAX_CLIENTS )
	{
		k->apos[0] = exportLastKnownPos[attackerClient][0];
		k->apos[1] = exportLastKnownPos[attackerClient][1];
		k->apos[2] = exportLastKnownPos[attackerClient][2];
		if ( k->weapon < 0 && snap->ps.clientNum == attackerClient )
		{
			k->weapon = snap->ps.weapon;
		}
	}
}

static void Export_ProcessEventSeq( int seq, int *prev, const int *events, const int *eventParms, const snapshot_t *snap, int entityNum, qboolean fromPs )
{
	int i;
	int p = *prev;

	if ( seq < p )
	{
		p -= 256;
	}
	if ( seq - p > 4 )
	{
		p = seq - 4;
	}

	for ( i = p; i != seq; i++ )
	{
		Export_FireEntityEvent( entityNum, events[i & 3], eventParms[i & 3], snap, fromPs );
	}

	*prev = seq;
}

static void Export_ProcessSnapshot( const snapshot_t *snap )
{
	int i;
	int seq;
	byte seen[MAX_GENTITIES];
	byte seenPlayer[MAX_CLIENTS];
	roundRec_t *r;

	if ( exportSnapshotCount == 0 )
	{
		exportFirstSnapshotTime = snap->serverTime;
	}
	exportSnapshotCount++;
	exportLastSnapshotTime = snap->serverTime;

	if ( exportRoundCount == 0 )
	{
		Export_MatchInfoChanged( snap->serverTime );
	}

	r = Export_CurrentRound();
	if ( r )
	{
		r->endT = snap->serverTime;
	}

	memset( seen, 0, sizeof( seen ) );
	memset( seenPlayer, 0, sizeof( seenPlayer ) );

	// pass 1: live players update lastKnownPos; corpses must not overwrite it
	for ( i = 0; i < snap->numEntities; i++ )
	{
		const entityState_t *es = &snap->entities[i];

		if ( es->eType == ET_PLAYER && es->clientNum >= 0 && es->clientNum < MAX_CLIENTS )
		{
			exportLastKnownPos[es->clientNum][0] = es->pos.trBase[0];
			exportLastKnownPos[es->clientNum][1] = es->pos.trBase[1];
			exportLastKnownPos[es->clientNum][2] = es->pos.trBase[2];
			seenPlayer[es->clientNum] = 1;
		}
	}

	// recorder position lives in ps; own entity is not in the snapshot
	if ( snap->ps.clientNum >= 0 && snap->ps.clientNum < MAX_CLIENTS )
	{
		exportLastKnownPos[snap->ps.clientNum][0] = snap->ps.origin[0];
		exportLastKnownPos[snap->ps.clientNum][1] = snap->ps.origin[1];
		exportLastKnownPos[snap->ps.clientNum][2] = snap->ps.origin[2];
	}

	for ( i = 0; i < snap->numClients && i < MAX_CLIENTS; i++ )
	{
		const clientState_t *cs = &snap->clients[i];
		int num = cs->number & ( MAX_CLIENTS - 1 );

		exportRoster[num].valid = 1;
		Q_strncpyz( exportRoster[num].name, cs->name, sizeof( exportRoster[num].name ) );
		exportRoster[num].team = cs->team;
		exportRoster[num].model = cs->modelindex;
	}

	for ( i = 0; i < snap->numEntities; i++ )
	{
		const entityState_t *es = &snap->entities[i];
		int entNum = es->number;
		byte *present;
		int *prev;

		if ( entNum < 0 || entNum >= MAX_GENTITIES )
		{
			continue;
		}

		if ( es->eType == ET_CORPSE && es->clientNum >= 0 && es->clientNum < MAX_CLIENTS && !seenPlayer[es->clientNum] )
		{
			exportLastKnownPos[es->clientNum][0] = es->pos.trBase[0];
			exportLastKnownPos[es->clientNum][1] = es->pos.trBase[1];
			exportLastKnownPos[es->clientNum][2] = es->pos.trBase[2];
		}

		seen[entNum] = 1;
		present = &exportEntPresentInPrev[entNum];
		prev = &exportEntPrevEventSeq[entNum];

		if ( !*present || ( ( exportEntPrevEFlags2[entNum] ^ es->eFlags ) & 2 ) )
		{
			*prev = es->eventSequence;
		}
		exportEntPrevEFlags2[entNum] = es->eFlags & 2;

		if ( es->eType > ET_EVENTS )
		{
			if ( *prev == 0 )
			{
				*prev = 1;
				Export_FireEntityEvent( entNum, es->eType - ET_EVENTS, es->eventParm, snap, qfalse );
			}
		}
		else
		{
			Export_ProcessEventSeq( es->eventSequence, prev, es->events, es->eventParms, snap, entNum, qfalse );
		}

		*present = 1;

		if ( es->eType == ET_MISSILE )
		{
			grenadeRec_t *g;
			int gi = exportGrenadeOpen[entNum] - 1;

			if ( gi < 0 && exportGrenadeCount < MAX_EXPORT_GRENADES )
			{
				gi = exportGrenadeCount++;
				exportGrenadeOpen[entNum] = gi + 1;
				g = &exportGrenades[gi];
				memset( g, 0, sizeof( *g ) );
				g->entNum = entNum;
				g->round = r ? r->round : -1;
				g->spawnT = snap->serverTime;
				g->weapon = es->weapon;
				g->spawn[0] = es->pos.trBase[0];
				g->spawn[1] = es->pos.trBase[1];
				g->spawn[2] = es->pos.trBase[2];
				g->vel[0] = es->pos.trDelta[0];
				g->vel[1] = es->pos.trDelta[1];
				g->vel[2] = es->pos.trDelta[2];
				g->thrower = -1;

				// thrower = nearest player to spawn pos (recorder pos comes from ps)
				{
					int c;
					float best = 250.0f * 250.0f;

					if ( snap->ps.clientNum >= 0 && snap->ps.clientNum < MAX_CLIENTS )
					{
						float dx = snap->ps.origin[0] - g->spawn[0];
						float dy = snap->ps.origin[1] - g->spawn[1];
						float dz = snap->ps.origin[2] - g->spawn[2];
						float d2 = dx * dx + dy * dy + dz * dz;

						if ( d2 < best )
						{
							best = d2;
							g->thrower = snap->ps.clientNum;
						}
					}

					for ( c = 0; c < MAX_CLIENTS; c++ )
					{
						float dx, dy, dz, d2;

						if ( !exportRoster[c].valid )
						{
							continue;
						}
						dx = exportLastKnownPos[c][0] - g->spawn[0];
						dy = exportLastKnownPos[c][1] - g->spawn[1];
						dz = exportLastKnownPos[c][2] - g->spawn[2];
						d2 = dx * dx + dy * dy + dz * dz;
						if ( d2 < best )
						{
							best = d2;
							g->thrower = c;
						}
					}
				}
			}

			if ( gi >= 0 )
			{
				g = &exportGrenades[gi];
				g->lastT = snap->serverTime;
				g->last[0] = es->pos.trBase[0];
				g->last[1] = es->pos.trBase[1];
				g->last[2] = es->pos.trBase[2];
			}
		}

		if ( es->eType == ET_PLAYER && exportEntCount < MAX_EXPORT_ENTSAMPLES )
		{
			entSample_t *s = &exportEnts[exportEntCount++];

			memset( s, 0, sizeof( *s ) );
			s->t = snap->serverTime;
			s->round = r ? r->round : -1;
			s->num = es->clientNum;
			s->x = es->pos.trBase[0];
			s->y = es->pos.trBase[1];
			s->z = es->pos.trBase[2];
			s->yaw = es->apos.trBase[1];
			s->pitch = es->apos.trBase[0];
			s->weapon = es->weapon;
			s->eFlags = es->eFlags;
			s->legsAnim = es->legsAnim;
			s->torsoAnim = es->torsoAnim;
		}
	}

	for ( i = 0; i < MAX_GENTITIES; i++ )
	{
		if ( !seen[i] )
		{
			exportEntPresentInPrev[i] = 0;
			exportGrenadeOpen[i] = 0;
		}
	}

	seq = snap->ps.eventSequence;
	Export_ProcessEventSeq( seq, &exportPrevPsEventSeq, snap->ps.events, snap->ps.eventParms, snap, snap->ps.clientNum, qtrue );

	// recorder shots: exact, from ammoclip deltas.
	// ammoclip is clip-indexed (BG_ClipForWeapon), not weapon-indexed, so the
	// slot->weapon binding is learned at runtime: a decrease counts only when the
	// slot is bound to the currently/previously held weapon, or binds on first use.
	// weapon drops (slot->0 on a non-held weapon) and grenade throws do not count.
	if ( exportPrevClipValid )
	{
		int shot = 0;
		int curW = snap->ps.weapon;
		int prevW = exportPrevPsWeapon;
		int s;

		for ( s = 0; s < 128; s++ )
		{
			int d = exportPrevClip[s] - snap->ps.ammoclip[s];

			if ( d <= 0 )
			{
				continue;
			}

			if ( exportClipBind[s] >= 0 )
			{
				if ( exportClipBind[s] == curW || exportClipBind[s] == prevW )
				{
					shot += d;
				}
			}
			else
			{
				// unbound slot decrease: attribute to the held weapon and bind it
				if ( curW > 0 && curW < 128 && !Export_IsNonBallisticWeapon( curW ) )
				{
					exportClipBind[s] = curW;
					shot += d;
				}
				else if ( prevW > 0 && prevW < 128 && !Export_IsNonBallisticWeapon( prevW ) )
				{
					exportClipBind[s] = prevW;
					shot += d;
				}
			}
		}
		if ( shot > 0 && snap->ps.clientNum >= 0 && snap->ps.clientNum < MAX_CLIENTS )
		{
			exportShots[snap->ps.clientNum] += shot;
		}
	}
	memcpy( exportPrevClip, snap->ps.ammoclip, sizeof( exportPrevClip ) );
	exportPrevClipValid = 1;
	exportPrevPsWeapon = snap->ps.weapon;

	if ( exportPovCount < MAX_EXPORT_POVSAMPLES )
	{
		povSample_t *s = &exportPov[exportPovCount++];

		memset( s, 0, sizeof( *s ) );
		s->t = snap->serverTime;
		s->round = r ? r->round : -1;
		s->x = snap->ps.origin[0];
		s->y = snap->ps.origin[1];
		s->z = snap->ps.origin[2];
		s->vx = snap->ps.velocity[0];
		s->vy = snap->ps.velocity[1];
		s->vz = snap->ps.velocity[2];
		s->pitch = snap->ps.viewangles[0];
		s->yaw = snap->ps.viewangles[1];
		s->roll = snap->ps.viewangles[2];
		s->weapon = snap->ps.weapon;
		s->health = snap->ps.stats[STAT_HEALTH];
		s->eFlags = snap->ps.eFlags;
		s->pmFlags = snap->ps.pm_flags;
		s->legsAnim = snap->ps.legsAnim;
		s->torsoAnim = snap->ps.torsoAnim;
		s->weapAnim = snap->ps.weapAnim;
	}
}

static void Export_Reset( void )
{
	int i;

	exportKillCount = 0;
	exportChatCount = 0;
	exportRoundCount = 0;
	exportPovCount = 0;
	exportEntCount = 0;
	exportPrevPsEventSeq = 0;
	exportLastSnapshotTime = 0;
	exportFirstSnapshotTime = 0;
	exportSnapshotCount = 0;
	memset( exportEntPrevEventSeq, 0, sizeof( exportEntPrevEventSeq ) );
	memset( exportEntPresentInPrev, 0, sizeof( exportEntPresentInPrev ) );
	memset( exportEntPrevEFlags2, 0, sizeof( exportEntPrevEFlags2 ) );
	memset( exportGrenadeOpen, 0, sizeof( exportGrenadeOpen ) );
	memset( exportShots, 0, sizeof( exportShots ) );
	exportPrevClipValid = qfalse;
	exportPrevPsWeapon = 0;
	memset( exportLastKnownPos, 0, sizeof( exportLastKnownPos ) );
	for ( i = 0; i < 128; i++ )
	{
		exportClipBind[i] = -1;
	}
	exportGrenadeCount = 0;
	for ( i = 0; i < MAX_CLIENTS; i++ )
	{
		exportRoster[i].valid = 0;
	}
}

static int exportEntCompare( const void *a, const void *b )
{
	const entSample_t *ea = (const entSample_t *)a;
	const entSample_t *eb = (const entSample_t *)b;

	if ( ea->num != eb->num )
	{
		return ( ea->num < eb->num ) ? -1 : 1;
	}
	if ( ea->round != eb->round )
	{
		return ( ea->round < eb->round ) ? -1 : 1;
	}
	return ( ea->t < eb->t ) ? -1 : ( ea->t > eb->t );
}

static int exportPovCompare( const void *a, const void *b )
{
	const povSample_t *pa = (const povSample_t *)a;
	const povSample_t *pb = (const povSample_t *)b;

	if ( pa->round != pb->round )
	{
		return ( pa->round < pb->round ) ? -1 : 1;
	}
	return ( pa->t < pb->t ) ? -1 : ( pa->t > pb->t );
}

static void Export_BuildOutDir( const char *demoPath, char *outDir, int outDirSize )
{
	const char *base;
	const char *slash;
	const char *dot;
	int dirLen;
	int baseLen;

	slash = demoPath;
	for ( base = demoPath; *base; base++ )
	{
		if ( *base == '/' || *base == '\\' )
		{
			slash = base + 1;
		}
	}
	base = slash;

	dot = strrchr( base, '.' );

	dirLen = slash - demoPath;
	baseLen = dot && dot > base ? (int)( dot - base ) : (int)strlen( base );

	if ( dirLen >= outDirSize )
	{
		dirLen = outDirSize - 1;
	}
	memcpy( outDir, demoPath, dirLen );
	outDir[dirLen] = 0;

	if ( baseLen > 0 && strlen( outDir ) + baseLen + 2 < (unsigned)outDirSize )
	{
		int len = strlen( outDir );

		memcpy( outDir + len, base, baseLen );
		outDir[len + baseLen] = 0;
		Q_strncat( outDir, "/", outDirSize );
		_mkdir( outDir );
	}
}

static void Export_StripColors( const char *in, char *out, int outSize )
{
	char *w = out;
	const char *p = in;

	while ( *p && w < out + outSize - 1 )
	{
		if ( *p == '^' && p[1] )
		{
			if ( p[1] >= '0' && p[1] <= '9' )
			{
				p += 2;
				continue;
			}
			if ( p[1] == '^' )
			{
				p += 2;
				*w++ = '^';
				continue;
			}
		}
		*w++ = *p++;
	}
	*w = 0;
}

static void Export_SanitizeFilename( const char *name, char *out, int outSize )
{
	char clean[64];
	char *w = out;
	const char *p;

	Export_StripColors( name, clean, sizeof( clean ) );

	for ( p = clean; *p; p++ )
	{
		unsigned char c = (unsigned char)*p;

		if ( ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) || ( c >= '0' && c <= '9' ) ||
			c == '_' || c == '-' )
		{
			if ( w < out + outSize - 1 )
			{
				*w++ = (char)c;
			}
		}
		else if ( c == ' ' || c == '.' || c == '|' || c == ':' || c >= 0x80 )
		{
			if ( w > out && w < out + outSize - 1 && w[-1] != '_' )
			{
				*w++ = '_';
			}
		}
	}
	*w = 0;

	// windows: no trailing "." / "_" / " "
	while ( w > out && ( w[-1] == '_' || w[-1] == ' ' || w[-1] == '.' ) )
	{
		*--w = 0;
	}
	if ( w == out )
	{
		Q_strncpyz( out, "player", outSize );
	}
}

static qboolean Export_IsBlankText( const char *s )
{
	char clean[192];
	const char *p;

	Export_StripColors( s, clean, sizeof( clean ) );
	for ( p = clean; *p; p++ )
	{
		if ( *p != ' ' && *p != '\t' )
		{
			return qfalse;
		}
	}
	return qtrue;
}

// zPAM "Round report" lines fed back to the recorder:
//   "B 00:59 ^9^2kill 1^9 100hp with 1 pellet (Shotgun)"
//   "01:26 ^9hit 35hp 1 pellet 4.6m range-3 (Shotgun)"
//   "B 01:00 ^1killed to Shoulder via Kar98k"
static void Export_WriteReportFields( FILE *f, const char *clean )
{
	const char *p = clean;
	int mm, ss, n;

	if ( strncmp( p, "B ", 2 ) == 0 )
	{
		p += 2;
	}
	mm = ss = n = 0;
	if ( sscanf( p, "%d:%d%n", &mm, &ss, &n ) >= 2 && n > 0 && p[n] == ' ' )
	{
		fprintf( f, ", \"reportTime\": \"%d:%02d\"", mm, ss );
		p += n + 1;
	}

	if ( strncmp( p, "Round report:", 13 ) == 0 )
	{
		fprintf( f, ", \"report\": \"round_report\"" );
		return;
	}

	if ( strncmp( p, "kill ", 5 ) == 0 || strncmp( p, "hit ", 4 ) == 0 )
	{
		int killKind = ( p[0] == 'k' );
		int cnt = 0, dmg = 0, pellets = 0;
		float range = 0.0f;
		const char *q;

		p += killKind ? 5 : 4;
		if ( sscanf( p, "%d", &cnt ) == 1 )
		{
			while ( *p && *p != ' ' )
				p++;
			while ( *p == ' ' )
				p++;
		}

		fprintf( f, ", \"report\": %s", killKind ? "\"kill\"" : "\"hit\"" );
		if ( cnt )
		{
			fprintf( f, ", \"count\": %i", cnt );
		}
		if ( sscanf( p, "%dhp", &dmg ) == 1 )
		{
			fprintf( f, ", \"dmg\": %i", dmg );
		}

		q = strstr( p, " to " );
		if ( q )
		{
			char bp[48];
			int bi = 0;

			q += 4;
			while ( *q && strncmp( q, " with", 5 ) != 0 && *q != '(' && bi < (int)sizeof( bp ) - 1 )
			{
				bp[bi++] = *q++;
			}
			while ( bi > 0 && bp[bi - 1] == ' ' )
				bi--;
			bp[bi] = 0;
			if ( bi )
			{
				fprintf( f, ", \"bodypart\": " );
				jsonWriteEscaped( f, bp );
			}
		}

		q = strstr( p, "with " );
		if ( q && sscanf( q + 5, "%d", &pellets ) == 1 )
		{
			fprintf( f, ", \"pellets\": %i", pellets );
		}

		q = strstr( p, "m range" );
		if ( q )
		{
			const char *rs = q;
			while ( rs > p && ( ( rs[-1] >= '0' && rs[-1] <= '9' ) || rs[-1] == '.' ) )
			{
				rs--;
			}
			if ( sscanf( rs, "%f", &range ) == 1 )
			{
				fprintf( f, ", \"range\": %.2f", range );
			}
		}

		q = strrchr( p, '(' );
		if ( q )
		{
			const char *we = strchr( q, ')' );
			if ( we && we > q + 1 )
			{
				char wname[64];
				int wi = (int)( we - q - 1 );
				if ( wi > (int)sizeof( wname ) - 1 )
				{
					wi = sizeof( wname ) - 1;
				}
				memcpy( wname, q + 1, wi );
				wname[wi] = 0;
				fprintf( f, ", \"weapon\": " );
				jsonWriteEscaped( f, wname );
			}
		}
		return;
	}

	if ( strncmp( p, "killed to ", 10 ) == 0 )
	{
		const char *via = strstr( p + 10, " via " );
		char bp[48];
		int bi;

		if ( via )
		{
			bi = (int)( via - ( p + 10 ) );
			if ( bi > (int)sizeof( bp ) - 1 )
			{
				bi = sizeof( bp ) - 1;
			}
			memcpy( bp, p + 10, bi );
			while ( bi > 0 && bp[bi - 1] == ' ' )
				bi--;
			bp[bi] = 0;

			fprintf( f, ", \"report\": \"death\", \"bodypart\": " );
			jsonWriteEscaped( f, bp );
			fprintf( f, ", \"weapon\": " );
			jsonWriteEscaped( f, via + 5 );
			return;
		}
	}
}

static void Export_WriteEventJson( FILE *f, const char *indent, const roundEventRec_t *ev )
{
	fprintf( f, "%s{\"t\": %i, \"kind\": ", indent, ev->t );

	if ( ev->kind == 's' )
	{
		fprintf( f, "\"score\", \"score1\": %i, \"score2\": %i}", ev->score1, ev->score2 );
		return;
	}

	{
		char clean[192];
		const char *p;

		Export_StripColors( ev->text, clean, sizeof( clean ) );
		p = clean;

		if ( strncmp( p, "GAME_PICKUP_", 12 ) == 0 )
		{
			fprintf( f, "\"pickup\", \"item\": " );
			jsonWriteEscaped( f, p + 12 );
			fprintf( f, "}" );
			return;
		}
		if ( strcmp( p, "bomb_planted" ) == 0 )
		{
			fprintf( f, "\"bomb_planted\"}" );
			return;
		}
		if ( strcmp( p, "allies_eliminated" ) == 0 || strcmp( p, "axis_eliminated" ) == 0 ||
			strcmp( p, "time_expired" ) == 0 || strcmp( p, "bomb_defused" ) == 0 ||
			strcmp( p, "allies_mission" ) == 0 || strcmp( p, "axis_mission" ) == 0 ||
			strcmp( p, "target_destroyed" ) == 0 || strcmp( p, "draw" ) == 0 )
		{
			fprintf( f, "\"end_reason\", \"reason\": " );
			jsonWriteEscaped( f, p );
			fprintf( f, "}" );
			return;
		}
		if ( strncmp( p, "MP_DISCONNECTED", 15 ) == 0 )
		{
			fprintf( f, "\"disconnect\", \"name\": " );
			jsonWriteEscaped( f, p + 15 );
			fprintf( f, "}" );
			return;
		}
		if ( strncmp( p, "MP_YOU_WILL_SPAWN_", 18 ) == 0 )
		{
			const char *wp = strstr( p, "WEAPON_" );

			fprintf( f, "\"spawn_weapon\", \"side\": " );
			if ( strstr( p, "_AXIS_" ) )
			{
				fprintf( f, "\"axis\"" );
			}
			else if ( strstr( p, "_ALLIES_" ) )
			{
				fprintf( f, "\"allies\"" );
			}
			else
			{
				fprintf( f, "null" );
			}
			if ( wp )
			{
				fprintf( f, ", \"weapon\": " );
				jsonWriteEscaped( f, wp + 7 );
			}
			fprintf( f, "}" );
			return;
		}

		// zPAM hit/kill report lines: optional "B " + "MM:SS " prefix, then kill/hit/killed/Round report
		{
			const char *q = p;
			int mm, ss, n;

			if ( strncmp( q, "B ", 2 ) == 0 )
			{
				q += 2;
			}
			mm = ss = n = 0;
			if ( sscanf( q, "%d:%d%n", &mm, &ss, &n ) >= 2 && n > 0 )
			{
				const char *q2 = q + n;
				if ( *q2 == ' ' )
				{
					q2++;
				}
				if ( strncmp( q2, "kill ", 5 ) == 0 || strncmp( q2, "hit ", 4 ) == 0 ||
					strncmp( q2, "killed ", 7 ) == 0 || strncmp( q2, "Round report:", 13 ) == 0 )
				{
					fprintf( f, "\"zpam\"" );
					Export_WriteReportFields( f, p );
					fprintf( f, ", \"text\": " );
					jsonWriteEscaped( f, ev->text );
					fprintf( f, "}" );
					return;
				}
			}
			else if ( strncmp( q, "Round report:", 13 ) == 0 )
			{
				fprintf( f, "\"zpam\"" );
				Export_WriteReportFields( f, p );
				fprintf( f, ", \"text\": " );
				jsonWriteEscaped( f, ev->text );
				fprintf( f, "}" );
				return;
			}
		}

		fprintf( f, "\"msg\", \"text\": " );
		jsonWriteEscaped( f, ev->text );
		fprintf( f, "}" );
	}
}

// kill entry with full detail (names/teams/positions) - used in match.json
static void Export_WriteKillJson( FILE *f, const char *indent, const killRec_t *kk )
{
	fprintf( f, "%s{\"t\": %i, \"kind\": \"kill\", \"a\": {\"num\": %i, \"name\": ", indent, kk->t, kk->attacker );
	if ( kk->attackerName[0] )
	{
		jsonWriteEscaped( f, kk->attackerName );
		fprintf( f, ", \"team\": " );
		if ( kk->attackerTeam >= 0 )
		{
			jsonWriteEscaped( f, exportTeamName( kk->attackerTeam ) );
		}
		else
		{
			fprintf( f, "null" );
		}
	}
	else
	{
		fprintf( f, "null, \"team\": null" );
	}
	fprintf( f, "}, \"v\": {\"num\": %i, \"name\": ", kk->victim );
	if ( kk->victimName[0] )
	{
		jsonWriteEscaped( f, kk->victimName );
		fprintf( f, ", \"team\": " );
		if ( kk->victimTeam >= 0 )
		{
			jsonWriteEscaped( f, exportTeamName( kk->victimTeam ) );
		}
		else
		{
			fprintf( f, "null" );
		}
	}
	else
	{
		fprintf( f, "null, \"team\": null" );
	}
	fprintf( f, "}, \"w\": " );
	if ( kk->weapon >= 0 )
	{
		fprintf( f, "{\"id\": %i, \"name\": ", kk->weapon );
		jsonWriteEscaped( f, CL_WeaponName( kk->weapon ) );
		fprintf( f, "}" );
	}
	else
	{
		fprintf( f, "null" );
	}
	fprintf( f, ", \"mod\": " );
	if ( kk->mod >= 0 )
	{
		jsonWriteEscaped( f, exportModNames[kk->mod] );
	}
	else
	{
		fprintf( f, "null" );
	}
	fprintf( f, ", \"icon\": " );
	if ( kk->icon[0] )
	{
		jsonWriteEscaped( f, kk->icon );
	}
	else
	{
		fprintf( f, "null" );
	}
	fprintf( f, ", \"apos\": [%.1f, %.1f, %.1f], \"vpos\": [%.1f, %.1f, %.1f]}",
		kk->apos[0], kk->apos[1], kk->apos[2], kk->vpos[0], kk->vpos[1], kk->vpos[2] );
}

// grenade trajectory + attributed kills - used in match.json
static void Export_WriteGrenadeJson( FILE *f, const char *indent, const grenadeRec_t *g )
{
	int endT = g->exploded ? g->explodeT : g->lastT;
	float ex = g->exploded ? g->explodePos[0] : g->last[0];
	float ey = g->exploded ? g->explodePos[1] : g->last[1];
	float ez = g->exploded ? g->explodePos[2] : g->last[2];
	int kfirst = 1;
	int k;

	fprintf( f, "%s{\"t\": %i, \"kind\": \"grenade\", \"thrower\": ", indent, g->spawnT );
	if ( g->thrower >= 0 && exportRoster[g->thrower].valid )
	{
		fprintf( f, "{\"num\": %i, \"name\": ", g->thrower );
		jsonWriteEscaped( f, exportRoster[g->thrower].name );
		fprintf( f, ", \"team\": " );
		jsonWriteEscaped( f, exportTeamName( exportRoster[g->thrower].team ) );
		fprintf( f, "}" );
	}
	else
	{
		fprintf( f, "null" );
	}
	fprintf( f, ", \"w\": {\"id\": %i, \"name\": ", g->weapon );
	jsonWriteEscaped( f, CL_WeaponName( g->weapon ) );
	fprintf( f, "}, \"from\": [%.1f, %.1f, %.1f], \"vel\": [%.1f, %.1f, %.1f], \"to\": [%.1f, %.1f, %.1f], \"explodeT\": %i, \"lifeMs\": %i, \"bounces\": %i, \"kills\": [",
		g->spawn[0], g->spawn[1], g->spawn[2],
		g->vel[0], g->vel[1], g->vel[2],
		ex, ey, ez, endT, endT - g->spawnT, g->bounces );

	for ( k = 0; k < exportKillCount; k++ )
	{
		const killRec_t *kk = &exportKills[k];

		if ( kk->attacker != g->thrower || g->thrower < 0 )
		{
			continue;
		}
		if ( kk->weapon != g->weapon )
		{
			continue;
		}
		if ( kk->t < endT - 500 || kk->t > endT + 500 )
		{
			continue;
		}
		if ( !kfirst )
		{
			fprintf( f, ", " );
		}
		kfirst = 0;
		fprintf( f, "{\"num\": %i, \"name\": ", kk->victim );
		if ( kk->victimName[0] )
		{
			jsonWriteEscaped( f, kk->victimName );
		}
		else
		{
			fprintf( f, "null" );
		}
		fprintf( f, "}" );
	}
	fprintf( f, "]}" );
}

static void Export_WriteJsonFiles( const char *demoName )
{
	char path[1032];
	FILE *f;
	int i, k;
	roundRec_t *r;

	qsort( exportPov, exportPovCount, sizeof( exportPov[0] ), exportPovCompare );
	qsort( exportEnts, exportEntCount, sizeof( exportEnts[0] ), exportEntCompare );

	// meta.json
	Export_Path( path, sizeof( path ), "meta.json" );
	f = fopen( path, "w" );
	if ( f )
	{
		char mapname[MAX_QPATH];
		char gametype[MAX_QPATH];
		char hostname[MAX_QPATH];
		char protocol[32];
		char shortversion[32];
		char team1[64];
		char team2[64];
		char zpam[32];
		char maxclients[16];

		Info_ValueForKey( CL_GetConfigstring( CS_SERVERINFO ), "mapname", mapname, sizeof( mapname ) );
		Info_ValueForKey( CL_GetConfigstring( CS_SERVERINFO ), "g_gametype", gametype, sizeof( gametype ) );
		Info_ValueForKey( CL_GetConfigstring( CS_SERVERINFO ), "sv_hostname", hostname, sizeof( hostname ) );
		Info_ValueForKey( CL_GetConfigstring( CS_SERVERINFO ), "protocol", protocol, sizeof( protocol ) );
		Info_ValueForKey( CL_GetConfigstring( CS_SERVERINFO ), "shortversion", shortversion, sizeof( shortversion ) );
		Info_ValueForKey( CL_GetConfigstring( CS_SERVERINFO ), "sv_maxclients", maxclients, sizeof( maxclients ) );
		Info_ValueForKey( CL_GetConfigstring( CS_SERVERINFO ), "_match_team1", team1, sizeof( team1 ) );
		Info_ValueForKey( CL_GetConfigstring( CS_SERVERINFO ), "_match_team2", team2, sizeof( team2 ) );
		Info_ValueForKey( CL_GetConfigstring( CS_SERVERINFO ), "_zpam", zpam, sizeof( zpam ) );

		fprintf( f, "{\n" );
		fprintf( f, "  \"demo\": " );
		jsonWriteEscaped( f, demoName );
		fprintf( f, ",\n  \"map\": " );
		jsonWriteEscaped( f, mapname );
		fprintf( f, ",\n  \"gametype\": " );
		jsonWriteEscaped( f, gametype );
		fprintf( f, ",\n  \"hostname\": " );
		jsonWriteEscaped( f, hostname );
		fprintf( f, ",\n  \"protocol\": " );
		jsonWriteEscaped( f, protocol );
		fprintf( f, ",\n  \"version\": " );
		jsonWriteEscaped( f, shortversion );
		fprintf( f, ",\n  \"zpam\": " );
		jsonWriteEscaped( f, zpam );
		fprintf( f, ",\n  \"maxclients\": %s", maxclients[0] ? maxclients : "0" );
		fprintf( f, ",\n  \"clientNum\": %i", cl.snap.ps.clientNum );
		fprintf( f, ",\n  \"recorder\": " );
		if ( cl.snap.ps.clientNum >= 0 && cl.snap.ps.clientNum < MAX_CLIENTS && exportRoster[cl.snap.ps.clientNum].valid )
		{
			jsonWriteEscaped( f, exportRoster[cl.snap.ps.clientNum].name );
		}
		else
		{
			fprintf( f, "null" );
		}
		fprintf( f, ",\n  \"matchTeam1\": " );
		jsonWriteEscaped( f, team1 );
		fprintf( f, ",\n  \"matchTeam2\": " );
		jsonWriteEscaped( f, team2 );
		fprintf( f, ",\n  \"firstServerTime\": %i", exportFirstSnapshotTime );
		fprintf( f, ",\n  \"lastServerTime\": %i", exportLastSnapshotTime );
		fprintf( f, ",\n  \"durationMs\": %i", exportLastSnapshotTime - exportFirstSnapshotTime );
		fprintf( f, ",\n  \"snapshots\": %i", exportSnapshotCount );
		fprintf( f, ",\n  \"files\": {\n" );
		{
			int rosterN = 0;
			int eventN = 0;
			for ( i = 0; i < MAX_CLIENTS; i++ )
			{
				if ( exportRoster[i].valid )
				{
					rosterN++;
				}
			}
			for ( i = 0; i < exportRoundCount; i++ )
			{
				eventN += exportRounds[i].eventCount;
			}
			fprintf( f, "    \"match.json\": {\"rounds\": %i, \"events\": %i, \"kills\": %i, \"grenades\": %i},\n",
				exportRoundCount, eventN + exportKillCount + exportGrenadeCount, exportKillCount, exportGrenadeCount );
			fprintf( f, "    \"chat.json\": %i,\n", exportChatCount );
			fprintf( f, "    \"pov.json\": %i,\n", exportPovCount );
			fprintf( f, "    \"scoreboard.json\": %i,\n", rosterN );
			fprintf( f, "    \"players/\": %i\n", rosterN );
		}
		fprintf( f, "  },\n  \"note\": \"fov not recorded in demos (client-side dvar); player pitch/roll limited by game\"" );
		fprintf( f, "\n}\n" );
		fclose( f );
	}

	// match.json - the whole match: per-round meta + merged chronological
	// event stream (round events, full kill entries, grenade trajectories)
	Export_Path( path, sizeof( path ), "match.json" );
	f = fopen( path, "w" );
	if ( f )
	{
		int ki = 0;
		int gi = 0;

		fprintf( f, "{\n  \"rounds\": [\n" );
		for ( i = 0; i < exportRoundCount; i++ )
		{
			r = &exportRounds[i];
			int ej = 0;
			int killsInRound = 0;

			for ( k = 0; k < exportKillCount; k++ )
			{
				if ( exportKills[k].round == r->round )
				{
					killsInRound++;
				}
			}

			fprintf( f, "    {\n      \"round\": %i,\n      \"phase\": %s,\n      \"start\": %i,\n      \"end\": %i,\n      \"score1\": %i,\n      \"score2\": %i,\n      \"team1\": ",
				r->round, r->ready ? "\"ready-up\"" : "\"live\"", r->startT, r->endT, r->score1, r->score2 );
			jsonWriteEscaped( f, r->team1 );
			fprintf( f, ",\n      \"team2\": " );
			jsonWriteEscaped( f, r->team2 );
			fprintf( f, ",\n      \"endReason\": " );
			jsonWriteEscaped( f, r->endReason );
			fprintf( f, ",\n      \"kills\": %i,\n      \"events\": [\n", killsInRound );

			fprintf( f, "        {\"t\": %i, \"kind\": \"round_start\"}", r->startT );

			while ( 1 )
			{
				const roundEventRec_t *ev = NULL;
				const killRec_t *kk = NULL;
				const grenadeRec_t *g = NULL;
				int tmin = 0x7FFFFFFF;

				while ( ej < r->eventCount && r->events[ej].kind != 's' && Export_IsBlankText( r->events[ej].text ) )
				{
					ej++;
				}
				if ( ej < r->eventCount )
				{
					ev = &r->events[ej];
					tmin = ev->t;
				}

				while ( ki < exportKillCount && exportKills[ki].t < r->startT )
				{
					ki++;
				}
				if ( ki < exportKillCount && exportKills[ki].round == r->round &&
					exportKills[ki].t >= r->startT && exportKills[ki].t <= r->endT )
				{
					kk = &exportKills[ki];
					if ( kk->t < tmin )
					{
						tmin = kk->t;
					}
				}

				while ( gi < exportGrenadeCount && exportGrenades[gi].spawnT < r->startT )
				{
					gi++;
				}
				if ( gi < exportGrenadeCount && exportGrenades[gi].round == r->round &&
					exportGrenades[gi].spawnT >= r->startT && exportGrenades[gi].spawnT <= r->endT )
				{
					g = &exportGrenades[gi];
					if ( g->spawnT < tmin )
					{
						tmin = g->spawnT;
					}
				}

				if ( ev == NULL && kk == NULL && g == NULL )
				{
					break;
				}

				fprintf( f, ",\n" );

				if ( kk && kk->t == tmin )
				{
					Export_WriteKillJson( f, "        ", kk );
					ki++;
				}
				else if ( g && g->spawnT == tmin )
				{
					Export_WriteGrenadeJson( f, "        ", g );
					gi++;
				}
				else
				{
					Export_WriteEventJson( f, "        ", ev );
					ej++;
				}
			}

			fprintf( f, ",\n        {\"t\": %i, \"kind\": \"round_end\"", r->endT );
			if ( r->endReason[0] )
			{
				fprintf( f, ", \"reason\": " );
				jsonWriteEscaped( f, r->endReason );
			}
			fprintf( f, "}\n      ]\n    }%s\n", i + 1 < exportRoundCount ? "," : "" );
		}

		// kills/grenades that matched no round (warmup, between rounds)
		{
			int first = 1;
			int n2, j2;
			int any = 0;

			for ( n2 = 0; n2 < exportKillCount; n2++ )
			{
				int matched = 0;
				for ( j2 = 0; j2 < exportRoundCount; j2++ )
				{
					if ( exportKills[n2].round == exportRounds[j2].round &&
						exportKills[n2].t >= exportRounds[j2].startT && exportKills[n2].t <= exportRounds[j2].endT )
					{
						matched = 1;
						break;
					}
				}
				if ( !matched )
				{
					any = 1;
					break;
				}
			}
			if ( !any )
			{
				for ( n2 = 0; n2 < exportGrenadeCount; n2++ )
				{
					int matched = 0;
					for ( j2 = 0; j2 < exportRoundCount; j2++ )
					{
						if ( exportGrenades[n2].round == exportRounds[j2].round &&
							exportGrenades[n2].spawnT >= exportRounds[j2].startT && exportGrenades[n2].spawnT <= exportRounds[j2].endT )
						{
							matched = 1;
							break;
						}
					}
					if ( !matched )
					{
						any = 1;
						break;
					}
				}
			}

			if ( any )
			{
				fprintf( f, "  ],\n  \"unassigned\": [\n" );
				for ( n2 = 0; n2 < exportKillCount; n2++ )
				{
					int matched = 0;
					for ( j2 = 0; j2 < exportRoundCount; j2++ )
					{
						if ( exportKills[n2].round == exportRounds[j2].round &&
							exportKills[n2].t >= exportRounds[j2].startT && exportKills[n2].t <= exportRounds[j2].endT )
						{
							matched = 1;
							break;
						}
					}
					if ( matched )
					{
						continue;
					}
					if ( !first )
					{
						fprintf( f, ",\n" );
					}
					first = 0;
					Export_WriteKillJson( f, "    ", &exportKills[n2] );
				}
				for ( n2 = 0; n2 < exportGrenadeCount; n2++ )
				{
					int matched = 0;
					for ( j2 = 0; j2 < exportRoundCount; j2++ )
					{
						if ( exportGrenades[n2].round == exportRounds[j2].round &&
							exportGrenades[n2].spawnT >= exportRounds[j2].startT && exportGrenades[n2].spawnT <= exportRounds[j2].endT )
						{
							matched = 1;
							break;
						}
					}
					if ( matched )
					{
						continue;
					}
					if ( !first )
					{
						fprintf( f, ",\n" );
					}
					first = 0;
					Export_WriteGrenadeJson( f, "    ", &exportGrenades[n2] );
				}
			}
		}
		fprintf( f, "\n  ]\n}\n" );
		fclose( f );
	}

	// chat.json - one message per line, flat chronological list
	Export_Path( path, sizeof( path ), "chat.json" );
	f = fopen( path, "w" );
	if ( f )
	{
		fprintf( f, "{\n  \"chat\": [\n" );
		for ( i = 0; i < exportChatCount; i++ )
		{
			chatRec_t *cc = &exportChat[i];

			fprintf( f, "    {\"t\": %i, \"round\": %i, \"type\": %s, \"team\": ",
				cc->t, cc->round, cc->type == 'i' ? "\"team\"" : "\"all\"" );
			jsonWriteEscaped( f, cc->team[0] ? cc->team : "none" );
			fprintf( f, ", \"name\": " );
			jsonWriteEscaped( f, cc->name );
			fprintf( f, ", \"dead\": %s, \"color\": ", cc->dead ? "true" : "false" );
			if ( cc->color )
			{
				fprintf( f, "\"^%c\"", cc->color );
			}
			else
			{
				fprintf( f, "null" );
			}
			fprintf( f, ", \"text\": " );
			jsonWriteEscaped( f, cc->text );
			fprintf( f, "}%s\n", i + 1 < exportChatCount ? "," : "" );
		}
		fprintf( f, "  ]\n}\n" );
		fclose( f );
	}

	// pov.json
	Export_Path( path, sizeof( path ), "pov.json" );
	f = fopen( path, "w" );
	if ( f )
	{
		int lastRound = -0x40000000;

		fprintf( f, "{\n  \"clientNum\": %i, \"name\": ", cl.snap.ps.clientNum );
		if ( exportRoster[cl.snap.ps.clientNum & ( MAX_CLIENTS - 1 )].valid )
		{
			jsonWriteEscaped( f, exportRoster[cl.snap.ps.clientNum & ( MAX_CLIENTS - 1 )].name );
		}
		else
		{
			fprintf( f, "null" );
		}
		fprintf( f, ",\n  \"format\": \"[t, round, x, y, z, vx, vy, vz, pitch, yaw, roll, weapon, health, eFlags, pmFlags, legsAnim, torsoAnim, weapAnim]\",\n  \"timeline\": {\n" );

		for ( i = 0; i < exportPovCount; i++ )
		{
			povSample_t *s = &exportPov[i];
			int comma;

			if ( s->round != lastRound )
			{
				if ( lastRound != -0x40000000 )
				{
					fprintf( f, "    ],\n" );
				}
				fprintf( f, "    \"%i\": [\n", s->round );
				lastRound = s->round;
			}
			comma = ( i + 1 < exportPovCount && exportPov[i + 1].round == s->round );
			fprintf( f, "      [%i, %i, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %i, %i, %i, %i, %i, %i, %i]%s\n",
				s->t, s->round, s->x, s->y, s->z, s->vx, s->vy, s->vz, s->pitch, s->yaw, s->roll,
				s->weapon, s->health, s->eFlags, s->pmFlags, s->legsAnim, s->torsoAnim, s->weapAnim,
				comma ? "," : "" );
		}
		if ( lastRound != -0x40000000 )
		{
			fprintf( f, "    ]\n" );
		}
		fprintf( f, "  }\n}\n" );
		fclose( f );
	}

	// players/ - one json per player (NN_name.json) + index.json
	{
		char pdir[1032];
		FILE *idx;
		int start;
		int idxFirst = 1;

		Q_strncpyz( pdir, exportOutDir, sizeof( pdir ) - 40 );
		Q_strncat( pdir, "players/", sizeof( pdir ) );
		_mkdir( pdir );

		Q_strncpyz( path, pdir, sizeof( path ) - 32 );
		Q_strncat( path, "index.json", sizeof( path ) );
		idx = fopen( path, "w" );
		if ( idx )
		{
			fprintf( idx, "{\n  \"players\": {\n" );
		}

		start = 0;
		while ( start < exportEntCount )
		{
			int num = exportEnts[start].num;
			int end = start;
			char fname[96];
			char fp[2048];
			int idx2;

			while ( end < exportEntCount && exportEnts[end].num == num )
			{
				end++;
			}

			if ( exportRoster[num & ( MAX_CLIENTS - 1 )].valid )
			{
				Export_SanitizeFilename( exportRoster[num & ( MAX_CLIENTS - 1 )].name, fname, sizeof( fname ) - 8 );
			}
			else
			{
				snprintf( fname, sizeof( fname ) - 8, "player%i", num );
			}

			snprintf( fp, sizeof( fp ), "%s%02i_%s.json", pdir, num, fname );
			fp[sizeof( fp ) - 1] = 0;
			f = fopen( fp, "w" );
			if ( f )
			{
				int lastRound2 = -0x40000000;

				fprintf( f, "{\n  \"num\": %i,\n  \"name\": ", num );
				if ( exportRoster[num & ( MAX_CLIENTS - 1 )].valid )
				{
					jsonWriteEscaped( f, exportRoster[num & ( MAX_CLIENTS - 1 )].name );
				}
				else
				{
					fprintf( f, "null" );
				}
				fprintf( f, ",\n  \"team\": " );
				if ( exportRoster[num & ( MAX_CLIENTS - 1 )].valid )
				{
					jsonWriteEscaped( f, exportTeamName( exportRoster[num & ( MAX_CLIENTS - 1 )].team ) );
				}
				else
				{
					fprintf( f, "null" );
				}
				fprintf( f, ",\n  \"model\": %i,\n  \"summary\": {",
					exportRoster[num & ( MAX_CLIENTS - 1 )].valid ? exportRoster[num & ( MAX_CLIENTS - 1 )].model : -1 );

				{
					int roundSeen[256];
					int wSeen[256 * 16];
					int wCount[256];
					int idx2;
					int rn;
					int kn;
					int wi;
					int first = 1;

					memset( roundSeen, 0, sizeof( roundSeen ) );
					memset( wSeen, 0, sizeof( wSeen ) );
					memset( wCount, 0, sizeof( wCount ) );

					for ( idx2 = start; idx2 < end; idx2++ )
					{
						int rr = exportEnts[idx2].round;
						int w = exportEnts[idx2].weapon;

						if ( rr < 0 || rr >= 256 )
						{
							continue;
						}

						roundSeen[rr] = 1;

						if ( w <= 0 )
						{
							continue;
						}

						for ( wi = 0; wi < wCount[rr]; wi++ )
						{
							if ( wSeen[rr * 16 + wi] == w )
							{
								break;
							}
						}
						if ( wi == wCount[rr] && wCount[rr] < 16 )
						{
							wSeen[rr * 16 + wCount[rr]++] = w;
						}
					}

					for ( rn = 0; rn < 256; rn++ )
					{
						int kills = 0;
						int deaths = 0;

						if ( !roundSeen[rn] )
						{
							continue;
						}

						for ( kn = 0; kn < exportKillCount; kn++ )
						{
							if ( exportKills[kn].round != rn )
							{
								continue;
							}
							if ( exportKills[kn].attacker == num )
							{
								kills++;
							}
							if ( exportKills[kn].victim == num )
							{
								deaths++;
							}
						}

						if ( !first )
						{
							fprintf( f, ", " );
						}
						first = 0;

						fprintf( f, "\"%i\": {\"kills\": %i, \"deaths\": %i, \"weapons\": [", rn, kills, deaths );
						for ( wi = 0; wi < wCount[rn]; wi++ )
						{
							if ( wi )
							{
								fprintf( f, ", " );
							}
							jsonWriteEscaped( f, CL_WeaponName( wSeen[rn * 16 + wi] ) );
						}
						fprintf( f, "]}" );
					}
				}

				fprintf( f, "},\n  \"format\": \"timeline entries: [t, round, x, y, z, yaw, pitch, weapon, eFlags, legsAnim, torsoAnim]\",\n  \"timeline\": {\n" );

				for ( idx2 = start; idx2 < end; idx2++ )
				{
					entSample_t *s = &exportEnts[idx2];
					int comma;

					if ( s->round != lastRound2 )
					{
						if ( lastRound2 != -0x40000000 )
						{
							fprintf( f, "    ],\n" );
						}
						fprintf( f, "    \"%i\": [\n", s->round );
						lastRound2 = s->round;
					}

					comma = ( idx2 + 1 < end && exportEnts[idx2 + 1].round == s->round );
					fprintf( f, "      [%i, %i, %.2f, %.2f, %.2f, %.2f, %.2f, %i, %i, %i, %i]%s\n",
						s->t, s->round, s->x, s->y, s->z, s->yaw, s->pitch, s->weapon, s->eFlags, s->legsAnim, s->torsoAnim,
						comma ? "," : "" );
				}
				if ( lastRound2 != -0x40000000 )
				{
					fprintf( f, "    ]\n" );
				}
				fprintf( f, "  }\n}\n" );
				fclose( f );

				if ( idx )
				{
					if ( !idxFirst )
					{
						fprintf( idx, ",\n" );
					}
					idxFirst = 0;
					fprintf( idx, "    \"%i\": {\"name\": ", num );
					if ( exportRoster[num & ( MAX_CLIENTS - 1 )].valid )
					{
						jsonWriteEscaped( idx, exportRoster[num & ( MAX_CLIENTS - 1 )].name );
						fprintf( idx, ", \"team\": " );
						jsonWriteEscaped( idx, exportTeamName( exportRoster[num & ( MAX_CLIENTS - 1 )].team ) );
					}
					else
					{
						fprintf( idx, "null, \"team\": null" );
					}
					fprintf( idx, ", \"file\": \"%02i_%s.json\"}", num, fname );
				}
			}

			start = end;
		}

		if ( idx )
		{
			int rec = cl.snap.ps.clientNum;
			int hasFile = 0;

			if ( rec >= 0 && rec < MAX_CLIENTS && exportRoster[rec].valid )
			{
				for ( i = 0; i < exportEntCount; i++ )
				{
					if ( exportEnts[i].num == rec )
					{
						hasFile = 1;
						break;
					}
				}
				if ( !hasFile )
				{
					if ( !idxFirst )
					{
						fprintf( idx, ",\n" );
					}
					fprintf( idx, "    \"%i\": {\"name\": ", rec );
					jsonWriteEscaped( idx, exportRoster[rec].name );
					fprintf( idx, ", \"team\": " );
					jsonWriteEscaped( idx, exportTeamName( exportRoster[rec].team ) );
					fprintf( idx, ", \"file\": \"../pov.json\", \"pov\": true}" );
				}
			}
			fprintf( idx, "\n  }\n}\n" );
			fclose( idx );
		}
	}

	// scoreboard.json - per-player aggregated stats
	Export_Path( path, sizeof( path ), "scoreboard.json" );
	f = fopen( path, "w" );
	if ( f )
	{
		int firstPlayer = 1;
		int teamKills[4] = {0};
		int teamDeaths[4] = {0};

		fprintf( f, "{\n  \"note\": \"shots: recorder = exact (ammoclip deltas); others = entity fire events, lower bound (PVS/event-ring loss); weapons: weapons used for kills\",\n  \"score\": {\"axis\": %i, \"allies\": %i},\n  \"players\": {\n",
			clScoreAxis, clScoreAllies );

		for ( i = 0; i < MAX_CLIENTS; i++ )
		{
			int kills = 0, deaths = 0, suicides = 0, teamkills = 0, headshots = 0, melee = 0, grenadeKills = 0;
			int wIds[64];
			int wCnt = 0;
			int rn;

			if ( !exportRoster[i].valid )
			{
				continue;
			}

			for ( k = 0; k < exportKillCount; k++ )
			{
				killRec_t *kk = &exportKills[k];
				int wi;

				if ( kk->victim == i )
				{
					deaths++;
				}
				if ( kk->attacker == i )
				{
					kills++;
					if ( kk->victim == i )
					{
						suicides++;
					}
					else if ( kk->attackerTeam >= 0 && kk->attackerTeam == kk->victimTeam )
					{
						teamkills++;
					}
					if ( kk->mod == 8 )
					{
						headshots++;
					}
					if ( kk->mod == 7 )
					{
						melee++;
					}
					if ( kk->weapon >= 0 && strstr( CL_WeaponName( kk->weapon ), "grenade" ) )
					{
						grenadeKills++;
					}
					if ( kk->weapon >= 0 )
					{
						for ( wi = 0; wi < wCnt; wi++ )
						{
							if ( wIds[wi] == kk->weapon )
							{
								break;
							}
						}
						if ( wi == wCnt && wCnt < 64 )
						{
							wIds[wCnt++] = kk->weapon;
						}
					}
				}
			}

			if ( exportRoster[i].team >= 0 && exportRoster[i].team < 4 )
			{
				teamKills[exportRoster[i].team] += kills;
				teamDeaths[exportRoster[i].team] += deaths;
			}

			if ( !firstPlayer )
			{
				fprintf( f, ",\n" );
			}
			firstPlayer = 0;

			fprintf( f, "    \"%i\": {\n      \"name\": ", i );
			jsonWriteEscaped( f, exportRoster[i].name );
			fprintf( f, ",\n      \"team\": " );
			jsonWriteEscaped( f, exportTeamName( exportRoster[i].team ) );
			fprintf( f, ",\n      \"score\": %i,\n      \"boardDeaths\": %i,\n      \"ping\": %i,\n      \"kills\": %i,\n      \"deaths\": %i,\n      \"suicides\": %i,\n      \"teamkills\": %i,\n      \"headshots\": %i,\n      \"melee\": %i,\n      \"grenadeKills\": %i,\n      \"shots\": %i,\n      \"weapons\": [",
				clScores[i], clDeaths[i], clPings[i],
				kills, deaths, suicides, teamkills, headshots, melee, grenadeKills, exportShots[i] );
			for ( k = 0; k < wCnt; k++ )
			{
				if ( k )
				{
					fprintf( f, ", " );
				}
				jsonWriteEscaped( f, CL_WeaponName( wIds[k] ) );
			}
			fprintf( f, "],\n      \"rounds\": {" );

			{
				int firstR = 1;
				for ( rn = 0; rn < 256; rn++ )
				{
					int rk = 0, rd = 0;
					for ( k = 0; k < exportKillCount; k++ )
					{
						if ( exportKills[k].round != rn )
						{
							continue;
						}
						if ( exportKills[k].attacker == i )
						{
							rk++;
						}
						if ( exportKills[k].victim == i )
						{
							rd++;
						}
					}
					if ( rk || rd )
					{
						if ( !firstR )
						{
							fprintf( f, ", " );
						}
						firstR = 0;
						fprintf( f, "\"%i\": {\"k\": %i, \"d\": %i}", rn, rk, rd );
					}
				}
			}
			fprintf( f, "}\n    }" );
		}
		fprintf( f, "\n  },\n  \"teams\": {\"axis\": {\"kills\": %i, \"deaths\": %i}, \"allies\": {\"kills\": %i, \"deaths\": %i}}\n}\n",
			teamKills[TEAM_AXIS], teamDeaths[TEAM_AXIS], teamKills[TEAM_ALLIES], teamDeaths[TEAM_ALLIES] );
		fclose( f );
	}
}

int main( int argc, char **argv )
{
	if( argc < 2 )
	{
		Com_Error( ERR_FATAL, "Usage: cod2demo [demofile]\n" );
	}

	// output dir: <demo dir>/<demo name>/
	Export_BuildOutDir( argv[1], exportOutDir, sizeof( exportOutDir ) );

	// log file inside the output dir
	snprintf(logFileName, sizeof(logFileName), "%slog.txt", exportOutDir);

	// open log once for the whole run
	logFile = fopen(logFileName, "w");
	if (!logFile)
	{
		Com_Error(ERR_FATAL, "Failed to open %s for writing.\n", logFileName);
	}

	FS_FOpenFileRead( argv[1], &demo.demofile, qtrue );

	if ( !demo.demofile )
	{
		Com_Error( ERR_FATAL, "Failed to open %s\n", argv[1] );
	}

	while (1) {
		int gamestatesBefore = demo.gameStatesParsed;
		bool exit = CL_ReadDemoMessage();

		if (!exit) break;

		if (demo.gameStatesParsed > gamestatesBefore) {
			char mapname[MAX_QPATH];
			char gametype[MAX_QPATH];
			char hostname[MAX_QPATH];
			char protocol[32];
			char shortversion[32];

			Info_ValueForKey( CL_GetConfigstring( CS_SERVERINFO ), "mapname", mapname, sizeof( mapname ) );
			Info_ValueForKey( CL_GetConfigstring( CS_SERVERINFO ), "g_gametype", gametype, sizeof( gametype ) );
			Info_ValueForKey( CL_GetConfigstring( CS_SERVERINFO ), "sv_hostname", hostname, sizeof( hostname ) );
			Info_ValueForKey( CL_GetConfigstring( CS_SERVERINFO ), "protocol", protocol, sizeof( protocol ) );
			Info_ValueForKey( CL_GetConfigstring( CS_SERVERINFO ), "shortversion", shortversion, sizeof( shortversion ) );

			Com_Printf( "=== demo: %s ===\n", argv[1] );
			Com_Printf( "map: %s, gametype: %s, hostname: %s, protocol: %s, version: %s\n",
				mapname, gametype, hostname, protocol, shortversion );
		}

		snapshot_t snapshot;
		qboolean ok = CL_GetSnapshot(cl.snap.messageNum, &snapshot);

		if (!ok) continue;

		Export_ProcessSnapshot(&snapshot);
	}

	fclose( demo.demofile );

	Export_CloseRounds( exportLastSnapshotTime );
	Export_WriteJsonFiles( argv[1] );

	if (logFile)
	{
		fclose(logFile);
		logFile = NULL;
	}

	return 0;
}