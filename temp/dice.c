/*
  Teem: Tools to process and visualize scientific data and images              
  Copyright (C) 2008, 2007, 2006, 2005  Gordon Kindlmann
  Copyright (C) 2004, 2003, 2002, 2001, 2000, 1999, 1998  University of Utah

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public License
  (LGPL) as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  The terms of redistributing and/or modifying this software also
  include exceptions to the LGPL that facilitate static linking.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "unrrdu.h"
#include "privateUnrrdu.h"
#include <stdlib.h>

#define INFO "Save all slices along one axis into separate files"
char *_unrrdu_diceInfoL = 
(INFO
 ". Calls \"unu slice\" for each position "
 "along the indicated axis, and saves out a different "
 "file for each sample along that axis. ");

int
unrrdu_diceMain(int argc, char **argv, char *me, hestParm *hparm) {
  hestOpt *opt = NULL;
  char *base, *err, fnout[AIR_STRLEN_MED], /* file name out */
    fffname[AIR_STRLEN_MED],  /* format for filename */
    *ftmpl;                   /* format template */
  Nrrd *nin, *nout = NULL, **nnout = NULL;
  int top, pret, start, fit;
  unsigned int axis;
  size_t pos;
  airArray *mop;
	int abort = 0;

  OPT_ADD_AXIS(axis, "axis to slice along");
  OPT_ADD_NIN(nin, "input nrrd");
  hestOptAdd(&opt, "s,start", "start", airTypeInt, 1, 1, &start, "0",
             "integer value to start numbering with");
  hestOptAdd(&opt, "ff,format", "form", airTypeString, 1, 1, &ftmpl, "",
             "a printf-style format to use for generating all "
             "filenames.  Use this to override the number of characters "
             "used to represent the slice position, or the file format "
             "of the output, e.g. \"-ff %03d.ppm\" for 000.ppm, "
             "001.ppm, etc. By default (not using this option), slices "
             "are saved in NRRD format (or PNM or PNG where possible) "
             "with shortest possible filenames.");
  hestOptAdd(&opt, "o,output", "prefix", airTypeString, 1, 1, &base, NULL,
             "output filename prefix (excluding info set via \"-ff\"), "
             "basically to set path of output files (so be sure to end "
             "with \"/\".");

  mop = airMopNew();
  airMopAdd(mop, opt, (airMopper)hestOptFree, airMopAlways);

  USAGE(_unrrdu_diceInfoL);
  PARSE();
  airMopAdd(mop, opt, (airMopper)hestParseFree, airMopAlways);

  if (start < 0) {
    fprintf(stderr, "%s: given start index (%d) less than zero\n", me, start);
    airMopError(mop);
    return 1;
  }
  if (!( axis < nin->dim )) {
    fprintf(stderr, "%s: given axis (%u) outside range [0,%u]\n",
            me, axis, nin->dim-1);
    airMopError(mop);
    return 1;
  }
  
  if (!airStrlen(ftmpl)) {
    top = start + nin->axis[axis].size-1;
    if (top > 9999999) {
      sprintf(fffname, "%%08d.nrrd");
    } else if (top > 999999) {
      sprintf(fffname, "%%07d.nrrd");
    } else if (top > 99999) {
      sprintf(fffname, "%%06d.nrrd");
    } else if (top > 9999) {
      sprintf(fffname, "%%05d.nrrd");
    } else if (top > 999) {
      sprintf(fffname, "%%04d.nrrd");
    } else if (top > 99) {
      sprintf(fffname, "%%03d.nrrd");
    } else if (top > 9) {
      sprintf(fffname, "%%02d.nrrd");
    } else {
      sprintf(fffname, "%%01d.nrrd");
    }
  }
  else {
    sprintf(fffname, "%s", ftmpl);
  }


  // Create an output filename template to pass to nrrdSaveMulti
  // Ex: "dir/%04d.png"
  sprintf(fnout, "%s%s", base, fffname);

  #pragma omp parallel for firstprivate(nout, nnout)
  for (pos=0; pos<nin->axis[axis].size; pos++) {
    if((nout == NULL) && (nnout == NULL)) {
	nout = nrrdNew();
	airMopAdd(mop, nout, (airMopper)nrrdNuke, airMopAlways);
	nnout = (Nrrd**) malloc(sizeof(Nrrd *));
	nnout[0] = nout;
    }
    if (nrrdSlice(nout, nin, axis, pos)) {
      airMopAdd(mop, err = biffGetDone(NRRD), airFree, airMopAlways);
      fprintf(stderr, "%s: error slicing nrrd:%s\n", me, err);
      airMopError(mop);
      abort = 1;
    }

    fprintf(stderr, "%s: ", me);
    fprintf(stderr, fnout, pos+start);
    fprintf(stderr, " ...\n");

    if (nrrdSaveMulti(fnout, AIR_CAST(const Nrrd *const *, nnout), 1, pos+start, NULL)) {
      airMopAdd(mop, err = biffGetDone(NRRD), airFree, airMopAlways);
      fprintf(stderr, "%s: error writing nrrd to \"%s\":%s\n",
              me, fnout, err);
      airMopError(mop);
      abort = 1;
    }
  }
  
  if(abort) {
    return 1;
  }

  airMopOkay(mop);
  return 0;
}

UNRRDU_CMD(dice, INFO);
