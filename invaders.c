#include "invaders.h"

// reads a byte from memory
static uint8_t invaders_rb(void* userdata, Word addr) {
  invaders* const si = (invaders*) userdata;

  if (addr >= 0x6000) {
    return 0;
  }
  if (addr >= 0x4000 && addr < 0x6000) {
    addr -= 0x2000; // RAM mirror
  }

  return si->memory[addr];
}

// writes a byte to memory
static void invaders_wb(void* userdata, Word addr, uint8_t val) {
  invaders* const si = (invaders*) userdata;

  // the game can only write to 0x2000-0x4000
  if (addr >= 0x2000 && addr < 0x4000) {
    si->memory[addr] = val;
  }
}

// port in (read)
static uint8_t port_in(void* userdata, uint8_t port) {
  invaders* const si = (invaders*) userdata;
  uint8_t value = 0xFF;

  // see https://computerarcheology.com/Arcade/SpaceInvaders/Hardware.html
  switch (port) {
  case 0: // 0000pr: INP0 (note: apparently never used by code)
    break;

  case 1: // 0001pr: INP1
    value = si->port1;
    break;

  case 2: // 0002pr: INP2
    value = si->port2;
    break;

  case 3: { // 0003pr: SHFT_IN
    // reading from port 3 returns the shift result:
    const Word shift = (si->shift_msb << 8) | si->shift_lsb;
    value = (shift >> (8 - si->shift_offset)) & 0xFF;
  } break;

  default: fprintf(stderr, "error: unknown IN port %02x\n", port); break;
  }

  return value;
}

// port out (write)
static void port_out(void* userdata, uint8_t port, uint8_t value) {
  invaders* const si = (invaders*) userdata;

  // see https://computerarcheology.com/Arcade/SpaceInvaders/Hardware.html
  switch (port) {
  case 2: // 0002pw: SHFTAMNT
    // sets the shift offset for the 8 bit result
    si->shift_offset = value & 7; // 0b111
    break;

  case 3: // 0003pw: SOUND1
    // plays a sound from bank 1
    invaders_play_sound(si, 1);
    break;

  case 4: // 0004pw: SHFT_DATA
    // writing to port 4 shifts MSB into LSB, and the new value into MSB:
    si->shift_lsb = si->shift_msb;
    si->shift_msb = value;
    break;

  case 5: // 0005pw: SOUND2
    // plays a sound from bank 2
    invaders_play_sound(si, 2);
    break;

  case 6: // 0006pw: WATCHDOG
    break;

  default: fprintf(stderr, "error: unknown OUT port %02x\n", port); break;
  }
}

void invaders_init(invaders* const si) {
  i8080_init(&si->cpu);
  si->cpu.userdata = si;
  si->cpu.read_byte = invaders_rb;
  si->cpu.write_byte = invaders_wb;
  si->cpu.port_in = port_in;
  si->cpu.port_out = port_out;

  memset(si->memory, 0, sizeof(si->memory));
  memset(si->screen_buffer, 0, sizeof(si->screen_buffer));
  si->next_interrupt = 0xcf;

  // PORT 1:
  // Bit Description
  // 0   Coin slot (1 = coin inserted)
  // 1   Two players button
  // 2   One player button
  // 3   n/a
  // 4   Player one - Fire button
  // 5   Player one - Left button
  // 6   Player one - Right button
  // 7   n/a
  si->port1 = 0;

  // PORT 2:
  // Bit Description
  // 0-1 DIP switch: number of ships (00 = 3 ships, 10 = 5 ships,
  //                                  01 = 4 ships, 11 = 6 ships)
  // 2   TILT
  // 3   n/a
  // 4   Player two - Fire button
  // 5   Player two - Left button
  // 6   Player two - Right button
  // 7   ???
  si->port2 = 0;

  si->shift_msb = 0;
  si->shift_lsb = 0;
  si->shift_offset = 0;

  si->last_out_port3 = 0;
  si->last_out_port5 = 0;

  si->colored_screen = true;
  si->update_screen = NULL;
}

