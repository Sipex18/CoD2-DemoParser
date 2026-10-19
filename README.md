# CoD2 Demo Parser

[![Platform](https://img.shields.io/badge/platform-Windows-blue)]()
[![Language](https://img.shields.io/badge/language-C%2B%2B-orange)]()
[![Game](https://img.shields.io/badge/game-CoD2%201.0%20%2F%201.3%20%2F%20CoD2x-green)]()

Parses Call of Duty 2 demo files (`.dm_1`) into structured, readable JSON — rounds, kills, chat, player timelines, grenades, scoreboard.

Extended version of [eyza-cod2/CoD2-DemoParser](https://github.com/eyza-cod2/CoD2-DemoParser). The original tool printed a raw snapshot dump; I expanded it to reconstruct the whole match — teams, rounds, killfeed, chat, positions — into per-demo JSON output, and fixed a series of parsing bugs along the way.

Supports classic **1.0**, **1.3**, and **CoD2x** (protocol 120) demos, including zPAM match demos.

## Usage

```
cod2demo.exe <demo>.dm_1
cod2demo_10.exe <demo>.dm_1   # 1.0 demos (smaller MAX_MSGLEN)
```

Output goes to a directory named after the demo, next to the `.dm_1` file:

```
my_match/
├── meta.json         demo info + file manifest (record counts per file)
├── match.json        the whole match: per-round meta + chronological event
│                     stream (kills with full detail, grenades, scores,
│                     plants/defuses, zPAM reports)
├── scoreboard.json   per-player stats: score, K/D, shots, weapons, per-round
├── chat.json         flat chronological chat feed (all/team, colors, dead)
├── pov.json          recorder's full playerstate timeline (~30 Hz)
├── players/          one file per player: NN_name.json + index.json
└── log.txt           configstrings + parse diagnostics
```

## What's inside

### Match reconstruction (SD / zPAM)

- **match.json** — one file, one story: each round object carries its meta (start/end, score, team names, end reason) plus a chronological `events` array. Kill events include attacker/victim (num, name, team), weapon id+name, mod, kill icon and both world positions; grenade events carry the full trajectory (from/vel/to, bounces, attributed kills). Kills or grenades outside any round land in a top-level `unassigned` list.
- **Rounds** — detected from `_match_info` / `_match_round` / `_match_score` configstring updates (works with zPAM 3.3x and 4.x formats: `Round N | MR12`, `Ready-up`, live scores `X:Y`)
- **Round ends** — `f` server announcements decoded: `allies_eliminated`, `axis_eliminated`, `time_expired`, `bomb_defused`, `allies_mission` / `axis_mission` (bomb exploded), `target_destroyed`, `draw`, `bomb_planted` as mid-round event
- **Killfeed** — `EV_OBITUARY` broadcast temp-entities (`eType = ET_EVENTS + 198`): attacker, victim, weapon name, means of death (headshot, melee, crush, falling, suicide), kill icon as shown in-game, plus attacker/victim world positions — embedded in `match.json`
- **Chat** — `h`/`i` server commands, fully decoded: message type (all/team), sender name, team, dead flag, color code; localization markers stripped, text transcoded CP1250 → UTF-8 byte-accurately (accents, icons, color codes preserved)
- **Scoreboard** — the `b` server command is parsed for live score/ping/deaths (the `CS_SCORES` configstrings only update at intermission, so the scoreboard command is the only source of mid-match scores)
- **Grenades** — `ET_MISSILE` entities tracked spawn → explode; thrower inferred from spawn position, kills attributed by time/weapon window — embedded in `match.json`
- **zPAM reports** — `Round report` / `hit` / `kill` lines parsed into structured fields (damage, pellets, range, bodypart, weapon)

### Player timelines

- **pov.json** — the recorder's playerstate every snapshot: origin, velocity, pitch/yaw/roll, weapon, health, eFlags/pmFlags, legs/torso/weapon animations
- **players/NN_name.json** — every other player's entity timeline, grouped by round, with a per-round summary (kills, deaths, weapons used)
- Filenames are sanitized for Windows; the recorder appears in `players/index.json` pointing at `pov.json`

### Shot counting

- Recorder: **exact**, computed from `ammoclip` deltas. Clip slots are bound to weapons at runtime (`BG_ClipForWeapon` semantics — clip-indexed, not weapon-indexed), so reloads, weapon drops, weapon switches and grenade throws don't inflate the count
- Other players: `EV_FIRE_WEAPON*` entity events — a lower bound, since the 4-deep event ring and PVS filtering can drop events (documented in `scoreboard.json`)

## Fixes over the original

The fork started as a bug-hunt against real match demos — the upstream code crashed or silently corrupted data on most of them.

**Protocol / parsing**

- Delta-removed entities/clients are now detected from `MSG_ReadDeltaStruct`'s return value instead of reading a nonexistent bit — removed entries no longer occupy parse slots (this alone was corrupting every snapshot after the first removal)
- `MAX_PARSE_CLIENTS` raised 128 → 2048 to match the real client's `0x7FF` mask, plus the missing `parseClientsNum` age check on delta frames
- "No delta" path copies `4 * numFields + 4` bytes — the exact struct size, matching the original client
- `Com_Error` now exits on fatal errors instead of returning into an infinite loop on truncated/corrupt demos
- Reliable-command guard in `CL_ReadDemoMessage` compares against `reliableSequence` (as the real client does)
- `svc_download` blocks are properly skipped (the real client's format), so demos containing downloads don't desync
- Server commands handled: `d`/`x`/`y`/`z` configstring updates mid-demo, `b` scoreboard, `h`/`i` chat, `f` announcements
- Event-sequence tracking resets on entity re-entry and teleport (eFlags bit), matching the client's `CG_CheckEvents`
- 128 KB stack buffers in the message decompressor moved to static storage

**Data correctness**

- Kill positions: the recorder's position comes from `ps.origin` (the recorder never appears as an entity in their own demo); corpse entities fill `lastKnownPos` for dead players but never overwrite a live player — zero `[0,0,0]` positions across all test demos
- Kill records store the roster **at kill time**, not at demo end — halftime side swaps resolve to the correct team/name
- CP1250 → UTF-8 table verified byte-for-byte against the reference codec (31 incorrect mappings fixed)
- qsort comparators rewritten overflow-safe; event ordering in `match.json` is chronological across merged kill/event/grenade streams

**Performance**

- Log file kept open for the whole run instead of `fopen`/`fclose` per line — 4m51s → 2.3s on a full match demo

## Limitations

- **FOV** is not in demos — it's a client-side dvar, never networked
- Other players' pitch/roll are limited to what the game transmits (yaw is exact)
- Non-recorder shot counts are lower-bound estimates (event ring / PVS loss)
- Old solo-recorded demos legitimately show few or no kills — the recorder was alone on the server

## Building

MinGW-w64 (tested with WinLibs / msys2):

```
gcc -g -static -Wno-write-strings -DCOD_VERSION=COD2_1_3 src/main.cpp -o bin/cod2demo.exe
gcc -g -static -Wno-write-strings -DCOD_VERSION=COD2_1_0 src/main.cpp -o bin/cod2demo_10.exe
```

`src/sizecheck.cpp` is a regression harness that asserts the network struct sizes against the real client (`entityState_t` = 240, `clientState_t` = 92, `playerState_t` = 9896 bytes).

## Credits

- [eyza](https://github.com/eyza-cod2) — original [CoD2-DemoParser](https://github.com/eyza-cod2/CoD2-DemoParser)
- [KILLTUBE/demoparser](https://github.com/KILLTUBE/demoparser) — message/Huffman code this is based on
- [voron00/CoD2rev_Server](https://github.com/voron00/CoD2rev_Server) — server-side reference used to verify message formats
- [CoD2x](https://cod2x.me/) — client reference for protocol 120 demos

## Contact

[![YouTube](https://img.shields.io/badge/YouTube-%40sipex18-red?logo=youtube)](https://www.youtube.com/@sipex18)
![Discord](https://img.shields.io/badge/Discord-sipex18-5865F2?logo=discord&logoColor=white)
