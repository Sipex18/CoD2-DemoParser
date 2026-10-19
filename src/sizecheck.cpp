#include <stdio.h>
#include <stdint.h>
#include "declarations.hpp"

int main()
{
	printf("entityState_t  = %zu (real client: 240)\n", sizeof(entityState_t));
	printf("clientState_t  = %zu (real client: 92)\n", sizeof(clientState_t));
	printf("playerState_t  = %zu (real client: 0x26A8 = 9896)\n", sizeof(playerState_t));
	printf("hudelem_t      = %zu\n", sizeof(hudelem_t));
	printf("hudElemState_t = %zu\n", sizeof(hudElemState_t));
	printf("clSnapshot_t   = %zu (real client: 0x26D8 = 9944)\n", sizeof(clSnapshot_t));
	printf("snapshot_t     = %zu\n", sizeof(snapshot_t));
	printf("entityStateFields  = %zu (real client: 59)\n", COUNT_OF(entityStateFields));
	printf("clientStateFields  = %zu (real client: 22)\n", COUNT_OF(clientStateFields));
	printf("playerStateFields  = %zu\n", COUNT_OF(playerStateFields));
	printf("hudElemFields      = %zu\n", COUNT_OF(hudElemFields));
	printf("objectiveFields    = %zu\n", COUNT_OF(objectiveFields));
	printf("MAX_PARSE_ENTITIES = %d (real client: 2048)\n", MAX_PARSE_ENTITIES);
	printf("MAX_PARSE_CLIENTS  = %d (real client: 2048)\n", MAX_PARSE_CLIENTS);
	return 0;
}