// advances emulation for `ms` milliseconds.
void invaders_update(invaders* const si, int ms) {
  // machine executes exactly CLOCK_SPEED cycles every second, so we need
  // to execute "ms * CLOCK_SPEED / 1000"
  int count = 0;
  while (count < ms * CLOCK_SPEED / 1000) {
    int cyc = si->cpu.cyc;
    i8080_step(&si->cpu);
    int elapsed = si->cpu.cyc - cyc;
    count += elapsed;

    // interrupt handling: two interrupts are requested:
    // - 0xcf (RST 8) when the beam is *near* the middle of the screen
    // - 0xd7 (RST 10) when the beam is at the end of the screen (line 224)
    //
    // For now, we just request one interrupt after one mid-frame, so
    // after "CYCLES_PER_FRAME / 2" cycles
    if (si->cpu.cyc >= CYCLES_PER_FRAME / 2) {
      si->cpu.cyc -= CYCLES_PER_FRAME / 2;

      i8080_interrupt(&si->cpu, si->next_interrupt);
      if (si->next_interrupt == 0xd7) {
        // we update the screen at the start of vblank,
        // which coincides with the request of RST 10 interrupt
        invaders_gpu_update(si);
      }
      si->next_interrupt = si->next_interrupt == 0xcf ? 0xd7 : 0xcf;
    }
  }
}

// updates the screen buffer according to what is in the video ram
void invaders_gpu_update(invaders* const si) {
  // the screen is 256 * 224 pixels, and is rotated anti-clockwise.
  // these are the overlay dimensions:
  // ,_______________________________.
  // |WHITE            ^             |
  // |                32             |
  // |                 v             |
  // |-------------------------------|
  // |RED              ^             |
  // |                32             |
  // |                 v             |
  // |-------------------------------|
  // |WHITE                          |
  // |         < 224 >               |
  // |                               |
  // |                 ^             |
  // |                120            |
  // |                 v             |
  // |                               |
  // |                               |
  // |                               |
  // |-------------------------------|
  // |GREEN                          |
  // | ^                  ^          |
  // |56        ^        56          |
  // | v       72         v          |
  // |____      v      ______________|
  // |  ^  |          | ^            |
  // |<16> |  < 118 > |16   < 122 >  |
  // |  v  |          | v            |
  // |WHITE|          |         WHITE|
  // `-------------------------------'

  // the screen is 256 * 224 pixels, and 1 byte contains 8 pixels
  for (int i = 0; i < 256 * 224 / 8; i++) {
    const int y = i * 8 / 256;
    const int base_x = (i * 8) % 256;
    const uint8_t cur_byte = si->memory[VRAM_ADDR + i];

    for (uint8_t bit = 0; bit < 8; bit++) {
      int px = base_x + bit;
      int py = y;
      const bool is_pixel_lit = (cur_byte >> bit) & 1;
      uint8_t r = 0, g = 0, b = 0;

      // colour handling:
      if (!si->colored_screen && is_pixel_lit) {
        r = 255;
        g = 255;
        b = 255;
      } else if (si->colored_screen && is_pixel_lit) {
        if (px < 16) {
          if (py < 16 || py > 118 + 16) {
            r = 255;
            g = 255;
            b = 255;
          } else {
            g = 255;
          }
        } else if (px >= 16 && px <= 16 + 56) {
          g = 255;
        } else if (px >= 16 + 56 + 120 && px < 16 + 56 + 120 + 32) {
          r = 255;
        } else {
          r = 255;
          g = 255;
          b = 255;
        }
      }

      // space invaders' screen is rotated 90 degrees anti-clockwise
      // so we invert the coordinates:
      const int temp_x = px;
      px = py;
      py = -temp_x + SCREEN_HEIGHT - 1;

      si->screen_buffer[py][px] = (Color){ r, g, b, 255 };
    }
  }

  si->update_screen(si);
}

// Might impl later...
void invaders_play_sound(invaders* const si, uint8_t bank) {
  // plays a sound if the corresponding bit have changed from 0 to 1
  uint8_t data = si->cpu.a;

  if (bank == 1) {
    if (data != si->last_out_port3) {
      si->last_out_port3 = data;
    }
  } else if (bank == 2) {
    if (data != si->last_out_port5) {
      si->last_out_port5 = data;
    }
  }
}

void invaders_get_hiscore(invaders* const si, uint8_t* value) {
  // scores are stored in RAM at 0x20F4 (high), 0x20F8 (P1) and 0x20FC (P2)
  // (two bytes each)

  const uint16_t SCORE_ADDR = 0x20F4;
  value[0] = si->memory[SCORE_ADDR + 0];
  value[1] = si->memory[SCORE_ADDR + 1];
}

void invaders_set_hiscore(invaders* const si, uint8_t value[2]) {
  // at the start of game (before drawing the "status bar"), the game
  // copies memory from $1B00-$1BBF (rom) to $2000-$20BF,
  // so we need to update this location in memory instead of 0x20F4.
  // see http://computerarcheology.com/Arcade/SpaceInvaders/Code.html

  const uint16_t SCORE_ADDR = 0x1BF4;
  si->memory[SCORE_ADDR + 0] = value[0];
  si->memory[SCORE_ADDR + 1] = value[1];
}
