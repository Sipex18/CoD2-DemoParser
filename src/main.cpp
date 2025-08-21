#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>

#include "declarations.hpp"

#define Com_DPrintf Com_Printf
#define Com_Memset memset
#define Q_vsnprintf vsnprintf
#define Q_strncpyz I_strncpyz
#define Com_Memcpy memcpy
#define Com_Memset memset


static char logFileName[1024];




#define ERR_FATAL 0
#define ERR_DROP 1
void Com_Error(int err, char* fmt,...)
{
	char buf[MAX_STRING_CHARS];

	va_list		argptr;

	va_start (argptr,fmt);
	Q_vsnprintf(buf, sizeof(buf), fmt, argptr);
	va_end (argptr);

	fputs(buf, stdout);
	//exit(1);
}

void Com_Printf( const char *fmt, ...)
{
	va_list		argptr;
	char		msg[MAXPRINTMSG];

	va_start (argptr,fmt);
	Q_vsnprintf (msg, sizeof(msg), fmt, argptr);
	va_end (argptr);

	msg[MAXPRINTMSG - 1] = 0;
	
	//printf("%s", msg);

	// Save to file
	FILE *fp = fopen(logFileName, "a");
	if (fp)
	{
		fprintf(fp, "%s", msg);
		fclose(fp);
	}
	else
	{
		printf("Failed to open output.log for writing.\n");
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

// BEGIN huffman.c from Enemy Territory
static int bloc = 0;

//bani - optimized version
//clears data along the way so we dont have to memset() it ahead of time
void    Huff_putBit( int bit, byte *fout, int *offset )
{
	int x, y;
	bloc = *offset;
	x = bloc >> 3;
	y = bloc & 7;
	if ( !y )
	{
		fout[ x ] = 0;
	}
	fout[ x ] |= bit << y;
	bloc++;
	*offset = bloc;
}

//bani - optimized version
//optimization works on gcc 3.x, but not 2.95 ? most curious.
int Huff_getBit( byte *fin, int *offset )
{
	int t;
	bloc = *offset;
	t = fin[ bloc >> 3 ] >> ( bloc & 7 ) & 0x1;
	bloc++;
	*offset = bloc;
	return t;
}

//bani - optimized version
//clears data along the way so we dont have to memset() it ahead of time
static void add_bit( char bit, byte *fout )
{
	int x, y;

	y = bloc >> 3;
	x = bloc++ & 7;
	if ( !x )
	{
		fout[ y ] = 0;
	}
	fout[ y ] |= bit << x;
}

//bani - optimized version
//optimization works on gcc 3.x, but not 2.95 ? most curious.
static int get_bit( byte *fin )
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
int Huff_Receive( node_t *node, int *ch, byte *fin )
{
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
		return 0;
//		Com_Error(ERR_DROP, "Illegal tree!\n");
	}
	return ( *ch = node->symbol );
}

/* Get a symbol */
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
//		Com_Error(ERR_DROP, "Illegal tree!\n");
	}
	*ch = node->symbol;
	*offset = bloc;
}

/* Send the prefix code for this node */
static void send( node_t *node, node_t *child, byte *fout )
{
	if ( node->parent )
	{
		send( node->parent, node, fout );
	}
	if ( child )
	{
		if ( node->right == child )
		{
			add_bit( 1, fout );
		}
		else
		{
			add_bit( 0, fout );
		}
	}
}

/* Send a symbol */
void Huff_transmit( huff_t *huff, int ch, byte *fout )
{
	int i;
	if ( huff->loc[ch] == NULL )
	{
		/* node_t hasn't been transmitted, send a NYT, then the symbol */
		Huff_transmit( huff, NYT, fout );
		for ( i = 7; i >= 0; i-- )
		{
			add_bit( (char)( ( ch >> i ) & 0x1 ), fout );
		}
	}
	else
	{
		send( huff->loc[ch], NULL, fout );
	}
}

