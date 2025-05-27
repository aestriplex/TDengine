/*
 * Copyright (c) 2019 TAOS Data, Inc. <jhtao@taosdata.com>
 *
 * This program is free software: you can use, redistribute, and/or modify
 * it under the terms of the GNU Affero General Public License, version 3
 * or later ("AGPL"), as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TDENGINE_GEOS_WRAPPER_H
#define TDENGINE_GEOS_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "os.h"

#ifdef USE_GEOS
#include "tgeosctx.h"

typedef enum {
  PURE_NUMBER = 0x0000,
  METER       = 0x0100,
  DEGREE      = 0x0200,

  NONE        = 0xffff,
} unit_t;

typedef struct {
  int srid;
  const char *name;
  const char *projString;
  unit_t defaultUnitXY;
  unit_t defaultUnitZ;
} SRSInfo;

typedef struct {
  GEOSGeometry *geom;
  SRSInfo *srs;
} TaosGeometry;

typedef struct {
  const GEOSPreparedGeometry *geom;
  SRSInfo *srs;
} TaosPreparedGeometry;

static SRSInfo implementedSrs[] = {
  {4326,  "WGS 84",       "+proj=longlat +datum=WGS84 +no_defs",                                   DEGREE,      NONE},
  {3857,  "Web Mercator", "+proj=merc +lon_0=0 +k=1 +x_0=0 +y_0=0 +datum=WGS84 +units=m +no_defs", METER,       NONE},
  {4979,  "WGS 84 3D",    "+proj=longlat +datum=WGS84 +no_defs +type=crs",                         DEGREE,      METER},
  {0,     "Cartesian",     NULL,                                                                   PURE_NUMBER, PURE_NUMBER},
};

#define WGS84_CODE          0
#define WEB_MERCATOR_CODE   1
#define WGS84_ALTITUDE_CODE 3
#define CARTESIAN_CODE      4

void geosFreeBuffer(void *buffer);

int32_t initTaosGeometry(TaosGeometry *geom, int srsCode);
int32_t initTaosPreparedGeometry(TaosPreparedGeometry *geom, int srsCode);
int32_t defaultUnitTaosGeometry(TaosGeometry *geom);
int32_t defaultUnitTaosPreparedGeometry(TaosPreparedGeometry *geom);

int32_t initCtxMakePoint();
int32_t doMakePoint(double x, double y, unsigned char **outputGeom, size_t *size);

int32_t initCtxGeomFromText();
int32_t doGeomFromText(const char *inputWKT, unsigned char **outputGeom, size_t *size);

int32_t initCtxAsText();
int32_t doAsText(const unsigned char *inputGeom, size_t size, char **outputWKT);
int32_t checkWKB(const unsigned char *wkb, size_t size);

int32_t initCtxRelationFunc();
int32_t doIntersects(const TaosGeometry *geom1, const TaosPreparedGeometry *preparedGeom1, const TaosGeometry *geom2,
                     bool swapped, char *res);
int32_t doEquals(const TaosGeometry *geom1, const TaosPreparedGeometry *preparedGeom1, const TaosGeometry *geom2,
                 bool swapped, char *res);
int32_t doTouches(const TaosGeometry *geom1, const TaosPreparedGeometry *preparedGeom1, const TaosGeometry *geom2,
                  bool swapped, char *res);
int32_t doCovers(const TaosGeometry *geom1, const TaosPreparedGeometry *preparedGeom1, const TaosGeometry *geom2,
                 bool swapped, char *res);
int32_t doContains(const TaosGeometry *geom1, const TaosPreparedGeometry *preparedGeom1, const TaosGeometry *geom2,
                   bool swapped, char *res);
int32_t doContainsProperly(const TaosGeometry *geom1, const TaosPreparedGeometry *preparedGeom1,
                           const TaosGeometry *geom2, bool swapped, char *res);

int32_t readGeometry(const unsigned char *input, TaosGeometry *outputGeom,
                     TaosPreparedGeometry *outputPreparedGeom);
void    destroyGeometry(TaosGeometry *geom, TaosPreparedGeometry *preparedGeom);

#else
int32_t initCtxAsText();
int32_t doAsText(const unsigned char *inputGeom, size_t size, char **outputWKT);
int32_t checkWKB(const unsigned char *wkb, size_t size);

#endif

#ifdef __cplusplus
}
#endif

#endif /*TDENGINE_GEOS_WRAPPER_H*/
