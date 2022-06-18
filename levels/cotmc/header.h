#ifndef COTMC_HEADER_H
#define COTMC_HEADER_H

#include "types.h"

// geo
extern const GeoLayout cotmc_geo_0001A0[];

// leveldata
extern const Gfx cotmc_seg7_dl_07007D48[];
extern const Gfx cotmc_seg7_dl_0700A160[];
extern const Gfx cotmc_seg7_dl_0700A4B8[];
extern const Collision cotmc_seg7_collision_level[];
extern const MacroObject cotmc_seg7_macro_objs[];
extern const Gfx cotmc_dl_water_begin[];
extern const Gfx cotmc_dl_water_end[];
extern Movtex cotmc_movtex_tris_water[];
extern const Gfx cotmc_dl_water[];

// script
extern const LevelScript level_cotmc_entry[];

#include "gear_1/geo_header.h"

#include "gear_1/collision_header.h"

#include "gear_2/geo_header.h"

#include "gear_2/collision_header.h"

#include "gear_3/geo_header.h"

#include "gear_3/collision_header.h"

#include "gear_4/collision_header.h"

#include "gear_4/geo_header.h"

#include "gear_kuromame/geo_header.h"

#include "levels/cotmc/header.inc.h"

#endif