void Huff_offsetTransmit( huff_t *huff, int ch, byte *fout, int *offset )
{
	bloc = *offset;
	send( huff->loc[ch], NULL, fout );
	*offset = bloc;
}

void Huff_Decompress( msg_t *mbuf, int offset )
{
	int ch, cch, i, j, size;
	byte seq[65536];
	byte*       buffer;
	huff_t huff;

	size = mbuf->cursize - offset;
	buffer = mbuf->data + offset;

	if ( size <= 0 )
	{
		return;
	}

	Com_Memset( &huff, 0, sizeof( huff_t ) );
	// Initialize the tree & list with the NYT node
	huff.tree = huff.lhead = huff.ltail = huff.loc[NYT] = &( huff.nodeList[huff.blocNode++] );
	huff.tree->symbol = NYT;
	huff.tree->weight = 0;
	huff.lhead->next = huff.lhead->prev = NULL;
	huff.tree->parent = huff.tree->left = huff.tree->right = NULL;

	cch = buffer[0] * 256 + buffer[1];
	// don't overflow with bad messages
	if ( cch > mbuf->maxsize - offset )
	{
		cch = mbuf->maxsize - offset;
	}
	bloc = 16;

	for ( j = 0; j < cch; j++ )
	{
		ch = 0;
		// don't overflow reading from the messages
		// FIXME: would it be better to have a overflow check in get_bit ?
		if ( ( bloc >> 3 ) > size )
		{
			seq[j] = 0;
			break;
		}
		Huff_Receive( huff.tree, &ch, buffer );               /* Get a character */
		if ( ch == NYT )                                /* We got a NYT, get the symbol associated with it */
		{
			ch = 0;
			for ( i = 0; i < 8; i++ )
			{
				ch = ( ch << 1 ) + get_bit( buffer );
			}
		}

		seq[j] = ch;                                    /* Write symbol */

		Huff_addRef( &huff, (byte)ch );                               /* Increment node */
	}
	mbuf->cursize = cch + offset;
	Com_Memcpy( mbuf->data + offset, seq, cch );
}

extern int oldsize;

