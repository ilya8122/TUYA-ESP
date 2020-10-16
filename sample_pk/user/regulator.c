#include "tuya_smart_api.h"
#include "define.h"

#include "tuya_msg.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void rele_on(LED_HANDLE Rele) {
  tuya_set_led_type(Rele, OL_HIGH, 0);
  PR_DEBUG("rele_on");
}
void rele_off(LED_HANDLE Rele) {
  tuya_set_led_type(Rele, OL_LOW, 0);
  PR_DEBUG("rele_off");
}

void rele_on_msg(LED_HANDLE Rele) {
  tuya_set_led_type(Rele, OL_HIGH, 0);
  tuya_msg(1, 0, 2);
  PR_DEBUG("rele_on_msg");
}
void rele_off_msg(LED_HANDLE Rele) {
  tuya_set_led_type(Rele, OL_LOW, 0);
  tuya_msg(1, 0, 1);
  PR_DEBUG("rele_off_msg");
}

void check_protecki(bool def_protechki, int ISTOCHNIK, bool w1, bool w2,
                    LED_HANDLE Rele) {
  if (def_protechki) {
    switch (ISTOCHNIK) {
      case 0:
        if (!w1) {
          rele_off_msg(Rele);
        }
        break;

      case 1:
        if (!w2) {
          rele_off_msg(Rele);
        }
        break;

      case 2:
        if (!w1 || !w2) {
          rele_off_msg(Rele);
        }
        break;

      case 3:
        if (!w1 && !w2) {
          rele_off_msg(Rele);
        }
        break;

      default:
        PR_DEBUG("ISTOCHNIK-UNKOWN");
        break;
    }
  }
}

