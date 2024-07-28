#include "Display_ST7796.h"

static constexpr const char *TAG = "Display";

static lgfx::Bus_SPI _bus_instance;
static lgfx::Panel_ST7796 _panel_instance;
static lgfx::Light_PWM _light_instance;
static lgfx::Touch_XPT2046 _touch_instance;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[2][480 * 29];

Display_ST7796::Display_ST7796(
    const uint32_t screenWidth,
    const uint32_t screenHeight) : _screenWidth(screenWidth), _screenHeight(screenHeight)
{
  _bus_instance.config(Display_ST7796::cfg_bus);
  _panel_instance.setBus(&_bus_instance);

  _panel_instance.config(Display_ST7796::cfg_panel);

  _light_instance.config(Display_ST7796::cfg_light);
  _panel_instance.setLight(&_light_instance);

  lgfx::v1::ITouch::config_t temp_cfg_touch = _touch_instance.config();
  lgfx::v1::ITouch::config_t *_cfg_touch = &temp_cfg_touch;

  _cfg_touch->pin_sclk = 14;
  _cfg_touch->pin_mosi = 13;
  _cfg_touch->pin_miso = 12;
  _cfg_touch->pin_cs = 33;

  _touch_instance.config(Display_ST7796::cfg_touch);
  _panel_instance.setTouch(&_touch_instance);

  lgfx_d.setPanel(&_panel_instance);
}

bool Display_ST7796::begin()
{
  lgfx_d.begin();
  lgfx_d.setRotation(3);
  lgfx_d.setBrightness(200);
  lgfx_d.setSwapBytes(true);

  lv_init();
  lv_disp_draw_buf_init(&draw_buf, buf[0], buf[1], 480 * 29);

  init_lcd();
  init_touch();

  lgfx_d.fillScreen(0x0000); // 黑
  delay(500);
  lgfx_d.fillScreen(0xF800); // 红
  delay(500);
  lgfx_d.fillScreen(0x07E0); // 绿
  delay(500);
  lgfx_d.fillScreen(0x001F); // 蓝
  delay(500);

  ESP_LOGD(TAG, "init done");

  return true;
}

void Display_ST7796::fillScreenBlack()
{
  lgfx_d.fillScreen(0x0000);
}

void Display_ST7796::fillScreenRed()
{
  lgfx_d.fillScreen(0xF800);
}

void Display_ST7796::fillScreenGreen()
{
  lgfx_d.fillScreen(0x07E0);
}

void Display_ST7796::fillScreenBlue()
{
  lgfx_d.fillScreen(0x001F);
}

void Display_ST7796::init_lcd()
{
  static lv_disp_drv_t disp_drv;                    // Descriptor of a display driver
  lv_disp_drv_init(&disp_drv);                      // Basic initialization
  disp_drv.flush_cb = my_disp_flush;                // Set your driver function
  disp_drv.draw_buf = &draw_buf;                    // Assign the buffer
  disp_drv.hor_res = _screenWidth;                  // horizontal resolution
  disp_drv.ver_res = _screenHeight;                 // vertical resolution
  lv_disp_drv_register(&disp_drv);                  // Finally register the driver
  lv_disp_set_bg_color(NULL, lv_color_hex(0x0000)); // background black
}

void Display_ST7796::init_touch()
{
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);
}

void Display_ST7796::my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  Display_ST7796 *instance = static_cast<Display_ST7796 *>(disp->user_data);
  if (instance->lgfx_d.getStartCount() == 0)
  { // Run if not already started
    instance->lgfx_d.startWrite();
  }
  instance->lgfx_d.pushImageDMA(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1, (lgfx::swap565_t *)&color_p->full);
  lv_disp_flush_ready(disp);
}

void Display_ST7796::my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  Display_ST7796 *instance = static_cast<Display_ST7796 *>(indev_driver->user_data);

  uint16_t touchX, touchY;
  bool touched = instance->lgfx_d.getTouch(&touchX, &touchY); // Use the instance to access lgfx_d
  Serial.print(touchX);
  Serial.print(" ");
  Serial.println(touchY);
  if (touched)
  {
    if (touchX < instance->_screenWidth && touchY < instance->_screenHeight) // Use the instance to access _screenWidth and _screenHeight
    {
      data->state = LV_INDEV_STATE_PR;
      data->point.x = touchX;
      data->point.y = touchY;
      Serial.printf("%d,%d\n", touchX, touchY);
    }
    else
    {
      data->state = LV_INDEV_STATE_REL;
    }
  }
}