void Huff_Compress( msg_t *mbuf, int offset )
{
	int i, ch, size;
	byte seq[65536];
	byte*       buffer;
	huff_t huff;

	size = mbuf->cursize - offset;
	buffer = mbuf->data + + offset;

	if ( size <= 0 )
	{
		return;
	}

	Com_Memset( &huff, 0, sizeof( huff_t ) );
	// Add the NYT (not yet transmitted) node into the tree/list */
	huff.tree = huff.lhead = huff.loc[NYT] =  &( huff.nodeList[huff.blocNode++] );
	huff.tree->symbol = NYT;
	huff.tree->weight = 0;
	huff.lhead->next = huff.lhead->prev = NULL;
	huff.tree->parent = huff.tree->left = huff.tree->right = NULL;
	huff.loc[NYT] = huff.tree;

	seq[0] = ( size >> 8 );
	seq[1] = size & 0xff;

	bloc = 16;

	for ( i = 0; i < size; i++ )
	{
		ch = buffer[i];
		Huff_transmit( &huff, ch, seq );                      /* Transmit symbol */
		Huff_addRef( &huff, (byte)ch );                               /* Do update */
	}

	bloc += 8;                                              // next byte

	mbuf->cursize = ( bloc >> 3 ) + offset;
	Com_Memcpy( mbuf->data + offset, seq, ( bloc >> 3 ) );
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
// END huffman.c from Enemy Territory

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

int MSG_WriteBitsCompress( const byte *datasrc, byte *buffdest, int bytecount)
{

	int offset;
	int i;

	if(bytecount <= 0)
	{
		return 0;
	}

	for(offset = 0, i = 0; i < bytecount; i++)
	{
		Huff_offsetTransmit( &msgHuff.compressor, (int)datasrc[i], buffdest, &offset );
	}
	return (offset + 7) / 8;
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

	if ( msg->readcount+sizeof(byte) > msg->cursize )
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

	if ( msg->readcount+sizeof(short) > msg->cursize )
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

	if ( msg->readcount+sizeof(int32_t) > msg->cursize )
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
	while ( l < sizeof( string ) - 1 );

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



void SHOWNET( msg_t *msg, char *s ) {
	Com_Printf( "%3i:%s\n", msg->readcount - 1, s );
}



qboolean    CL_GetSnapshot( int snapshotNumber, snapshot_t *snapshot ) {
	clSnapshot_t    *clSnap;
	int i, count;

	if ( snapshotNumber > cl.snap.messageNum ) {
		Com_Error( ERR_DROP, "CL_GetSnapshot: snapshotNumber > cl.snapshot.messageNum" );
	}

	// if the frame has fallen out of the circular buffer, we can't return it
	if ( cl.snap.messageNum - snapshotNumber >= PACKET_BACKUP ) {
		return qfalse;
	}

	// if the frame is not valid, we can't return it
	clSnap = &cl.snapshots[snapshotNumber & PACKET_MASK];
	if ( !clSnap->valid ) {
		return qfalse;
	}

	// if the entities in the frame have fallen out of their
	// circular buffer, we can't return it
	if ( cl.parseEntitiesNum - clSnap->parseEntitiesNum >= MAX_PARSE_ENTITIES ) {
		return qfalse;
	}

	// write the snapshot
	snapshot->snapFlags = clSnap->snapFlags;
	snapshot->serverCommandSequence = clSnap->serverCommandNum;
	snapshot->ping = clSnap->ping;
	snapshot->serverTime = clSnap->serverTime;
	memcpy( snapshot->areamask, clSnap->areamask, sizeof( snapshot->areamask ) );
	snapshot->ps = clSnap->ps;
	count = clSnap->numEntities;
	if ( count > MAX_ENTITIES_IN_SNAPSHOT ) {
		Com_DPrintf( "CL_GetSnapshot: truncated %i entities to %i\n", count, MAX_ENTITIES_IN_SNAPSHOT );
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

	// FIXME: configstring changes and server commands!!!

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
		if (MSG_ReadBit(msg))
		{
			if (MSG_ReadBit(msg))
			{
				*toF = MSG_ReadLong(msg);
				if (print)
					Com_Printf("%s:%f ", field->name, *(float*)toF);
			}
			else
			{
				readbits = MSG_ReadBits(msg, 5);
				readbyte = 32 * MSG_ReadByte(msg) + readbits - 4096;
				*(float*)toF = (float)readbyte;
				if (print)
					Com_Printf("%s:%i ", field->name, readbyte);
			}
		}
		else
			*toF = 0;
		return;

	case -99:
		if (MSG_ReadBit(msg))
		{
			if (MSG_ReadBit(msg))
			{
				*toF = MSG_ReadLong(msg);
				if (print)
					Com_Printf("%s:%f ", field->name, *(float*)toF);
			}
			else
			{
				readbits = MSG_ReadBits(msg, 2);
				readbyte = 4 * MSG_ReadByte(msg) + readbits - 512;
				*(float*)toF = (float)readbyte;
				if (print)
					Com_Printf("%s:%i ", field->name, readbyte);
			}
		}
		else
			*toF = 0;
		return;

	case -100:
		if (MSG_ReadBit(msg))
			*(float*)toF = MSG_ReadAngle16(msg);
		else
			*toF = 0;
		return;

	default:
		if (MSG_ReadBit(msg))
		{
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
		}
		else
			*toF = 0;
		return;
	}
}

void MSG_ReadDeltaStruct(msg_t* msg, const void* from, void* to, unsigned int number, int numFields, int indexBits, netField_t* stateFields)
{
	netField_t* field;
	int lc;
	int i;

	// check for a remove
	if (MSG_ReadBit(msg) == 1) {
		Com_Printf("%3i: #%-3i remove\n", msg->readcount, number);
		return;
	}

	// check for no delta
	if (MSG_ReadBit(msg) == 0)
	{
		//Com_Memcpy(to, from, 4 * numFields + 4);
		Com_Memcpy(to, from, (numFields + sizeof(intptr_t)) * sizeof(from));	
		return;
	}

	lc = MSG_ReadByte(msg);

	if (lc > numFields)
	{
		msg->overflowed = qtrue;
		return;
	}

	Com_Printf("%3i: #%-3i ", msg->readcount, *(uint32_t*)to);

	*(uint32_t*)to = number;

	for (i = 0, field = stateFields; i < lc; i++, field++)
	{
		MSG_ReadDeltaField(msg, from, to, field, 1);
	}

	for (i = lc, field = &stateFields[lc]; i < numFields; i++, field++)
	{
		*(uint32_t*)((byte*)to + field->offset) = *(uint32_t*)((byte*)from + field->offset);
	}

	Com_Printf("\n");
}

void MSG_ReadDeltaEntity(msg_t* msg, entityState_t* from, entityState_t* to, int number)
{
	MSG_ReadDeltaStruct(msg, from, to, number, COUNT_OF(entityStateFields), 10, entityStateFields);
}

void MSG_ReadDeltaClient(msg_t *msg, clientState_t *from, clientState_t *to, int number)
{
	clientState_t nullstate;

	if ( !from )
	{
		from = &nullstate;
		Com_Memset(&nullstate, 0, sizeof(nullstate));
	}

	MSG_ReadDeltaStruct(msg, from, to, number, COUNT_OF(clientStateFields), 6, clientStateFields);
}



void MSG_ReadDeltaHudElems(msg_t *msg, hudelem_t *from, hudelem_t *to, int count)
{
	int inuse, lc, i, j, k;

	inuse = MSG_ReadBits(msg, 5);

	for ( i = 0; i < inuse; ++i )
	{
		lc = MSG_ReadBits(msg, 5);

		for ( j = 0; j <= lc; ++j )
		{
			MSG_ReadDeltaField(msg, &from[i], &to[i], &hudElemFields[j], 1);
		}

		for ( k = j; j < COUNT_OF(hudElemFields); ++j, ++k )
		{
			*(int32_t *)((byte *)(from + i) + hudElemFields[k].offset) = *(int32_t *)((byte *)(to + i) + hudElemFields[k].offset);
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
			MSG_ReadDeltaField(msg, from, to, &objFields[i], 1);
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
	int bitstemp;
	uint32_t unsignedbits;
	int i, j, k, lc;

	if ( !from )
	{
		from = &nullstate;
		Com_Memset(&nullstate, 0, sizeof(nullstate));
	}

	Com_Memcpy(to, from, sizeof(playerState_t));

	lc = 0;

	qboolean print = qtrue; // only present in client aka cl_shownet

	Com_Printf("%3i: playerstate ", msg->readcount);


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
				if ( print )
					Com_Printf("{0}%s:%f ", field->name, *(float *)toF);
			}
			else
			{
				readbits = MSG_ReadBits(msg, 5);
				readbyte = 32 * MSG_ReadByte(msg) + readbits - 4096;
				*(float *)toF = (float)readbyte;
				if ( print )
					Com_Printf("{1}%s:%i ", field->name, readbyte);
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
			if ( print )
				Com_Printf("{2}%s:%i ", field->name, *toF);
			break;
		}
	}

	for ( i = lc, field = &playerStateFields[lc]; i < COUNT_OF(playerStateFields); i++, field++ )
	{
		fromF = ( int32_t * )( (byte *)from + field->offset );
		toF = ( int32_t * )( (byte *)to + field->offset );

		*toF = *fromF;
	}

	// stats
	int statsbits = 0;



	if ( MSG_ReadBits( msg, 1 ) ) {  // one general bit tells if any of this infrequently changing stuff has changed
		statsbits = MSG_ReadBits(msg, 6);

		
		Com_Printf( "%s ", "PS_STATS" );

		if ( (statsbits & 1) != 0 )
			to->stats[STAT_HEALTH] = MSG_ReadShort(msg);

		if ( (statsbits & 2) != 0 )
			to->stats[STAT_DEAD_YAW] = MSG_ReadShort(msg);

		if ( (statsbits & 4) != 0 )
			to->stats[STAT_MAX_HEALTH] = MSG_ReadShort(msg);

		if ( (statsbits & 8) != 0 )
			to->stats[STAT_IDENT_CLIENT_NUM] = MSG_ReadBits(msg, 6);

		if ( (statsbits & 0x10) != 0 )
			to->stats[STAT_IDENT_CLIENT_HEALTH] = MSG_ReadShort(msg);

		if ( (statsbits & 0x20) != 0 )
			to->stats[STAT_SPAWN_COUNT] = MSG_ReadByte(msg);
	}



	// ammo stored
	int ammobits = 0;
	if ( MSG_ReadBit(msg) )
	{
		for ( i = 0; i < 4; ++i )
		{
			if ( MSG_ReadBit(msg) )
			{
				Com_Printf( "%s ", "PS_AMMO" );
				ammobits = MSG_ReadShort(msg);

				for ( j = 0; j < 16; ++j )
				{
					if ( ((ammobits >> j) & 1) != 0 )
					{
						to->ammo[j + 16 * i] = MSG_ReadShort(msg);
					}
				}
			}
		}
	}
/*
	// ammo stored
	if ( MSG_ReadBits( msg, 1 ) ) {     // check for any ammo change (0-63)
		for ( j = 0; j < 4; j++ ) {
			if ( MSG_ReadBits( msg, 1 ) ) {
				Com_Printf( "PS_AMMO" );
				bits = MSG_ReadShort( msg );
				for ( i = 0 ; i < 16 ; i++ ) {
					if ( bits & ( 1 << i ) ) {
						to->ammo[i + ( j * 16 )] = MSG_ReadShort( msg );
					}
				}
			}
		}
	}
*/


	// ammo in clip
	int clipbits = 0;

	for ( i = 0; i < 4; ++i )
	{
		if ( MSG_ReadBit(msg) )
		{
			Com_Printf( "%s ", "PS_AMMOCLIP" );
			clipbits = MSG_ReadShort(msg);

			for ( j = 0; j < 16; ++j )
			{
				if ( ((clipbits >> j) & 1) != 0 )
				{
					to->ammoclip[j + 16 * i] = MSG_ReadShort(msg);
				}
			}
		}
	}
/*
	// ammo in clip
	for ( j = 0; j < 4; j++ ) {
		if ( MSG_ReadBits( msg, 1 ) ) {
			Com_Printf( "PS_AMMOCLIP" );
			bits = MSG_ReadShort( msg );
			for ( i = 0 ; i < 16 ; i++ ) {
				if ( bits & ( 1 << i ) ) {
					to->ammoclip[i + ( j * 16 )] = MSG_ReadShort( msg );
				}
			}
		}
	}
*/

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

	Com_Printf("\n");
}













void CL_DeltaEntity( msg_t *msg, clSnapshot_t *frame, int newnum, entityState_t *old,
					 qboolean unchanged ) {
	
	entityState_t   *state = NULL;

	// save the parsed entity state into the big circular buffer so
	// it can be used as the source for a later delta
	state = &cl.parseEntities[cl.parseEntitiesNum & ( MAX_PARSE_ENTITIES - 1 )];


	if ( unchanged ) {
		*state = *old;
	} else {
		MSG_ReadDeltaEntity( msg, old, state, newnum );
	}

	if ( state->number == ( MAX_GENTITIES - 1 ) ) {
		return;     // entity was delta removed
	}

	cl.parseEntitiesNum++;
	frame->numEntities++;
}



void CL_DeltaClient( msg_t *msg, clSnapshot_t *frame, int newnum, clientState_t *old,
					 qboolean unchanged ) {
	
	clientState_t   *state = NULL;

	// save the parsed entity state into the big circular buffer so
	// it can be used as the source for a later delta
	state = &cl.parseClients[cl.parseClientsNum & ( MAX_PARSE_CLIENTS - 1 )];


	if ( unchanged ) {
		*state = *old;
	} else {
		MSG_ReadDeltaClient( msg, old, state, newnum );
	}

	// asi neni
	if ( state->number == ( MAX_GCLIENTS - 1 ) ) {
		return;     // entity was delta removed
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
			// one or more entities from the old packet are unchanged
			//if ( cl_shownet->integer == 3 ) {
				//Com_Printf( "%3i:  unchanged: %i\n", msg->readcount, oldnum );
			//}
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
			//if ( cl_shownet->integer == 3 ) {
				Com_Printf( "%3i:  delta: %i\n", msg->readcount, newnum );
			//}
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
			//if ( cl_shownet->integer == 3 ) {
				Com_Printf( "%3i:  baseline: %i\n", msg->readcount, newnum );
			//}
			CL_DeltaEntity( msg, newframe, newnum, &cl.entityBaselines[newnum], qfalse );
			continue;
		}

	}

	// any remaining entities in the old frame are copied over
	while ( oldnum != 99999 ) {
		// one or more entities from the old packet are unchanged
		//if ( cl_shownet->integer == 3 ) {
			//Com_Printf( "%3i:  unchanged: %i\n", msg->readcount, oldnum );
		//}
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

	//if ( cl_shownuments->integer ) {
		Com_Printf( "Entities in packet: %i\n", newframe->numEntities );
	//}
}

// Unknown func
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
				( oldframe->parseClientsNum + oldindex ) & ( MAX_PARSE_ENTITIES - 1 )];
			oldnum = oldstate->number;
		}
	}

	while ( 1 ) {

		// Is next client avaivle
		clientAvaible = MSG_ReadBit( msg );
      	if (clientAvaible == 0) {
			break;
		}

		// read the entity index number
		newnum = MSG_ReadBits( msg, GCLIENTNUM_BITS );

		// toto tam neni
		/*if ( newnum == ( MAX_GCLIENTS - 1 ) ) {
			break;
		}*/

		if ( msg->readcount > msg->cursize ) {
			Com_Error( ERR_DROP,"CL_ParsePacketClients: end of message" );
		}

		while ( oldnum < newnum ) {
			// one or more entities from the old packet are unchanged
			//if ( cl_shownet->integer == 3 ) {
				//Com_Printf( "%3i:  unchanged: %i\n", msg->readcount, oldnum );
			//}
			CL_DeltaClient( msg, newframe, oldnum, oldstate, qtrue );

			oldindex++;

			if ( oldindex >= oldframe->numClients ) {
				oldnum = 99999;
			} else {
				oldstate = &cl.parseClients[
					( oldframe->parseClientsNum + oldindex ) & ( MAX_PARSE_ENTITIES - 1 )];
				oldnum = oldstate->number;
			}
		}
		if ( oldnum == newnum ) {
			// delta from previous state
			//if ( cl_shownet->integer == 3 ) {
				Com_Printf( "%3i:  delta: %i\n", msg->readcount, newnum );
			//}

			// [SOMETHING MISSING HRER]
			/*
				puVar6 = local_d0;
				if (local_d0 == (uint *)0x0) {
					puVar6 = local_c8;
					puVar7 = puVar6;
					for (iVar4 = 0x17; iVar4 != 0; iVar4 = iVar4 + -1) {
					*puVar7 = 0;
					puVar7 = puVar7 + 1;
					}
				}
			*/


			CL_DeltaClient( msg, newframe, newnum, oldstate, qfalse );

			oldindex++;

			if ( oldindex >= oldframe->numClients ) {
				oldnum = 99999;
			} else {
				oldstate = &cl.parseClients[
					( oldframe->parseClientsNum + oldindex ) & ( MAX_PARSE_ENTITIES - 1 )];
				oldnum = oldstate->number;
			}
			continue;
		}

		if ( oldnum > newnum ) {
			// delta from baseline
			//if ( cl_shownet->integer == 3 ) {
				Com_Printf( "%3i:  baseline: %i\n", msg->readcount, newnum );
			//}

			// [SOMETHING MISSING HRER]
			/*
				puVar8 = local_68;
				for (iVar4 = 0x17; iVar4 != 0; iVar4 = iVar4 + -1) {
					*puVar8 = 0;
					puVar8 = puVar8 + 1;
				}
			*/


			CL_DeltaClient( msg, newframe, newnum, &cl.clientBaselines[newnum], qfalse );
			continue;
		}

	}

	// any remaining entities in the old frame are copied over
	while ( oldnum != 99999 ) {
		// one or more entities from the old packet are unchanged
		//if ( cl_shownet->integer == 3 ) {
			//Com_Printf( "%3i:  unchanged: %i\n", msg->readcount, oldnum );
		//}
		CL_DeltaClient( msg, newframe, oldnum, oldstate, qtrue );

		oldindex++;

		if ( oldindex >= oldframe->numClients ) {
			oldnum = 99999;
		} else {
			oldstate = &cl.parseClients[
				( oldframe->parseClientsNum + oldindex ) & ( MAX_PARSE_ENTITIES - 1 )];
			oldnum = oldstate->number;
		}
	}

	//if ( cl_shownuments->integer ) {
		Com_Printf( "Clients in packet: %i\n", newframe->numClients );
	//}
}




