/*
** Nofrendo (c) 1998-2000 Matthew Conte (matt@conte.com)
**
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of version 2 of the GNU Library General 
** Public License as published by the Free Software Foundation.
**
** This program is distributed in the hope that it will be useful, 
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
** Library General Public License for more details.  To obtain a 
** copy of the GNU Library General Public License, write to the Free 
** Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** Any permitted reproduction of these routines, in whole or in part,
** must bear this legend.
**
**
** map78.c
**
** mapper 78 interface
** $Id: map078.c,v 1.2 2001/04/27 14:37:11 neil Exp $
*/

#include "../noftypes.h"
#include "../nes/nes_mmc.h"
#include "../nes/nes_ppu.h"

/* mapper 78: Holy Diver, Cosmo Carrier */
/* ($8000-$FFFF) D2-D0 = switch $8000-$BFFF */
/* ($8000-$FFFF) D7-D4 = switch PPU $0000-$1FFF */
/* ($8000-$FFFF) D3 = switch mirroring */
static void map78_write(uint32 address, uint8 value)
{
   UNUSED(address);

   mmc_bankrom(16, 0x8000, value & 7);
   mmc_bankvrom(8, 0x0000, (value >> 4) & 0x0F);

   /* Ugh! Same abuse of the 4-screen bit as with Mapper #70 */
   if (mmc_getinfo()->flags & ROM_FLAG_FOURSCREEN)
   {
      if (value & 0x08)
         ppu_mirror(0, 1, 0, 1); /* vert */
      else
         ppu_mirror(0, 0, 1, 1); /* horiz */
   }
   else
   {
      int mirror = (value >> 3) & 1;
      ppu_mirror(mirror, mirror, mirror, mirror);
   }
}

static map_memwrite map78_memwrite[] =
{
   { 0x8000, 0xFFFF, map78_write },
   {     -1,     -1, NULL }
};

mapintf_t map78_intf =
{
   78, /* mapper number */
   "Mapper 78", /* mapper name */
   NULL, /* init routine */
   NULL, /* vblank callback */
   NULL, /* hblank callback */
   NULL, /* get state (snss) */
   NULL, /* set state (snss) */
   NULL, /* memory read structure */
   map78_memwrite, /* memory write structure */
   NULL /* external sound device */
};

/*
** $Log: map078.c,v $
** Revision 1.2  2001/04/27 14:37:11  neil
** wheeee
**
** Revision 1.1  2001/04/27 12:54:40  neil
** blah
**
** Revision 1.1.1.1  2001/04/27 07:03:54  neil
** initial
**
** Revision 1.1  2000/10/24 12:19:33  matt
** changed directory structure
**
** Revision 1.7  2000/10/22 19:17:46  matt
** mapper cleanups galore
**
** Revision 1.6  2000/10/22 15:03:14  matt
** simplified mirroring
**
** Revision 1.5  2000/10/21 19:33:38  matt
** many more cleanups
**
** Revision 1.4  2000/08/16 02:50:11  matt
** random mapper cleanups
**
** Revision 1.3  2000/07/10 05:29:03  matt
** cleaned up some mirroring issues
**
** Revision 1.2  2000/07/06 02:48:43  matt
** clearly labelled structure members
**
** Revision 1.1  2000/07/06 01:01:56  matt
** initial revision
**
*/
