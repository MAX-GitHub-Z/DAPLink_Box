

#include "cdc_user.h"



void tud_cdc_rx_cb(uint8_t itf) {
  uint8_t buf[64];
  uint32_t count = tud_cdc_read(buf, sizeof(buf));
  tud_cdc_write(buf, count);  // Echo ªÿ»•
  tud_cdc_write_flush();
}