void CL_ParseSnapshot( msg_t *msg ) {
	int len;
	clSnapshot_t    *old;
	clSnapshot_t newSnap;
	int deltaNum;
	int oldMessageNum;
	int i, packetNum;


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
		} else {
			newSnap.valid = qtrue;  // valid delta parse
		}
	}

	// read areamask
	//P len = MSG_ReadByte( msg );

	//P
	//if ( len > sizeof( newSnap.areamask ) ) {
	//	Com_Error( ERR_DROP,"CL_ParseSnapshot: Invalid size %d for areamask.", len );
	//	return;
	//}

	//P MSG_ReadData( msg, &newSnap.areamask, len );

	// read playerinfo
	SHOWNET( msg, "playerstate" );
	if ( old ) {
		MSG_ReadDeltaPlayerstate( msg, &old->ps, &newSnap.ps );
	} else {
		MSG_ReadDeltaPlayerstate( msg, NULL, &newSnap.ps );
	}

	// read packet entities
	SHOWNET( msg, "packet entities" );
	CL_ParsePacketEntities( msg, old, &newSnap );



	// New by eyza
	SHOWNET( msg, "packet clients" );

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
	// calculate ping time
	/*Pfor ( i = 0 ; i < PACKET_BACKUP ; i++ ) {
		packetNum = ( clc.netchan.outgoingSequence - 1 - i ) & PACKET_MASK;
		if ( cl.snap.ps.commandTime >= cl.outPackets[ packetNum ].p_serverTime ) {
			cl.snap.ping = cls.realtime - cl.outPackets[ packetNum ].p_realtime;
			break;
		}
	}*/
	// save the frame off in the backup array for later delta comparisons
	cl.snapshots[cl.snap.messageNum & PACKET_MASK] = cl.snap;

	//if ( cl_shownet->integer == 3 ) {
		Com_Printf( "   snapshot:%i  delta:%i  ping:%i\n", cl.snap.messageNum,
					cl.snap.deltaNum, cl.snap.ping );
	//}

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

			Com_Printf("    configstring[%i]: %s\n", i, s);
			//Com_Printf("%s\n", s);

			if ( len + 1 + cl.gameState.dataCount > MAX_GAMESTATE_CHARS )
			{
				Com_Error( ERR_DROP, "MAX_GAMESTATE_CHARS exceeded" );
			}

			// append it to the gameState string buffer
			cl.gameState.stringOffsets[ i ] = cl.gameState.dataCount;
			memcpy( cl.gameState.stringData + cl.gameState.dataCount, s, len + 1 );
			cl.gameState.dataCount += len + 1;

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
}