void check_REGULATOR(int REGULATOR, int temp, int temp_min, int temp_max,
                     bool def_protechki, int ISTOCHNIK, int w1, int w2,
                     int LOW_LEVEL, int HIGH_LEVEL, LED_HANDLE Rele) {
  check_protecki(def_protechki, ISTOCHNIK, w1, w2, Rele);

  switch (REGULATOR) {
    case 0:  // off
      break;

    case Heater:  // heater
      if (temp >= temp_max) {
        rele_off_msg(Rele);
      } else if (temp < temp_min) {
        rele_on_msg(Rele);
      }
      break;

    case Conditioner:  // conditioner
      if (temp <= temp_min) {
        rele_off_msg(Rele);
      } else if (temp >= temp_max) {
        rele_on_msg(Rele);
      }
      break;

    case Inrange:  // inrange
      if (temp_min <= temp && temp <= temp_max) {
        rele_on_msg(Rele);
      } else {
        rele_off_msg(Rele);
      }
      break;

    case OutRange:  // OutRange
      if (temp_min <= temp && temp <= temp_max) {
        rele_off_msg(Rele);
      } else {
        rele_on_msg(Rele);
      }
      break;

    case HeaterW:  // 5 HeaterW
      if (def_protechki) {
        def_protechki = false;
        tuya_msg(Water_def, 0, 1);
      }
      switch (ISTOCHNIK) {
        case 0:
          if (!w1) {
            rele_off_msg(Rele);
          }
          break;

        case 1:
          if (!w2) {
            rele_off_msg(Rele);
          }
          break;

        case 2:
          if (!w1 || !w2) {
            rele_off_msg(Rele);
          }
          break;

        case 3:
          if (!w1 && !w2) {
            rele_off_msg(Rele);
          }
          break;

        default:
          PR_DEBUG("ISTOCHNIK-UNKOWN");
          break;
      }

      if (temp >= temp_max) {
        rele_off_msg(Rele);
      } else if (temp < temp_min) {
        rele_on_msg(Rele);
      }
      break;

    case ConditionerW:  // 6 ConditionerW
      if (def_protechki) {
        def_protechki = false;
        tuya_msg(Water_def, 0, 1);
      }
      switch (ISTOCHNIK) {
        case 0:
          if (!w1) {
            rele_off_msg(Rele);
          }
          break;
        case 1:
          if (!w2) {
            rele_off_msg(Rele);
          }
          break;
        case 2:
          if (!w1 || !w2) {
            rele_off_msg(Rele);
          }
          break;
        case 3:
          if (!w1 && !w2) {
            rele_off_msg(Rele);
          }
          break;

        default:
          PR_DEBUG("ISTOCHNIK-UNKOWN");
          break;
      }

      if (temp <= temp_min) {
        rele_off_msg(Rele);
      } else if (temp >= temp_max) {
        rele_on_msg(Rele);
      }
      break;

    case InrangeW:  // 7 InrangeW
      if (def_protechki) {
        def_protechki = false;
        tuya_msg(Water_def, 0, 1);
      }
      switch (ISTOCHNIK) {
        case 0:
          if (!w1) {
            rele_off_msg(Rele);
          }
          break;
        case 1:
          if (!w2) {
            rele_off_msg(Rele);
          }
          break;
        case 2:
          if (!w1 || !w2) {
            rele_off_msg(Rele);
          }
          break;
        case 3:
          if (!w1 && !w2) {
            rele_off_msg(Rele);
          }
          break;

        default:
          PR_DEBUG("ISTOCHNIK-UNKOWN");
          break;
      }

      if (temp_min <= temp && temp <= temp_max) {
        rele_on_msg(Rele);
      } else {
        rele_off_msg(Rele);
      }
      break;

    case OutRangeW:  // 8 OutRangeW
      if (def_protechki) {
        def_protechki = false;
        tuya_msg(Water_def, 0, 1);
      }
      switch (ISTOCHNIK) {
        case 0:
          if (!w1) {
            rele_off_msg(Rele);
          }
          break;
        case 1:
          if (!w2) {
            rele_off_msg(Rele);
          }
          break;
        case 2:
          if (!w1 || !w2) {
            rele_off_msg(Rele);
          }
          break;
        case 3:
          if (!w1 && !w2) {
            rele_off_msg(Rele);
          }
          break;

        default:
          PR_DEBUG("ISTOCHNIK-UNKOWN");
          break;
      }

      if (temp_min <= temp && temp <= temp_max) {
        rele_off_msg(Rele);
      } else {
        rele_on_msg(Rele);
      }

      break;

    case Filling:  // 9 Filling
      if (def_protechki == true) {
        def_protechki = false;
        tuya_msg(Water_def, 0, 1);
      }

      switch (LOW_LEVEL) {
        case 0:
          w1 = tuya_read_gpio_level(W1_io12);
          break;

        case 1:
          w2 = tuya_read_gpio_level(W2_io13);
          break;
      }
      switch (HIGH_LEVEL) {
        case 0:
          w1 = tuya_read_gpio_level(W1_io12);
          break;

        case 1:
          w2 = tuya_read_gpio_level(W2_io13);
          break;
      }

      if (w1 == 1 && w2 == 0) {
        rele_on_msg(Rele);
      } else if (w1 == 0 && w2 == 0) {
        rele_off_msg(Rele);
      } else {
      }
      break;

    case Drain:  // 10 Drain
      if (def_protechki == true) {
        def_protechki = false;
        tuya_msg(Water_def, 0, 1);
      }
      switch (LOW_LEVEL) {
        case 0:
          w1 = tuya_read_gpio_level(W1_io12);
          break;

        case 1:
          w2 = tuya_read_gpio_level(W2_io13);
          break;
      }
      switch (HIGH_LEVEL) {
        case 0:
          w1 = tuya_read_gpio_level(W1_io12);
          break;

        case 1:
          w2 = tuya_read_gpio_level(W2_io13);
          break;
      }
      if (w1 == 0 && w2 == 0) {
        rele_on_msg(Rele);
      } else if (w1 == 1 && w2 == 0) {
        rele_off_msg(Rele);
      }
      break;
    default:
      PR_DEBUG("REGULATOR-UNKOWN");
      break;
  }
}