int CL_ParseServerMessage( msg_t *msg )
{
	int cmd;
	byte buffer[MAX_MSGLEN];
	msg_t decompressMsg;

	Com_Printf("------------------\n");

	// get the reliable sequence acknowledge number
	clc.reliableAcknowledge = MSG_ReadLong( msg );

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
			Com_Printf( "%3i:%s\n", msg->readcount - 1, "END OF MESSAGE\n" );
			return 1;
		}

		if ( !svc_strings[cmd] )
		{
			Com_Printf( "%3i:BAD CMD %i\n", decompressMsg.readcount - 1, cmd );
		}
		else
		{
			Com_Printf( "%3i:%s\n", decompressMsg.readcount - 1, svc_strings[cmd] );
		}

		// other commands
		switch ( cmd )
		{
		default:
			Com_Error( ERR_DROP,"CL_ParseServerMessage: Illegible server message %d\n", cmd );
			return 0;

		case svc_nop:
			Com_Printf("svc_nop\n");
			break;
		case svc_serverCommand:
			Com_Printf("svc_serverCommand\n");
			CL_ParseCommandString( &decompressMsg );
			break;
		case svc_gamestate:
			//Com_Printf("svc_gamestate\n");
			CL_ParseGamestate( &decompressMsg );
			break;
		case svc_snapshot:
			Com_Printf("svc_snapshot\n");
			CL_ParseSnapshot( &decompressMsg );
			break;
		case svc_download:
			Com_Printf("svc_download\n");
			break;
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
	byte bufData[ MAX_MSGLEN ];
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

int main( int argc, char **argv )
{
	//printf("Test %d\n", COD_VERSION);
	//printf("Test %d\n", MAX_MSGLEN);
	//printf("Test %d\n", sizeof(client_t));
	//printf("Test %d\n", sizeof(huff_t));

	if( argc < 2 )
	{
		Com_Error( ERR_FATAL, "Usage: cod2demo [demofile]\n" );
	}

	
	// Log name
	snprintf(logFileName, sizeof(logFileName), "%s.log", argv[1]);

	// Clear log file
	FILE *fp = fopen(logFileName, "w");
	if (fp) {
		fclose(fp);
	}



	FS_FOpenFileRead( argv[1], &demo.demofile, qtrue );

	while (1) {
		bool exit = CL_ReadDemoMessage();

		if (!exit) break;


		snapshot_t snapshot;
		qboolean ok = CL_GetSnapshot(cl.snap.messageNum, &snapshot);

		if (!ok) continue;




		Com_Printf("-------------------------\n");
		Com_Printf("Clients: %i\n", snapshot.numClients);
		for (int i = 0; i < snapshot.numClients; i++) {
			clientState_t client = snapshot.clients[i];
			Com_Printf("number: %i, name: %s, team: %i\n", client.number, client.name, client.team);	
		}
		/*
		Com_Printf("-------------------------\n");
		//Com_Printf("-------------------------");
		Com_Printf("Entities: %i\n", cl.parseEntitiesNum);
		for (int i = 0; i < cl.parseEntitiesNum; i++) {
			entityState_t entity = cl.parseEntities[i];
			Com_Printf("number: %3i, index: %2i, clientNum: %2i, origin: %f %f %f\n", entity.number, entity.index, entity.clientNum, entity.origin2[0], entity.origin2[1], entity.origin2[2]);	
		}*/
		Com_Printf("-------------------------\n");
		Com_Printf("Playerstate:\n");
		playerState_t ps = snapshot.ps;
		Com_Printf("clientNum: %3i, ammoClip: %2i, ammo: %3i, weapon: %2i, origin: %f %f %f\n", 
			ps.clientNum, ps.ammoclip, ps.ammo, ps.weapon, ps.origin[0], ps.origin[1], ps.origin[2]);

		Com_Printf("\n\n");
	}
	


	fclose( demo.demofile );
	
	Com_Printf("Press any key to exit...\n");
	int c = fgetc(stdin);  // Read a character from stdin

	return 1;
}